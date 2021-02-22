/**
 * SearchComp.cc - Compare search algorithems.
 * (c), freeansts. All rights reserved.
 **/

#include <iostream>
#include <random>
#include <iomanip>
#include <chrono>
using namespace std;

const int MAX = 999999; // Size of data dictionary
const int MIN = 0;

int *arr; // Gloable data dictionary
int key;  // The key number to be searched for

/* 1. SequenceSearch
 * A linear search sequentially checks each element of the list until it 
 * finds an element that matches the target value. If the algorithm reaches
 * the end of the list, the search terminates unsuccessfully.
 * Average Search Length (ASL) = (n+1) / 2, Average performance O(n/2), Space:O(1)
 */
int SequenceSearch(int A[], int n, int T)
{
    for (int i = 0; i < n; i++)
        if (A[i] == T)
            return i;
    return -1;
}

/* 2. BinarySearch
 * Binary search works on sorted arrays. Binary search begins by comparing
 * an element in the middle of the array with the target value. If the target
 * value matches the element, its position in the array is returned. 
 * If the target value is less than the element, the search continues in the
 * lower half of the array. If the target value is greater than the element, 
 * the search continues in the upper half of the array. By doing this,
 * the algorithm eliminates the half in which the target value cannot lie 
 * in each iteration.
 * ASL = log(n+1), Average Performance: O(log n), Space complexity: O(1)
 */
int BinarySearch(int A[], int low, int high, int T)
{
    int L = low, R = high - 1, m;
    while (L <= R)
    {
        m = (L + R) / 2;
        if (A[m] < T)
            L = m + 1;
        else if (A[m] > T)
            R = m - 1;
        else
            return m;
    }
    return -1;
}

/* 3. Interpolation Search
 * The Interpolation Search is an improvement over Binary Search for instances,
 * where the values in a sorted array are uniformly distributed. 
 * Binary Search always goes to the middle element to check. On the other hand, 
 * interpolation search may go to different locations according to the value 
 * of the key being searched. For example, if the value of the key is closer
 * to the last element, interpolation search is likely to start search 
 * toward the end side.
 * O(log log n)
 */
int InterpolationSearch(int A[], int n, int T)
{
    // Find indexes of two corners
    int lo = 0, hi = (n - 1);

    // Since array is sorted, an element present
    // in array must be in range defined by corner
    while (lo <= hi && T >= A[lo] && T <= A[hi])
    {
        if (lo == hi)
        {
            if (A[lo] == T)
                return lo;
            return -1;
        }
        // Probing the position with keeping
        // uniform distribution in mind.
        int pos = lo + (((double)(hi - lo) /
                         (A[hi] - A[lo])) *
                        (T - A[lo]));

        // Condition of target found
        if (A[pos] == T)
            return pos;

        // If x is larger, x is in upper part
        if (A[pos] < T)
            lo = pos + 1;

        // If x is smaller, x is in the lower part
        else
            hi = pos - 1;
    }
    return -1;
}

/* 4. FibonacciSearch
 * Fibonacci Search is a comparison-based technique that uses Fibonacci 
 * numbers to search an element in a sorted array.
 * O(log n)
 */
int FibonacciSearch(int A[], int n, int T)
{
    /* Initialize fibnacci numbers */
    int fibMMm2 = 0;              // (m-2)'th Fibonacci No.
    int fibMMm1 = 1;              // (m-1)'th Fibonacci No.
    int fibM = fibMMm2 + fibMMm1; // m'th Fibonacci

    /* fibM is going to store the smallest Fibonacci
       Number greater than or equal to n */
    while (fibM < n)
    {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }

    // Marks the eliminated range from front
    int offset = -1;

    /* while there are elements to be inspected. Note that
       we compare arr[fibMm2] with x. When fibM becomes 1,
       fibMm2 becomes 0 */
    while (fibM > 1)
    {
        // Check if fibMm2 is a valid location
        int i = min(offset + fibMMm2, n - 1);

        /* If T is greater than the value at index fibMm2,
           cut the subarray array from offset to i */
        if (A[i] < T)
        {
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        }

        /* If T is greater than the value at index fibMm2,
           cut the subarray after i+1  */
        else if (A[i] > T)
        {
            fibM = fibMMm2;
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        }

        /* element found. return index */
        else
            return i;
    }

    /* comparing the last element with x */
    if (fibMMm1 && A[offset + 1] == T)
        return offset + 1;

    /*element not found. return -1 */
    return -1;
}

/*5. ExponentialSearch
 * Exponential search allows for searching through a sorted, unbounded list 
 * for a specified input value (the search "key"). The algorithm consists of 
 * two stages. The first stage determines a range in which the search key 
 * would reside if it were in the list. In the second stage, a binary search 
 * is performed on this range. In the first stage, assuming that the list 
 * is sorted in ascending order, the algorithm looks for the first exponent, 
 * j, where the value 2j is greater than the search key. This value, 2j 
 * becomes the upper bound for the binary search with the previous power of 
 * 2, 2j - 1, being the lower bound for the binary search.
 * Average performance: O(log n), Space complexity: O(1).
 */
int ExponentialSearch(int A[], int n, int T)
{
    if (n == 0)
        return -1;
    int bound = 1;
    while (bound < n && A[bound] < T)
    {
        bound *= 2;
    }
    return BinarySearch(A, bound / 2, min(bound + 1, n), T);
}

/* 6. TernarySearch
 * Ternary search is a divide and conquer algorithm that can be used to find 
 * an element in an array. It is similar to binary search where we divide 
 * the array into two parts but in this algorithm, we divide the given array 
 * into three parts and determine which has the key (searched element). 
 * We can divide the array into three parts by taking m1 and m2 which 
 * can be calculated as shown below. Initially, l and r will be equal to 0 
 * and n-1 respectively, where n is the length of the array. 
 * O(log3 n)
 */
