#pragma once
#include <vector>
#include "Address.h"
#include "Path.h"
#include <optional>
class AddressList : public Path
{
public:
	AddressList();
	AddressList(std::vector<Address> in_path);

	std::optional<Address> last();
	std::string display() override;
	void print();

	std::vector<double> x(double shift = 0);
	std::vector<double> y(double shift = 0);

	AddressList reverse_route(int m,int n,bool tour = false);

	AddressList greedy_route();
	AddressList opt2_route(bool tour = false);

};