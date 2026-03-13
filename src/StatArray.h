#ifndef STATARRAY_H
#define STATARRAY_H
#include <vector>
#include <type_traits>

using std::vector;

template<typename T>
class StatArray {
    private:
        size_t size;
        bool owns_memory;
        T* m_data;
        mutable double mean_;

        // helpers
        double calculate_moment(int power);

    public:
        StatArray(int number_of_elements);

        StatArray(T* data, size_t size, bool owns_memory);

        StatArray(const StatArray<T>& st);

        void invalidate_cache();

        size_t getSize() const;

        T operator[](int index) const;

        T& operator[](int index);

        template<typename U>
        auto operator+(const StatArray<U>& other) const -> StatArray<decltype(U() + T())>* {
            size_t self_s = this->getSize();
            size_t other_s = other.getSize();
            if (self_s != other_s) {
                throw std::invalid_argument("The StatArrays are not of equal length!!");
            }
            using res_type = decltype(U() + T());
            StatArray<res_type>* res = new StatArray<res_type>(self_s);
            for (size_t i = 0; i < self_s; i++) {
                (*res)[i] = this->m_data[i] + other[i];
            }
            return res;
        }

        template<typename U>
        auto operator+(const U& value) const -> StatArray<decltype(U() + T())>* {
            size_t self_s = this->getSize();
            using res_type = decltype(U() + T());
            StatArray<res_type>* res = new StatArray<res_type>(self_s);
            for (size_t i = 0; i < self_s; i++) {
                (*res)[i] = this->m_data[i] + value;
            }
            return res;
        }

        template<typename U>
        auto operator-(const StatArray<U>& other) const -> StatArray<decltype(U() - T())>* {
            size_t self_s = this->getSize();
            size_t other_s = other.getSize();
            if (self_s != other_s) {
                throw std::invalid_argument("The StatArrays are not of equal length!!");
            }
            using res_type = decltype(U() - T());
            StatArray<res_type>* res = new StatArray<res_type>(self_s);
            for (size_t i = 0; i < self_s; i++) {
                (*res)[i] = this->m_data[i] - other[i];
            }
            return res;
        }

        template<typename U>
        auto operator-(const U& value) const -> StatArray<decltype(U() - T())>* {
            size_t self_s = this->getSize();
            using res_type = decltype(U() - T());
            StatArray<res_type>* res = new StatArray<res_type>(self_s);
            for (size_t i = 0; i < self_s; i++) {
                (*res)[i] = this->m_data[i] - value;
            }
            return res;
        }

        template<typename U>
        auto operator*(const StatArray<U>& other) const -> StatArray<decltype(U() * T())>* {
            size_t self_s = this->getSize();
            size_t other_s = other.getSize();
            if (self_s != other_s) {
                throw std::invalid_argument("StatArrays are not of equal length!!");
            }
            using res_type = decltype(U() * T());
            StatArray<res_type>* res = new StatArray<res_type>(self_s);
            for (size_t i = 0; i < self_s; i++) {
                (*res)[i] = this->m_data[i] * other[i];
            }
            return res;
        }

        template<typename U>
        auto operator*(const U& value) const -> StatArray<decltype(U() * T())>* {
            size_t self_s = this->getSize();
            using res_type = decltype(U() * T());
            StatArray<res_type>* res = new StatArray<res_type>(self_s);
            for (size_t i = 0; i < self_s; i++) {
                (*res)[i] = this->m_data[i] * value;
            }
            return res;
        }

        template<typename U>
        auto operator/(const StatArray<U>& other) const -> StatArray<decltype(U() / T())>* {
            size_t self_s = this->getSize();
            size_t other_s = other.getSize();
            if (self_s != other_s) {
                throw std::invalid_argument("StatArrays are not of equal length!!");
            }
            using res_type = decltype(U() / T());
            StatArray<res_type>* res = new StatArray<res_type>(self_s);
            for (size_t i = 0; i < self_s; i++) {
                (*res)[i] = this->m_data[i] / other[i];
            }
            return res;
        }

        template<typename U>
        auto operator/(const U& value) const -> StatArray<decltype(U() / T())>* {
            size_t self_s = this->getSize();
            using res_type = decltype(U() / T());
            StatArray<res_type>* res = new StatArray<res_type>(self_s);
            for (size_t i = 0; i < self_s; i++) {
                (*res)[i] = this->m_data[i] / value;
            }
            return res;
        }

