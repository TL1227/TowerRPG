#ifndef UI_H
#define UI_H

#include <ft2build.h>
#include FT_FREETYPE_H  
#include <glm/glm.hpp>

#include "shader.h"

#include <map>

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class UI
{
public:
	void InitUi();
	void DrawUi(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color);

	std::map<char, Character> Characters;
private:
    unsigned int VAO;
    unsigned int VBO;
};

#endif
