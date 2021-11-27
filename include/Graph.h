#pragma once
#include <cmath>
#include <matplot/matplot.h>
#include "Options.h"
#include "AddressList.h"
#include "Route.h"
using namespace matplot;
class Graph
{
private:
	matplot::figure_handle f;
	matplot::axes_handle ax;
	//matplot::network_handle g;
	//matplot::axes_object_handle pl;
	std::vector<std::pair<size_t, size_t>> edges;
	std::vector<double> x;
	std::vector<double> y;
	std::vector<std::string> cs;
	std::vector<std::string> display;
	double origin;
	int vertexID;
	int depotID;

	void update(std::vector<std::pair<size_t, size_t>>& e, std::vector<double>& xs, std::vector<double>& ys,std::string c);
	std::vector<std::pair<size_t, size_t>> updateEdges(AddressList& a, bool tour = false);
	void finalize();
	void makeLegend();
	std::string disp(AddressList& a, std::string c);
	std::string disp(Route& a, std::string c);
public:
	Graph();
	void plot(AddressList& a, std::string c, bool tour = false);
	void plot(Route& a, std::string c);
	//void wipe();
	void show(bool screen = true);
	void save(std::string filename = "plot");
};