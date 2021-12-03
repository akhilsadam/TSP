#include <Options.h>
#include <vector>
#include "Address.h"
#include "Path.h"
#include "AddressList.h"
#include "Route.h"
#include "Graph.h"
#include <variant>
#include <MultiplePath.h>
#include <Way.h>
#include <ostream>
using way = std::variant<AddressList, Route>;

MultiplePath::MultiplePath() {}
MultiplePath::MultiplePath(std::vector<AddressList> in_paths)
{
	for (auto path : in_paths)
	{
		schedule.push_back(path);
	}
}
MultiplePath::MultiplePath(std::vector<Route> in_paths)
{
	for (auto path : in_paths)
	{
		schedule.push_back(path);
	}
}
MultiplePath::MultiplePath(std::vector<way> in_paths): schedule(in_paths)
{
	if (schedule.size() > colors.size())
	{
		std::cout << "[ERROR]: Cannot handle so many paths in MultiplePath object" << std::endl;
		throw("");
	}
}
int MultiplePath::size()
{
	return schedule.size();
}
int MultiplePath::n_deliveries()
{
	int n = 0;
	for (auto path : schedule)
	{
		n += Way::size(path);
	}
	return n;
}
double MultiplePath::length()
{
	double sum{0};
	for (auto path : schedule)
	{
		sum += Way::length(path);
	}
	return sum;
}
way& MultiplePath::operator[](int i)
{
	return schedule[i];
}
void MultiplePath::swap(int p1, int p2, int v1, int v2)
{
	int npaths = size();
	assert(p1 >= 0); assert(p1 < npaths);
	assert(p2 >= 0); assert(p2 < npaths);
	assert(v1 >= 0); assert(v1 < Way::size(schedule[p1]));
	assert(v2 >= 0); assert(v2 < Way::size(schedule[p2]));

	way w1 = schedule[p1];
	way w2 = schedule[p2];

	Address a1 = Way::get(w1,v1);
	Address a2 = Way::get(w2,v2);

	Way::set(w1,v1,a2);
	Way::set(w2,v2,a1);
}
void MultiplePath::swap(int p1, int p2, int v11, int v12, int v21, int v22)
{
	int npaths = size();
	assert(p1 >= 0); assert(p1 < npaths);
	assert(p2 >= 0); assert(p2 < npaths);
	assert(v11 >= 0); assert(v11 < Way::size(schedule[p1]));
	assert(v21 >= 0); assert(v21 < Way::size(schedule[p2]));
	assert(v12 > v11); assert(v12 < Way::size(schedule[p1]));
	assert(v22 > v21); assert(v22 < Way::size(schedule[p2]));

	int l1 = v12 - v11;
	int l2 = v22 - v21;

	way w1 = schedule[p1];
	way w2 = schedule[p2];
	// cross-insertion
	//way w3 = Way::insert(w2, Way::begin(w2) + v21, Way::begin(w1) + v11, Way::begin(w1) + v12);
	w2 = Way::insert(w1,w2,v21,v11,v12);
	//w1 = Way::insert(w1, Way::begin(w1) + v11, Way::begin(w2) + v21 + l1, Way::begin(w2) + v22 + l1);
	w1 = Way::insert(w2,w1,v11,v21 + l1,v22 + l1);

	// erasure
	/*w1 = Way::erase(w1, Way::begin(w1) + v11 + l2, Way::begin(w1) + v12 + l2);*/
	w1 = Way::erase(w1, v11 + l2, v12 + l2);
	/*w2 = Way::erase(w2, Way::begin(w2) + v21 + l1, Way::begin(w2) + v22 + l1);*/
	w2 = Way::erase(w2, v21 + l1, v22 + l1);
	// replacement
	schedule[p1] = w1;
	schedule[p2] = w2;
}
MultiplePath MultiplePath::swapAndOptimize(int p1, int p2, int v11, int v12, int v21, int v22, bool opt2)
{
	MultiplePath newPathSet = *this;
	newPathSet.swap(p1, p2, v11, v12, v21, v22);

	way w1 = newPathSet[p1];
	way w2 = newPathSet[p2];

	if (opt2) 
	{
		newPathSet[p1] = Way::opt2(w1);
		newPathSet[p2] = Way::opt2(w2);
	}
	else
	{
		newPathSet[p1] = Way::greedy(w1);
		newPathSet[p2] = Way::greedy(w2);
	}

	return newPathSet;
}
MultiplePath MultiplePath::opt2_heuristic(bool opt2)
{
	int start{ 0 }; //assuming cannot change start and end(since the depot position is fixed)
	MultiplePath best = *this;
	int npath = size();
	int N, M;


	MultiplePath current;
	bool restart = false;
	while (true)
	{
		for (int i = 0; i < npath-1; i++) // first path
		{
			for (int j = i + 1; j < npath; j++) // second path
			{
				N = Way::size(best.schedule[i]);
				M = Way::size(best.schedule[j]);

				for (int v11 = start; v11 < N - 1; v11++) // first index, first path
				{
					for (int v12 = v11 + 1; v12 < N; v12++) // second index, first path
					{
						for (int v21 = start; v21 < M - 1; v21++) // first index, second path
						{
							for (int v22 = v21 + 1; v22 < M; v22++) // second index, second path
							{
								//std::cout << "N: " << N << " M:" << M << " v12:" << v12 << " v22:" << v22 << std::endl;
								current = best.swapAndOptimize(i, j, v11, v12, v21, v22, opt2);
								if (current.length() < best.length())
								{
									best = current;
									restart = true;
									std::cout << best.n_deliveries() << " ";
									break;
								}
							}
							if (restart) break;
						}
						if (restart) break;
					}
					if (restart) break;
				}
				if (restart) break;
			}
			if (restart) break;
		}
		//std::cout << restart << std::endl;
		if (!restart)
		{
			break;
		}
		restart = false;
	}

	return best;
}
void MultiplePath::plot(std::string name)
{
	Graph plt;
	for (int i = 0; i<schedule.size();i++)
	{
		auto path = schedule[i];
		if (path.index() == 0)
		{
			plt.plot(std::get<AddressList>(path), colors[i]);
		}
		else
		{
			plt.plot(std::get<Route>(path), colors[i],true);
		}
	}
	plt.show(false);
	plt.save(name);
};


