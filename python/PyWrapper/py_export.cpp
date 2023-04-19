#include <cstdint>
#include <typeinfo>

#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../../src/CGraph.h"
#include "../PyObject/CGraphPy.h"

#include "py_GNode.h"
#include "py_GElement.h"

namespace py = pybind11;
using namespace CGraph;

PYBIND11_MODULE(pyCGraph, m) {
    m.doc() = "Python interface for CGraph";

    py::class_<CStatus>(m, "CStatus")
        .def(py::init<>())
        .def("getCode", &CStatus::getCode)
        .def("isOK", &CStatus::isOK);


    py::class_<GPipelinePy, std::unique_ptr<GPipelinePy, py::nodelete>>(m, "GPipelinePy")
        .def(py::init<>())
        .def("init", &GPipelinePy::init)
        .def("run", &GPipelinePy::run, py::call_guard<py::gil_scoped_release>())
        .def("process", &GPipelinePy::process, py::call_guard<py::gil_scoped_release>())
        .def("destroy", &GPipelinePy::destroy)
        .def("dump", [](GPipelinePy& self) {
            return self.dump();
        })
        .def("registerGElement", &GPipelinePy::registerGElement,
            py::arg("elementRef"),
            py::arg("dependElements") = GElementPtrSet{},
            py::arg("name") = CGRAPH_EMPTY,
            py::arg("loop") = 1);

    py::class_<GElement, GElementPy, std::unique_ptr<GElement, py::nodelete> >(m, "GElement")
        .def(py::init<>())
        .def("init", &GElement::init)
        .def("run", &GElement::run)
        .def("destroy", &GElement::destroy);

    py::class_<GNode, GNodePy, GElement, std::unique_ptr<GNode, py::nodelete> >(m, "GNode")
        .def(py::init<>())
        .def("init", &GNode::init)
        .def("run", &GNode::run)
        .def("destroy", &GNode::destroy);
}
