#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "Matrix.h"
#include <pybind11/numpy.h>

namespace py = pybind11;

template<typename T>
void bind_class(py::module_ m, const char* class_name) {
    py::class_<Matrix<T>>(m, class_name, py::buffer_protocol())
        .def(py::init<size_t,size_t>())
        .def(py::init([](py::array_t<T>& arr) {
            py::buffer_info info = arr.request();
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
        .def_property_readonly("size", &Matrix<T>::size, py::return_value_policy::reference_internal)
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

void init_matrix(py::module_& m) {
    bind_class<int>(m, "_MatrixInt");
    bind_class<double>(m, "_MatrixDouble");
    bind_class<long long>(m, "_MatrixLong");
}