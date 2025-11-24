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

enum class TextAlign
{
    None,
    Center
};

class UI
{
public:
    UI(int screenWidth, int screenHeight);
	void InitUi();
	void InitQuad();
	void DrawText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color, TextAlign ta = TextAlign::None);
	void DrawBattle(Shader& shader);
	void DrawBattle(Shader& shader, int tex);

	std::map<char, Character> Characters;
    unsigned int QuadVAO;
    unsigned int QuadVBO;
    unsigned int QuadEBO;
    int BattleHudTex;
private:
    int GetStringPixelLength(std::string &text);
    unsigned int VAO;
    unsigned int VBO;
    int ScreenWidth;
    int ScreenHeight;

};

#endif
