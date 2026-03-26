#ifndef MATRIX_H
#define MATRIX_H

template<typename T>
class Matrix{

    private:
        T* m_data;
        size_t m_rows;
        size_t m_cols;
        bool owns_memory;

    public:
        Matrix(size_t rows, size_t cols);

        Matrix(T* data, size_t rows, size_t cols, bool owns_memory);

        Matrix(const Matrix<T>& mt);

        size_t size() const;

        std::pair<size_t, size_t> shape() const;

        size_t rows() const;

        size_t cols() const;

        T operator[](int index) const;

        T& operator[](int index);

        template<typename U>
        auto operator+(const U& value) const -> Matrix<decltype(U() + T())>* {
            int self_r = static_cast<int>(this->rows());
            int self_c = static_cast<int>(this->cols());
            using restype = decltype(T() + U());
            Matrix<restype>* res = new Matrix<restype>(self_r, self_c);
            for (int i = 0; i < self_c*self_r; i++) {
                (*res)[i] = this->m_data[i] + value;
            }
            return res;
        }

        template<typename U>
        auto operator+(const Matrix<U>& other) const -> Matrix<decltype(U() + T())>* {
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            size_t other_r = other.rows();
            size_t other_c = other.cols();
            if (self_r != other_r || self_c != other_c) {
                throw std::invalid_argument("The shape of the Matrices do not match!!");
            }
            using restype = decltype(U() + T());
            Matrix<restype>* res = new Matrix<restype>(self_r, self_c);
            for (size_t i = 0; i < self_r*self_c; i++) {
                (*res)[i] = this->m_data[i] + other[i];
            }
            return res;
        }

        template<typename U>
        Matrix<T>& operator+=(const U& value) {
            using restype = decltype(T() + U());
            if (!std::is_same<restype, T>::value) {
                throw std::invalid_argument("The datatypes don't match!!");
            }
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            for (size_t i = 0; i < self_r*self_c; i++) {
                this->m_data[i] += value;
            }
            return *this;
        }

        template<typename U>
        Matrix<T>& operator+=(const Matrix<U>& other) {
            using restype = decltype(U() + T());
            if (!std::is_same<restype, T>::value) {
                throw std::invalid_argument("The datatypes don't match!!");
            }
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            size_t other_r = other.rows();
            size_t other_c = other.cols();
            if (self_r != other_r || self_c != other_c) {
                throw std::invalid_argument("The shape of the Matrices do not match!!");
            }
            for (size_t i = 0; i < self_r*self_c; i++) {
                this->m_data[i] += other[i];
            }
            return *this;
        }

        template<typename U>
        auto operator-(const U& value) const -> Matrix<decltype(U() - T())>* {
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            using restype = decltype(T() - U());
            Matrix<restype>* res = new Matrix<restype>(self_r, self_c);
            for (size_t i = 0; i < self_c*self_r; i++) {
                (*res)[i] = this->m_data[i] - value;
            }
            return res;
        }

        template<typename U>
        auto operator-(const Matrix<U>& other) const -> Matrix<decltype(U() - T())>* {
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            size_t other_r = other.rows();
            size_t other_c = other.cols();
            if (self_r != other_r || self_c != other_c) {
                throw std::invalid_argument("The shape of the Matrices do not match!!");
            }
            using restype = decltype(U() - T());
            Matrix<restype>* res = new Matrix<restype>(self_r, self_c);
            for (size_t i = 0; i < self_r*self_c; i++) {
                (*res)[i] = this->m_data[i] - other[i];
            }
            return res;
        }

        template<typename U>
        Matrix<T>& operator-=(const U& value) {
            using restype = decltype(T() - U());
            if (!std::is_same<restype, T>::value) {
                throw std::invalid_argument("The datatypes don't match!!");
            }
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            for (size_t i = 0; i < self_r*self_c; i++) {
                this->m_data[i] -= value;
            }
            return *this;
        }

        template<typename U>
        Matrix<T>& operator-=(const Matrix<U>& other) {
            using restype = decltype(U() - T());
            if (!std::is_same<restype, T>::value) {
                throw std::invalid_argument("The datatypes don't match!!");
            }
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            size_t other_r = other.rows();
            size_t other_c = other.cols();
            if (self_r != other_r || self_c != other_c) {
                throw std::invalid_argument("The shape of the Matrices do not match!!");
            }
            for (size_t i = 0; i < self_r*self_c; i++) {
                this->m_data[i] -= other[i];
            }
            return *this;
        }

