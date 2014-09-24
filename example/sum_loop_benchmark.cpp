#include <iostream>
#include "../Benchmark2_draft.hpp"

int main(int argc, char const *argv[])
{
	Benchmark< std::ofstream >  benchmark("test_runable/sum_loop > tmp");
    benchmark.execute();
	return 0;
}