        template<typename U>
        StatArray<T>& operator+=(const StatArray<U>& other) {
            using res_type = decltype(U() + T());
            if (!std::is_same<res_type, T>::value) {
                throw std::invalid_argument("The datatypes don't match!!");
            }
            size_t self_s = this->getSize();
            size_t other_s = other.getSize();
            if (self_s != other_s) {
                throw std::invalid_argument("The StatArrays are not of equal length!!");
            }
            for (size_t i = 0; i < self_s; i++) {
                this->m_data[i] += other[i];
            }
            return *this;
        }

        template<typename U>
        StatArray<T>& operator+=(const U& value) {
            using res_type = decltype(U() + T());
            if (!std::is_same<T, res_type>::value) {
                throw std::invalid_argument("The StatArrays are not of the same length!!");
            }
            size_t self_s = this->getSize();
            for (size_t i = 0; i < self_s; i++) {
                this->m_data[i] += value;
            }
            return *this;
        }

        template<typename U>
        StatArray<T>& operator-=(const StatArray<U>& other) {
            using res_type = decltype(U() - T());
            if (!std::is_same<res_type, T>::value) {
                throw std::invalid_argument("The datatypes don't match!!");
            }
            size_t self_s = this->getSize();
            size_t other_s = other.getSize();
            if (self_s != other_s) {
                throw std::invalid_argument("The StatArrays are not of same size!!");
            }
            for (size_t i = 0; i < self_s; i++) {
                this->m_data[i] -= other[i];
            }
            return *this;
        }

        template<typename U>
        StatArray<T>& operator-=(const U& value) {
            using res_type = decltype(U() - T());
            if (!std::is_same<T, res_type>::value) {
                throw std::invalid_argument("The StatArrays are not of the same length!!");
            }
            size_t self_s = this->getSize();
            for (size_t i = 0; i < self_s; i++) {
                this->m_data[i] -= value;
            }
            return *this;
        }

        template<typename U>
        StatArray<T>& operator*=(const StatArray<U>& other) {
            using res_type = decltype(U() * T());
            if (!std::is_same<T, res_type>::value) {
                throw std::invalid_argument("The StatArrays are not of the same length!!");
            }
            size_t self_s = this->getSize();
            size_t other_s = other.getSize();
            if (self_s != other_s) {
                throw std::invalid_argument("The StatArrays are not of same size!!");
            }
            for (size_t i = 0; i < self_s; i++) {
                this->m_data[i] *= other[i];
            }
            return *this;
        }

        template<typename U>
        StatArray<T>& operator*=(const U& value) {
            using res_type = decltype(U() * T());
            if (!std::is_same<T, res_type>::value) {
                throw std::invalid_argument("The StatArrays are not of the same length!!");
            }
            size_t self_s = this->getSize();
            for (size_t i = 0; i < self_s; i++) {
                this->m_data[i] *= value;
            }
            return *this;
        }

        template<typename U>
        StatArray<T>& operator/=(const StatArray<U>& other) {
            using res_type = decltype(U() / T());
            if (!std::is_same<T, res_type>::value) {
                throw std::invalid_argument("The StatArrays are not of the same length!!");
            }
            size_t self_s = this->getSize();
            size_t other_s = other.getSize();
            if (self_s != other_s) {
                throw std::invalid_argument("The StatArrays are not of same size!!");
            }
            for (size_t i = 0; i < self_s; i++) {
                this->m_data[i] /= other[i];
            }
            return *this;
        }

        template<typename U>
        StatArray<T>& operator/=(const U& value) {
            using res_type = decltype(U() / T());
            if (!std::is_same<T, res_type>::value) {
                throw std::invalid_argument("The StatArrays are not of the same length!!");
            }
            size_t self_s = this->getSize();
            for (size_t i = 0; i < self_s; i++) {
                this->m_data[i] /= value;
            }
            return *this;
        }

        double mean(bool re_calculate = false) const;

        double std(int ddof = 0);

        double variance(int ddof = 0);

        double median();

        double skew(bool bias = true);

        double kurtosis(bool bias = true);

        T* _data();

        const T* _data() const;

        ~StatArray();
};


#endif