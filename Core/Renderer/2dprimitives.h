#pragma once
#include "glad/glad.h"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderables.h"
#include <algorithm>
#include <iostream>
namespace Renderer {
class Triangle : public Renderable {
public:
  float vertices[9];

  Triangle(glm::vec3 left, glm::vec3 top, glm::vec3 right) {
    auto src = glm::value_ptr(left);
    std::copy(src, src + 3, vertices);
    src = glm::value_ptr(top);
    std::copy(src, src + 3, vertices + 3);
    src = glm::value_ptr(right);
    std::copy(src, src + 3, vertices + 6);

    // std::cerr << vertices[0] << " " << vertices[1] << " " << vertices[2] << '
    // '
    //           << "\n";
    // std::cerr << vertices[3] << " " << vertices[4] << " " << vertices[5] << '
    // '
    //           << "\n";
    // std::cerr << vertices[6] << " " << vertices[7] << " " << vertices[8] << '
    // '
    //           << "\n";

    glCreateBuffers(1, &VBO);
    glCreateBuffers(1, &EBO);
    glCreateVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
  }

  void render(const ShaderProgram &s) override {}

private:
  GLuint VAO, VBO, EBO;
};
} // namespace Renderer
