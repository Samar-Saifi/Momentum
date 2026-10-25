//
// Created by mdsamar on 24/10/26.
//

#ifndef MOMENTUM_PRIMITIVESHAPES_H
#define MOMENTUM_PRIMITIVESHAPES_H
#include "Material.h"
#include "Mesh.h"


class PrimitiveShapes {
public:
    static std::shared_ptr<Mesh> CreateCube(float size = 1.0f, std::shared_ptr<Material> mat = nullptr );
    static std::shared_ptr<Mesh> CreatePlane(float width = 1.0f, float height = 1.0f, std::shared_ptr<Material> mat = nullptr );
    static std::shared_ptr<Mesh> CreateSphere(float radius = 1.0f, unsigned int ring = 20, unsigned int sector = 20, std::shared_ptr<Material> mat = nullptr );
    static std::shared_ptr<Mesh> CreateCapsule(float radius = 0.5f, float height = 1.0f, unsigned ring = 12, unsigned int sectors = 16, std::shared_ptr<Material> mat = nullptr );
};


#endif //MOMENTUM_PRIMITIVESHAPES_H
