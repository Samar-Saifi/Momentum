//
// Created by mdsamar on 24/10/26.
//

#include "PrimitiveShapes.h"

#include <vector>

#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/constants.hpp"

static std::shared_ptr<Mesh> ToMesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices,std::shared_ptr<Material> mat) {
    auto va = std::make_shared<VertexArray>();
    auto vb = std::make_shared<VertexBuffer>(const_cast<float*>(vertices.data()), vertices.size() * sizeof(float));
    auto ib = std::make_shared<IndexBuffer>(const_cast<unsigned int*>(indices.data()), indices.size());
    va->SetBuffer(vb, ib, 8 * sizeof(float));
    return std::make_shared<Mesh>(va, mat);
}

std::shared_ptr<Mesh> PrimitiveShapes::CreateCube(float size, std::shared_ptr<Material> mat) {
    float h = size / 2.0f;
    std::vector<float> vertices = {
        -h, -h,  h,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
         h, -h,  h,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
         h,  h,  h,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
        -h,  h,  h,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,

         h, -h, -h,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
        -h, -h, -h,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
        -h,  h, -h,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
         h,  h, -h,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,

        -h, -h, -h,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
        -h, -h,  h,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
        -h,  h,  h,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
        -h,  h, -h,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,

         h, -h,  h,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
         h, -h, -h,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
         h,  h, -h,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
         h,  h,  h,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,

        -h,  h,  h,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
         h,  h,  h,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
         h,  h, -h,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
        -h,  h, -h,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,

        -h, -h, -h,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
         h, -h, -h,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
         h, -h,  h,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
        -h, -h,  h,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f
    };

    std:: vector<unsigned int> indices;
    for (unsigned int i = 0; i < 6; ++i) {
        unsigned int offset = i * 4;
        indices.push_back(offset + 0);
        indices.push_back(offset + 1);
        indices.push_back(offset + 2);
        indices.push_back(offset + 2);
        indices.push_back(offset + 3);
        indices.push_back(offset + 0);
    }

    return ToMesh(vertices, indices, mat);
}

std::shared_ptr<Mesh> PrimitiveShapes::CreatePlane(float width, float height, std::shared_ptr<Material> mat) {
    float w = width / 2.0f;
    float h = height / 2.0f;

    glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);
    std::vector<float> vertices; std::vector<unsigned int> indices;
    vertices = {
        -w, 0.0f, -h,   normal.x, normal.y, normal.z,   0.0f,  0.0f,
         w, 0.0f, -h,   normal.x, normal.y, normal.z,  10.0f,  0.0f,
         w, 0.0f,  h,   normal.x, normal.y, normal.z,  10.0f, 10.0f,
        -w, 0.0f,  h,   normal.x, normal.y, normal.z,   0.0f, 10.0f
    };

    indices = { 0, 1, 2, 2, 3, 0};

    return ToMesh(vertices, indices, mat);
}

std::shared_ptr<Mesh> PrimitiveShapes::CreateSphere(float radius, unsigned int ring, unsigned int sector,
    std::shared_ptr<Material> mat) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int r = 0; r <= ring; ++r) {
        float v = static_cast<float>(r) / static_cast<float>(ring);
        float phi = v * glm::pi<float>();

        for (unsigned int s = 0; s <= sector; ++s) {
            float u = static_cast<float>(s) / static_cast<float>(sector);
            float theta = u * glm::two_pi<float>();

            float x = std::sin(phi) * std::cos(theta);
            float y = std::cos(phi);
            float z = std::sin(phi) * std::sin(theta);

            glm::vec3 normal(x, y, z);
            glm::vec3 pos = normal * radius;

            vertices.push_back(pos.x);
            vertices.push_back(pos.y);
            vertices.push_back(pos.z);

            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    for (unsigned int r = 0; r < ring; ++r) {
        for (unsigned int s = 0; s < sector; ++s) {
            unsigned int cur = r * (sector + 1) + s;
            unsigned int next = cur + sector + 1;

            indices.push_back(cur);
            indices.push_back(next);
            indices.push_back(cur + 1);

            indices.push_back(next);
            indices.push_back(next + 1);
            indices.push_back(cur + 1);
        }
    }

    return ToMesh(vertices, indices, mat);
}

std::shared_ptr<Mesh> PrimitiveShapes::CreateCapsule(float radius, float height, unsigned ring, unsigned int sectors,
    std::shared_ptr<Material> mat) {
    std:: vector<float> vertices; std::vector<unsigned int> indices;
    float h = height/2.0f;
    unsigned int halfRings = ring/2;
    if (halfRings < 2) halfRings = 2;

    for (unsigned int i = 0; i <= ring + 1; i++) {
        float phi, yOffset;
        if ( i < halfRings ) {
            float vTop = (float)i / (float)halfRings;
            phi = vTop * (glm::pi<float>() * 0.5f);
            yOffset = -h;
        }else {
            float vBottom = (float)i / (float)halfRings;
            phi = (glm::pi<float>() * 0.5f) + (vBottom - 1) * (glm::pi<float>() * 0.5f);
            yOffset = -h;
        }

        float v = (float) i / ((float)ring + 1);

        for (unsigned int j = 0; j <= sectors; j++) {
            float u = (float)j / (float)sectors;
            float theta = u * glm::pi<float>();

            float nx = std::sin(phi) * std::cos(theta);
            float ny = std::cos(phi);
            float nz = std::sin(phi) * std::sin(theta);

            glm::vec3 normal(nx, ny, nz);
            glm::vec3 pos = normal * radius;
            pos.y += yOffset;

            vertices.push_back(pos.x);
            vertices.push_back(pos.y);
            vertices.push_back(pos.z);

            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);

            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    for (unsigned int i = 0; i <= ring + 1; i++) {
        for (int j = 0; j <= sectors; j++) {
            unsigned int c = i * (sectors + 1) + j;
            unsigned int n = c + sectors + 1;

            indices.push_back(c);
            indices.push_back(n);
            indices.push_back(c+1);

            indices.push_back(n);
            indices.push_back(n+1);
            indices.push_back(c+1);
        }
    }

    return ToMesh(vertices, indices, mat);
}
