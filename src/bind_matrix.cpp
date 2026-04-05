#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "Matrix.h"
#include <pybind11/numpy.h>

namespace py = pybind11;
using std::string;

template<typename T>
void bind_class_matrix(py::module_ m, const char* class_name) {
    py::class_<Matrix<T>>(m, class_name, py::buffer_protocol())
        .def(py::init<size_t,size_t>())
        .def(py::init([](py::array_t<T>& arr) {
            py::buffer_info info = arr.request();
            int dims = info.ndim;
            if (dims != 2) {
                throw std::invalid_argument("NumPy array of invalid number of dimensions passed!!");
            }
            size_t rows = info.shape[0];
            size_t cols = info.shape[1];
            T* data = static_cast<T*>(info.ptr);
            return new Matrix<T>(data, rows, cols, false);
        }), py::return_value_policy::take_ownership, py::keep_alive<0, 1>())
        .def("copy", [](const Matrix<T>& self) {
            return new Matrix<T>(self);
        }, py::return_value_policy::take_ownership)
        .def("__copy__", [](const Matrix<T>& self) {
            return new Matrix<T>(self);
        }, py::return_value_policy::take_ownership)
        .def("__deepcopy__", [](const Matrix<T>& self) {
            return new Matrix<T>(self);
        }, py::return_value_policy::take_ownership)
        .def("__repr__", [](const Matrix<T>& self) -> string {
            string res = "";
            size_t rows = self.rows();
            size_t cols = self.cols();
            for (size_t i = 0; i < rows; i++) {
                for (size_t j = 0; j < cols; j++) {
                    res += std::to_string(self[i*cols + j]) + " ";
                }
                if (i != rows - 1) {
                    res += "\n";
                }
            }
            return res;
        })
        .def("__getitem__", [](const Matrix<T>& self, std::pair<int, int> index) {
            int self_r = static_cast<int>(self.rows());
            int self_c = static_cast<int>(self.cols());
            int i = index.first;
            int j = index.second;
            if (i >= self_r || j >= self_c) {
                throw std::out_of_range("Out of bounds access!");
            }
            return self[i*self_c+j];
        })
        .def("__setitem__", [](Matrix<T>& self, std::pair<int, int> index, T value) {
            int self_r = static_cast<int>(self.rows());
            int self_c = static_cast<int>(self.cols());
            int i = index.first;
            int j = index.second;
            if (i >= self_r || j >= self_c) {
                throw std::out_of_range("Out of bounds access!");
            }
            self[i*self_c+j] = value;
        })
        .def_property_readonly("size", &Matrix<T>::size, py::return_value_policy::reference_internal)
        .def("__add__", [](const Matrix<T>& self, int value) {
            return self + value;
        }, py::return_value_policy::take_ownership)
        .def("__add__", [](const Matrix<T>& self, double value) {
            return self + value;
        }, py::return_value_policy::take_ownership)
        .def("__add__", [](const Matrix<T>& self, long long value) {
            return self + value;
        }, py::return_value_policy::take_ownership)
        .def("__add__", [](const Matrix<T>& self, const Matrix<int>& other) {
            return self + other;
        }, py::return_value_policy::take_ownership)
        .def("__add__", [](const Matrix<T>& self, const Matrix<double>& other) {
            return self + other;
        }, py::return_value_policy::take_ownership)
        .def("__add__", [](const Matrix<T>& self, const Matrix<long long>& other) {
            return self + other;
        }, py::return_value_policy::take_ownership)
        // -------------------------------------------------------------------------------
        .def("__iadd__", [](Matrix<T>& self, int value) {
            return self += value;
        }, py::return_value_policy::reference_internal)
        .def("__iadd__", [](Matrix<T>& self, double value) {
            return self += value;
        }, py::return_value_policy::reference_internal)
        .def("__iadd__", [](Matrix<T>& self, long long value) {
            return self += value;
        }, py::return_value_policy::reference_internal)
        .def("__iadd__", [](Matrix<T>& self, const Matrix<int>& other) {
            return self += other;
        }, py::return_value_policy::reference_internal)
        .def("__iadd__", [](Matrix<T>& self, const Matrix<double>& other) {
            return self += other;
        }, py::return_value_policy::reference_internal)
        .def("__iadd__", [](Matrix<T>& self, const Matrix<long long>& other) {
            return self += other;
        }, py::return_value_policy::reference_internal)
        // -------------------------------------------------------------------------------
        .def("__sub__", [](const Matrix<T>& self, int value) {
            return self - value;
        }, py::return_value_policy::take_ownership)
        .def("__sub__", [](const Matrix<T>& self, double value) {
            return self - value;
        }, py::return_value_policy::take_ownership)
        .def("__sub__", [](const Matrix<T>& self, long long value) {
            return self - value;
        }, py::return_value_policy::take_ownership)
        .def("__sub__", [](const Matrix<T>& self, const Matrix<int>& other) {
            return self - other;
        }, py::return_value_policy::take_ownership)
        .def("__sub__", [](const Matrix<T>& self, const Matrix<double>& other) {
            return self - other;
        }, py::return_value_policy::take_ownership)
        .def("__sub__", [](const Matrix<T>& self, const Matrix<long long>& other) {
            return self - other;
        }, py::return_value_policy::take_ownership)
        // -------------------------------------------------------------------------------
        .def("__isub__", [](Matrix<T>& self, int value) {
            return self -= value;
        }, py::return_value_policy::reference_internal)
        .def("__isub__", [](Matrix<T>& self, double value) {
            return self -= value;
        }, py::return_value_policy::reference_internal)
        .def("__isub__", [](Matrix<T>& self, long long value) {
            return self -= value;
        }, py::return_value_policy::reference_internal)
        .def("__isub__", [](Matrix<T>& self, const Matrix<int>& other) {
            return self -= other;
        }, py::return_value_policy::reference_internal)
        .def("__isub__", [](Matrix<T>& self, const Matrix<double>& other) {
            return self -= other;
        }, py::return_value_policy::reference_internal)
        .def("__isub__", [](Matrix<T>& self, const Matrix<long long>& other) {
            return self -= other;
        }, py::return_value_policy::reference_internal)
        // -------------------------------------------------------------------------------
        .def("__mul__", [](const Matrix<T>& self, int value) {
            return self * value;
        }, py::return_value_policy::take_ownership)
        .def("__mul__", [](const Matrix<T>& self, double value) {
            return self * value;
        }, py::return_value_policy::take_ownership)
        .def("__mul__", [](const Matrix<T>& self, long long value) {
            return self * value;
        }, py::return_value_policy::take_ownership)
        .def("__mul__", [](const Matrix<T>& self, const Matrix<int>& other) {
            return self * other;
        }, py::return_value_policy::take_ownership)
        .def("__mul__", [](const Matrix<T>& self, const Matrix<double>& other) {
            return self * other;
        }, py::return_value_policy::take_ownership)
        .def("__mul__", [](const Matrix<T>& self, const Matrix<long long>& other) {
            return self * other;
        }, py::return_value_policy::take_ownership)
        // -------------------------------------------------------------------------------
        .def("__imul__", [](Matrix<T>& self, int value) {
            return self *= value;
        }, py::return_value_policy::reference_internal)
        .def("__imul__", [](Matrix<T>& self, double value) {
            return self *= value;
        }, py::return_value_policy::reference_internal)
        .def("__imul__", [](Matrix<T>& self, long long value) {
            return self *= value;
        }, py::return_value_policy::reference_internal)
        .def("__imul__", [](Matrix<T>& self, const Matrix<int>& other) {
            return self *= other;
        }, py::return_value_policy::reference_internal)
        .def("__imul__", [](Matrix<T>& self, const Matrix<double>& other) {
            return self *= other;
        }, py::return_value_policy::reference_internal)
        .def("__imul__", [](Matrix<T>& self, const Matrix<long long>& other) {
            return self *= other;
        }, py::return_value_policy::reference_internal)
        // -------------------------------------------------------------------------------
        .def("__truediv__", [](const Matrix<T>& self, int value) {
            return self / value;
        }, py::return_value_policy::take_ownership)
        .def("__truediv__", [](const Matrix<T>& self, double value) {
            return self / value;
        }, py::return_value_policy::take_ownership)
        .def("__truediv__", [](const Matrix<T>& self, long long value) {
            return self / value;
        }, py::return_value_policy::take_ownership)
        .def("__truediv__", [](const Matrix<T>& self, const Matrix<int>& other) {
            return self / other;
        }, py::return_value_policy::take_ownership)
        .def("__truediv__", [](const Matrix<T>& self, const Matrix<double>& other) {
            return self / other;
        }, py::return_value_policy::take_ownership)
        .def("__truediv__", [](const Matrix<T>& self, const Matrix<long long>& other) {
            return self / other;
        }, py::return_value_policy::take_ownership)
        // -------------------------------------------------------------------------------
        .def("__itruediv__", [](Matrix<T>& self, int value) {
            return self /= value;
        }, py::return_value_policy::reference_internal)
        .def("__itruediv__", [](Matrix<T>& self, double value) {
            return self /= value;
        }, py::return_value_policy::reference_internal)
        .def("__itruediv__", [](Matrix<T>& self, long long value) {
            return self /= value;
        }, py::return_value_policy::reference_internal)
        .def("__itruediv__", [](Matrix<T>& self, const Matrix<int>& other) {
            return self /= other;
        }, py::return_value_policy::reference_internal)
        .def("__itruediv__", [](Matrix<T>& self, const Matrix<double>& other) {
            return self /= other;
        }, py::return_value_policy::reference_internal)
        .def("__itruediv__", [](Matrix<T>& self, const Matrix<long long>& other) {
            return self /= other;
        }, py::return_value_policy::reference_internal)
        // -------------------------------------------------------------------------------
        // anytime you access a templated method or a templated variable of a class which is also a template, you must use the template keyword
        .def("dot", &Matrix<T>::template dot<int>, py::return_value_policy::take_ownership)
        .def("dot", &Matrix<T>::template dot<long long>, py::return_value_policy::take_ownership)
        .def("dot", &Matrix<T>::template dot<double>, py::return_value_policy::take_ownership)
        .def("__matmul__", [](const Matrix<T>& self, const Matrix<int>& other) {
            return self.dot(other);
        }, py::return_value_policy::take_ownership)
        .def("__matmul__", [](const Matrix<T>& self, const Matrix<double>& other) {
            return self.dot(other);
        }, py::return_value_policy::take_ownership)
        .def("__matmul__", [](const Matrix<T>& self, const Matrix<long long>& other) {
            return self.dot(other);
        }, py::return_value_policy::take_ownership)
        .def("mean", &Matrix<T>::mean, py::arg("axis") = 1, py::return_value_policy::take_ownership)
        .def("sum", &Matrix<T>::sum, py::arg("axis") = 1, py::return_value_policy::take_ownership)
        .def("var", &Matrix<T>::var, py::arg("axis") = 1, py::return_value_policy::take_ownership)
        .def("std", &Matrix<T>::std, py::arg("axis") = 1, py::return_value_policy::take_ownership)
        .def_buffer([](Matrix<T>& m) -> py::buffer_info {
            return py::buffer_info(
                m.data(),
                sizeof(T),
                py::format_descriptor<T>::format(),
                2,
                { m.rows(), m.cols() },
                { sizeof(T)*m.cols(), sizeof(T) }
            );
        });
}

