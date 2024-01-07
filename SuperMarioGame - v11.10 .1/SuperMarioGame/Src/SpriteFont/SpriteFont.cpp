#include "SpriteFont.h"

#include "../SpriteBatch/SpriteBatch.h"

#include <SDL/SDL.h>

int closestPow2(int i) {
    double logbase2 = log(i) / log(2);
    return round(pow(2, ceil(logbase2)));
}

#define MAX_TEXTURE_RES 4096


SpriteFont::SpriteFont(const char* font, int size) {
    init(font, size);
}


void SpriteFont::init(const char* font, int size) {
    // Initialize SDL_ttf
    if (!TTF_WasInit()) {
        TTF_Init();
    }
    f = TTF_OpenFont(font, size);
    if (f == nullptr) {
        fprintf(stderr, "Failed to open TTF font %s\n", font);
        fflush(stderr);
        throw 281;
    }
}

SpriteFont::~SpriteFont() {
	dispose();
}

void SpriteFont::dispose() {
    if (f != nullptr) {
		TTF_CloseFont(f);
	}
    TTF_Quit();
}

GLuint SpriteFont::createTextTexture(const std::string& text, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderUTF8_Blended(f, text.c_str(), color);
    if (surface == nullptr) {
        // Handle error
    }

    // Generate OpenGL texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);

    

    SDL_FreeSurface(surface);
    _texID = texture;
    return texture;
}

void SpriteFont::draw(SpriteBatch& batch, glm::vec2 position){
    Color color(255, 255, 255, 255);
    glm::vec4 pos(position.x, position.y, 18, 36);
    batch.draw(pos, glm::vec4(-1.0f, -1.0f, 2.0f, 2.0f), _texID, 0.0f, color);
}