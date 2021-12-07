#pragma once
#include <string>
class Address
{
private:
	double i, j;
public:
	Address();
	Address(double i, double j);
	double distance(Address a);
	std::string display();
	void print();

	bool operator==(const Address& rhs) const;
	bool operator!=(const Address& rhs) const;
	const double x();
	const double y();

	static std::string to_string(double a); // should really be in a utils class
};