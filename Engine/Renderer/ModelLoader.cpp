//
// Created by mdsamar on 24/10/26.
//

#include "ModelLoader.h"

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <iostream>
#include <tiny_gltf.h>

#include "glm/gtc/type_ptr.hpp"

struct AccessorView {
    const unsigned char* data = nullptr;
    unsigned int stride = 0;
    unsigned int count = 0;
};

static AccessorView GetAccessorView(const tinygltf::Model& model, int accessorIndex) {
    AccessorView view;
    if (accessorIndex < 0 || accessorIndex >= model.accessors.size()) return view;
    const auto& accessor = model.accessors[accessorIndex];
    if (accessor.bufferView < 0 || accessor.bufferView >= model.bufferViews.size()) return view;
    const auto& bufferView = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bufferView.buffer];

    view.data = &buffer.data[bufferView.byteOffset + accessor.byteOffset];
    view.count = accessor.count;
    view.stride = accessor.ByteStride(bufferView);
    if (view.stride == 0) {
        view.stride = tinygltf::GetComponentSizeInBytes(accessor.componentType) * tinygltf::GetComponentSizeInBytes(accessor.type);
    }
    return view;
}

static std::vector<unsigned int> GetIndices(const tinygltf::Model& model, int accessorIndex) {
    std::vector<unsigned int> indices;
    if (accessorIndex < 0 || accessorIndex >= model.accessors.size()) return indices;
    const auto& accessor = model.accessors[accessorIndex];
    if (accessor.bufferView < 0 || accessor.bufferView >= model.bufferViews.size()) return indices;
    const auto& bufferView = model.bufferViews[accessor.bufferView];
    const auto& buffer = model.buffers[bufferView.buffer];
    const unsigned char* data = &buffer.data[bufferView.byteOffset + accessor.byteOffset];

    indices.reserve(accessor.count);
    if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
        const uint16_t* p = reinterpret_cast<const uint16_t*>(data);
        for (int i = 0; i < accessor.count; i++) indices.push_back(p[i]);
    }else if (accessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT) {
        const uint32_t* p = reinterpret_cast<const uint32_t*>(data);
        for (size_t i = 0; i < accessor.count; ++i) indices.push_back(p[i]);
    } else if (accessor.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE) {
        const uint8_t* p = reinterpret_cast<const uint8_t*>(data);
        for (size_t i = 0; i < accessor.count; ++i) indices.push_back(p[i]);
    }
    return indices;
}


