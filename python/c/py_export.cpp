#include <cstdint>
#include <typeinfo>
#include <fstream>

#include "py_bind.h"

using namespace CGraph;
using GElementSet = std::set<GElementPtr>;

PYBIND11_MODULE(cgraph, m) {

    m.doc() = "Python interface for CGraph";

    m.def("PipelineCreate", []() {
        return GPipelineFactory::create();
    },
    py::return_value_policy::reference);

    m.def("PipelineRemove", [](GPipelinePtr pipeline) {
        GPipelineFactory::remove(pipeline);
    });

    m.def("cast_set", []() { return GElementPtrSet{}; });
    m.def("load_set", [](const GElementPtrSet &s) { 
        std::ofstream out = std::ofstream("output.txt");
        for (auto it : s) {
            out << it << ", ";     
        }
        out << ", size = " << s.size();
        out.flush();
     });

    py::class_<GPipeline, std::unique_ptr<GPipeline, py::nodelete>>(m, "GPipeline")
        .def("init", &GPipeline::init)
        .def("run", &GPipeline::run, py::call_guard<py::gil_scoped_release>())
        .def("process", &GPipeline::process)
        .def("destroy", &GPipeline::destroy)
        .def("registerGElement", &GPipeline::registerPyGElement,
            py::arg("elementRef"),
            py::arg("dependElements"),
            py::arg("name") = CGRAPH_EMPTY,
            py::arg("loop") = 1);

    py::class_<CStatus>(m, "CStatus")
        .def(py::init<>())
        .def("getCode", &CStatus::getCode)
        .def("isOK", &CStatus::isOK);

    class PyGElement : public GElement {
    public:
    /* Inherit the constructors */
    using GElement::GElement;

    /* Trampoline (need one for each virtual function) */
    CStatus run() override {
        PYBIND11_OVERRIDE_PURE(
            CStatus,    /* Return type */
            GElement,      /* Parent class */
            run,         /* Name of function in C++ (must match Python name) */
        );
    }

    CStatus destroy() override {
        PYBIND11_OVERRIDE_PURE(
            CStatus,    /* Return type */
            GElement,      /* Parent class */
            destroy,     /* Name of function in C++ (must match Python name) */
        );
    }
    };

    class PyGNode: public GNode {
    public:
        /* Inherit the constructors */
        using GNode::GNode;

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

    // py::class_<GNode, GElement>(m, "GNode");

    py::class_<GElement, PyGElement, std::unique_ptr<GElement, py::nodelete> /* <--- trampoline*/>(m, "GElement")
        .def(py::init<>())
        .def("run", &GElement::run)
        .def("destroy", &GElement::destroy);



    py::class_<GNode, PyGNode, GElement /* <--- trampoline*/>(m, "GNode")
        .def(py::init<>())
        .def("run", &GNode::run)
        .def("destroy", &GNode::destroy);

    // py::class_<GNode, GElement>(m, "GNode")
    //     .def(py::init<>());
        // .def("run", &GNode::run)
        // .def("destroy", &GNode::destroy);

}