#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "StatArray.h"

namespace py = pybind11;
using std::vector;
using std::string;

PYBIND11_MODULE(stats, m, py::mod_gil_not_used()) {
    py::class_<StatArray>(m, "StatArray")
        .def(py::init<const vector<double>&>())
        .def("append", &StatArray::append)
        .def_property_readonly("size", &StatArray::getSize)
        .def("__repr__", [](const StatArray& arr) -> string {
            string res = "";
            for (size_t i = 0; i < arr.getSize(); i++) {
                string num = std::to_string(arr[i]);
                res += num + " ";
            }
            return res;
        })
        .def("__getitem__", [](const StatArray& arr, size_t index) -> double {
            if (index >= arr.getSize() || index < 0) {
                throw py::index_error();
            }
            return arr[index];
        })
        .def("__setitem__", [](StatArray& arr, size_t index, double val) {
            if (index >= arr.getSize() || index < 0) {
                throw py::index_error();
            }
            arr[index] = val;
            arr.invalidate_cache();
        })
        .def("mean", &StatArray::mean)
        .def("std", &StatArray::std, py::arg("ddof") = 0)
        .def("var", &StatArray::variance, py::arg("ddof") = 0)
        .def("skew", &StatArray::skew, py::arg("bias") = true)
        .def("kurtosis", &StatArray::kurtosis, py::arg("bias") = true);
}