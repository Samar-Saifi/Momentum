//
// Created by mdsamar on 11/10/26.
//

#include "Texture.h"

#include <vector>
#include <glad/glad.h>

Texture::Texture(unsigned int width, unsigned int height, unsigned int color) : m_width(width), m_height(height){
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    std::vector<unsigned int> buffer(width * height, color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
}

Texture::~Texture() {
    glDeleteTextures(1, &m_ID);
}

void Texture::Bind() const {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
