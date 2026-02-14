#ifndef STATARRAY_H
#define STATARRAY_H
#include <vector>

using std::vector;

class StatArray {
    private:
        vector<double> arr;
        double mean_;

        // helpers
        double calculate_moment(int power);

    public:
        StatArray(const vector<double>& vec);

        void invalidate_cache();

        void append(double val);

        size_t getSize() const;

        double operator[](size_t index) const;

        double& operator[](size_t index);

        double mean(bool re_calculate = false);

        double std(int ddof = 0);

        double variance(int ddof = 0);

        double skew(bool bias = true);

        double kurtosis(bool bias = true);
};


#endif