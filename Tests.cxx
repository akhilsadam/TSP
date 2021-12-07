// Tests every exercise.
//
#include <iostream>
#include <algorithm>
#include <vector>
#include <chrono>
using namespace std::chrono;
#include "Options.h"
#include "Assertion.h"
#include "Address.h"
#include "AddressList.h"
#include <Route.h>
#include <Graph.h>
#include <MultiplePath.h>
#include <random>
#include <matplot/matplot.h>
using namespace matplot;
using way = std::variant<AddressList, Route>;

//#define latest

int main()
{
	std::cout << "****************" << std::endl;
	std::cout << "Testing start..." << std::endl;
	std::cout << "****************" << std::endl;

	std::cout << "Using precision to " << minprecision << " decimal places" << std::endl;

	// Note that this is only a moderate amount of testing (not really asserting every exception/etc...).

	std::mt19937 rng;
	std::vector<double> optlen, greedlen;
	std::vector<double> optT, greedT;
	system_clock::time_point start;
	system_clock::time_point stop;

#ifndef latest

	std::cout << "\n--59.1--\n" << std::endl;
	Address a(1., 1.), b(2., 2.);
	a.print();

#ifdef L1norm
	assertion(a.distance(b), 2);
#elif defined L2norm
	assertion(a.distance(b), sqrt(2));
#elif defined maxnorm
	assertion(a.distance(b), 1);
#endif

	// note from now on tests will not test other distances;
	// we will assume that the distance is working as expected.
	// 
	// That is, we expect that no implementation specific bugs arise, since
	// we do not require anything of the distance property beyond that it returns a double.
	// 
	// Note we do not even require that distance is a metric 
	// (since we can have directional distances),
	// but we will require that the distance is a geodesic distance 
	// (a path of a set of a metric space), so that simulation works as intended.
	// 
	// We will assume that all implemented distances are geodesic (which is the case),
	// since that should cover all possible requirements on the distance function,
	// so there is no need for further testing.
	//
#ifdef L2norm

	std::cout << "\n--59.2.ab--" << std::endl;
	Address c(3., 3.);
	AddressList path;
	path.add_address(c);
	assertion(path.length(), 0);
	path.add_address(b);
	assertion(path.length(), sqrt(2));
	path.add_address(c);
	assertion(path.length(), 2 * sqrt(2));

	std::cout << "--59.2.c--\n" << std::endl;
	assert(path.index_closest_to(c) == c);
	assert(path.index_closest_to(a) == b);
	path.print();

	std::cout << "\n--59.2.2--\n" << std::endl;
	Route tour;
	Address d;
	tour.print();
	tour.add_address(path);
	tour.print();
	assertion(tour.length(), 6 * sqrt(2));
	tour.add_address(c);
	assertion(tour.length(), 6 * sqrt(2));
	assert(tour.index_closest_to(c) == c);
	assert(tour.index_closest_to(a) == d);

	std::cout << "\n--59.3.a--\n" << std::endl;
	path = AddressList ({ c,d,a,b });
	AddressList path2{ {c,a,d,b} };
	AddressList greedyPath = path.greedy_route();
	path.print();
	greedyPath.print();
	assert(path2 == greedyPath);
	path2.print();
	greedyPath = path2.greedy_route();
	assertion(greedyPath.length(), 5 * sqrt(2));
	greedyPath.print();

	std::cout << "\n--59.3.b--\n" << std::endl;
	Route greedyTour = tour.greedy_route();
	Route testTour{ {b,c} };
	tour.print();
	greedyTour.print();
	testTour.print();
	assert(greedyTour == testTour);
	

	std::cout << "\n--59.3ex--" << std::endl;
	std::cout << "TEST from book:\n" << std::endl;
	Route deliveries;
	deliveries.add_address(Address(0, 5));
	deliveries.add_address(Address(5, 0));
	deliveries.add_address(Address(5, 5));
	std::cerr << "Travel in order: " << deliveries.length() << "\n";
	// note this is not 5 as in the textbook since my implementation is different:
	// a graph should have size: N where N is the number of vertices (not size: N+1 = 5),
	// but rather size: N = 4. (Note the two notions are equivalent.)
	assert(deliveries.size() == 4);
	auto route = deliveries.greedy_route();
	assert(route.size() == 4);
	auto len = route.length();
	std::cerr << "Square route: " << route.display()
		<< "\n has length " << len << "\n";

	std::cout << "-- Test Plotting --" << std::endl;
	Graph plt;
	plt.plot(deliveries, std::string("red"));
	plt.plot(route, std::string("blue"));
	plt.show(false);
	plt.save("greedy-test");
	plt = Graph();

	std::cout << "\n--59.4.a--\n" << std::endl;
	path = AddressList( {c,b,c} );
	AddressList rpath = path.reverse_route(1, 2);
	assertion(rpath.length(), sqrt(2));
	path.print();
	rpath.print();
	rpath = path.reverse_route(0, 2);
	rpath.print();
	path.add_address(d);
	rpath = path.reverse_route(1, 3);
	rpath.print();

#endif

	std::cout << "\n--59.4.b--\n" << std::endl;
	path2.print();
	AddressList opt2path = path2.opt2_route();
	opt2path.print();


	std::cout << "\n--59.4.c--\n" << std::endl;
	Address e(0.75, 2.5), f(3.5, 4);
	route.add_address(a); route.add_address(e); route.add_address(f);
	route.print();
	bool exception_thrown = true;
	AddressList r_route = route.reverse_route(0, 2, false);
	r_route.print();

	//note that the above behavior is expected and will properly throw an error if
	// false is changed to true (no way to unit-test this behavior without 
	// using some other assertion mechanism (as far as I know, assert is a c-function, 
	// so using a try-catch block does not catch the exception).

	//note assuming no location needs to be visited twice...
	//note depot CANNOT be in the address list!

	std::cout << "\n--59.4.d--\n" << std::endl;
	Route opt2route = route.opt2_route();
	plt.plot(route, std::string("red"));
	plt.plot(opt2route, std::string("blue"));
	plt.show(false);
	plt.save("2-opt-test");
	plt = Graph();

	plt.plot(deliveries, std::string("red"));
	opt2route = deliveries.opt2_route();
	plt.plot(opt2route, std::string("blue"));
	plt.show(false);
	plt.save("2-opt-test_2");
	opt2route.print();
	plt = Graph();

	std::cout << "\n--59.5--\n" << std::endl;

	std::string ans(" The runtime complexity: \n\
			We have a reverse_route method of O(n), \
			and call that method for all m<n options. \
			So we have approximately n^2/2 calls to reverse_route, at minimum, \
			and approximately n^3/2 calls at maximum (due to the while loop). \
			We also have a comparision and copy call, the comparision \
			being of order 2n and the (sometimes called) copy of order n. \
			So we have that the time complexity is bounded between O(n^3) and O(n^4): \
			 O(n^2/2)*O(n+n+2n) <= TC <= O(n^2/2)*O(n+n+2n) \
			==> O(n^3) <= TC <= O(n^4), \
			which is pretty bad. Note the memory complexity is clearly O(n), \
			since we only store a couple of AddressLists. \
			Comparing to the greedy method, that has a runtime complexity of under O(n^3), as implemented,\
			and may be of O(n^2 log n).\n\n\
			Note the removeDuplicates method is just\
			an input sanitizer (of about O(n^2/2)). ");
	std::cout << ans << std::endl;

	std::cout << "\n--59.6--\n" << std::endl;
	std::vector<Address> walk{ Address({0,1}), Address({2,0}), Address({3,1}),
			Address({2,4}), Address({-5,5}), Address({-2,4}), Address({3,3}),
			Address({2,8}), Address({-3.5,5}), Address({-4.2,4}), Address({1.5,3}),
			Address({-6.2,4}), Address({1.5,-8}), Address({-2,-2}),
			Address({3.2,4}), Address({1.4,-8}), Address({-1,-2}) };

	// no need to loop over greedlen as it is deterministic.
	Route r;

	for (int i = 0; i < loopMax; i++)
	{
		r.clear();
		std::shuffle(walk.begin(), walk.end(), rng);
		r.add_address(AddressList(walk));

		start = high_resolution_clock::now();
		Route opt = r.opt2_route();
		stop = high_resolution_clock::now();
		optlen.push_back(opt.length());
		optT.push_back(duration_cast<milliseconds>(stop - start).count());
		//opt.print();

		start = high_resolution_clock::now();
		Route greed = r.greedy_route();
		stop = high_resolution_clock::now();
		greedT.push_back(duration_cast<milliseconds>(stop - start).count());
		greedlen.push_back(greed.length());

		plt.plot(opt, std::string("blue"));
		plt.plot(greed, std::string("red"), 1);
		plt.show(false);
		plt.save("greed_vs_opt2_" + std::to_string(i));
		plt = Graph();
	}

	{
		auto h1 = hist(optlen);
		hold(on);
		auto h2 = hist(greedlen);
		//h1->normalization(histogram::normalization::probability);
		h1->bin_width(0.25);
		h1->face_alpha(0.5);
		//h2->normalization(histogram::normalization::probability);
		h2->bin_width(0.25);
		h2->face_alpha(0.5);
		title("Length Histogram for greedy,opt2 algorithms");
		xlabel("Length (opt2 is blue)");
		ylabel("Counts");
		save("img/greed_vs_opt2_hist.jpg");
		hold(off); cla();
	}

	{
		auto h3 = hist(optT); h3->num_bins(50);
		hold(on);
		auto h4 = hist(greedT); h4->num_bins(50);
		//h1->normalization(histogram::normalization::probability);
		h3->bin_width(0.25);
		h3->face_alpha(0.5);
		//h2->normalization(histogram::normalization::probability);
		h4->bin_width(0.25);
		h4->face_alpha(0.5);
		title("Time Histogram for greedy,opt2 algorithms");
		xlabel("Time [ms] (opt2 is blue)");
		ylabel("Counts");
		save("img/greed_vs_opt2_histT.jpg");
		hold(off); cla();
	}

	auto optS = scatter(optT, optlen, 8);
	optS->marker_face_color({ .5f, .7f, 1.f });
	optS->marker_color({ .5f, .7f, 1.f });
	optS->marker_style(line_spec::marker_style::circle);
	hold(on);
	auto greedS = scatter(greedT, greedlen, 8);
	greedS->marker_face_color({ 1.f, .3f, .2f });
	greedS->marker_color({ 1.f, .3f, .2f });
	greedS->marker_style(line_spec::marker_style::circle);
	title("Length vs Time for greedy,opt2 algorithms");
	xlabel("Time [us] (opt2 is blue)");
	ylabel("Length");
	save("img/greed_vs_opt2_LT.jpg");
	hold(off); cla();

#endif

	std::cout << "\n--59.7--\n" << std::endl;
	std::vector<way> itinerary{ AddressList({Address({0,1}), Address({2,0}), Address({3,1}),
			Address({2,4}), Address({-5,5}), Address({-2,4}), Address({3,3}),
			Address({2,8})}),
		AddressList({Address({-4.2,4}), Address({1.5,3}),
			Address({-6.2,4}), Address({1.5,-8}), Address({-2,-2}),
			Address({3.2,4}), Address({1.4,-8}), Address({-1,-2}) }),
		AddressList({Address({-8,-4.2}), Address({1.5,3.5}), Address({4.5,7.5})}) };
		//AddressList({Address({-8,-4.2}), Address({1.5,3.5}), Address({4.5,7.5})}),
		//AddressList({Address({2,2}), Address({3,3}), Address({1,1}),Address({8,8})}) };
		/*Route({Address({1,2}), Address({2,3}), Address({3,4}),Address({3,2})}) };*/
	MultiplePath system{ itinerary };
	system.plot("system");
	//system.print();
	MultiplePath solvedopt = system.opt2_heuristic();
	solvedopt.plot("system_solved_opt2");
	MultiplePath solvedgreed = system.opt2_heuristic(false);
	solvedgreed.plot("system_solved_greedy");

	std::cout << "\n--59.7.b--\n" << std::endl;
	itinerary = std::vector<way>({ Route({Address({0,1}), Address({2,0}), Address({3,1}),
			Address({2,4}), Address({-5,5}), Address({-2,4}), Address({3,3}),
			Address({2,8})}),
		Route({Address({-4.2,4}), Address({1.5,3}),
			Address({-6.2,4}), Address({1.5,-8}), Address({-2,-2}),
			Address({3.2,4}), Address({1.4,-8}), Address({-1,-2}) }),
		Route({Address({-8,-4.2}), Address({1.5,3.5}), Address({-7,-4.6})}) });
	//Route({Address({1,2}), Address({2,3}), Address({3,4}),Address({3,2})}) };

	system = MultiplePath(itinerary);
	system.plot("system_route");
	system.print();
	optlen.clear(); optT.clear();
	greedlen.clear(); greedT.clear();

	for (int i = 0; i < loopMax; i++)
	{
		
		std::shuffle(itinerary.begin(), itinerary.end(), rng);
		system = MultiplePath(itinerary);

		start = high_resolution_clock::now();
		solvedopt = system.opt2_heuristic();
		stop = high_resolution_clock::now();
		optlen.push_back(solvedopt.length());
		optT.push_back(duration_cast<milliseconds>(stop - start).count());

		start = high_resolution_clock::now();
		solvedgreed = system.opt2_heuristic(false);
		stop = high_resolution_clock::now();
		greedT.push_back(duration_cast<milliseconds>(stop - start).count());
		greedlen.push_back(solvedgreed.length());

		solvedopt.plot("system_solved_opt2_route_" + std::to_string(i));
		solvedgreed.plot("system_solved_greedy_route_" + std::to_string(i));
	}
	/*

	auto optS = scatter(optT, optlen, 8);
	//optS->marker_face_color({ 0.2f, .6f, 1.f });
	optS->marker_face_color({ .5f, .7f, 1.f });
	optS->marker_color({ .5f, .7f, 1.f });
	optS->marker_style(line_spec::marker_style::circle);
	hold(on);
	auto greedS = scatter(greedT, greedlen, 8);
	//greedS->marker_face_color({ 1.f, .3f, .3f });
	greedS->marker_face_color({ 1.f, .3f, .2f });
	greedS->marker_color({ 1.f, .3f, .2f });
	greedS->marker_style(line_spec::marker_style::circle);
	title("Length vs Time for greedy,opt2 algorithms");
	xlabel("Time [us] (opt2 is blue)");
	ylabel("Length");
	save("img/greed_vs_opt2_M_LT.jpg");
	hold(off); cla();
	*/
	//
	std::cout << "\n****************" << std::endl;
	std::cout << "Testing Success!" << std::endl;
	std::cout << "****************" << std::endl;

	return 0;
}