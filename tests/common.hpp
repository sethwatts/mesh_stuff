#pragma once

#include "mesh/io.hpp"

using io::Mesh;
using io::Element;

using vec3 = std::array<double, 3>;

constexpr float h = 0.5;

// note: the node position arrays below are not
// the official reference-element positions!

//  2           
//  |`\.        
//  |  `\.      
//  5    `4     
//  |      `\.  
//  |        `\.
//  0-----3-----1
constexpr vec3 triangle_nodes[6] = {
  {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, 
  {h, 0, 0}, {h, h, 0}, {0, h, 0}
};

//  3-----6-----2 
//  |           | 
//  |           | 
//  7     8     5 
//  |           | 
//  |           | 
//  0-----4-----1 
constexpr vec3 quadrilateral_nodes[9] = {
  {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0},
  {h, 0, 0}, {1, h, 0}, {h, 1, 0}, {0, h, 0},
  {h, h, 0}
};

//                          
//             2            
//           ,/|`\.         
//         ,/  |  `\.       
//       ,6    '.   `5      
//     ,/       8     `\.   
//   ,/         |       `\. 
//  0--------4--'.---------1 
//   `\.         |      ,/' 
//      `\.      |    ,9    
//         `7.   '. ,/      
//            `\. |/        
//               `3         
//
constexpr vec3 tetrahedron_nodes[10] = {
  {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1},
  {h, 0, 0}, {h, h, 0}, {0, h, 0}, {0, 0, h}, {0, h, h}, {h, 0, h}
};

//                4              
//              ,/|\.            
//            ,/ .'|\.           
//          ,/   | | \.          
//        ,/    .' | `.          
//      ,7      |  12  \.        
//    ,/       .'   |   \.       
//  ,/         9    |    11      
// 0--------6-.'----3    `.      
//   `\        |      `\    \.   
//     `5     .'(13)    10   \.  
//       `\   |           `\  \. 
//         `\.'             `\`  
//            1--------8-------2 
constexpr vec3 pyramid_nodes[14] = {
  {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, {0, 0, 1},
  {h, 0, 0}, {0, h, 0}, {0, 0, h}, {1, h, 0}, {h, 0, h}, {h, 1, 0}, {h, h, h}, {0, h, h},
  {h, h, 0}
};

//          3          
//        ,/|`\.       
//      12  |  13      
//    ,/    |    `\.   
//   4------14-----5   
//   |      8      |   
//   |    ,/|`\    |   
//   |  15  |  16  |   
//   |,/    |    `\|   
//   10-----17-----11  
//   |      0      | 
//   |    ,/ `\    | 
//   |  ,6     `7  | 
//   |,/         `\| 
//   1------9------2 
constexpr vec3 prism_nodes[18] = {
  {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 0, 1}, {0, 1, 1},
  {h, 0, 0}, {0, h, 0}, {0, 0, h}, {h, h, 0}, {1, 0, h}, {0, 1, h},
  {h, 0, 1}, {0, h, 1}, {h, h, 1}, {h, 0, h}, {0, h, h}, {h, h, h}  
};

//                     
//  3----13----2       
//  |\         |\. 
//  |15    24  | 14    
//  9  \ 20    11 \.  
//  |   7----19+---6   
//  |22 |  26  | 23|   
//  0---+-8----1   |   
//   \ 17    25 \  18  
//   10 |  21    12|   
//     \|         \|   
//      4----16----5   
//
constexpr vec3 hexahedron_nodes[27] = {
  {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0}, 
  {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1},
  {h, 0, 0}, {0, h, 0}, {0, 0, h}, {1, h, 0},
  {1, 0, h}, {h, 1, 0}, {1, 1, h}, {0, 1, h},
  {h, 0, 1}, {0, h, 1}, {1, h, 1}, {h, 1, 1},
  {h, h, 0}, {h, 0, h}, {0, h, h}, {1, h, h},
  {h, 1, h}, {h, h, 1}, {h, h, h}
};

inline std::vector<int> range(int n) {
  std::vector<int> values(n);
  for (int i = 0; i < n; i++) { values[i] = i; }
  return values;
}

inline Mesh single_element_mesh(Element::Type type) {
  int n = nodes_per_elem(type);
  switch (type) {
    case Element::Type::Tri3:
    case Element::Type::Tri6:
      return Mesh{
        std::vector<vec3>(triangle_nodes, triangle_nodes + n),
        std::vector<Element>{{type, range(n)}}
      };
    case Element::Type::Quad4:
    case Element::Type::Quad8:
    case Element::Type::Quad9:
      return Mesh{
        std::vector<vec3>(quadrilateral_nodes, quadrilateral_nodes + n),
        std::vector<Element>{{type, range(n)}}
      };
    case Element::Type::Tet4:
    case Element::Type::Tet10:
      return Mesh{
        std::vector<vec3>(tetrahedron_nodes, tetrahedron_nodes + n),
        std::vector<Element>{{type, range(n)}}
      };
    case Element::Type::Pyr5:
    case Element::Type::Pyr13:
    case Element::Type::Pyr14:
      return Mesh{
        std::vector<vec3>(pyramid_nodes, pyramid_nodes + n),
        std::vector<Element>{{type, range(n)}}
      };
    case Element::Type::Prism6:
    case Element::Type::Prism15:
    case Element::Type::Prism18:
      return Mesh{
        std::vector<vec3>(prism_nodes, prism_nodes + n),
        std::vector<Element>{{type, range(n)}}
      };
    case Element::Type::Hex8:
    case Element::Type::Hex20:
    case Element::Type::Hex27:
      return Mesh{
        std::vector<vec3>(hexahedron_nodes, hexahedron_nodes + n),
        std::vector<Element>{{type, range(n)}}
      };

    case Element::Type::Line2:
    case Element::Type::Line3:
    case Element::Type::Unsupported:
      return Mesh{};
  }
}