int TernarySearch(int A[], int l, int r, int T)
{
    int m1 = 0, m2 = 0;
    if (l <= r)
    {
        m1 = l + (r - l) / 3;
        m2 = r - (r - l) / 3;
        if (A[m1] == T)
            return m1;
        if (A[m2] == T)
            return m2;
        if (T < A[m1])
            return TernarySearch(A, l, m1 - 1, T);
        else if (T > A[m2])
            return TernarySearch(A, m2 + 1, r, T);
        else
            return TernarySearch(A, m1 + 1, m2 - 1, T);
    }
    return -1;
}

/*7. JumpSearch
 * Jump Search is a searching algorithm for sorted arrays. The basic idea is
 * to check fewer elements (than linear search) by jumping ahead by fixed 
 * steps or skipping some elements in place of searching all elements.
 * O(√n), O(1)
 */
int JumpSearch(int A[], int n, int T)
{
    int step = sqrt(n);
    int prev = 0;
    while (A[min(step, n) - 1] < T)
    {
        prev = step;
        step += sqrt(n);
        if (prev >= n)
            return -1;
    }

    while (A[prev] < T)
    {
        prev++;
        if (prev == min(step, n))
            return -1;
    }

    if (A[prev] == T)
        return prev;

    return -1;
}

/* 
 * bucketSort() - Non-Comparision Sort algorithm, bucket sorting
 * O(n+k), O(n+k), Stable
 */
void bucketSort(int A[], int n, int max)
{
    int i, j;
    int buckets[max];

    // Initiate all values to 0 in buckets
    memset(buckets, 0, max * sizeof(int));

    // 1. counting
    for (i = 0; i < n; i++)
        buckets[A[i]]++;

    // 2. sorting
    for (i = 0, j = 0; i < max; i++)
    {
        while ((buckets[i]--) > 0)
            A[j++] = i;
    }
}

/*
 * GenKeyNumber() - Generate key number randomly.
 */
int GenKeyNumber()
{
    random_device rd;                           //obtain a random number from hardware
    mt19937 gen(rd());                          //seed the generator
    uniform_int_distribution<> distr(MIN, MAX); //define the range

    //assign the rand value to key number
    return distr(gen);
}

void BuildDataDictionary()
{
    // Define the array that holds all data
    arr = new int[MAX];

    // Assign values to array with random numbers
    auto t0 = chrono::high_resolution_clock::now(); //get start time
    for (int i = MIN; i <= MAX; i++)
        arr[i] = GenKeyNumber();                    //this forms a sorted serise of [MIN, MAX]
    auto t1 = chrono::high_resolution_clock::now(); //get start time
    cout << "Building radom data set [" + to_string(MIN) + ", " + to_string(MAX) + "] ... " << chrono::duration_cast<chrono::microseconds>(t1 - t0).count() << " ms." << endl;
    // Sort the array for future searching
    bucketSort(arr, MAX, MAX + 1);
    auto t2 = chrono::high_resolution_clock::now(); //get start time
    cout << "Bucket Sorting for searching ... " << chrono::duration_cast<chrono::microseconds>(t2 - t1).count() << " ms." << endl;
}

/*
 * Display search results - Lambda expression way.
 */
static auto dispResult = [](string str, int index, auto diffTime) {
    if (index == -1) // No searching matches found
        cout << left << setw(20) << str << " key(" << key << ") NOT found, " << chrono::duration_cast<chrono::microseconds>(diffTime).count() << " ms." << endl;
    else // Found matches display
        cout << left << setw(20) << str << " key(" << key << ") found @ index: " << index << ", " << chrono::duration_cast<chrono::microseconds>(diffTime).count() << " ms." << endl;
};

void test()
{
    int index;

    cout << "Comparing Searching Algorithms (c++) ..." << endl;
    auto t0 = chrono::high_resolution_clock::now(); //get start time

    index = SequenceSearch(arr, MAX, key);
    auto t1 = chrono::high_resolution_clock::now(); //get start time
    dispResult("1. Sequence", index, t1 - t0);

    index = BinarySearch(arr, MIN, MAX, key);
    auto t2 = chrono::high_resolution_clock::now(); //get start time
    dispResult("2. Binary", index, t2 - t1);

    index = InterpolationSearch(arr, MAX, key);
    auto t3 = chrono::high_resolution_clock::now(); //get start time
    dispResult("3. Interpolation", index, t3 - t2);

    index = FibonacciSearch(arr, MAX - 1, key);
    auto t4 = chrono::high_resolution_clock::now(); //get start time
    dispResult("4. Fibonacci", index, t4 - t3);

    index = ExponentialSearch(arr, MAX - 1, key);
    auto t5 = chrono::high_resolution_clock::now(); //get start time
    dispResult("5. Exponential", index, t5 - t4);

    index = TernarySearch(arr, MIN, MAX - 1, key);
    auto t6 = chrono::high_resolution_clock::now(); //get start time
    dispResult("6. Ternary", index, t6 - t5);

    index = JumpSearch(arr, MAX - 1, key);
    auto t7 = chrono::high_resolution_clock::now(); //get start time
    dispResult("7. Jump", index, t7 - t6);

    cout << "//////////////////////////////////////////////////////////" << endl;
    cout << left << setw(20) << "Total searching time: " << chrono::duration_cast<chrono::microseconds>(t7 - t0).count() << " ms." << endl;
}

int main(int argc, char const *argv[])
{
    try
    {
        // Instantiation
        BuildDataDictionary();
        // Generate key
        key = GenKeyNumber();
        // Start test
        test();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    delete[] arr;
    return 0;
}