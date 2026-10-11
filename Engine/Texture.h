//
// Created by mdsamar on 11/10/26.
//

#ifndef MOMENTUM_TEXTURE_H
#define MOMENTUM_TEXTURE_H


class Texture {
public:
    Texture(unsigned int width, unsigned int height, unsigned int color = 0xFFFFFFFF);
    ~Texture();

    void Bind() const;
    void Unbind() const;

private:
    unsigned int m_ID = 0;
    unsigned int m_width = 0;
    unsigned int m_height = 0;
};


#endif //MOMENTUM_TEXTURE_H
