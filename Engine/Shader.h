//
// Created by mdsamar on 03/10/26.
//

#ifndef MOMENTUM_SHADER_H
#define MOMENTUM_SHADER_H
#include <string>
#include "glm/glm.hpp"


class Shader {
public:
    Shader(const std::string& vertPath, const std::string& fragPath);
    ~Shader();

    void Bind();
    void Unbind();

    void SetMat4(const std::string& name, const glm::mat4& mat);
    void SetVec4(const std::string& name, glm::vec4& vec);

private:
    unsigned int m_ID = 0;
    static std::string ReadStringFromFile(const std::string& path);
};


#endif //MOMENTUM_SHADER_H
