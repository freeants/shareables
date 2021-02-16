/*
    File: SortComp.cxx - Compare sort algorithms.
    Copyright:  (c) freeants. All rights reserved.
 */
#include <chrono>
#include <ctime>
#include <random>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstring>
#include <vector>

using namespace std;

int max_size; // Size of data dictionary

int *a; // Data dictionary
int *t; // Temp data dictionary

/*
 * Verify if the array was sorted.
 */
bool isSorted(int *arr)
{
    for (int i = 1; i < max_size; i++)
        if (arr[i] < arr[i - 1])
            return false;
    return true;
}

//void dispResult(string str, auto diffTime, int *arr)
//above is okay for g++, for clang, no auto param allowed.
static auto dispResult =[](string str, auto diffTime, int *arr)
{
    cout << left << setw(20) << str << setw(20) << chrono::duration_cast<chrono::microseconds>(diffTime).count() << isSorted(arr) << endl;
};

/* 1.
 * bubbleSort() - Comparision Sort algorithm, exchange sorting
 * O(n²), O(1), Stable
 */
void bubbleSort(int *arr, int n)
{
    bool swapped = true;
    int j = 0;
    int tmp;
    while (swapped)
    {
        swapped = false;
        j++;
        for (int i = 0; i < n - j; i++)
        {
            if (arr[i] > arr[i + 1])
            {
                tmp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = tmp;
                swapped = true;
            }
        }
    }
}

/* 2.
 * quickSort() - Comparision Sort algorithm, exchange sorting
 * O(nlog(n)), O(nlog(n)), Unstable
 */
void quickSort(int *arr, int left, int right)
{
    int i = left, j = right;
    int tmp;
    int pivot = arr[(left + right) / 2];

    /* partition */
    while (i <= j)
    {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j)
        {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    };

    /* recursion */
    if (left < j)
        quickSort(arr, left, j);
    if (i < right)
        quickSort(arr, i, right);
}

/* 3.
 * insertionSort() - Comparision Sort algorithm, insertion sorting
 * O(n²), O(1), Stable
 */
void insertionSort(int *arr, int n)
{
    int i, j, tmp;
    for (i = 1; i < n; i++)
    {
        j = i;
        while (j > 0 && arr[j - 1] > arr[j])
        {
            tmp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = tmp;
            j--;
        }
    }
}

/* 4.
 * shellSort() - Comparision Sort algorithm, insertion sorting
 * O(n^1.3), O(1), Unstable
 */
void shellSort(int *arr, int n)
{
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            int j = i;
            int current = arr[i];
            while (j - gap >= 0 && current < arr[j - gap])
            {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = current;
        }
    }
}

/* 5.
 * selectionSort() - Comparision Sort algorithm, selection sorting
 * O(n²), O(1), Unstable
 */
void selectionSort(int *arr, int n)
{
    int i, j, minIndex, tmp;
    for (i = 0; i < n - 1; i++)
    {
        minIndex = i;
        for (j = i + 1; j < n; j++)
            if (arr[j] < arr[minIndex])
                minIndex = j;
        if (minIndex != i)
        {
            tmp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = tmp;
        }
    }
}

/* 6.
 * heapSort() - Comparision Sort algorithm, selection sorting
 * O(nlogn), O(1), Unstable
 */
