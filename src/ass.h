#ifndef ASS_H
#define ASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct TextureInfo {
    unsigned int id;
    aiTextureType type;
    std::string path;
};

struct VertInd {
    std::vector<float> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<TextureInfo> Textures;
};

class Ass
{
public:
	std::vector<VertInd> Import();
};

#endif

