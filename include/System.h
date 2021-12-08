#pragma once
#include <Options.h>
#include <vector>
#include "Address.h"
#include "Path.h"
#include "AddressList.h"
#include "Route.h"
#include "Graph.h"
#include <variant>
#include <Way.h>
#include <random>
#include <MultiplePath.h>
using way = std::variant<AddressList, Route>;

class System : public MultiplePath
{
private:
	int nVehicle;
	std::vector<Address> currentPositions;
	bool opt2;
	double speed; // units are in distance-units per hour
	double time;  // units in hours

	void lapseTime(double hours);
public:
	System(std::vector<way> itinerary, bool opt2 = true, double speed = 10);

	void lapse(double hours,bool optimize = true);
	void lapse(double hours,int n, std::mt19937 rng_mt);
	void cleanPaths();
	void addRandom(int n, std::mt19937 rng_mt);
	void heuristic(bool opt2);
	double getTime();
};