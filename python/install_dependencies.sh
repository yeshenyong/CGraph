#!/bin/bash
#
# A script to install dependencies and third parties for CGraph.

set -eo pipefail

cg_root_dir=$(dirname "$(realpath "$0")")
third_party_dir=${cg_root_dir}/../3rd-party

# pybind11
echo "-- preparing pybind11 ..."
pushd "${third_party_dir}/pybind11"
git submodule update --init pybind11
popd