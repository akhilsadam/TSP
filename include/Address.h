#pragma once
#include <string>
class Address
{
private:
	double i, j;
	bool prime=false;
public:
	Address();
	Address(double i, double j);
	Address(double i, double j,bool prime);

	double distance(Address a);
	std::string display();
	void print();

	bool operator==(const Address& rhs) const;
	bool operator!=(const Address& rhs) const;
	const double x();
	const double y();
	const bool isPrime();

	void makePrime();

	static std::string to_string(double a); // should really be in a utils class
};