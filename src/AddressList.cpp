#include "AddressList.h"
#include <iostream>
#include <optional>
#include <cassert>

AddressList::AddressList() {}
AddressList::AddressList(std::vector<Address> in_path) : path(in_path) {}

std::vector<Address>::iterator AddressList::begin() {
	return path.begin();
}

std::vector<Address>::iterator AddressList::end() {
	return path.end();
}
std::vector<Address>::reverse_iterator AddressList::rbegin()
{
	return path.rbegin();
}
std::vector<Address>::reverse_iterator AddressList::rend()
{
	return path.rend();
}

void AddressList::clear()
{
	path.clear();
}

void AddressList::add_address(Address a) 
{
	path.push_back(a);
}
void AddressList::remove_last()
{
	path.pop_back();
}
double AddressList::length()
{
	double len = 0.0;
	for (int i=0; i < std::size(path)-1;i++)
	{
		len += path[i].distance(path[i + 1]);
	}
	return len;
}
Address AddressList::index_closest_to(Address a)
{
	std::optional<Address> opt = AddressList::index_closest_to(a,*this);
	if (opt.has_value())
	{
		return opt.value();
	}
	else
	{
		std::cout << "[ERROR] Empty path must have no vertices!" << std::endl;
		throw("");
		return Address();
	}
}
std::optional<Address> AddressList::index_closest_to(Address a, AddressList& blacklist, bool masking)
{

	// mask setup to ignore addresses in blacklist
	// expensive operation, so separating like this.
	bool mask = true; // per Address mask
	bool maskNotEmpty = false; // make sure mask was used atleast once!

	int start = 0;
	int lengthT = std::size(path)-start;
	if (lengthT < 1)
	{
		std::cout << "[ERROR]: Empty path has no vertices!" << std::endl;
		throw("");
	}
	if (lengthT == 1)
	{
		if (masking)
		{
			if (!blacklist.in_path(path[start]))
				return path[start];
			return {};
		}
		else return path[start];
	}
	// min_distance = -1 coupled with the division prevents another comparision for
	// the initial case. (Setting it to a.distance(path[0]) will not work with the masking
	// when only one unmasked variable remains in path.)

	double min_distance = -1; 
	double ith_distance;
	int j = start;
	for (int i = start; i < lengthT; i++)
	{
		if (masking)
		{
			mask = !blacklist.in_path(path[i]);
		}
		if(mask)
		{ 
			ith_distance = a.distance(path[i]);
			if (ith_distance/min_distance < 1)
			{
				maskNotEmpty = true;
				min_distance = ith_distance;
				j = i;
			}
		}
	}

	if (maskNotEmpty)
	{
		return path[j];
	}
	else return {};
}

Address AddressList::last()
{
	if (std::size(path) == 0)
	{
		std::cout << "[ERROR] No last element in path of 0 length." << std::endl;
		throw("");
	}
	return path[std::size(path)-1];
}
bool AddressList::in_path(Address a)
{
	for (auto b : path)
	{
		if (b == a)
		{
			return true;
		}
	}
	return false;
}
const Address& AddressList::operator[](int i) const
{
	// no bounds checking (for speed, but user must be cautious).
	return path[i];
}
bool AddressList::operator==(const AddressList& rhs)
{
	if (size() != rhs.size())
		return false;
	for (int i = 0; i < size(); i++)
	{
		if (path[i] != rhs[i])
			return false;
	}
	return true;
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

int AddressList::size() const
{
	return std::size(path);
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
	Address current(0, 0);
	AddressList greed;
	std::optional<Address> opt;
	greed.add_address(index_closest_to(current));
	for(int i = 0; i < pathLen; i++)
	{
		opt = index_closest_to(current, greed, true);
		if (opt.has_value())
		{
			current = opt.value();
			//std::cout << opt.value().display();
			greed.add_address(current);
		}
		else
		{
			break;
		}
	}
	return greed;
}
AddressList AddressList::opt2_route(bool tour)
{
	int start = 0;
	AddressList best = *this;
	int N = size() - 1;

	if (tour)
	{
		best.add_address(Address());
		start++;
	}

	
	AddressList current;
	bool restart = false;
	while (true)
	{
		for (int n = start; n <= N; n++)
		{
			for (int m = start; m < n; m++)
			{
				current = best.reverse_route(m, n, tour);
				if (current.length() < best.length())
				{
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

