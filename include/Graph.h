#pragma once
#include <cmath>
#include <matplot/matplot.h>
#include <TextFlow.h>
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
	std::vector<double> x = { 0 };
	std::vector<double> y = { 0 };
	std::vector<std::string> cs;
	std::vector<int> lw;
	std::vector<std::string> display;
	double origin;
	int vertexID;
	int depotID;
	int maxspace = 2;

	void update(std::vector<std::pair<size_t, size_t>>& e, std::vector<double>& xs, std::vector<double>& ys,std::string c, int linewidth = 2, bool tour = false);
	std::vector<std::pair<size_t, size_t>> updateEdges(AddressList& a, bool tour = false);
	void finalize();
	void makeLegend();

public:
	Graph();
	void plot(AddressList& a, std::string c, int linewidth = 2, bool tour = false);
	void plot(Route& a, std::string c, int linewidth = 2);
	//void wipe();
	void show(bool screen = true);
	void save(std::string filename = "plot");
	static void printV(std::vector<double> v);
};