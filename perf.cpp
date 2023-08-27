#include "set.hpp"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

namespace
{
    int get_random_int(int rand_max)
    {
        return static_cast<int>(std::rand() * (static_cast<double>(rand_max) / RAND_MAX));
    }

} // unnamed namespace


// Yields a new sg::set, inserting randomly generated sample_size items
// from the range [0, range].
sg::set<int> generate_set(unsigned int sample_size, unsigned int range)
{
    std::srand(std::time(nullptr));
    sg::set<int> result;

    for(int i = 0; i < sample_size; ++i)
    {
        int random_number = get_random_int(range);
        result.insert(random_number);
    }

    return result;
}

// Evaluates how much on average it takes to search items of a set
// from a range [0, range].
double average_search_time(sg::set<int>& set, unsigned int range)
{
    auto start = std::chrono::high_resolution_clock::now();
    for(int number = 0; number < range; ++number)
    {
        [[maybe_unused]] auto iter = set.search(number);
    }
    auto end = std::chrono::high_resolution_clock::now();

    // Total time of all search operations summated
    double total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    return total_time / range;
}

void main_perf()
{
    constexpr unsigned int point_count = 6;
    constexpr unsigned int random_range = 1e8;
    std::array<int, point_count> sample_sizes
    {
        100,       // 10^2
        1000,      // 10^3
        10000,     // 10^4
        100000,    // 10^5
        1000000,   // 10^6
        10000000,  // 10^7
    };

    for(int point = 0; point < point_count; ++point)
    {
        unsigned int sample_size = sample_sizes[point];
        sg::set<int> set = generate_set(sample_size, random_range);
        double time = average_search_time(set, random_range);

        std::cout << "Number of elements: " << set.size() << "; ";
        std::cout << "Average search time: " << time << " ms" << std::endl;
    }
}

int main()
{
    main_perf();

    return 0;
}