        template<typename U>
        auto operator*(const U& value) const -> Matrix<decltype(U() * T())>* {
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            using restype = decltype(U() * T());
            Matrix<restype>* res = new Matrix<restype>(self_r, self_c);
            for (size_t i = 0; i < self_r*self_c; i++) {
                (*res)[i] = this->m_data[i] * value;
            }
            return res;
        }

        template<typename U>
        auto operator*(const Matrix<U>& other) const -> Matrix<decltype(U() * T())>* {
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            size_t other_r = other.rows();
            size_t other_c = other.cols();
            if (self_r != other_r || self_c != other_c) {
                throw std::invalid_argument("The shape of the Matrices do not match!!");
            }
            using restype = decltype(U() * T());
            Matrix<restype>* res = new Matrix<restype>(self_r, self_c);
            for (size_t i = 0; i < self_r*self_c; i++) {
                (*res)[i] = this->m_data[i] * other[i];
            }
            return res;
        }

        template<typename U>
        Matrix<T>& operator*=(const U& value) {
            using restype = decltype(T() * U());
            if (!std::is_same<restype, T>::value) {
                throw std::invalid_argument("The datatypes don't match!!");
            }
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            for (size_t i = 0; i < self_r*self_c; i++) {
                this->m_data[i] *= value;
            }
            return *this;
        }

        template<typename U>
        Matrix<T>& operator*=(const Matrix<U>& other) {
            using restype = decltype(U() * T());
            if (!std::is_same<restype, T>::value) {
                throw std::invalid_argument("The datatypes don't match!!");
            }
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            size_t other_r = other.rows();
            size_t other_c = other.cols();
            if (self_r != other_r || self_c != other_c) {
                throw std::invalid_argument("The shape of the Matrices do not match!!");
            }
            for (size_t i = 0; i < self_r*self_c; i++) {
                this->m_data[i] *= other[i];
            }
            return *this;
        }

        template<typename U>
        auto operator/(const U& value) const -> Matrix<decltype(T() / U())>* {
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            using restype = decltype(T() / U());
            Matrix<restype>* res = new Matrix<restype>(self_r, self_c);
            for (size_t i = 0; i < self_r*self_c; i++) {
                (*res)[i] = this->m_data[i] / value;
            }
            return res;
        }

        template<typename U>
        auto operator/(const Matrix<U>& other) const -> Matrix<decltype(T() / U())>* {
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            size_t other_r = other.rows();
            size_t other_c = other.cols();
            if (self_r != other_r || self_c != other_c) {
                throw std::invalid_argument("The shape of the Matrices do not match!!");
            }
            using restype = decltype(T() / U());
            Matrix<restype>* res = new Matrix<restype>(self_r, self_c);
            for (size_t i = 0; i < self_r*self_c; i++) {
                (*res)[i] = this->m_data[i] / other[i];
            }
            return res;
        }

        template<typename U>
        Matrix<T>& operator/=(const U& value) {
            using restype = decltype(U() / T());
            if (!std::is_same<restype, T>::value) {
                throw std::invalid_argument("The datatypes don't match!!");
            }
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            for (size_t i = 0; i < self_r*self_c; i++) {
                this->m_data[i] /= value;
            }
            return *this;
        }

        template<typename U>
        Matrix<T>& operator/=(const Matrix<U>& other) {
            using restype = decltype(T() / U());
            if (!std::is_same<restype, T>::value) {
                throw std::invalid_argument("The datatypes don't match!!");
            }
            size_t self_r = this->rows();
            size_t self_c = this->cols();
            size_t other_r = other.rows();
            size_t other_c = other.cols();
            if (self_r != other_r || self_c != other_c) {
                throw std::invalid_argument("The shape of the Matrices do not match!!");
            }
            for (size_t i = 0; i < self_r*self_c; i++) {
                this->m_data[i] /= other[i];
            }
            return *this;
        }

        const T* data() const;
        
        T* data();

        ~Matrix();
};

#endif