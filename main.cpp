#include <iostream>
#include <vector>
#include <chrono>
#include <random>

class SortingComparison {
public:
    // insrtion sort
    void insertionSort(std::vector<int>& arr) 
    {
        for (int p = 1; p < arr.size(); ++p) 
        {
            int tmp = std::move(arr[p]);
            int j;
            for (j = p; j > 0 && tmp < arr[j - 1]; --j) 
            {
                arr[j] = std::move(arr[j - 1]);
            }
            arr[j] = std::move(tmp);
        }
    }

    // quick sort
    int median3(std::vector<int>& arr, int left, int right) 
    {
        int center = (left + right) / 2;

        if (arr[center] < arr[left]) std::swap(arr[left], arr[center]);
        if (arr[right] < arr[left]) std::swap(arr[left], arr[right]);
        if (arr[right] < arr[center]) std::swap(arr[center], arr[right]);

        std::swap(arr[center], arr[right - 1]);
        return arr[right - 1];
    }

    void quickSort(std::vector<int>& arr, int left, int right) 
    {
        if (left + 10 <= right) 
        {
            const int pivot = median3(arr, left, right);

            int i = left, j = right - 1;
            for (;;) 
            {
                while (arr[++i] < pivot) {}
                while (pivot < arr[--j]) {}

                if (i < j) 
                {
                    std::swap(arr[i], arr[j]);
                } 
                
                else
                {
                    break;
                }
            }

            std::swap(arr[i], arr[right - 1]); 

            quickSort(arr, left, i - 1);
            quickSort(arr, i + 1, right);     
        } 
        else 
        {
            insertionSort(arr); 
        }
    }

    void merge(std::vector<int>& a, std::vector<int>& tmpArray, int leftPos, int rightPos, int rightEnd) 
    {
        int leftEnd = rightPos - 1;
        int tmpPos = leftPos;
        int numElements = rightEnd - leftPos + 1;

        while (leftPos <= leftEnd && rightPos <= rightEnd)
        {
            if (a[leftPos] <= a[rightPos])
            {
                tmpArray[tmpPos++] = std::move(a[leftPos++]);
            }
            else
            {
                tmpArray[tmpPos++] = std::move(a[rightPos++]);
            }
        }

        while (leftPos <= leftEnd)
        {
            tmpArray[tmpPos++] = std::move(a[leftPos++]);
        }

        while (rightPos <= rightEnd)
        {
            tmpArray[tmpPos++] = std::move(a[rightPos++]);
        }

        for (int i = 0; i < numElements; ++i, --rightEnd)
        {
            a[rightEnd] = std::move(tmpArray[rightEnd]);
        }
    }

    void mergeSort(std::vector<int>& arr, std::vector<int>& tmpArray, int left, int right) 
    {
        if (left < right)
        {
            int center = (left + right) / 2;
            mergeSort(arr, tmpArray, left, center);
            mergeSort(arr, tmpArray, center + 1, right);
            merge(arr, tmpArray, left, center + 1, right);
        }
    }

    void mergeSort(std::vector<int>& arr, int left, int right) 
    {
        std::vector<int> tmpArray(arr.size());
        mergeSort(arr, tmpArray, left, right);
    }
    

    // generate random integers
    std::vector<int> generateRandomArray(int size) {
        std::vector<int> arr(size);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 100000);
        for (int& x : arr) {
            x = dist(gen);
        }
        return arr;
    }

    // test sorted
    bool isSorted(const std::vector<int>& arr) {
        for (size_t i = 1; i < arr.size(); ++i) {
            if (arr[i] < arr[i - 1]) {
                return false;
            }
        }
        return true;
    }

    // comparison run time for three sorting algorithms
    void compareSortingAlgorithms() {
        std::vector<int> sizes = {1000, 10000, 100000}; // data scales
        for (int size : sizes) {
            std::vector<int> arr = generateRandomArray(size);
            
            std::cout << "Data size: " << size << "\n";

            // test insertion sort
            std::vector<int> arr_copy = arr;
            auto start = std::chrono::high_resolution_clock::now();
            insertionSort(arr_copy);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            std::cout << "Insertion Sort: " << elapsed.count() << " seconds\n";
            std::cout << "Sorted: " << (isSorted(arr_copy) ? "Yes" : "No") << "\n";

            // test quick sort
            arr_copy = arr;
            start = std::chrono::high_resolution_clock::now();
            quickSort(arr_copy, 0, arr_copy.size() - 1);
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            std::cout << "Quick Sort: " << elapsed.count() << " seconds\n";
            std::cout << "Sorted: " << (isSorted(arr_copy) ? "Yes" : "No") << "\n";

            // test merge sort
            arr_copy = arr;
            start = std::chrono::high_resolution_clock::now();
            mergeSort(arr_copy, 0, arr_copy.size() - 1);
            end = std::chrono::high_resolution_clock::now();
            elapsed = end - start;
            std::cout << "Merge Sort: " << elapsed.count() << " seconds\n";
            std::cout << "Sorted: " << (isSorted(arr_copy) ? "Yes" : "No") << "\n\n";
        }
    }

private:
    // Some internal helper functions
};

int main() {
    SortingComparison sorter;
    sorter.compareSortingAlgorithms();
    return 0;
}
