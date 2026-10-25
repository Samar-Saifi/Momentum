//
// Created by mdsamar on 11/10/26.
//

#ifndef MOMENTUM_TEXTURE_H
#define MOMENTUM_TEXTURE_H
#include <string>

#include "glm/vec2.hpp"


class Texture {
public:
    Texture(unsigned int width, unsigned int height, unsigned int color = 0xFFFFFFFF);
    Texture(unsigned int width, unsigned int height, const void* data, unsigned int channels = 4);
    Texture(const std::string& filePath);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;
    glm::vec2 GetTextureResolution() const { return glm::vec2(m_Width, m_Height); };

private:
    unsigned int m_ID = 0;
    unsigned int m_Width = 0;
    unsigned int m_Height = 0;
    std::string m_FilePath;
};


#endif //MOMENTUM_TEXTURE_H
