#include "Address.h"
#include "AddressList.h"
#include "Route.h"
#include <optional>

Route::Route()
{
	add_address(depot);
}
Route::Route(std::vector<Address> in_path)
{
	add_address(depot);
	for (auto a : in_path)
	{
		add_address(a);
	}
}
void Route::add_address(Address a)
{
	if (in_path(a))
	{
		//std::cout << "[WARNING] vertex " << a.display() << " pre - existing in tour." << std::endl;
		return;
	}
	else
	{
		AddressList::add_address(a);
	}
}

void Route::add_address(AddressList a)
{
	for (Address e : a)
	{
		Route::add_address(e);
	};
}

double Route::length(int n)
{
	if ((n==-1) || (n > size()))
	{
		std::optional<Address> last = AddressList::last();
		if (last.has_value())
		{
			return AddressList::length() + last.value().distance(depot);
		}
		return AddressList::length();		
	}
	return AddressList::length(n);
}

std::string Route::display()
{
	return AddressList::display() + depot.display();
}

void Route::print()
{
	std::cout << "TOUR:[" << display() << "]" << std::endl;
}
Route Route::greedy_route()
{
	AddressList temp{ *this };
	temp.add_address(depot);
	AddressList greedypath = temp.greedy_route();
	Route greedytour;
	greedytour.add_address(greedypath);
	return greedytour;
};
Route Route::opt2_route()
{
	
	AddressList opt2path = AddressList(*this).opt2_route(true);
	Route opt2tour;
	opt2tour.add_address(opt2path);
	return opt2tour;
};
