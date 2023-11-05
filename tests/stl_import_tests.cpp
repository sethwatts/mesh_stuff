#include "gtest/gtest.h"

#include "mesh/io.hpp"

#include <cmath>

using namespace io;

TEST(stl, import_ascii) {
    Mesh mesh = import_stl("../data/ascii.stl");
    export_stl(mesh, "sphericon.stl");
}

TEST(stl, import_binary) {
    Mesh mesh = import_stl("../data/binary.stl");
    export_stl(mesh, "bunny.stl");
}