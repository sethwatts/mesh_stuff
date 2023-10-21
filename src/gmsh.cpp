#include "mesh/io.hpp"

#include "util.hpp"

#include <map>
#include <tuple>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

namespace gmsh {

  using io::Element;

  Element::Type element_type(int i){
    switch (i) {
      case  1: return Element::Type::Line2;
      case  8: return Element::Type::Line3;
      case  2: return Element::Type::Tri3;
      case  9: return Element::Type::Tri6;
      case  3: return Element::Type::Quad4;
      case 16: return Element::Type::Quad8;
      case 10: return Element::Type::Quad9;
      case  4: return Element::Type::Tet4;
      case 11: return Element::Type::Tet10;
      case  7: return Element::Type::Pyr5;
      case 19: return Element::Type::Pyr13;
      case 14: return Element::Type::Pyr14;
      case  6: return Element::Type::Prism6;
      case 18: return Element::Type::Prism15;
      case 13: return Element::Type::Prism18;
      case  5: return Element::Type::Hex8;
      case 17: return Element::Type::Hex20;
      case 12: return Element::Type::Hex27;
      default: return Element::Type::Unsupported;
    }
  }

  int element_type(Element::Type type){
    switch (type) {
      case Element::Type::Line2:       return 1;
      case Element::Type::Line3:       return 8;
      case Element::Type::Tri3:        return 2;
      case Element::Type::Tri6:        return 9;
      case Element::Type::Quad4:       return 3;
      case Element::Type::Quad8:       return 16;
      case Element::Type::Quad9:       return 10;
      case Element::Type::Tet4:        return 4;
      case Element::Type::Tet10:       return 11;
      case Element::Type::Pyr5:        return 7;
      case Element::Type::Pyr13:       return 19;
      case Element::Type::Pyr14:       return 14;
      case Element::Type::Prism6:      return 6;
      case Element::Type::Prism15:     return 18;
      case Element::Type::Prism18:     return 13;
      case Element::Type::Hex8:        return 5;
      case Element::Type::Hex20:       return 17;
      case Element::Type::Hex27:       return 12;
      case Element::Type::Unsupported: return -1;
    }
  }

  // no permutations necessary here, we use gmsh's numbering

}

static bool export_gmsh_v22_binary(const io::Mesh & mesh, std::string filename) {

  std::ofstream outfile(filename);

  //////////////////
  // write header //
  //////////////////
  const double version = 2.2;
  const int filetype = 1; // denotes binary encoding
  const int datasize = 8;
  const int one = 1;
  outfile << "$MeshFormat\n";
  outfile << version << " " << filetype << " " << datasize << std::endl;
  outfile.write((char*)&one, sizeof(int)); // used for checking endianness
  outfile << "$EndMeshFormat\n";

  /////////////////
  // write nodes //
  /////////////////
  outfile << "$Nodes\n";
  outfile << mesh.nodes.size() << std::endl;
  for (int i = 0; i < mesh.nodes.size(); i++) {
    outfile.write((char*)&i, sizeof(int));
    outfile.write((char*)&mesh.nodes[i], sizeof(double) * 3); 
  }
  outfile << "$EndNodes\n";

  /////////////////
  // write elems //
  /////////////////
  outfile << "$Elements\n";
  outfile << mesh.elements.size() << std::endl;

  // group elements together by type 
  std::map< io::Element::Type, std::vector< const io::Element * > > element_blocks;
  for (int i = 0; i < mesh.elements.size(); i++) {
    const io::Element * e = &mesh.elements[i];
    element_blocks[e->type].push_back(e);
  }

  // then write each block out
  for (auto [type, block] : element_blocks) {
    int etype = gmsh::element_type(type);
    int block_size = int(block.size());
    int num_tags = int(block[0]->tags.size());
    int ids_per_elem = int(block[0]->node_ids.size());
    outfile.write((char*)&etype, sizeof(int));
    outfile.write((char*)&block_size, sizeof(int));
    outfile.write((char*)&num_tags, sizeof(int));
    for (auto * elem : block) {
      if (elem->tags.size() != num_tags) {
        std::cout << "error: elements with incompatible number of tags" << std::endl;
        return true;
      }

      if (elem->node_ids.size() != ids_per_elem) {
        std::cout << "error: detected elements with incorrect number of node ids" << std::endl;
        return true;
      }

      outfile.write((char*)&elem->tags[0], sizeof(int) * elem->tags.size());
      outfile.write((char*)&elem->node_ids[0], sizeof(int) * elem->node_ids.size());
    }
  }

  outfile << "$EndElements\n";

  outfile.close();

  return false;

}

static bool export_gmsh_v22_ascii(const io::Mesh & mesh, std::string filename) {

  std::ofstream outfile(filename);

  //////////////////
  // write header //
  //////////////////
  const double version = 2.2;
  const int filetype = 0; // denotes ascii encoding
  const int datasize = 8;
  const int one = 1;
  outfile << "$MeshFormat\n";
  outfile << version << " " << filetype << " " << datasize << std::endl;
  outfile << "$EndMeshFormat\n";

  /////////////////
  // write nodes //
  /////////////////
  outfile << "$Nodes\n";
  outfile << mesh.nodes.size() << std::endl;
  for (int i = 0; i < mesh.nodes.size(); i++) {
    outfile << i << " " << mesh.nodes[i] << '\n';
  }
  outfile << "$EndNodes\n";

  /////////////////
  // write elems //
  /////////////////
  outfile << "$Elements\n";
  for (int i = 0; i < mesh.elements.size(); i++) {
    auto [type, tags, ids] = mesh.elements[i];
    outfile << i << " " << gmsh::element_type(type) << " " << tags.size();
    for (auto tag : tags) { outfile << " " << tag; }
    for (auto id : ids) { outfile << " " << id; }
  }
  outfile << "$EndElements\n";

  outfile.close();

  return false;

}

