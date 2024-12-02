#include <iostream>
#include "DisjointSets.h"
using namespace std;

// Test main; all finds on same output line should be identical


int main() 
{
    int numElements = 10;
    DisjointSets ds(numElements);

    ds.unionSets(1, 2);
    ds.unionSets(3, 4);
    ds.unionSets(2, 4);

    std::cout << "Find(1): " << ds.find(1) << std::endl;
    std::cout << "Find(3): " << ds.find(3) << std::endl;

    ds.find(1);
    std::cout << "After path compression find(1): " << ds.find(1) << std::endl;

    return 0;
}
