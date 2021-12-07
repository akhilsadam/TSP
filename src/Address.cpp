#include <math.h>
#include "Address.h"
#include "Options.h"
#include <iostream>
#include <string>


std::string Address::to_string(double a)
{
	return std::to_string(a).substr(0, std::to_string(printDigits).find(".") + printDecimalDigits + 1);
}

Address::Address() : i(0), j(0) {};

Address::Address(double i, double j) : i(i), j(j) {}
Address::Address(double i, double j, bool prime) : i(i), j(j), prime(prime) {}

double Address::distance(Address a)
{
	#ifdef L1norm
		return fabs(a.i - i) + fabs(a.j - j);
	#elif defined L2norm
		return sqrt(powf(a.i - i,2) + powf(a.j - j,2));
	#elif defined maxnorm
	return max(fabs(a.i - i), fabs(a.j - j));
	#endif
}

std::string Address::display()
{
	if (prime)	return "(" + Address::to_string(i) + "," + Address::to_string(j) + ",Prime)";
	return "(" + Address::to_string(i) + "," + Address::to_string(j) + ")";
}

void Address::print()
{
	std::cout << "Address:" << display() << std::endl;
}
bool Address::operator==(const Address& rhs) const
{
	return (i==rhs.i) && (j==rhs.j);
}
bool Address::operator!=(const Address& rhs) const
{
	return !Address::operator==(rhs);
}
const double Address::x()
{
	return i;
}
const double Address::y()
{
	return j;
}
const bool Address::isPrime()
{
	return prime;
}

void Address::makePrime()
{
	prime = true;
}
