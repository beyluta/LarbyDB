#pragma once
#include <iostream>
#include <vector>

class Sort
{
    private:
    static void Merge(std::vector<int> left, std::vector<int> right, std::vector<int> &unsorted);

    public:
    static void MergeSort(std::vector<int> &unsorted);
};