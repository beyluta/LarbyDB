#include "sort.h"

void Sort::Merge(std::vector<int> left, std::vector<int> right, std::vector<int> &unsorted)
{
    int leftIndex = 0;
    int rightIndex = 0;
    int mergedIndex = 0;
    int leftLength = left.size();
    int rightLength = right.size();

    while (leftIndex < leftLength && rightIndex < rightLength)
    {
        if (left[leftIndex] <= right[rightIndex])
        {
            unsorted[mergedIndex++] = left[leftIndex++];
        }
        else
        {
            unsorted[mergedIndex++] = right[rightIndex++];
        }
    }

    while (leftIndex < leftLength)
    {
        unsorted[mergedIndex++] = left[leftIndex++];
    }

    while (rightIndex < rightLength)
    {
        unsorted[mergedIndex++] = right[rightIndex++];
    }
}

void Sort::MergeSort(std::vector<int> &unsorted)
{
    int length = unsorted.size();
    int middle = length / 2;

    if (length == 1)
    {
        return;
    }

    std::vector<int> left;
    for (int i = 0; i < middle; i++)
    {
        left.push_back(unsorted[i]);
    }

    std::vector<int> right;
    for (int i = middle; i < length; i++)
    {
        right.push_back(unsorted[i]);
    }

    Sort::MergeSort(left);
    Sort::MergeSort(right);
    Sort::Merge(left, right, unsorted);
}