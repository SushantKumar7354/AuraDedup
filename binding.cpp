#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "pipeline.hpp"

namespace py = pybind11;

PYBIND11_MODULE(aura_engine, m)
{
    m.doc() = "AuraDedup C++ engine: duplicate/near-duplicate image detection";

    m.def("find_duplicates", &find_duplicates_in_folder,
          py::arg("folder"), py::arg("threshold") = 5,
          "Scan `folder` recursively and return groups of duplicate/near-duplicate "
          "image paths. threshold is the max Hamming distance (0-64) to count as "
          "a match; 0 = exact hash match only.");
}