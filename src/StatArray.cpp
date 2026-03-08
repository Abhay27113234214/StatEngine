#include <iostream>
#include <vector>
#include "StatArray.h"
#include <cmath>
#include <algorithm>
#include <cstring>

using std::vector;

template<typename T>
StatArray<T>::StatArray(int number_of_elements) : mean_(NAN) {
    if (number_of_elements < 0) throw std::invalid_argument("Invalid argument for number of elements");
    this->size = number_of_elements;
    this->m_data = new T[this->size];
    this->owns_memory = true;
}

template<typename T>
StatArray<T>::StatArray(T* data, size_t size, bool owns_memory) {
    this->m_data = data;
    this->size = size;
    this->owns_memory = owns_memory;
}

template<typename T> 
StatArray<T>::StatArray(const StatArray& st)  {
    this->size = st.getSize();
    this->m_data = new T[this->size];
    std::copy(st._data(), st._data() + this->size, this->m_data);
    this->mean_ = st.mean();
    this->owns_memory = true;
}

template<typename T>
void StatArray<T>::invalidate_cache() { this->mean_ = NAN; }

template<typename T>
size_t StatArray<T>::getSize() const{
    return this->size;
}

template<typename T>
T& StatArray<T>::operator[](int index) {
    int n = static_cast<int>(this->size);
    if (index >= 0) {
        if (index >= n) {
            throw std::out_of_range("Out of bounds access");
        }
        return this->m_data[index];
    } else {
        if (index < -1*n) {
            throw std::out_of_range("Out of bounds access");
        }
        return this->m_data[n+index];
    }
}

template<typename T>
T StatArray<T>::operator[](int index) const{
    int n = static_cast<int>(this->size);
    if (index >= 0) {
        if (index >= n) {
            throw std::out_of_range("Out of bounds access");
        }
        return this->m_data[index];
    } else {
        if (index < -1*n) {
            throw std::out_of_range("Out of bounds access");
        }
        return this->m_data[n+index];
    }
}

template<typename T>
double StatArray<T>::mean(bool re_calculate) const {
    if (!re_calculate && !std::isnan(this->mean_)) {
        return this->mean_;
    }
    double sum = 0.0;
    for (size_t i = 0; i < this->size; i++) {
        sum += this->m_data[i];
    }
    this->mean_ = sum / this->size;
    return mean_;
}

template<typename T>
double StatArray<T>::median() { // isme changes karne padenge 
    int n = this->size;
    if (n <= 0) {
        return NAN;
    }
    T* temp = new T[this->size];
    std::memcpy(temp, this->m_data, this->size*sizeof(T));
    std::sort(temp, temp + this->size);
    double median;
    if (n % 2 == 0) {
        median = (temp[(n-1)/2] + temp[n/2])/2;
    } else {
        median = temp[n/2];
    }
    delete[] temp;
    return median;
}

template<typename T>
double StatArray<T>::calculate_moment(int power) {
    if (std::isnan(this->mean_)) {
        this->mean();
    }
    double power_sum = 0.0;
    if (power == 0) {
        return 1;
    } else if (power == 1) {
        return 0;
    } else if (power == 2) {
        for (size_t i = 0; i < this->size; i++) {
            power_sum += (this->m_data[i] - this->mean_) * (this->m_data[i] - this->mean_);
        }
    } else if (power >= 3) {
        for (size_t i = 0; i < this->size; i++) {
            power_sum += pow((this->m_data[i] - this->mean_), power);
        }
    }
    return power_sum;
}

template<typename T>
double StatArray<T>::std(int ddof) {
    double moment = this->calculate_moment(2);
    return sqrt(moment/(this->size - ddof));
}

template<typename T>
double StatArray<T>::variance(int ddof) {
    double moment = this->calculate_moment(2);
    return (moment/(this->size - ddof));
}

template<typename T>
double StatArray<T>::skew(bool bias) {
    int n = this->size;
    double moment_3rd = this->calculate_moment(3)/n;
    double moment_2nd = this->calculate_moment(2)/n;
    double skewness = (moment_3rd/(moment_2nd * sqrt(moment_2nd)));
    if (!bias) {
        if (n <= 2) return NAN;
        skewness = (sqrt(n*(n-1))/(n-2))*skewness;
    }
    return skewness;
}

template<typename T>
double StatArray<T>::kurtosis(bool bias) {
    int n = this->size;
    double moment_4th = this->calculate_moment(4) / n;
    double moment_2nd = this->calculate_moment(2) / n;
    double kurt = ((moment_4th)/(moment_2nd*moment_2nd)) - 3;
    if (!bias) {
        if (n <= 3) return NAN;
        kurt = ((n-1.0)/((n-2.0)*(n-3.0)))*((n+1.0)*kurt + 6.0);
    }
    return kurt;
}

template<typename T> 
T* StatArray<T>::_data() {
    return this->m_data;
}

template<typename T>
const T* StatArray<T>::_data() const {
    return this->m_data;
}

template<typename T>
StatArray<T>::~StatArray() {
    if (this->owns_memory) {
        delete[] this->m_data;
    }
    this->owns_memory = false;
}

// explicit instantiation directives 
template class StatArray<int>;
template class StatArray<double>;
template class StatArray<long long>;