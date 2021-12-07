#include <optional>
#include <iostream>
#include <ostream>
#include <Options.h>
#include <Path.h>
#include <Address.h>
#include <cassert>
#include <vector>
#include <algorithm>

Path::Path(){}
Path::Path(std::vector<Address> in_path): path(in_path){}
std::string Path::disp(std::string c)
{
    std::string out;
    out = display();


    if (out.length() > textwidth)
    {
        out = "...";
    }
    return c + ":" + out + " L = " + std::to_string(length());
}

std::string Path::display()
{
	return "";
}

int Path::size() const
{
	return path.size();
}

double Path::length()
{
	double len = 0.0;
	int L = path.size();
	if (L == 0) return len;
	for (int i = 0; i < L - 1; i++)
	{
		len += path[i].distance(path[i + 1]);
	}
	return len;
}

std::vector<Address>::iterator Path::begin() {
	return path.begin();
}

std::vector<Address>::iterator Path::end() {
	return path.end();
}
std::vector<Address>::reverse_iterator Path::rbegin()
{
	return path.rbegin();
}
std::vector<Address>::reverse_iterator Path::rend()
{
	return path.rend();
}

void Path::insert(int position, const Address a)
{
	path.insert(path.begin()+position, a);
}

void Path::insert(std::vector<Address>::iterator position, std::vector<Address>::iterator first, std::vector<Address>::iterator last)
{
	path.insert(position, first, last);
}

void Path::erase(std::vector<Address>::iterator first, std::vector<Address>::iterator last)
{
	path.erase(first, last);
}

void Path::erase(std::vector<Address>::iterator first)
{
	path.erase(first,first+1);
}

void Path::clear()
{
	path.clear();
}

bool Path::in_path(Address a)
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

void Path::add_address(Address a)
{
	path.push_back(a);
}
void Path::remove_last()
{
	path.pop_back();
}

Address Path::index_closest_to(Address a)
{
	std::optional<Address> opt = Path::index_closest_to(a, *this);
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
void Path::removeDuplicates()
{
	auto last = std::unique(path.begin(), path.end());
	path.erase(last, path.end());
}
void Path::set(int i, Address a)
{
	assert(i >= 0); assert(i < path.size());
	path[i] = a;
}
std::optional<Address> Path::index_closest_to(Address a, Path& blacklist, bool masking,bool notEndpoint)
{

	// mask setup to ignore addresses in blacklist
	// expensive operation, so separating like this.
	bool mask = true; // per Address mask
	bool maskNotEmpty = false; // make sure mask was used atleast once!

	int start = 0;
	int lengthT = path.size() - start;
	//std::cout << lengthT << std::endl;


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

	if (notEndpoint) // Not including start or endpoint.
	{
		//start++;
		lengthT--;
	}

	//std::cout << lengthT << std::endl;
	int j = start;
	//std::cout << start << "," << lengthT << std::endl;
	for (int i = start; i < lengthT; i++)
	{
		if (masking)
		{
			mask = !blacklist.in_path(path[i]);
			//std::cout << " m " << mask << std::endl;
			//std::cout << " bl " << blacklist.display() << std::endl;
		}
		if (mask)
		{
			ith_distance = a.distance(path[i]);
			//std::cout << " d " << ith_distance << std::endl;
			if (ith_distance / min_distance < 1)
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

const Address& Path::operator[](int i) const
{
	// no bounds checking (for speed, but user must be cautious).
	return path[i];
}
bool Path::operator==(const Path& rhs)
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