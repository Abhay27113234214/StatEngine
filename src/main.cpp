#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "StatArray.h"
#include <pybind11/numpy.h>

namespace py = pybind11;
using std::vector;
using std::string;

template <typename t>
void bind_class(py::module_& m, const char* class_name) {
    py::class_<StatArray<t>>(m, class_name, py::buffer_protocol())
        .def(py::init<int>())
        .def(py::init([](py::array_t<t>& np_arr) {
            py::buffer_info info = np_arr.request();
            return new StatArray<t>(static_cast<t*>(info.ptr), info.size, false);
        }), py::return_value_policy::take_ownership, py::keep_alive<0, 1>())
        .def("copy", [](const StatArray<t>& self) {
            return new StatArray<t>(self); 
        }, py::return_value_policy::take_ownership)
        .def("__copy__", [](const StatArray<t>& self) {
            return new StatArray<t>(self);
        }, py::return_value_policy::take_ownership)
        .def("__deepcopy__", [](const StatArray<t>& self, py::dict) {
            return new StatArray<t>(self);
        }, py::return_value_policy::take_ownership)
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
        .def("kurtosis", &StatArray<t>::kurtosis, py::arg("bias") = true)
        .def_buffer([](StatArray<t>& arr) -> py::buffer_info {
            return py::buffer_info(
                arr._data(),
                sizeof(t),
                py::format_descriptor<t>::format(),
                1,
                { arr.getSize() },
                { sizeof(t) }
            );
        });
}

py::object StatArray_factory_list(const py::list& ls) {
    bool decimal = false;
    for (auto i : ls) {
        if (py::isinstance<py::float_>(i)) {
            decimal = true;
            break;
        }
    }
    if (decimal) {
        size_t list_size = ls.size();
        double* my_arr = new double[list_size];
        for (size_t i = 0; i < list_size; i++) {
            my_arr[i] = ls[i].cast<double>();
        }
        StatArray<double>* arr = new StatArray<double>(my_arr, list_size, true);
        return py::cast(arr);
    }
    size_t list_size = ls.size();
    int* my_arr = new int[list_size];
    for (size_t i = 0; i < list_size; i++) {
        my_arr[i] = ls[i].cast<int>();
    }
    StatArray<int>* arr = new StatArray<int>(my_arr, list_size, true);
    return py::cast(arr);
}

template<typename t>
py::object StatArray_factory_buffer(const py::array_t<t>& np_arr) {
    void* ptr = np_arr.request().ptr;
    size_t size = np_arr.request().size;
    if (np_arr.dtype().is(py::dtype::of<int>())) {
        StatArray<int>* arr = new StatArray<int>(static_cast<int*>(ptr), size, false);
        return py::cast(arr);
    } else if (np_arr.dtype().is(py::dtype::of<double>())) {
        StatArray<double>* arr = new StatArray<double>(static_cast<double*>(ptr), size, false);
        return py::cast(arr);
    } else if (np_arr.dtype().is(py::dtype::of<long long>())) {
        StatArray<long long>* arr = new StatArray<long long>(static_cast<long long*>(ptr), size, false);
        return py::cast(arr);
    } else {
        throw std::invalid_argument("Invalid value for the Argument!!");
    }
}

PYBIND11_MODULE(stats, m, py::mod_gil_not_used()) {
    bind_class<int>(m, "_StatArrayInt");
    bind_class<double>(m, "_StatArrayDouble");
    bind_class<long long>(m, "_StatArrayLong");
    m.def("StatArray", &StatArray_factory_list, py::return_value_policy::take_ownership);
    m.def("StatArray", &StatArray_factory_buffer<int>, py::return_value_policy::take_ownership, py::keep_alive<0, 1>());
    m.def("StatArray", &StatArray_factory_buffer<double>, py::return_value_policy::take_ownership, py::keep_alive<0, 1>());
    m.def("StatArray", &StatArray_factory_buffer<long long>, py::return_value_policy::take_ownership, py::keep_alive<0, 1>());
}   