#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

extern "C" {

    struct StatResult {
        double mean;
        double standard_deviation;
        double variance;
        double median;
    };

    StatResult* calculate_stats(double* arr, int size) {
        double sum = 0.0;
        for (int i = 0; i < size; i++) {
            sum += arr[i];
        }

        double mean = sum / size;

        double squared_diff = 0.0;
        for (int i = 0; i < size; i++) {
            squared_diff += pow((mean - arr[i]), 2);
        }
        double standard_deviation = sqrt(squared_diff/size);

        double* new_arr = new double[size];
        for (int i = 0; i < size; i++) {
            new_arr[i] = arr[i];
        }

        sort(new_arr + 0 , new_arr + size);

        double median = new_arr[size/2];
        if (size % 2 != 0) {
            median = new_arr[size/2];
        } else {
            median = (new_arr[size/2 - 1] + new_arr[size/2])/2.0;
        }
        delete[] new_arr;

        StatResult* res = new StatResult;
        res->mean = mean;
        res->standard_deviation = standard_deviation;
        res->variance = standard_deviation*standard_deviation;
        res->median = median;
        return res;
    }

    void delete_StatResult(StatResult* ptr) {
        if (ptr){
            delete ptr;
        }
        return;
    }
    
}