// factory functions 
py::object bind_2dlist(const py::list& mat) {
    size_t rows = mat.size();
    size_t cols = (rows > 0) ? mat[0].cast<py::list>().size() : 0;
    bool decimal = false;
    for (auto row_element: mat) {
        auto row = row_element.cast<py::list>();
        for (auto ele : row) {
            if (py::isinstance<py::float_>(ele)) {
                decimal = true;
                break;
            }
        }
    }
    if (decimal) {
        double* my_mat = new double[rows * cols];
        int i = 0;
        int j = 0;
        for (auto row_element : mat) {
            auto row = row_element.cast<py::list>();
            for (auto ele : row) {
                my_mat[i*cols + j] = ele.cast<double>();
                j++;
            }
            i++;
            j = 0;
        }
        Matrix<double>* res = new Matrix<double>(my_mat, rows, cols, true);
        return py::cast(res);
    }
    int* my_mat = new int[rows * cols];
    int i = 0;
    int j = 0;
    for (auto row_element : mat) {
        auto row = row_element.cast<py::list>();
        for (auto ele: row) {
            my_mat[i * cols + j] = ele.cast<int>();
            j++;
        }
        i++;
        j = 0;
    }
    Matrix<int>* res = new Matrix<int>(my_mat, rows, cols, true);
    return py::cast(res);
}

