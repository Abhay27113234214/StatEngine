#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

extern "C" {

    double mean(double* arr, int size) {
        if (size <= 0 || arr == nullptr) {
            return NAN;
        }
        double sum = 0;
        for (int i = 0; i < size; i++) {
            sum += arr[i];
        }
        double mean = sum / size;
        return mean;
    }

    double ssd(double* arr, int size, double mean) {
        if (size <= 0 || arr == nullptr) {
            return NAN;
        }
        double squared_diff = 0.0;
        for (int i = 0; i < size; i++) {
            squared_diff += (mean - arr[i]) * (mean - arr[i]);
        }
        return squared_diff;
    }

    double median(double* arr, int size) {
        if (size <= 0 || arr == nullptr) {
            return NAN;
        }
        double* new_arr = new double[size];
        for (int i = 0; i < size; i++) {
            new_arr[i] = arr[i];
        }
        sort(new_arr, new_arr+size);
        double med = new_arr[size/2];
        if (size % 2 != 0) {
            med = new_arr[size/2];
        } else {
            med = (new_arr[size/2 - 1] + new_arr[size/2])/2.0;
        }
        delete[] new_arr;
        return med;
    }
    
}