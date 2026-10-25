//
// Created by mdsamar on 24/10/26.
//

#ifndef MOMENTUM_MODELLOADER_H
#define MOMENTUM_MODELLOADER_H
#include <vector>

#include "Material.h"
#include "Mesh.h"
#include "Texture.h"


struct AnimationSampler {
    std::vector<float> timestamps;
    std::vector<glm::vec3> translations;
    std::vector<glm::quat> rotations;
    std::vector<glm::vec3> scales;
};

struct AnimationTrack {
    int targetNode = -1;
    std::string targetPath;
    AnimationSampler sampler;
};

struct AnimationClip {
    std::string name;
    float duration;
    std::vector<AnimationTrack> tracks;
};

struct Node {
    std::string name;
    int index = -1;
    int parentIndex = -1;
    std::vector<int> children;
    glm::mat4 localTransform = glm::mat4(1.0f);
    int meshIndex = -1;
    int skinIndex = -1;
};

struct Skin {
    std::string name;
    std::vector<int> JointNodeIndices;
    std::vector<glm::mat4> InverseBindMatrices;
};

struct Model {
    std::string name;
    std::string path;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<Node> nodes;
    std::vector<Skin> skins;
    std::vector<AnimationClip> animationClips;
};

class ModelLoader {
public:
    static std::shared_ptr<Model> LoadModel(const std::string& path, std::shared_ptr<Shader> defaultShader = nullptr);
    static std::shared_ptr<Mesh> LoadMesh(const std::string& path, std::shared_ptr<Material> defaultMaterial = nullptr);
};


#endif //MOMENTUM_MODELLOADER_H
