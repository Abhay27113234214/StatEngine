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

        const T* data() const;
        
        T* data();

        ~Matrix();
};

#endif