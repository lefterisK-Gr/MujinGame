#pragma once

#ifndef SpriteFont_h__
#define SpriteFont_h__

#include <TTF/SDL_ttf.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include "../Vertex.h"


struct GLTexture;
class SpriteBatch;

class SpriteFont {
public:
    SpriteFont() {};
    SpriteFont(const char* font, int size);
    ~SpriteFont();

    void init(const char* font, int size);

    /// Destroys the font resources
    void dispose();
    GLuint createTextTexture(const std::string& text, SDL_Color color);
    /// Draws using a spritebatch
    void draw(SpriteBatch& batch, glm::vec2 position);
    
    GLuint _texID;
private:
    TTF_Font* f;
};



#endif // SpriteFont_h__