template<typename T> 
py::object bind_npmat(const py::array_t<T>& np_mat) {
    py::buffer_info info = np_mat.request();
    if (info.readonly) {
        throw std::invalid_argument("StatEngine cannot take ownership of a read-only NumPy array!");
    }
    if (info.ndim != 2) {
        throw std::invalid_argument("Invalid number of dimensions for a Matrix!!");
    }
    size_t rows = info.shape[0];
    size_t cols = info.shape[1];
    void* ptr = info.ptr;
    if (np_mat.dtype().is(py::dtype::of<int>())) {
        Matrix<int>* res = new Matrix<int>(static_cast<int*>(ptr), rows, cols, false);
        return py::cast(res);
    } else if (np_mat.dtype().is(py::dtype::of<double>())) {
        Matrix<double>* res = new Matrix<double>(static_cast<double*>(ptr), rows, cols, false);
        return py::cast(res);
    } else if (np_mat.dtype().is(py::dtype::of<long long>())) {
        Matrix<long long>* res = new Matrix<long long>(static_cast<long long*>(ptr), rows, cols, false);
        return py::cast(res);
    } else {
        throw std::invalid_argument("NumPy array of invalid dtype passed!!!");
    }
}

void init_matrix(py::module_& m) {
    bind_class_matrix<int>(m, "_MatrixInt");
    bind_class_matrix<double>(m, "_MatrixDouble");
    bind_class_matrix<long long>(m, "_MatrixLong");
    m.def("Matrix", &bind_2dlist, py::return_value_policy::take_ownership);
    m.def("Matrix", &bind_npmat<int>, py::return_value_policy::take_ownership, py::keep_alive<0, 1>());
    m.def("Matrix", &bind_npmat<double>, py::return_value_policy::take_ownership, py::keep_alive<0, 1>());
    m.def("Matrix", &bind_npmat<long long>, py::return_value_policy::take_ownership, py::keep_alive<0, 1>());
}

