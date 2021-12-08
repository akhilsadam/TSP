#include <System.h>
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

System::System(std::vector<way> itinerary, bool opt2, double speed):
	MultiplePath(itinerary), speed(speed), opt2(opt2)
{
	nVehicle = size();
	for (int i = 0; i < nVehicle; i++)
	{
		currentPositions.push_back(Address());
	}
	cleanPaths();
	heuristic(opt2);
	time = 0;
}

void System::lapseTime(double hours)
{
	for (int v = 0; v < nVehicle; v++)
	{
		auto w0 = schedule[v];
		if (!w0.valueless_by_exception())
		{
			//std::cout << "222 get variant" << std::endl;
			auto w = Way::lapse(w0, speed * hours);
			//std::cout << "333 get variant" << std::endl;
			schedule[v] = w;
			//std::cout << "444 get variant" << std::endl;
		}
	}
	time += hours;
}

void System::lapse(double hours, int n, std::mt19937 rng_mt)
{
	lapseTime(hours);
	cleanPaths();
	addRandom(n, rng_mt);
	heuristic(opt2);
}

void System::lapse(double hours, bool optimize)
{
	lapseTime(hours);
	if (optimize)
	{
		cleanPaths();
		heuristic(opt2);
	}
}

double System::getTime()
{
	return time;
}

void System::cleanPaths()
{
	for (int i = 0; i < size(); i++)
	{
		way w = schedule[i];
		if (!w.valueless_by_exception()) {
			if (Way::size(w) == 0)
			{
				// make sure that path is not zero so can be optimized with others.
				Address x;
				auto w1 = Way::append(w, x);
				schedule[i] = Way::append(w1, x);
				//schedule.erase(schedule.begin() + i);
				//i--;
				/*std::cout << "000 get variant" << std::endl;*/
			}
		}
		else
		{
			schedule[i] = AddressList();
			//break;
		}
	}
}

void System::addRandom(int n, std::mt19937 rng_mt)
{
	// https://stackoverflow.com/questions/45069219/how-to-succinctly-portably-and-thoroughly-seed-the-mt19937-prng
// danger -- should really keep the above in mind, but am currently not bothering as this is 
// definitely not optimal enough to run so many times that we need more than 32bytes of information as a seed.
	std::uniform_real_distribution<> dist(-15.0, 15.0); // default range for x,y
	std::uniform_int_distribution<> index(0,schedule.size()-1); //assign to random path
	std::random_device rd;
	rng_mt.seed(rd());
	for (int i = 0; i < n; i++)
	{	
		Address x{ dist(rng_mt),dist(rng_mt) };
		int k = index(rng_mt);
		way w = schedule[k];
		schedule[k] = Way::insert(w,0, x);
		//x.print();
	}
}
void System::heuristic(bool opt2)
{
	MultiplePath mp = MultiplePath::opt2_heuristic(opt2);
	schedule = mp.getSchedule();
}

