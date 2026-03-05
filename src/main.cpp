#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "StatArray.h"

namespace py = pybind11;
using std::vector;
using std::string;

template <typename t>
void bind_class(py::module_& m, const char* class_name) {
    py::class_<StatArray<t>>(m, class_name)
        .def(py::init<const vector<t>&>())
        .def("append", &StatArray<t>::append)
        .def_property_readonly("size", &StatArray<t>::getSize, py::return_value_policy::reference_internal)
        .def("__repr__", [](const StatArray<t>& arr) -> string {
            string res = "";
            for (size_t i = 0; i < arr.getSize(); i++) {
                string num = std::to_string(arr[i]);
                res += num + " ";
            }
            return res;
        })
        .def("__getitem__", [](const StatArray<t>& arr, int index) -> t {
            return arr[index];
        })
        .def("__setitem__", [](StatArray<t>& arr, int index, double val) {
            arr[index] = val;
            arr.invalidate_cache();
        })
        .def("__len__", [](StatArray<t>& arr) {
            return arr.getSize();
        })
        .def("mean", &StatArray<t>::mean, py::arg("re_calculate") = false)
        .def("std", &StatArray<t>::std, py::arg("ddof") = 0)
        .def("var", &StatArray<t>::variance, py::arg("ddof") = 0)
        .def("median", &StatArray<t>::median)
        .def("skew", &StatArray<t>::skew, py::arg("bias") = true)
        .def("kurtosis", &StatArray<t>::kurtosis, py::arg("bias") = true);
}

py::object StatArray_factory(const py::list& ls) {
    bool decimal = false;
    for (auto i : ls) {
        if (py::isinstance<py::float_>(i)) {
            decimal = true;
            break;
        }
    }
    if (decimal) {
        StatArray<double>* st = new StatArray<double>(ls.cast<vector<double>>());
        return py::cast(st);   
    }
    StatArray<int>* st = new StatArray<int>(ls.cast<vector<int>>());
    return py::cast(st);
}

PYBIND11_MODULE(stats, m, py::mod_gil_not_used()) {
    bind_class<int>(m, "_StatArrayInt");
    bind_class<double>(m, "_StatArrayDouble");
    m.def("StatArray", &StatArray_factory, py::return_value_policy::take_ownership);
}   