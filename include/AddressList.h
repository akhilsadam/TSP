#pragma once
#include <vector>
#include "Address.h"
#include <optional>
class AddressList
{
private:
	std::vector<Address> path;
	std::optional<Address> index_closest_to(Address a, AddressList& mask, bool masking = false);
	void remove_last();
	
public:
	AddressList();
	AddressList(std::vector<Address> in_path);

	int size() const;
	std::vector<Address>::iterator begin();
	std::vector<Address>::iterator end();
	std::vector<Address>::reverse_iterator rbegin();
	std::vector<Address>::reverse_iterator rend();
	void clear();
	const Address& operator[](int i) const;

	bool operator==(const AddressList& rhs);

	void add_address(Address a);
	double length();
	Address index_closest_to(Address a);

	Address last();
	bool in_path(Address a);
	std::string display();
	void print();

	std::vector<double> x(double shift = 0);
	std::vector<double> y(double shift = 0);

	AddressList reverse_route(int m,int n,bool tour = false);

	AddressList greedy_route();
	AddressList opt2_route(bool tour = false);
};