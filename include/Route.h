#pragma once
#include "Address.h"
#include "AddressList.h"
#include <iostream>
class Route : public AddressList
{
private:
	Address depot = Address();
public:
	Route();
	Route(std::vector<Address> in_path);
	void add_address(Address a);
	void add_address(AddressList a);
	double length(int n = -1) override;
	std::string display();
	void print();
	Route greedy_route();
	Route opt2_route();
};