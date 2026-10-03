//
// Created by mdsamar on 03/10/26.
//

#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include  <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    std::string vertString = ReadStringFromFile(vertPath);
    const char* vertSrc = vertString.c_str();
    glShaderSource(vs, 1, &vertSrc, nullptr);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragString = ReadStringFromFile(fragPath);
    const char* fragSrc = fragString.c_str();
    glShaderSource(fs, 1, &fragSrc, nullptr);
    glCompileShader(fs);

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vs);
    glAttachShader(m_ID, fs);
    glLinkProgram(m_ID);

    glDeleteShader(vs);
    glDeleteShader(fs);

    std::cout << "Shader Compiled Successfully!" << std::endl;
}

Shader::~Shader() {
    glDeleteProgram(m_ID);
}

void Shader::Bind() {
    glUseProgram(m_ID);
}

void Shader::Unbind() {
    glUseProgram(0);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) {
    int loc = glGetUniformLocation(m_ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetVec4(const std::string& name, glm::vec4& value){
    int loc = glGetUniformLocation(m_ID, name.c_str());
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

std::string Shader::ReadStringFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "ERROR: can't open shader file: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

