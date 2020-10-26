/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Joseph Scalera
 * Version     : 1.0
 * Date        : 10/22/2020
 * Description : Counts the number of inversions in an array.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.w
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);

/**
 * Counts the number of inversions in an array in theta(n^2) time.
 */
long count_inversions_slow(int array[], int length) {
    // TODO

    long counter = 0;

    for (int i = 0; i < length; i++){
        for (int j = i+1; j < length; j++){
            if (array[i] > array[j]){
                counter++;
            }
        }
    }
    return counter;
}   

/**
 * Counts the number of inversions in an array in theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) {
    // TODO
    // Hint: Use mergesort!

    int * scratch = new int[length];
    long inversions = mergesort(array, scratch, 0, length-1);
    delete [] scratch;
    return inversions;
}

long merge(int array[], int scratch[], int low, int mid, int high){
    int i = low;
    int j = mid;
    int index = low;
    long inversions = 0;
    
    //Copy elements from array to scratch
    while (i <= (mid - 1) && j <= high){
        if (array[i] <= array[j]){
            scratch[index] = array[i];
            index++;
            i++;
        }
        else {
            scratch[index] = array[j];
            index++;
            j++;
            inversions += mid-i;    //All elements from [i, mid] are bigger than array[j]
        }
    }

    //Copy the unprocessed interval, [i,mid], to scratch
    //When j > high
    while (i <= mid - 1){
        scratch[index] = array[i];
        i++;
        index++;
    }

    //Copy the unprocessed interval, [j,high], to scratch
    //When i == mid
    while (j <= high){
        scratch[index] = array[j];
        j++;
        index++;
    }

    //Copy elements from scratch back into array
    for (int i = low; i <= high; i++){
        array[i] = scratch[i];
    }

    return inversions;
}
//performs merge sort and returns the total number of inversions 
static long mergesort(int array[], int scratch[], int low, int high) {
    // TODO
    long inversions = 0;
    if (low < high){
        //(high+low)/2 = low + (high-low)/2
        //Prevents Overflow when high is the maximum possible value of an integer
        int mid = low + (high-low)/2;

        //Recursively call mergesort the two array intervals to merge. mid+1 prevents overlap
        inversions += mergesort(array, scratch, low, mid);
        inversions += mergesort(array, scratch, mid + 1, high);

        //Merge the the sub array intervals 
        inversions += merge(array, scratch, low, mid + 1, high);
    }
    return inversions;
}

int main(int argc, char *argv[]) {
    // TODO: parse command-line argument
    string speed;

    //if there is 1 argument
    if (argc == 2){
        speed = argv[1];
        //Use the slow version
        if (speed.compare("slow") != 0){
            cerr << "Error: Unrecognized option '" << speed << "'." << endl;
            return 1;
        }
        else {
            speed = "slow";
        }
    }
    //Greater than 1 throws exception
    else if (argc > 1){
        cerr << "Usage: ./inversioncounter [slow]" << endl;
        return 1;
    }
    //Otherwise use merge sort
    else {
        speed = "fast";
    }

    cout << "Enter sequence of integers, each followed by a space: " << flush;

    istringstream iss;
    int value, index = 0;
    vector<int> values;
    string str;
    str.reserve(11);
    char c;
    while (true) {
        c = getchar();
        const bool eoln = c == '\r' || c == '\n';
        if (isspace(c) || eoln) {
            if (str.length() > 0) {
                iss.str(str);
                if (iss >> value) {
                    values.push_back(value);
                } else {
                    cerr << "Error: Non-integer value '" << str
                         << "' received at index " << index << "." << endl;
                    return 1;
                }
                iss.clear();
                ++index;
            }
            if (eoln) {
                break;
            }
            str.clear();
        } else {
            str += c;
        }
    }

    if (values.size() == 0){
        cerr << "Error: Sequence of integers not received." << endl;
        return 1;
    }

    // TODO: produce output
    if (speed.compare("slow") == 0){
        long inversions = count_inversions_slow(&values[0], values.size());
        cout << "Number of inversions: " << inversions << endl;
    }
    else {
        long inversions = count_inversions_fast(&values[0], values.size());
        cout << "Number of inversions: " << inversions << endl;
    }

    return 0;
}
