#include "mesh/io.hpp"

#include <fstream>

#if 0
static constexpr uint32_t VTK_TRIANGLE = 5;
static constexpr uint32_t VTK_TETRAHEDRON = 10;
static constexpr uint32_t VTK_QUADRATIC_TRIANGLE = 22;
static constexpr uint32_t VTK_QUADRATIC_TETRAHEDRON = 24;

template < int dim >
void export_vtk_impl(const SimplexMesh<dim> & mesh, std::string filename) {

  bool is_quadratic = mesh.quadratic_nodes.size() > 0;

  std::ofstream outfile(filename, std::ios::binary);

  // note: apparently the legacy VTK files expect big-endian for
  //       binary format, so we'll only support ASCII here
  outfile << "# vtk DataFile Version 3.0\n";
  outfile << "--------------------------\n";
  outfile << "ASCII\n";
  outfile << "DATASET UNSTRUCTURED_GRID\n";
  if (is_quadratic) {
    outfile << "POINTS " << mesh.vertices.size() + mesh.quadratic_nodes.size() << " float\n";
  } else {
    outfile << "POINTS " << mesh.vertices.size() << " float\n";
  }
  for (auto p : mesh.vertices) {
    outfile << p[0] << " " << p[1] << " " << p[2] << '\n';
  }
  if (is_quadratic) {
    for (auto p : mesh.quadratic_nodes) {
      outfile << p[0] << " " << p[1] << " " << p[2] << '\n';
    }
  }

  int32_t nelems = mesh.elements.size();
  int32_t entries_per_elem;
  if (dim == 2 && !is_quadratic) entries_per_elem = 3;
  if (dim == 3 && !is_quadratic) entries_per_elem = 4;
  if (dim == 2 &&  is_quadratic) entries_per_elem = 6;
  if (dim == 3 &&  is_quadratic) entries_per_elem = 10;

  outfile << "CELLS " << nelems << " " << nelems * (entries_per_elem + 1) << '\n';
  for (int i = 0; i < mesh.elements.size(); i++) {
    outfile << entries_per_elem;
    for (auto id : mesh.elements[i]) { outfile << " " << id; }
    if (is_quadratic) {
      for (auto id : mesh.elements_quadratic_ids[i]) { outfile << " " << id + mesh.vertices.size(); }
    }
    outfile << '\n';
  }

  outfile << "CELL_TYPES " << nelems << '\n';
  int32_t type;
  if (dim == 2 && !is_quadratic) type = VTK_TRIANGLE;
  if (dim == 3 && !is_quadratic) type = VTK_TETRAHEDRON;
  if (dim == 2 &&  is_quadratic) type = VTK_QUADRATIC_TRIANGLE;
  if (dim == 3 &&  is_quadratic) type = VTK_QUADRATIC_TETRAHEDRON;

  for (auto e : mesh.elements) {
    outfile << type << '\n';
  }
  
  outfile.close();

}

void export_vtk(const SimplexMesh<2> & mesh, std::string filename) {
  export_vtk_impl(mesh, filename);
}

void export_vtk(const SimplexMesh<3> & mesh, std::string filename) {
  export_vtk_impl(mesh, filename);
}
#endif