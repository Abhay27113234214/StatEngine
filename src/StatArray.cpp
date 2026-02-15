#include <iostream>
#include <vector>
#include "StatArray.h"
#include <cmath>
#include <algorithm>

using std::vector;

StatArray::StatArray(const vector<double>& vec) : arr(vec), mean_(NAN) {};

void StatArray::invalidate_cache() { this->mean_ = NAN; }

void StatArray::append(double val) {
    this->arr.push_back(val);
    this->invalidate_cache();
}

size_t StatArray::getSize() const{
    return this->arr.size();
}

double& StatArray::operator[](int index) {
    int n = static_cast<int>(this->arr.size());
    if (index >= 0) {
        if (index >= n) {
            throw std::out_of_range("Out of bounds access");
        }
        return this->arr[index];
    } else {
        if (index < -1*n) {
            throw std::out_of_range("Out of bounds access");
        }
        return this->arr[n+index];
    }
}

double StatArray::operator[](int index) const{
    int n = static_cast<int>(this->arr.size());
    if (index >= 0) {
        if (index >= n) {
            throw std::out_of_range("Out of bounds access");
        }
        return this->arr[index];
    } else {
        if (index < -1*n) {
            throw std::out_of_range("Out of bounds access");
        }
        return this->arr[n+index];
    }
}

double StatArray::mean(bool re_calculate) {
    if (!re_calculate && !std::isnan(this->mean_)) {
        return this->mean_;
    }
    double sum = 0.0;
    for (size_t i = 0; i < this->arr.size(); i++) {
        sum += this->arr[i];
    }
    this->mean_ = sum / this->arr.size();
    return mean_;
}

double StatArray::median() {
    int n = this->arr.size();
    if (n <= 0) {
        return NAN;
    }
    vector<double> temp(this->arr.begin(), this->arr.end());
    sort(temp.begin(), temp.end());
    double median;
    if (n % 2 == 0) {
        median = (temp[(n-1)/2] + temp[n/2])/2;
    } else {
        median = temp[n/2];
    }
    return median;
}

double StatArray::calculate_moment(int power) {
    if (std::isnan(this->mean_)) {
        this->mean();
    }
    double power_sum = 0.0;
    if (power == 0) {
        return 1;
    } else if (power == 1) {
        return 0;
    } else if (power == 2) {
        for (size_t i = 0; i < this->arr.size(); i++) {
            power_sum += (this->arr[i] - this->mean_) * (this->arr[i] - this->mean_);
        }
    } else if (power >= 3) {
        for (size_t i = 0; i < this->arr.size(); i++) {
            power_sum += pow((this->arr[i] - this->mean_), power);
        }
    }
    return power_sum;
}

double StatArray::std(int ddof) {
    double moment = this->calculate_moment(2);
    return sqrt(moment/(this->arr.size() - ddof));
}

double StatArray::variance(int ddof) {
    double moment = this->calculate_moment(2);
    return (moment/(this->arr.size() - ddof));
}

double StatArray::skew(bool bias) {
    int n = this->arr.size();
    double moment_3rd = this->calculate_moment(3)/n;
    double moment_2nd = this->calculate_moment(2)/n;
    double skewness = (moment_3rd/(moment_2nd * sqrt(moment_2nd)));
    if (!bias) {
        if (n <= 2) return NAN;
        skewness = (sqrt(n*(n-1))/(n-2))*skewness;
    }
    return skewness;
}

double StatArray::kurtosis(bool bias) {
    int n = this->arr.size();
    double moment_4th = this->calculate_moment(4) / n;
    double moment_2nd = this->calculate_moment(2) / n;
    double kurt = ((moment_4th)/(moment_2nd*moment_2nd)) - 3;
    if (!bias) {
        if (n <= 3) return NAN;
        kurt = ((n-1.0)/((n-2.0)*(n-3.0)))*((n+1.0)*kurt + 6.0);
    }
    return kurt;
}   