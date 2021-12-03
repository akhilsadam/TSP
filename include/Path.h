#pragma once
#include <vector>
#include "Address.h"
#include <optional>
#include <Options.h>
#include <Address.h>
// abstract-ish base class for AddressList (paths) and Route (tours)
// --unfortunately I cannot make only virtual methods without implementation (like an interface) 
//   due to g++ restrictions (it needs at least one virtual method with a default implementation)
// probably ought to move more methods back to this class...
class Path
{
protected:
	std::vector<Address> path;
	virtual std::optional<Address> index_closest_to(Address a, Path& mask, bool masking = false);
	void remove_last();
public:
	Path();
	Path(std::vector<Address> in_path);

	std::string disp(std::string c);
	virtual std::string display();
	
	int size() const;
	std::vector<Address>::iterator begin();
	std::vector<Address>::iterator end();
	std::vector<Address>::reverse_iterator rbegin();
	std::vector<Address>::reverse_iterator rend();
	void insert(int position, const Address a);
	void insert(std::vector<Address>::iterator position, std::vector<Address>::iterator first, std::vector<Address>::iterator last);
	void erase(std::vector<Address>::iterator first, std::vector<Address>::iterator last);
	void clear();

	bool in_path(Address a);

	void add_address(Address a);
	virtual double length();
	Address index_closest_to(Address a);

	void set(int i, Address a);

	const Address& operator[](int i) const;
	bool operator==(const Path& rhs);
};