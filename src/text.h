#ifndef TEXT_H
#define TEXT_H

#include <ft2build.h>
#include FT_FREETYPE_H  
#include <glm/glm.hpp>

#include <map>
#include <vector>

#include "shader.h"

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

enum class TextAlign
{
    None,
    Center
};

class Text
{
public:
    Text(int screenWidth, int screenHeight);
	void Draw(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color, TextAlign ta = TextAlign::None);
    void DrawList(Shader& shader, std::vector<std::string> list, float x, float y, float scale, glm::vec3 color, TextAlign ta = TextAlign::None, int highlight = -1);
private:
    int GetStringPixelLength(std::string &text);
    unsigned int VAO;
    unsigned int VBO;
    int ScreenWidth;
    int ScreenHeight;
    float LineHeight;
	std::map<char, Character> Characters;
};

#endif
