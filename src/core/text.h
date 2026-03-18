#ifndef TEXT_H
#define TEXT_H

#include <map>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class Text
{
public:
    Text(int screenWidth, int screenHeight);
    ~Text();

    bool loadFont(const std::string& fontPath, int fontSize);

    void renderText(
        const std::string& text,
        float x,
        float y,
        float scale,
        glm::vec3 color,
        float angleRadians = 0.0f
    );

private:
    std::map<char, Character> characters;

    GLuint VAO;
    GLuint VBO;
    GLuint shaderProgram;

    void initRenderData(int width, int height);
    GLuint compileShader();
};

#endif // TEXT_H