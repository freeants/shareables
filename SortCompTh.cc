/*
    File: SortCompTh.cc - Compare sort algorithms, multi-threaded way.
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
#include <thread>

using namespace std;

int max_size; // Size of data dictionary

int *a;                                     // Data dictionary
int *d0, *d1, *d2, *d3, *d4, *d5, *d6, *d7; // Temp data dictionary

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
void mergeSort(int arr[], int low, int high)
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

    memset(buckets, 0, max * sizeof(int));

    for (i = 0; i < n; i++)
        buckets[arr[i]]++;

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
 * Copy array to another.
 */
void copyArry(int *x, int *y)
{
    for (int i = 0; i < max_size; i++)
        y[i] = x[i];
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
    d0 = new int[max_size];
    d1 = new int[max_size];
    d2 = new int[max_size];
    d3 = new int[max_size];
    d4 = new int[max_size];
    d5 = new int[max_size];
    d6 = new int[max_size];
    d7 = new int[max_size];

    // Assign values to array
    for (int i = 0; i < max_size; i++)
        a[i] = GenRandomNumber(); //this forms randomly filled numbers

    auto t1 = chrono::high_resolution_clock::now(); //get end time
    cout << chrono::duration_cast<chrono::microseconds>(t1 - t0).count() << " micro(μ) seconds" << endl;
}

/*
 * Main routine that carries out the tests - multi threaded.
 */
void test()
{
    cout << "Comparing sort algorithms (C++, threaded) ..." << endl;
    cout << left << setw(20) << "Algorithm" << setw(20) << "Time elapsed(μ)"
         << setw(20) << "Is sorted? " << setw(20) << "Thread ID" << endl;

    /** start the array copy thread */
    thread cp_t0(copyArry, a, d0);
    thread cp_t1(copyArry, a, d1);
    thread cp_t2(copyArry, a, d2);
    thread cp_t3(copyArry, a, d3);
    thread cp_t4(copyArry, a, d4);
    thread cp_t5(copyArry, a, d5);
    thread cp_t6(copyArry, a, d6);
    thread cp_t7(copyArry, a, d7);
    cp_t0.join();
    cp_t1.join();
    cp_t2.join();
    cp_t3.join();
    cp_t4.join();
    cp_t5.join();
    cp_t6.join();
    cp_t7.join();

    /** start the array sort threads */
    auto t0 = chrono::high_resolution_clock::now(); //get start time
    thread st_t0(bubbleSort, d0, max_size);
    thread st_t1(quickSort, d1, 0, max_size);
    thread st_t2(insertionSort, d2, max_size);
    thread st_t3(shellSort, d3, max_size);
    thread st_t4(selectionSort, d4, max_size);
    thread st_t5(heapSort, d5, max_size);
    thread st_t6(mergeSort, d6, 0, max_size);
    thread st_t7(bucketSort, d7, max_size, max_size + 1);
    st_t0.join();   auto t1 = chrono::high_resolution_clock::now(); //get start time
    dispResult("1.Bubble", t1 - t0, d0);
    st_t1.join();   auto t2 = chrono::high_resolution_clock::now(); //get start time
    dispResult("2.Quick", t2 - t1, d1);
    st_t2.join();   auto t3 = chrono::high_resolution_clock::now(); //get start time
    dispResult("3.Insertion", t3 - t2, d2);
    st_t3.join();   auto t4 = chrono::high_resolution_clock::now(); //get start time
    dispResult("4.Shell", t4 - t3, d3);
    st_t4.join();   auto t5 = chrono::high_resolution_clock::now(); //get start time
    dispResult("5.Selection", t5 - t4, d4);
    st_t5.join();   auto t6 = chrono::high_resolution_clock::now(); //get start time
    dispResult("6.Heap", t6 - t5, d5);
    st_t6.join();   auto t7 = chrono::high_resolution_clock::now(); //get start time
    dispResult("7.Merge", t7 - t6, d6);
    st_t7.join();   auto t8 = chrono::high_resolution_clock::now(); //get start time
    dispResult("8.Bucket", t8 - t7, d7);
    
    auto t = chrono::high_resolution_clock::now(); //get end time

    /** print results */
    auto timeElapsed = chrono::duration_cast<chrono::microseconds>(t - t0).count();
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

    delete[] a;
    delete[] d0;
    delete[] d1;
    delete[] d2;
    delete[] d3;
    delete[] d4;
    delete[] d5;
    delete[] d6;
    delete[] d7;
    return 0;
}