// To heapify a subtree rooted with node i which is
// an index in arr[]. n is size of heap
void heapify(int *arr, int n, int i)
{
    int largest = i;   // Initialize largest as root
    int l = 2 * i + 1; // left = 2*i + 1
    int r = 2 * i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i)
    {
        swap(arr[i], arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// main function to do heap sort
void heapSort(int *arr, int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i >= 0; i--)
    {
        // Move current root to end
        swap(arr[0], arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

/* 7.
 * mergeSort() - Comparision Sort algorithm, merge sorting
 * O(nlog(n)), O(n), Stable
 */
void merge(int *arr, int low, int mid, int high)
{

    int i = low, j = mid + 1, k = 0;
    int *temp = new int[high - low + 1];
    while (i <= mid && j <= high)
    {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while (i <= mid)
        temp[k++] = arr[i++];
    while (j <= high)
        temp[k++] = arr[j++];
    for (i = low, k = 0; i <= high; i++, k++)
        arr[i] = temp[k];
    delete[] temp;
}
void mergeSort(int *arr, int low, int high)
{
    if (low >= high)
    {
        return;
    }
    int mid = low + (high - low) / 2;
    mergeSort(arr, low, mid);
    mergeSort(arr, mid + 1, high);
    merge(arr, low, mid, high);
}

/* 8.
 * bucketSort() - Non-Comparision Sort algorithm, bucket sorting
 * O(n+k), O(n+k), Stable
 */
void bucketSort(int *arr, int n, int max)
{
    int i, j;
    int buckets[max];

    // Initiate all values to 0 in buckets
    memset(buckets, 0, max * sizeof(int));

    // 1. counting
    for (i = 0; i < n; i++)
        buckets[arr[i]]++;

    // 2. sorting
    for (i = 0, j = 0; i < max; i++)
    {
        while ((buckets[i]--) > 0)
            arr[j++] = i;
    }
}

/*
 * GenRandomNumber() - Generate number randomly in rang [0, max_size].
 */
int GenRandomNumber()
{
    random_device rd;                              //obtain a random number from hardware
    mt19937 gen(rd());                             //seed the generator
    uniform_int_distribution<> distr(0, max_size); //define the range

    //return the rand value
    return distr(gen);
}

void getInput()
{
    cout << "Enter the size of data set: ";
    cin >> max_size;
}

/*
 * Build data dictionary and count timing.
 */
void BuildDataDictionary()
{
    cout << "Building data dictionary ... (size: " << max_size << ") - ";

    auto t0 = chrono::high_resolution_clock::now(); //get start time
    // Define the array that holds all data
    a = new int[max_size];
    t = new int[max_size];

    // Assign values to array
    for (int i = 0; i < max_size; i++)
        a[i] = GenRandomNumber(); //this forms randomly filled numbers

    auto t1 = chrono::high_resolution_clock::now(); //get end time
    cout << chrono::duration_cast<chrono::microseconds>(t1 - t0).count() << " micro(μ) seconds" << endl;
}

/*
 * Copy array to another.
 */
void copyArry(int *x, int *y)
{
    for (int i = 0; i < max_size; i++)
        y[i] = x[i];
}

/*
 * Main routine that carries out the tests.
 */
void test()
{
    cout << "Comparing sort algorithms (C++) ..." << endl;
    cout << left << setw(20) << "Algorithm" << setw(20) << "Time elapsed(μ)"
         << "Is sorted?" << endl;

    auto t0 = chrono::high_resolution_clock::now(); //get start time
    copyArry(a, t);
    bubbleSort(t, max_size);
    auto t1 = chrono::high_resolution_clock::now(); //get end time
    dispResult("1.Bubble", t1 - t0, t);

    copyArry(a, t);
    quickSort(t, 0, max_size - 1);
    auto t2 = chrono::high_resolution_clock::now(); //get end time
    dispResult("2.Quick", t2 - t1, t);

    copyArry(a, t);
    insertionSort(t, max_size);
    auto t3 = chrono::high_resolution_clock::now(); //get end time
    dispResult("3.Insertion", t3 - t2, t);

    copyArry(a, t);
    shellSort(t, max_size);
    auto t4 = chrono::high_resolution_clock::now(); //get end time
    dispResult("4.Shell", t4 - t3, t);

    copyArry(a, t);
    selectionSort(t, max_size);
    auto t5 = chrono::high_resolution_clock::now(); //get end time
    dispResult("5.Selection", t5 - t4, t);

    copyArry(a, t);
    heapSort(t, max_size);
    auto t6 = chrono::high_resolution_clock::now(); //get end time
    dispResult("6.Heap", t6 - t5, t);

    copyArry(a, t);
    mergeSort(t, 0, max_size);
    auto t7 = chrono::high_resolution_clock::now(); //get end time
    dispResult("7.Merge", t7 - t6, t);

    copyArry(a, t);
    bucketSort(t, max_size, max_size + 1);
    auto t8 = chrono::high_resolution_clock::now(); //get end time
    dispResult("8.Bucket", t8 - t7, t);

    auto timeElapsed = chrono::duration_cast<chrono::microseconds>(t4 - t0).count();
    auto timenow = chrono::system_clock::to_time_t(chrono::system_clock::now());
    cout << "////////////////////////////////////////////////////////" << endl;
    cout << left << setw(20) << " Total time" << setw(20) << timeElapsed << (double)timeElapsed / 1000000 << " seconds"
         << endl;
    cout << left << setw(20) << " Completed @ " << setw(20) << ctime(&timenow) << endl;
}

int main()
{
    try
    {
        // Get input
        getInput();

        // Instantiation
        BuildDataDictionary();

        // Start test
        test();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    delete[] a; // free resources
    delete[] t;
    return 0;
}