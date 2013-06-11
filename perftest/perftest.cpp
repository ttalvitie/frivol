#include <frivol/frivol.hpp>
#include <frivol/containers/priority_queues/dummy_priority_queue.hpp>
#include <frivol/containers/search_trees/dummy_search_tree.hpp>
#include <fstream>
#include <random>
#include <chrono>
#include <iostream>

// Return the number of seconds it takes to run 'func'. Runs 'func' repeatedly
// until 'time' seconds has elapsed.
template <typename FuncT>
double getExecutionTime(FuncT func, double time) {
	auto start = std::chrono::high_resolution_clock::now();
	auto elapsed = 0;
	int times = 0;
	while(elapsed < time) {
		func();
		++times;
		auto now = std::chrono::high_resolution_clock::now();
		elapsed =
			10e-6 * std::chrono::duration_cast<std::chrono::microseconds>(now - start).count();
	}
	return elapsed / (double)times;
}

int main() {
	// Output files for test run times.
	std::ofstream default_out("default_out.txt"); // Default data structures.
	std::ofstream dummy_out("dummy_out.txt"); // Dummy data structures.
	
	// Initialize random number generator.
	std::mt19937 rng;
	std::uniform_real_distribution<double> dist(0, 1);
	
	for(int sitecount = 0; sitecount < 10000; ++sitecount) {
		std::cout << "Site count " << sitecount << ".\n";
		// Generate the sites with random sampling.
		frivol::containers::Array<frivol::Point<>> sites(sitecount);
		for(int sitei = 0; sitei < sitecount; ++sitei) {
			sites[sitei] = frivol::Point<>(dist(rng), dist(rng));
		}
		
		// Run the algorithms and save results.
		frivol::VoronoiDiagram<> default_diagram(0);
		frivol::VoronoiDiagram<> dummy_diagram(0);
		
		double default_runtime = getExecutionTime([&]() {
			default_diagram = frivol::computeVoronoiDiagram(sites);
		}, 0.3);
		double dummy_runtime = getExecutionTime([&]() {
			typedef frivol::Policy<
				double,
				frivol::containers::priority_queues::DummyPriorityQueue,
				frivol::containers::search_trees::DummySearchTree
			> Policy;
			default_diagram = frivol::computeVoronoiDiagram<Policy>(sites);
		}, 0.3);
		
		default_out << sitecount << " " << default_runtime << "\n";
		dummy_out << sitecount << " " << dummy_runtime << "\n";
		
		default_out.flush();
		dummy_out.flush();
	}
	
	default_out.close();
	dummy_out.close();
	
	if(!default_out.good() || !dummy_out.good()) {
		std::cerr << "Writing output failed.\n";
		return 1;
	}
	
	return 0;
}
