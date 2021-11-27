// Tests every exercise.
//
#include <iostream>
#include <algorithm>
#include <vector>
#include "Options.h"
#include "Assertion.h"
#include "Address.h"
#include "AddressList.h"
#include <Route.h>
#include <Graph.h>
#include <random>
int main()
{
	std::cout << "****************" << std::endl;
	std::cout << "Testing start..." << std::endl;
	std::cout << "****************" << std::endl;
	
	std::cout << "Using precision to " << minprecision << " decimal places" << std::endl;

	// Note that this is only a moderate amount of testing (not really asserting every exception/etc...).

	std::cout << "\n--59.1--\n" << std::endl;
	Address a(1., 1.), b(2.,2.);
	a.print();

	#ifdef L1norm
		assertion(a.distance(b),2);
	#elif defined L2norm
		std::cout << a.distance(b) << std::endl;
		std::cout << sqrt(2) << std::endl;
		std::cout << a.distance(b) -sqrt(2) << std::endl;
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
		assertion(path.length(), 2*sqrt(2));

		std::cout << "--59.2.c--\n" << std::endl;
		assert(path.index_closest_to(c)==c);
		assert(path.index_closest_to(a)==b);
		path.print();

		std::cout << "\n--59.2.2--\n" << std::endl;
		Route tour;
		Address d;
		tour.print();
		tour.add_address(path);
		tour.print(); 
		assertion(tour.length(), 6*sqrt(2));
		tour.add_address(c);
		assertion(tour.length(), 6*sqrt(2));
		assert(tour.index_closest_to(c) == c);
		assert(tour.index_closest_to(a) == d);

		std::cout << "\n--59.3.a--\n" << std::endl;
		AddressList greedyPath = path.greedy_route();
		AddressList path2; path2.add_address(b); path2.add_address(c);
		assert(path2 == greedyPath);
		path.print();
		greedyPath.print();
		path2.add_address(d);
		path2.print();
		greedyPath = path2.greedy_route();
		assertion(greedyPath.length(), 3*sqrt(2));
		greedyPath.print();

		std::cout << "\n--59.3.b--\n" << std::endl;
		Route greedyTour = tour.greedy_route();
		assert(greedyTour == greedyPath);
		greedyTour.print();

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
		plt.plot(route,std::string("blue"));
		plt.show(false);
		plt.save("greedy-test");
		plt = Graph();

		std::cout << "\n--59.4.a--\n" << std::endl;
		AddressList rpath = path.reverse_route(1, 2);
		assertion(rpath.length(), sqrt(2));
		path.print();
		rpath.print();
		rpath = path.reverse_route(0, 2);
		rpath.print();
		path.add_address(d);
		rpath = path.reverse_route(1, 3);
		rpath.print();

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
		plt = Graph();
		
		std::cout << "\n--59.5--\n" << std::endl;
		
		std::string ans(" The runtime complexity: \n\
			We have a reverse_route method of O(n), \
			and call that method for all m<n options. \
			So we have approximately n^2/2 calls to reverse_route. \
			We also have a comparision and copy call, the comparision \
			being of order 2n and the (sometimes called) copy of order n. \
			Therefore the time complexity is ~ O(n^2/2)*O(n+n+2n) = O(2n^3) ~= O(n^3),\
			which is pretty bad. Note the memory complexity is clearly O(n), \
			since we only store a couple of AddressLists. ");
		std::cout << ans << std::endl;

		std::cout << "\n--59.6--\n" << std::endl;
		std::vector<Address> itinerary{ Address({0,1}), Address({2,0}), Address({3,1}),
				Address({2,4}), Address({-5,5}), Address({-2,4}), Address({3,3}) };
		std::mt19937 rng;
		Route r;
		for (int i = 0; i < 5; i++)
		{
			r.clear();
			std::shuffle(itinerary.begin(), itinerary.end(),rng);
			r.add_address(AddressList(itinerary));
			Route greed = r.greedy_route();
			Route opt = r.opt2_route();
			std::cout << opt.length() << std::endl;
			plt.plot(opt, std::string("blue"));
			plt.plot(greed, std::string("red"));
			plt.show(false);
			plt.save("greed_vs_opt_" + std::to_string(i));
			plt = Graph();
		}



	#endif
	//
	std::cout << "\n****************" << std::endl;
	std::cout << "Testing Success!" << std::endl;
	std::cout << "****************" << std::endl;

	return 0;
}