#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "StatArray.h"
#include <pybind11/numpy.h>
#include <stdexcept>

namespace py = pybind11;
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
        .def("__getitem__", [](StatArray<t>& self, py::slice slice) {
            size_t start, stop, step, slice_length;
            if (!slice.compute(self.getSize(), &start, &stop, &step, &slice_length)) {
                throw py::error_already_set();
            }
            if (step != 1) {
                throw std::invalid_argument("Slice step must be 1. Strided memory is not supported yet!!");
            }
            t* data = self._data() + start;
            StatArray<t>* res = new StatArray<t>(data, slice_length, false);
            return res;
        }, py::return_value_policy::take_ownership, py::keep_alive<0, 1>())
        .def("__setitem__", [](StatArray<t>& arr, int index, double val) {
            arr[index] = val;
            arr.invalidate_cache();
        })
        .def("__len__", [](StatArray<t>& arr) {
            return arr.getSize();
        })
        .def("__add__", [](StatArray<t>& self, const StatArray<int>& other) {
            return self + other;
        }, py::return_value_policy::take_ownership)
        .def("__add__", [](StatArray<t>& self, const StatArray<double>& other) {
            return self + other;
        }, py::return_value_policy::take_ownership)
        .def("__add__", [](StatArray<t>& self, const StatArray<long long>& other) {
            return self + other;
        }, py::return_value_policy::take_ownership)
        .def("__add__", [](StatArray<t>& self, int value) {
            return self + value;
        }, py::return_value_policy::take_ownership)
        .def("__add__", [](StatArray<t>& self, double value) {
            return self + value;
        }, py::return_value_policy::take_ownership)
        .def("__add__", [](StatArray<t>& self, long long value) {
            return self + value;
        }, py::return_value_policy::take_ownership)
        .def("__iadd__", [](StatArray<t>& self, const StatArray<int>& other) {
            return self += other;
        }, py::return_value_policy::reference_internal)
        .def("__iadd__", [](StatArray<t>& self, const StatArray<double>& other) {
            return self += other;
        }, py::return_value_policy::reference_internal)
        .def("__iadd__", [](StatArray<t>& self, const StatArray<long long>& other) {
            return self += other;
        }, py::return_value_policy::reference_internal)
        .def("__iadd__", [](StatArray<t>& self, const int& value) {
            return self += value;
        }, py::return_value_policy::reference_internal)
        .def("__iadd__", [](StatArray<t>& self, const double& value) {
            return self += value;
        }, py::return_value_policy::reference_internal)
        .def("__iadd__", [](StatArray<t>& self, const long long& value) {
            return self += value;
        }, py::return_value_policy::reference_internal)
        .def("__sub__", [](StatArray<t>& self, const StatArray<int>& other) {
            return self - other;
        }, py::return_value_policy::take_ownership)
        .def("__sub__", [](StatArray<t>& self, const StatArray<double>& other) {
            return self - other;
        }, py::return_value_policy::take_ownership)
        .def("__sub__", [](StatArray<t>& self, const StatArray<long long>& other) {
            return self - other;
        }, py::return_value_policy::take_ownership)
        .def("__sub__", [](StatArray<t>& self, int val) {
            return self - val;
        }, py::return_value_policy::take_ownership)
        .def("__sub__", [](StatArray<t>& self, double val) {
            return self - val;
        }, py::return_value_policy::take_ownership)
        .def("__sub__", [](StatArray<t>& self, long long val) {
            return self - val;
        }, py::return_value_policy::take_ownership)
        .def("__isub__", [](StatArray<t>& self, const StatArray<int>& other) {
            return self -= other;
        }, py::return_value_policy::reference_internal)
        .def("__isub__", [](StatArray<t>& self, const StatArray<double>& other) {
            return self -= other;
        }, py::return_value_policy::reference_internal)
        .def("__isub__", [](StatArray<t>& self, const StatArray<long long>& other) {
            return self -= other;
        }, py::return_value_policy::reference_internal)
        .def("__isub__", [](StatArray<t>& self, const int& value) {
            return self -= value;
        }, py::return_value_policy::reference_internal)
        .def("__isub__", [](StatArray<t>& self, const double& value) {
            return self -= value;
        }, py::return_value_policy::reference_internal)
        .def("__isub__", [](StatArray<t>& self, const long long& value) {
            return self -= value;
        }, py::return_value_policy::reference_internal)
        .def("__mul__", [](StatArray<t>& self, const StatArray<int>& other) {
            return self * other;
        }, py::return_value_policy::take_ownership)
        .def("__mul__", [](StatArray<t>& self, const StatArray<double>& other) {
        return self * other;
        }, py::return_value_policy::take_ownership)
        .def("__mul__", [](StatArray<t>& self, const StatArray<long long>& other) {
            return self * other;
        }, py::return_value_policy::take_ownership)
        .def("__mul__", [](StatArray<t>& self, int val) {
            return self * val;
        }, py::return_value_policy::take_ownership)
        .def("__mul__", [](StatArray<t>& self, double val) {
            return self * val;
        }, py::return_value_policy::take_ownership)
        .def("__mul__", [](StatArray<t>& self, long long val) {
            return self * val;
        }, py::return_value_policy::take_ownership)
        .def("__imul__", [](StatArray<t>& self, const StatArray<int>& other) {
            return self *= other;
        }, py::return_value_policy::reference_internal)
        .def("__imul__", [](StatArray<t>& self, const StatArray<double>& other) {
            return self *= other;
        }, py::return_value_policy::reference_internal)
        .def("__imul__", [](StatArray<t>& self, const StatArray<long long>& other) {
            return self *= other;
        }, py::return_value_policy::reference_internal)
        .def("__imul__", [](StatArray<t>& self, const int& value) {
            return self *= value;
        }, py::return_value_policy::reference_internal)
        .def("__imul__", [](StatArray<t>& self, const double& value) {
            return self *= value;
        }, py::return_value_policy::reference_internal)
        .def("__imul__", [](StatArray<t>& self, const long long& value) {
            return self *= value;
        }, py::return_value_policy::reference_internal)
        .def("__truediv__", [](StatArray<t>& self, const StatArray<int>& other) {
            return self / other;
        }, py::return_value_policy::take_ownership)
        .def("__truediv__", [](StatArray<t>& self, const StatArray<double>& other) {
        return self / other;
        }, py::return_value_policy::take_ownership)
        .def("__truediv__", [](StatArray<t>& self, const StatArray<long long>& other) {
            return self / other;
        }, py::return_value_policy::take_ownership)
        .def("__truediv__", [](StatArray<t>& self, int val) {
            return self / val;
        }, py::return_value_policy::take_ownership)
        .def("__truediv__", [](StatArray<t>& self, double val) {
            return self / val;
        }, py::return_value_policy::take_ownership)
        .def("__truediv__", [](StatArray<t>& self, long long val) {
            return self / val;
        }, py::return_value_policy::take_ownership)
        .def("__itruediv__", [](StatArray<t>& self, const StatArray<int>& other) {
            return self /= other;
        }, py::return_value_policy::reference_internal)
        .def("__itruediv__", [](StatArray<t>& self, const StatArray<double>& other) {
            return self /= other;
        }, py::return_value_policy::reference_internal)
        .def("__itruediv__", [](StatArray<t>& self, const StatArray<long long>& other) {
            return self /= other;
        }, py::return_value_policy::reference_internal)
        .def("__itruediv__", [](StatArray<t>& self, const int& value) {
            return self /= value;
        }, py::return_value_policy::reference_internal)
        .def("__itruediv__", [](StatArray<t>& self, const double& value) {
            return self /= value;
        }, py::return_value_policy::reference_internal)
        .def("__itruediv__", [](StatArray<t>& self, const long long& value) {
            return self /= value;
        }, py::return_value_policy::reference_internal)
        .def("__neg__", [](StatArray<t>& self) {
            return self * -1;
        }, py::return_value_policy::take_ownership)
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
    py::buffer_info info = np_arr.request();
    void* ptr = info.ptr;
    if (info.readonly) {
        throw std::invalid_argument("StatEngine cannot take ownership of a read-only NumPy array!");
    }
    size_t size = info.size;
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

void init_stat_array(py::module_& m) {
    bind_class<int>(m, "_StatArrayInt");
    bind_class<double>(m, "_StatArrayDouble");
    bind_class<long long>(m, "_StatArrayLong");
    m.def("StatArray", &StatArray_factory_list, py::return_value_policy::take_ownership);
    m.def("StatArray", &StatArray_factory_buffer<int>, py::return_value_policy::take_ownership, py::keep_alive<0, 1>());
    m.def("StatArray", &StatArray_factory_buffer<double>, py::return_value_policy::take_ownership, py::keep_alive<0, 1>());
    m.def("StatArray", &StatArray_factory_buffer<long long>, py::return_value_policy::take_ownership, py::keep_alive<0, 1>());
}   