#include "Renderer/shapes3d.h"
#include "Renderer/mesh.h"
namespace Renderer {
//  -1,1__________________ 1,1
//     |                 |
//     |                 |
//     |       0,0       |
//     |                 |
//     |                 |
//-1,-1__________________| 1,-1
//

Shapes3D Cuboid(float xLen, float yLen, float zLen, const Material &mat) {

  float halfWidth = xLen / 2.0;
  float halfHeight = yLen / 2.0f;
  float halfThickness = zLen / 2.0f;
  std::vector<Mesh> meshes;
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;

  // Back (-Z)
  vertices.push_back(
      Vertex{.position = {-halfWidth, -halfHeight, -halfThickness},
             .normal = {0, 0, -1},
             .texCoord = {0, 0}});
  vertices.push_back(
      Vertex{.position = {halfWidth, -halfHeight, -halfThickness},
             .normal = {0, 0, -1},
             .texCoord = {1, 0}});
  vertices.push_back(Vertex{.position = {halfWidth, halfHeight, -halfThickness},
                            .normal = {0, 0, -1},
                            .texCoord = {1, 1}});
  vertices.push_back(
      Vertex{.position = {-halfWidth, halfHeight, -halfThickness},
             .normal = {0, 0, -1},
             .texCoord = {0, 1}});

  // Front (+Z)
  vertices.push_back(
      Vertex{.position = {-halfWidth, -halfHeight, halfThickness},
             .normal = {0, 0, 1},
             .texCoord = {0, 0}});
  vertices.push_back(Vertex{.position = {halfWidth, -halfHeight, halfThickness},
                            .normal = {0, 0, 1},
                            .texCoord = {1, 0}});
  vertices.push_back(Vertex{.position = {halfWidth, halfHeight, halfThickness},
                            .normal = {0, 0, 1},
                            .texCoord = {1, 1}});
  vertices.push_back(Vertex{.position = {-halfWidth, halfHeight, halfThickness},
                            .normal = {0, 0, 1},
                            .texCoord = {0, 1}});

  // Left (-X)
  vertices.push_back(
      Vertex{.position = {-halfWidth, -halfHeight, -halfThickness},
             .normal = {-1, 0, 0},
             .texCoord = {0, 0}});
  vertices.push_back(
      Vertex{.position = {-halfWidth, -halfHeight, halfThickness},
             .normal = {-1, 0, 0},
             .texCoord = {1, 0}});
  vertices.push_back(Vertex{.position = {-halfWidth, halfHeight, halfThickness},
                            .normal = {-1, 0, 0},
                            .texCoord = {1, 1}});
  vertices.push_back(
      Vertex{.position = {-halfWidth, halfHeight, -halfThickness},
             .normal = {-1, 0, 0},
             .texCoord = {0, 1}});

  // Right (+X)
  vertices.push_back(Vertex{.position = {halfWidth, -halfHeight, halfThickness},
                            .normal = {1, 0, 0},
                            .texCoord = {0, 0}});
  vertices.push_back(
      Vertex{.position = {halfWidth, -halfHeight, -halfThickness},
             .normal = {1, 0, 0},
             .texCoord = {1, 0}});
  vertices.push_back(Vertex{.position = {halfWidth, halfHeight, -halfThickness},
                            .normal = {1, 0, 0},
                            .texCoord = {1, 1}});
  vertices.push_back(Vertex{.position = {halfWidth, halfHeight, halfThickness},
                            .normal = {1, 0, 0},
                            .texCoord = {0, 1}});

  // Bottom (-Y)
  vertices.push_back(
      Vertex{.position = {-halfWidth, -halfHeight, halfThickness},
             .normal = {0, -1, 0},
             .texCoord = {0, 0}});
  vertices.push_back(Vertex{.position = {halfWidth, -halfHeight, halfThickness},
                            .normal = {0, -1, 0},
                            .texCoord = {1, 0}});
  vertices.push_back(
      Vertex{.position = {halfWidth, -halfHeight, -halfThickness},
             .normal = {0, -1, 0},
             .texCoord = {1, 1}});
  vertices.push_back(
      Vertex{.position = {-halfWidth, -halfHeight, -halfThickness},
             .normal = {0, -1, 0},
             .texCoord = {0, 1}});

  // Top (+Y)
  vertices.push_back(
      Vertex{.position = {-halfWidth, halfHeight, -halfThickness},
             .normal = {0, 1, 0},
             .texCoord = {0, 0}});
  vertices.push_back(Vertex{.position = {halfWidth, halfHeight, -halfThickness},
                            .normal = {0, 1, 0},
                            .texCoord = {1, 0}});
  vertices.push_back(Vertex{.position = {halfWidth, halfHeight, halfThickness},
                            .normal = {0, 1, 0},
                            .texCoord = {1, 1}});
  vertices.push_back(Vertex{.position = {-halfWidth, halfHeight, halfThickness},
                            .normal = {0, 1, 0},
                            .texCoord = {0, 1}});

  // Indices
  for (unsigned int i = 0; i < 24; i += 4) {
    indices.push_back(i + 0);
    indices.push_back(i + 1);
    indices.push_back(i + 2);

    indices.push_back(i + 2);
    indices.push_back(i + 3);
    indices.push_back(i + 0);
  }
  Mesh mesh(vertices, indices, mat);
  meshes.push_back(mesh);
  return Shapes3D(meshes, mat);
}

Shapes3D Cuboid(float xLen, float yLen, float zLen, const glm::vec4 &color) {
  Material mat{.fillColor = color};
  return Cuboid(xLen, yLen, zLen, mat);
}

Shapes3D Cube(float sideLen, const glm::vec4 &color) {
  return Cuboid(sideLen, sideLen, sideLen, color);
}

Shapes3D Cube(float sideLen, const Material &mat) {
  return Cuboid(sideLen, sideLen, sideLen, mat);
}
} // namespace Renderer
