#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

extern "C" {

    double calculate_moment(double* arr, int size, int power, double mean) {
        if (size <= 0 || arr == nullptr) {
            return NAN;
        }
        double power_sum = 0.0;
        if (power == 0) {
            return 1.0;
        } else if (power == 1) {
            return 0;
        } else if (power == 2) {
            for (int i = 0; i < size; i++) {
                power_sum += (arr[i] - mean)*(arr[i] - mean);
            }
        } else {
            for (int i = 0; i < size; i++) {
                power_sum += pow((arr[i] - mean), power);
            }
        }
        return power_sum / size;
    }

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