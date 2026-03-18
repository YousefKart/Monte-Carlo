#include "text.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

Text::Text(int width, int height)
{
    shaderProgram = compileShader();
    initRenderData(width, height);
}

Text::~Text()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

bool Text::loadFont(const std::string& fontPath, int fontSize)
{
    FT_Library ft;

    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType\n";
        return false;
    }

    FT_Face face;

    if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font\n";
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph\n";
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character =
        {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<GLuint>(face->glyph->advance.x)
        };

        characters.insert(std::pair<char, Character>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return true;
}

void Text::renderText(
    const std::string& text,
    float x,
    float y,
    float scale,
    glm::vec3 color,
    float angleRadians)
{
    glUseProgram(shaderProgram);

    glUniform3f(
        glGetUniformLocation(shaderProgram, "textColor"),
        color.x, color.y, color.z
    );

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    const float cosA = std::cos(angleRadians);
    const float sinA = std::sin(angleRadians);

    float cursor = 0.0f;
    for (auto c : text)
    {
        Character ch = characters[c];

        // glyph quad corners in local (unrotated) space, relative to text origin
        float lx = cursor + ch.Bearing.x * scale;
        float ly = -(ch.Size.y - ch.Bearing.y) * scale;
        float w  = ch.Size.x * scale;
        float h  = ch.Size.y * scale;

        // rotate each corner around the text origin (x, y)
        float p0x = x + lx       * cosA - (ly + h) * sinA;
        float p0y = y + lx       * sinA + (ly + h) * cosA;
        float p1x = x + lx       * cosA - ly       * sinA;
        float p1y = y + lx       * sinA + ly       * cosA;
        float p2x = x + (lx + w) * cosA - ly       * sinA;
        float p2y = y + (lx + w) * sinA + ly       * cosA;
        float p3x = x + (lx + w) * cosA - (ly + h) * sinA;
        float p3y = y + (lx + w) * sinA + (ly + h) * cosA;

        float vertices[6][4] =
        {
            { p0x, p0y, 0.0f, 0.0f },
            { p1x, p1y, 0.0f, 1.0f },
            { p2x, p2y, 1.0f, 1.0f },

            { p0x, p0y, 0.0f, 0.0f },
            { p2x, p2y, 1.0f, 1.0f },
            { p3x, p3y, 1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        cursor += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::initRenderData(int width, int height)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        4,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        0
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

GLuint Text::compileShader()
{
    const char* vertexShaderSrc = R"(
    #version 330 core
    layout (location = 0) in vec4 vertex;

    out vec2 TexCoords;

    void main()
    {
        gl_Position = vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
    }
    )";

    const char* fragmentShaderSrc = R"(
    #version 330 core
    in vec2 TexCoords;
    out vec4 FragColor;

    uniform sampler2D text;
    uniform vec3 textColor;

    void main()
    {
        float alpha = texture(text, TexCoords).r;
        FragColor = vec4(textColor, alpha);
    }
    )";

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertex);

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragment);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}