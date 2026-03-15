#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>
#include "StatArray.h"
#include <pybind11/numpy.h>

namespace py = pybind11;
using std::string;

void init_stat_array(py::module_& m);
void init_matrix(py::module_& m);

PYBIND11_MODULE(stats, m, py::mod_gil_not_used()) {

    m.doc() = "Core C++ backend for the math engine";
    init_stat_array(m);
    init_matrix(m);
    
}