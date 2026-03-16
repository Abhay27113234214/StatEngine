#include <stdexcept>
#include "Matrix.h"

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
    if (index < 0 || index > size) {
        throw std::out_of_range("Out of bounds access!");
    }
    return this->m_data[index];
}

template<typename T>
T& Matrix<T>::operator[](int index) {
    int size = static_cast<int>(this->size());
    if (index < 0 || index > size) {
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