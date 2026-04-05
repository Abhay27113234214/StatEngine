#include <stdexcept>
#include "Matrix.h"
#include <vector>
#include "StatArray.h"
#include <cmath>

using std::vector;

template<typename T>
Matrix<T>::Matrix(size_t rows, size_t cols) {
    this->m_rows = rows;
    this->m_cols = cols;
    this->m_data = new T[this->m_cols * this->m_rows]{0};
    this->owns_memory = true;
}

template<typename T>
Matrix<T>::Matrix(T* data, size_t rows, size_t cols, bool owns_memory) {
    this->m_rows = rows;
    this->m_cols = cols;
    this->m_data = data;
    this->owns_memory = owns_memory;
}

template<typename T>
Matrix<T>::Matrix(const Matrix<T>& mt) {
    auto other_shape = mt.shape();
    this->m_rows = other_shape.first;
    this->m_cols = other_shape.second;
    this->m_data = new T[mt.size()];
    std::copy(mt.data(), mt.data() + mt.size(), this->m_data);
    this->owns_memory = true;
}

template<typename T>
size_t Matrix<T>::size() const {
    return this->m_rows * this->m_cols;
}

template<typename T>
std::pair<size_t, size_t> Matrix<T>::shape() const {
    return {this->m_rows, this->m_cols};
}

template<typename T>
size_t Matrix<T>::rows() const {
    return this->m_rows;
}

template<typename T>
size_t Matrix<T>::cols() const {
    return this->m_cols;
}

template<typename T>
T Matrix<T>::operator[](int index) const {
    int size = static_cast<int>(this->size());
    if (index < 0 || index >= size) {
        throw std::out_of_range("Out of bounds access!");
    }
    return this->m_data[index];
}

template<typename T>
T& Matrix<T>::operator[](int index) {
    int size = static_cast<int>(this->size());
    if (index < 0 || index >= size) {
        throw std::out_of_range("Out of bounds access!");
    }
    return this->m_data[index];
}

template<typename T>
T* Matrix<T>::data() {
    return this->m_data;
}

template<typename T>
const T* Matrix<T>::data() const {
    return this->m_data;
}

template<typename T>
StatArray<double>* Matrix<T>::mean(int axis) const{
    size_t s_r = this->rows();
    size_t s_c = this->cols();
    if (axis == 1) {
        StatArray<double>* res = new StatArray<double>(s_r);
        for (size_t i = 0; i < s_r; i++) {
            for (size_t j = 0; j < s_c; j++) {
                (*res)[i] += this->m_data[i*s_c + j];
            }
            (*res)[i] = (*res)[i] / s_c;
        }
        return res;
    } else if (axis == 0) {
        StatArray<double>* res = new StatArray<double>(s_c);
        for (size_t i = 0; i < s_c; i++) {
            for (size_t j = 0; j < s_r; j++) {
                (*res)[i] += this->m_data[j*s_c + i];
            }
            (*res)[i] = (*res)[i] / s_r;
        }
        return res;
    } 
    throw std::invalid_argument("Invalid value for the axis argument!");
}

template<typename T>
StatArray<T>* Matrix<T>::sum(int axis) const {
    size_t s_r = this->rows();
    size_t s_c = this->cols();
    if (axis == 1) {
        StatArray<T>* res = new StatArray<T>(s_r);
        for (size_t i = 0; i < s_r; i++) {
            for (size_t j = 0; j < s_c; j++) {
                (*res)[i] += this->m_data[i*s_c + j];
            }
        }
        return res;
    } else if (axis == 0) {
        StatArray<T>* res = new StatArray<T>(s_c);
        for (size_t i = 0; i < s_c; i++) {
            for (size_t j = 0; j < s_r; j++) {
                (*res)[i] += this->m_data[j*s_c + i];
            }
        }
        return res;
    }
    throw std::invalid_argument("Invalid value for the axis argument!");
}

template<typename T>
StatArray<double>* Matrix<T>::var(int axis) const {
    size_t s_r = this->rows();
    size_t s_c = this->cols();
    StatArray<double>* means = this->mean(axis);
    if (axis == 1) {
        StatArray<double>* res = new StatArray<double>(s_r);
        for (size_t i = 0; i < s_r; i++) {
            for (size_t j = 0; j < s_c; j++) {
                (*res)[i] += std::pow((this->m_data[i*s_c+j] - (*means)[i]), 2);
            }
            (*res)[i] = (*res)[i] / s_c;
        }
        return res;
    } else if (axis == 0) {
        StatArray<double>* res = new StatArray<double>(s_c);
        for (size_t i = 0; i < s_c; i++) {
            for (size_t j = 0; j < s_r; j++) {
                (*res)[i] += std::pow((this->m_data[j*s_c+i] - (*means)[i]), 2);
            }
            (*res)[i] = (*res)[i] / s_r;
        }
        return res;
    }
    throw std::invalid_argument("Invalid value for the axis argument!");
}

template<typename T>
StatArray<double>* Matrix<T>::std(int axis) const {
    if (axis != 1 || axis != 0) {
        throw std::invalid_argument("invalid value for the axis argument!");
    }
    StatArray<double>* vars = this->var(axis);
    size_t size = vars->getSize();
    StatArray<double>* res = new StatArray(size);
    for (size_t i = 0; i < size; i++) {
        (*res)[i] = std::pow((*vars)[i], (1/2));
    }
    return res;
}

template<typename T>
Matrix<T>::~Matrix()  {
    if (this->owns_memory) {
        delete[] m_data;
    }
    this->owns_memory = false;
}

// explicit instantiation directives 
template class Matrix<int>;
template class Matrix<double>;
template class Matrix<long long>;