io::Mesh import_gmsh_v22_ascii(std::ifstream & infile) {

  io::Mesh mesh;

  ////////////////
  // read nodes //
  ////////////////
  std::string line;
  infile >> line;
  if (line != "$Nodes") exit_with_error("invalid file format (nodes)");

  int node_id;
  int num_nodes;
  infile >> num_nodes;
  mesh.nodes.resize(num_nodes);

  for (int i = 0; i < num_nodes; i++) {
    infile >> node_id; // node id field is unused
    mesh.nodes[i] = ascii_read_array<double,3>(infile);
  }

  infile >> line;
  if (line != "$EndNodes") exit_with_error("invalid file format (nodes)");

  ////////////////
  // read elems //
  ////////////////

  // skip through any data sections other than $Elements
  do {
    getline(infile, line);
  } while (infile && line != "$Elements");

  int num_elems;
  infile >> num_elems;

  mesh.elements.resize(num_elems);

  for (int i = 0; i < num_elems; i++) {
    auto [elem_id, elem_type, num_tags] = ascii_read_array<int, 3>(infile);

    auto & e = mesh.elements[elem_id];

    e.type = gmsh::element_type(elem_type);

    e.tags.resize(num_tags);
    for (int j = 0; j < num_tags; j++) infile >> e.tags[j];

    int npe = nodes_per_elem(e.type);
    e.node_ids.resize(npe);
    for (int j = 0; j < npe; j++) infile >> e.node_ids[j];

    getline(infile, line);
  }

  infile >> line;
  if (line != "$EndElements") exit_with_error("invalid file format (elems)");

  infile.close();

  return mesh;
}

io::Mesh import_gmsh_v22_binary(std::ifstream & infile, bool swap_bytes) {

  io::Mesh mesh;

  auto read = [&](auto * ptr, std::streamsize size){
    infile.read((char*)ptr, size);
    if (swap_bytes) {

    }
  };

  ////////////////
  // read nodes //
  ////////////////
  std::string line;
  infile >> line;
  if (line != "$Nodes") exit_with_error("invalid file format (nodes)");

  int id;
  int num_nodes;
  infile >> num_nodes;
  mesh.nodes.resize(num_nodes);
  getline(infile, line); // skip the newline
  for (int i = 0; i < num_nodes; i++) {
    infile.read((char*)&id, sizeof(int)); // node id field is unused
    infile.read((char*)&mesh.nodes[id], sizeof(double) * 3);
  }

  infile >> line;
  if (line != "$EndNodes") exit_with_error("invalid file format (nodes)");

  // skip through any data sections other than $Elements
  do {
    getline(infile, line);
  } while (infile && line != "$Elements");

  int num_elems;
  infile >> num_elems;
  mesh.elements.resize(num_elems);

  getline(infile, line); // skip the newline after the number of elements

  // iterate through all the blocks of elements
  // until all of the elements are accounted for
  int element_count = 0;
  do {
    auto [gmsh_type, block_size, num_tags] = binary_read_array<int,3>(infile);

    io::Element::Type type = gmsh::element_type(gmsh_type);
    int npe = nodes_per_elem(type);

    for (int i = 0; i < block_size; i++) {
      mesh.elements[i].tags.resize(num_tags);
      infile.read((char*)&mesh.elements[i].tags[0], sizeof(int) * num_tags);

      mesh.elements[i].node_ids.resize(npe);
      infile.read((char*)&mesh.elements[i].node_ids[0], sizeof(int) * npe);
    }

    element_count += block_size;
  } while (element_count < num_elems);

  infile >> line;
  if (line != "$EndElements") exit_with_error("invalid file format (elems)");

  infile.close();

  return mesh;

}

namespace io {

bool export_gmsh_v22(const Mesh & mesh, std::string filename, FileEncoding enc) {
  if (enc == FileEncoding::ASCII) {
    return export_gmsh_v22_ascii(mesh, filename);
  } else {
    return export_gmsh_v22_binary(mesh, filename);
  }
}

Mesh import_gmsh_v22(std::string filename) {

  std::ifstream infile(filename);

  if (!infile) {
    std::cout << "error: " << filename << " not found" << std::endl;
    exit(1);
  }

  int unused;
  std::string str;
  std::string line;

  /////////////////
  // read header //
  /////////////////
  infile >> line;
  if (line != "$MeshFormat") exit_with_error("invalid file format (header)" + line);

  double version_number;
  int filetype;
  int datasize;

  infile >> version_number >> filetype >> datasize;
  if (version_number != 2.2) exit_with_error("unsupported version number");
  if (filetype > 1) exit_with_error("unsupported file type");
  if (datasize != 8) exit_with_error("invalid data size field");

  infile >> str;
  if (str != "$EndMeshFormat") exit_with_error("invalid file format (end header):" + line);

  if (filetype == 0) {
    return import_gmsh_v22_ascii(infile);
  } else {
    getline(infile, line); // skip the newline
    int one;
    infile.read((char*)&one, 4);
    bool swap_bytes = (one != 1);
    return import_gmsh_v22_binary(infile, swap_bytes);
  }

}

} // namespace io