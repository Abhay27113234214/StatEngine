#include <iostream>
#include <cmath>

using namespace std;

extern "C" {

    struct StatResult {
        double mean;
        double standard_deviation;
        double variance;
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

        StatResult* res = new StatResult;
        res->mean = mean;
        res->standard_deviation = standard_deviation;
        res->variance = standard_deviation*standard_deviation;
        return res;
    }

    void delete_StatResult(StatResult* ptr) {
        delete ptr;
        return;
    }
    
}