//
// Created by mdsamar on 10/10/26.
//

#ifndef MOMENTUM_MESH_H
#define MOMENTUM_MESH_H
#include <memory>

#include "VertexArray.h"

struct Mesh {
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<Material> material;

    Mesh() = default;
    Mesh(std::shared_ptr<VertexArray> vertexArray, std::shared_ptr<Material> material) : vertexArray(vertexArray), material(material) {};
};
#endif //MOMENTUM_MESH_H
