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
	void add_address(Address a);
	void add_address(AddressList a);
	double length() override;
	std::string display();
	void print();
	Route greedy_route();
	Route opt2_route();
};