std::shared_ptr<Model> ModelLoader::LoadModel(const std::string& filepath, std::shared_ptr<Shader> defaultShader) {
    tinygltf::TinyGLTF loader;
    tinygltf::Model gltfModel;
    std::string err, warn;

    std::string ext = std::filesystem::path(filepath).extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    bool success = false;
    if (ext == ".glb") {
        success = loader.LoadBinaryFromFile(&gltfModel, &err, &warn, filepath);
    } else {
        success = loader.LoadASCIIFromFile(&gltfModel, &err, &warn, filepath);
    }

    if (!warn.empty()) {
        std::cout << "[MOMENTUM GLTF WARN] " << warn << "\n";
    }

    if (!success) {
        std::cerr << "[MOMENTUM GLTF ERROR] Failed to load glTF file '" << filepath << "': " << err << "\n";
        return nullptr;
    }

    auto result = std::make_shared<Model>();
    result->path = filepath;
    result->name = std::filesystem::path(filepath).stem().string();

    for (const auto& image : gltfModel.images) {
        if (!image.image.empty() && image.width > 0 && image.height > 0) {
            auto tex = std::make_shared<Texture>( image.width, image.height, image.image.data(), image.component);
            result->textures.push_back(tex);
        }
    }

    for (const auto& gMat : gltfModel.materials) {
        auto mat = std::make_shared<Material>(defaultShader);

        auto baseColor = gMat.pbrMetallicRoughness.baseColorFactor;

        if (baseColor.size() == 4) {
            mat->SetColor(glm::vec4(baseColor[0], baseColor[1], baseColor[2], baseColor[3]));
        }

        int texIndex = gMat.pbrMetallicRoughness.baseColorTexture.index;
        if (texIndex >= 0 && texIndex < static_cast<int>(gltfModel.textures.size())) {
            int sourceImage = gltfModel.textures[texIndex].source;
            if (sourceImage >= 0 && sourceImage < static_cast<int>(result->textures.size())) {
                mat->SetTexture(result->textures[sourceImage]);
            }
        }

        result->materials.push_back(mat);
    }

    for (const auto& gMesh : gltfModel.meshes) {
        for (const auto& primitive : gMesh.primitives) {
            auto posIt = primitive.attributes.find("POSITION");
            if (posIt == primitive.attributes.end()) continue;

            auto posView = GetAccessorView(gltfModel, posIt->second);
            if (!posView.data || posView.count == 0) continue;

            auto normIt = primitive.attributes.find("NORMAL");
            AccessorView normView = (normIt != primitive.attributes.end()) ? GetAccessorView(gltfModel, normIt->second) : AccessorView{};

            auto uvIt = primitive.attributes.find("TEXCOORD_0");
            AccessorView uvView = (uvIt != primitive.attributes.end()) ? GetAccessorView(gltfModel, uvIt->second) : AccessorView{};

            std::vector<uint32_t> indices = GetIndices(gltfModel, primitive.indices);

            std::vector<float> vertexdata;
            vertexdata.reserve(posView.count * 8);

            for (size_t i = 0; i < posView.count; ++i) {
                const float* p = reinterpret_cast<const float*>(posView.data + i * posView.stride);
                glm::vec3 pos(p[0], p[1], p[2]);

                glm::vec3 norm(0.0f, 1.0f, 0.0f);
                if (normView.data && i < normView.count) {
                    const float* n = reinterpret_cast<const float*>(normView.data + i * normView.stride);
                    norm = glm::vec3(n[0], n[1], n[2]);
                }

                glm::vec2 uv(0.0f);
                if (uvView.data && i < uvView.count) {
                    const float* u = reinterpret_cast<const float*>(uvView.data + i * uvView.stride);
                    uv = glm::vec2(u[0], u[1]);
                }

                vertexdata.push_back(pos.x);
                vertexdata.push_back(pos.y);
                vertexdata.push_back(pos.z);
                vertexdata.push_back(norm.x);
                vertexdata.push_back(norm.y);
                vertexdata.push_back(norm.z);
                vertexdata.push_back(uv.x);
                vertexdata.push_back(uv.y);
            }

            if (indices.empty()) {
                indices.reserve(posView.count);
                for (uint32_t i = 0; i < static_cast<uint32_t>(posView.count); ++i) {
                    indices.push_back(i);
                }
            }

            std::shared_ptr<Material> assignedMaterial = nullptr;
            if (primitive.material >= 0 && primitive.material < static_cast<int>(result->materials.size())) {
                assignedMaterial = result->materials[primitive.material];
            } else if (!result->materials.empty()) {
                assignedMaterial = result->materials[0];
            } else {
                assignedMaterial = std::make_shared<Material>(defaultShader);
            }

            auto va = std::make_shared<VertexArray>();
            auto vb = std::make_shared<VertexBuffer>(vertexdata.data(), static_cast<uint32_t>(vertexdata.size() * sizeof(float)));
            auto ib = std::make_shared<IndexBuffer>(indices.data(), static_cast<uint32_t>(indices.size()));
            va->SetBuffer(vb, ib, 8 * sizeof(float));

            result->meshes.push_back(std::make_shared<Mesh>(va, assignedMaterial));
        }
    }

    for (size_t i = 0; i < gltfModel.nodes.size(); ++i) {
        const auto& gNode = gltfModel.nodes[i];
        Node node;
        node.name = gNode.name.empty() ? ("Node_" + std::to_string(i)) : gNode.name;
        node.index = static_cast<int>(i);
        node.meshIndex = gNode.mesh;
        node.skinIndex = gNode.skin;
        node.children = gNode.children;

        if (gNode.matrix.size() == 16) {
            node.localTransform = glm::make_mat4(gNode.matrix.data());
        } else {
            glm::vec3 translation(0.0f);
            if (gNode.translation.size() == 3) {
                translation = glm::vec3(gNode.translation[0], gNode.translation[1], gNode.translation[2]);
            }

            glm::quat rotation(1.0f, 0.0f, 0.0f, 0.0f);
            if (gNode.rotation.size() == 4) {
                rotation = glm::quat(
                    static_cast<float>(gNode.rotation[0]),
                    static_cast<float>(gNode.rotation[1]),
                    static_cast<float>(gNode.rotation[2]),
                    static_cast<float>(gNode.rotation[3])
                );
            }

            glm::vec3 scale(1.0f);
            if (gNode.scale.size() == 3) {
                scale = glm::vec3(gNode.scale[0], gNode.scale[1], gNode.scale[2]);
            }

            node.localTransform = glm::translate(glm::mat4(1.0f), translation) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
        }
        result->nodes.push_back(node);
    }

    for (const auto& gSkin : gltfModel.skins) {
        Skin skin;
        skin.name = gSkin.name;
        skin.JointNodeIndices = gSkin.joints;

        if (gSkin.inverseBindMatrices >= 0) {
            auto view = GetAccessorView(gltfModel, gSkin.inverseBindMatrices);
            if (view.data) {
                const float* matrices = reinterpret_cast<const float*>(view.data);
                for (size_t m = 0; m < view.count; ++m) {
                    skin.InverseBindMatrices.push_back(glm::make_mat4(matrices + m * 16));
                }
            }
        }
        result->skins.push_back(skin);
    }

    for (const auto& gAnim : gltfModel.animations) {
        AnimationClip clip;
        clip.name = gAnim.name.empty() ? ("Anim_" + std::to_string(result->animationClips.size())) : gAnim.name;

        for (const auto& gChannel : gAnim.channels) {
            if (gChannel.sampler < 0 || gChannel.sampler >= static_cast<int>(gAnim.samplers.size())) continue;
            const auto& gSampler = gAnim.samplers[gChannel.sampler];

            AnimationTrack channel;
            channel.targetNode = gChannel.target_node;
            channel.targetPath = gChannel.target_path;

            auto timeView = GetAccessorView(gltfModel, gSampler.input);
            if (timeView.data) {
                const float* times = reinterpret_cast<const float*>(timeView.data);
                for (size_t t = 0; t < timeView.count; ++t) {
                    float time = times[t];
                    channel.sampler.timestamps.push_back(time);
                    if (time > clip.duration) clip.duration = time;
                }
            }

            auto outView = GetAccessorView(gltfModel, gSampler.output);
            if (outView.data) {
                const float* values = reinterpret_cast<const float*>(outView.data);
                if (channel.targetPath == "translation") {
                    for (size_t v = 0; v < outView.count; ++v) {
                        channel.sampler.translations.push_back(glm::vec3(values[v * 3 + 0], values[v * 3 + 1], values[v * 3 + 2]));
                    }
                } else if (channel.targetPath == "rotation") {
                    for (size_t v = 0; v < outView.count; ++v) {
                        channel.sampler.rotations.push_back(glm::quat(values[v * 4 + 3], values[v * 4 + 0], values[v * 4 + 1], values[v * 4 + 2]));
                    }
                } else if (channel.targetPath == "scale") {
                    for (size_t v = 0; v < outView.count; ++v) {
                        channel.sampler.scales.push_back(glm::vec3(values[v * 3 + 0], values[v * 3 + 1], values[v * 3 + 2]));
                    }
                }
            }

            clip.tracks.push_back(channel);
        }
        result->animationClips.push_back(clip);
    }

    std::cout << "[MOMENTUM GLTF INFO] Loaded glTF Model: '" << filepath << "' ("
              << result->meshes.size() << " meshes, "
              << result->materials.size() << " materials, "
              << result->animationClips.size() << " animations)\n";

    return result;
}

std::shared_ptr<Mesh> ModelLoader::LoadMesh(const std::string& filepath, std::shared_ptr<Material> fallbackMaterial) {
    auto model = LoadModel(filepath, fallbackMaterial ? fallbackMaterial->GetShader() : nullptr);
    if (model && !model->meshes.empty()) {
        return model->meshes[0];
    }
    return nullptr;
}