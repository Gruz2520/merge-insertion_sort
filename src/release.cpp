#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <string>

using namespace std;
using namespace std::chrono;

class ArrayGenerator {
public:
    vector<int> generateRandomArray(int size, int minValue, int maxValue) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(minValue, maxValue);

        vector<int> arr(size);
        for (int i = 0; i < size; ++i) {
            arr[i] = distrib(gen);
        }
        return arr;
    }

    vector<int> generateReverseSortedArray(int size) {
        vector<int> arr(size);
        for (int i = 0; i < size; ++i) {
            arr[i] = size - i;
        }
        return arr;
    }

    vector<int> generateAlmostSortedArray(int size, int numSwaps) {
        vector<int> arr(size);
        for (int i = 0; i < size; ++i) {
            arr[i] = i + 1;
        }

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, size - 1);

        for (int i = 0; i < numSwaps; ++i) {
            int idx1 = distrib(gen);
            int idx2 = distrib(gen);
            swap(arr[idx1], arr[idx2]);
        }
        return arr;
    }
};


class SortTester {
public:
    long long measureMergeSort(vector<int>& arr) {
        vector<int> temp = arr; 
        auto start = high_resolution_clock::now();
        mergeSort(temp, 0, temp.size() - 1);
        auto elapsed = high_resolution_clock::now() - start;
        return duration_cast<microseconds>(elapsed).count();
    }

    long long measureHybridMergeSort(vector<int>& arr, int threshold) {
         vector<int> temp = arr;
        auto start = high_resolution_clock::now();
        hybridMergeSort(temp, 0, temp.size() - 1, threshold);
        auto elapsed = high_resolution_clock::now() - start;
        return duration_cast<microseconds>(elapsed).count();
    }



private:
    void insertionSort(vector<int>& arr, int l, int r) {
        for (int i = l + 1; i <= r; ++i) {
            int key = arr[i];
            int j = i - 1;
            while (j >= l && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }

    void merge(vector<int>& arr, int l, int m, int r) {
        
        int n1 = m - l + 1;
        int n2 = r - m;

        vector<int> L(n1), R(n2);

        for (int i = 0; i < n1; i++)
            L[i] = arr[l + i];
        for (int j = 0; j < n2; j++)
            R[j] = arr[m + 1 + j];

        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

    void mergeSort(vector<int>& arr, int l, int r) {
        if (l < r) {
            int m = l + (r - l) / 2;
            mergeSort(arr, l, m);
            mergeSort(arr, m + 1, r);
            merge(arr, l, m, r);
        }
    }

     void hybridMergeSort(vector<int>& arr, int l, int r, int threshold) {
        if (r - l + 1 <= threshold) {
            insertionSort(arr, l, r);
            return;
        }
        if (l < r) {
            int m = l + (r - l) / 2;
            hybridMergeSort(arr, l, m, threshold);
            hybridMergeSort(arr, m + 1, r, threshold);
            merge(arr, l, m, r);
        }
    }
};


int main() {
    
    ArrayGenerator generator;
    SortTester tester;

    const int MIN_SIZE = 500;
    const int MAX_SIZE = 10000;
    const int STEP = 100;
    const int MIN_VALUE = 0;
    const int MAX_VALUE = 6000;
    const int NUM_SWAPS = 100; 
    const int NUM_TRIALS = 5; 

    vector<int> thresholds = {5, 10, 15, 20, 30, 50};

    ofstream randomData("../data/random_data.csv");
    ofstream reverseData("../data/reverse_data.csv");
    ofstream almostData("../data/almost_data.csv");

    randomData << "Size,MergeSort,Hybrid5,Hybrid10,Hybrid15,Hybrid20,Hybrid30,Hybrid50" << endl;
    reverseData << "Size,MergeSort,Hybrid5,Hybrid10,Hybrid15,Hybrid20,Hybrid30,Hybrid50" << endl;
    almostData << "Size,MergeSort,Hybrid5,Hybrid10,Hybrid15,Hybrid20,Hybrid30,Hybrid50" << endl;

    for (int size = MIN_SIZE; size <= MAX_SIZE; size += STEP) {
        vector<int> randomArray = generator.generateRandomArray(size, MIN_VALUE, MAX_VALUE);
        vector<int> reverseArray = generator.generateReverseSortedArray(size);
        vector<int> almostSortedArray = generator.generateAlmostSortedArray(size, NUM_SWAPS);

        long long totalMergeSortRandom = 0;
        long long totalMergeSortReverse = 0;
        long long totalMergeSortAlmost = 0;

          for (int trial = 0; trial < NUM_TRIALS; ++trial) {
            totalMergeSortRandom += tester.measureMergeSort(randomArray);
            totalMergeSortReverse += tester.measureMergeSort(reverseArray);
            totalMergeSortAlmost += tester.measureMergeSort(almostSortedArray);
        }

        randomData << size << "," << (double)totalMergeSortRandom / NUM_TRIALS;
        reverseData << size << "," << (double)totalMergeSortReverse / NUM_TRIALS;
        almostData << size << "," << (double)totalMergeSortAlmost / NUM_TRIALS;


        for (int threshold : thresholds) {
             long long totalHybridRandom = 0;
             long long totalHybridReverse = 0;
             long long totalHybridAlmost = 0;
            for (int trial = 0; trial < NUM_TRIALS; ++trial) {

                totalHybridRandom += tester.measureHybridMergeSort(randomArray, threshold);
                totalHybridReverse += tester.measureHybridMergeSort(reverseArray, threshold);
                totalHybridAlmost += tester.measureHybridMergeSort(almostSortedArray, threshold);

            }
            randomData << "," << (double)totalHybridRandom / NUM_TRIALS;
            reverseData << "," << (double)totalHybridReverse / NUM_TRIALS;
            almostData << "," << (double)totalHybridAlmost / NUM_TRIALS;
        }

        randomData << endl;
        reverseData << endl;
        almostData << endl;

         if (size == MAX_SIZE) {
           cout << (double)totalMergeSortRandom / NUM_TRIALS << endl;
            cout << (double)totalMergeSortReverse / NUM_TRIALS << endl;
            cout << (double)totalMergeSortAlmost / NUM_TRIALS << endl;
        }

    }

    randomData.close();
    reverseData.close();
    almostData.close();

    return 0;
}