#include "Path.h"
#include "AddressList.h"
#include <iostream>
#include <optional>
#include <cassert>

AddressList::AddressList() {}
AddressList::AddressList(std::vector<Address> in_path) : Path(in_path) {}

std::optional<Address> AddressList::last()
{
	if (std::size(path) == 0)
	{
		std::cout << "[Warning] No last element in path of 0 length." << std::endl;
		return{};
	}
	return path[std::size(path)-1];
}

std::string AddressList::display()
{
	std::string out = "";
	for (auto a : path)
	{
		out += a.display();
		out += ",";
	}
	return out;
}
void AddressList::print()
{
	std::cout << "PATH:[" << display() << "]" << std::endl;
}

std::vector<double> AddressList::x(double shift)
{
	std::vector<double> xs;
	for (auto a : path)
	{
		xs.push_back(a.x() + shift);
	}
	return xs;
}
std::vector<double> AddressList::y(double shift)
{
	std::vector<double> ys;
	for (auto a : path)
	{
		ys.push_back(a.y() + shift);
	}
	return ys;
}

AddressList AddressList::reverse_route(int m,int n, bool tour)
{
	int N = path.size() - 1;
	if (tour)
	{
		assert(m > 0); assert(m < n); assert(n < N);
	}
	else
	{
		assert(m >= 0); assert(m < n); assert(n <= N);
	}

	std::vector<Address> path2;
	path2.insert(path2.begin(), path.begin(), path.begin() + m);
	path2.insert(path2.begin() + m, path.rbegin() + (N - n), path.rbegin() + (N - m + 1));
	path2.insert(path2.begin() + (n+1), path.begin() + (n+1), path.end());

	assert(path2.size() == path.size());

	return path2;
}

AddressList AddressList::greedy_route()
{
	// Note using masking over deletion or copy (saves memory and avoids errors).
	int pathLen = path.size();

	if (pathLen <= 1) 
	{
		std::cout << "0-length addresslist" << std::endl;
		return *this; //0-length case
	}

	AddressList greed;
	std::optional<Address> opt;
	Address current = path[0];

	greed.add_address(current);

	for(int i = 1; i < pathLen; i++)
	{
		opt = index_closest_to(current, greed, true, true);
		if (opt.has_value())
		{
			current = opt.value();
			//std::cout << opt.value().display();
			greed.add_address(current);
			//greed.print();
		}
		else
		{
			//this->print();
			//current.print();
			//greed.print();
			//std::cout << "Break" << std::endl;
			break;
		}
	}
	
	greed.add_address(path[pathLen - 1]);

	//assert(greed.size() == pathLen); // will fail if duplicate addresses are entered!
	return greed;
}
AddressList AddressList::opt2_route(bool tour)
{
	int N = size() - 1;
	if (N <= 0) return *this; //0-length case

	int start = 0;
	AddressList best = *this;

	start++;
	//N--;

	if (tour)
	{
		best.add_address(Address());
		N++;
	}

	
	AddressList current;
	bool restart = false;
	while (true)
	{
		for (int n = start; n < N; n++)
		{
			for (int m = start; m < n; m++)
			{
				current = best.reverse_route(m, n, tour);
				if (current.length() < best.length())
				{
					//std::cout << best.size() << "=b,c=" << current.size() << std::endl;
					//best.print();
					//current.print();
					assert(best.size() == current.size());
					best = current;
					restart = true;
					break;
				}
			}
			if (restart)
			{
				break;
			}
		}
		//std::cout << restart << std::endl;
		if (!restart)
		{
			break;
		}
		restart = false;
	}

	if (tour)
	{
		best.remove_last();
	}
	
	return best;
}

