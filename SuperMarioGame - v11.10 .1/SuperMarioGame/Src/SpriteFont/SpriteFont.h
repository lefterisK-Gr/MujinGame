#pragma once

#ifndef SpriteFont_h__
#define SpriteFont_h__

#include <TTF/SDL_ttf.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>

#include "../Vertex.h"


struct GLTexture;
class SpriteBatch;

struct CharGlyph {
public:
    char character;
    glm::vec4 uvRect;
    glm::vec2 size;
};

#define FIRST_PRINTABLE_CHAR ((char)32)
#define LAST_PRINTABLE_CHAR ((char)126)

/// For text justification
enum class Justification {
    LEFT, MIDDLE, RIGHT
};

class SpriteFont {
public:
    SpriteFont() {};
    SpriteFont(const char* font, int size, char cs, char ce);
    SpriteFont(const char* font, int size) :
        SpriteFont(font, size, FIRST_PRINTABLE_CHAR, LAST_PRINTABLE_CHAR) {
    }

    void init(const char* font, int size);
    void init(const char* font, int size, char cs, char ce);

    /// Destroys the font resources
    void dispose();

    int getFontHeight() const {
        return _fontHeight;
    }

    /// Measures the dimensions of the text
    glm::vec2 measure(const char* s);

    /// Draws using a spritebatch
    void draw(SpriteBatch& batch, const char* s, glm::vec2 position, glm::vec2 scaling,
        float depth, Color tint, Justification just = Justification::LEFT);
private:
    static std::vector<int>* createRows(glm::ivec4* rects, int rectsLength, int r, int padding, int& w);

    int _regStart, _regLength;
    CharGlyph* _glyphs;
    int _fontHeight;

    unsigned int _texID;
};



#endif // SpriteFont_h__