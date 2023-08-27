#include "set.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>

namespace
{
    std::string get_yes_no(bool condition)
    {
        return condition ? "YES" : "NO";
    }

    int get_random_int(int rand_max)
    {
        return static_cast<int>(std::rand() * (static_cast<double>(rand_max) / RAND_MAX));
    }

} // unnamed namespace

// Tests addition of new elements and tests whether the tree
// properly checks for duplicates.
// (std::set is used as a reference)
void test0(unsigned int sample_size, unsigned int random_range, bool verbose = true)
{
    bool total_test_result = true;

    std::srand(1);
    std::set<int> stl_set;
    sg::set<int> sg_set;

    for(int i = 0; i < sample_size; ++i)
    {
        int random_number = get_random_int(random_range);

        auto [stl_iter, stl_added] = stl_set.insert(random_number);

        auto sg_iter = sg_set.insert(random_number);
        bool sg_added = sg_iter != sg_set.end();

        if(verbose)
        {
            std::cout << "[Adding: " << std::setw(5) << random_number << "] ";
            std::cout << "std::set added: " << std::setw(5) << std::boolalpha << stl_added << ", ";
            std::cout << "sg::set added: " << std::setw(5) << std::boolalpha << sg_added << "; ";
            std::cout << "same: " << get_yes_no(stl_added == sg_added) << std::endl;
        }

        total_test_result = total_test_result && (stl_added == sg_added);
    }

    std::cout << "Total test0 result: " << get_yes_no(total_test_result) << std::endl;
}

// Generates a big tree and checks that in the range [0, random_range]
// search results correspond to the numbers really added
// (again, std::set is used as a reference).
void test1(unsigned int sample_size, unsigned int random_range, bool verbose = true)
{
    bool total_test_result = true;

    std::srand(1);
    std::set<int> stl_set;
    sg::set<int> sg_set;

    for(int i = 0; i < sample_size; ++i)
    {
        int random_number = get_random_int(random_range);

        stl_set.insert(random_number);
        sg_set.insert(random_number);
    }

    for(int number = 0; number < random_range; ++number)
    {
        auto stl_iter = stl_set.find(number);
        auto sg_iter = sg_set.search(number);

        bool stl_found = (stl_iter != stl_set.end());
        bool sg_found = (sg_iter != sg_set.end());

        if(verbose)
        {
            std::cout << "[Checking: " << std::setw(5) << number << "] ";
            std::cout << "std::set found: " << std::setw(5) << std::boolalpha << stl_found << ", ";
            std::cout << "sg::set found: " << std::setw(5) << std::boolalpha << sg_found << "; ";
            std::cout << "same: " << get_yes_no(stl_found == sg_found) << std::endl;
        }

        total_test_result = total_test_result && (stl_found == sg_found);
    }

    std::cout << "Total test1 result: " << get_yes_no(total_test_result) << std::endl;
}

int main_tests(int argc, char** argv)
{
    test0(10000, 1000, false);
    test1(10000, 20000, false);

    return 0;
}
