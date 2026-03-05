#ifndef STATARRAY_H
#define STATARRAY_H
#include <vector>

using std::vector;

template<typename T>
class StatArray {
    private:
        vector<T> arr;
        double mean_;

        // helpers
        double calculate_moment(int power);

    public:
        StatArray(const vector<T>& vec);

        StatArray(int number_of_elements);

        void invalidate_cache();

        void append(T val);

        size_t getSize() const;

        T operator[](int index) const;

        T& operator[](int index);

        double mean(bool re_calculate = false);

        double std(int ddof = 0);

        double variance(int ddof = 0);

        double median();

        double skew(bool bias = true);

        double kurtosis(bool bias = true);
};


#endif