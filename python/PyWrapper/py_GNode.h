#include <cstdint>
#include <typeinfo>

#include <Python.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "../../src/CGraph.h"
#include "../PyObject/CGraphPy.h"

namespace py = pybind11;
using namespace CGraph;

class GNodePy: public GNode {
public:
    /* Inherit the constructors */
    using GNode::GNode;

    CStatus init() override {
        PYBIND11_OVERRIDE_PURE(
            CStatus,    /* Return type */
            GNode,      /* Parent class */
            init,         /* Name of function in C++ (must match Python name) */
        );
    }

    /* Trampoline (need one for each virtual function) */
    CStatus run() override {
        PYBIND11_OVERRIDE_PURE(
            CStatus,    /* Return type */
            GNode,      /* Parent class */
            run,         /* Name of function in C++ (must match Python name) */
        );
    }

    CStatus destroy() override {
        PYBIND11_OVERRIDE_PURE(
            CStatus,    /* Return type */
            GNode,      /* Parent class */
            destroy,     /* Name of function in C++ (must match Python name) */
        );
    }
};


