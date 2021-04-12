#pragma once

#include "IDisplay.hpp"
#include "Exception.hpp"
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

namespace Arcade
{
    class SDLTextureObj
    {
    public:
        SDL_Texture *m_img {};
        SDL_Rect m_rect {};
        SDL_Point m_center {};
        SDLTextureObj(const SDLTextureObj&) = delete;
        SDLTextureObj(SDLTextureObj &&) = delete;
        SDLTextureObj& operator=(const SDLTextureObj&) = delete;
        SDLTextureObj& operator=(SDLTextureObj &&) = delete;
        SDLTextureObj(const std::string &path, SDL_Renderer *renderer);
        SDLTextureObj(Arcade::Text text, TTF_Font *font, SDL_Renderer *renderer);
        ~SDLTextureObj();
        void setPosition(float x, float y);
    };

    class SDLSoundObj
    {
    public:
        Mix_Music *m_sound;
        SDLSoundObj(const SDLSoundObj&) = delete;
        SDLSoundObj(SDLSoundObj &&) = delete;
        SDLSoundObj& operator=(const SDLSoundObj&) = delete;
        SDLSoundObj& operator=(SDLSoundObj &&) = delete;
        SDLSoundObj(const std::string &path) : m_sound(Mix_LoadMUS(path.c_str()))
        {
            if (!m_sound)
                throw Arcade::MissingAsset("Error while loading sound : " + path);
        };
        ~SDLSoundObj()
        {
            Mix_FreeMusic(m_sound);
        };
        void play()
        {
            Mix_PlayMusic(m_sound, 1);
        };
    };

    class SDL : public IDisplay
    {
    private:
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        TTF_Font *m_font;
        std::map<std::string, std::shared_ptr<SDLTextureObj>> m_texture_map {};
        std::map<std::string, std::shared_ptr<SDLSoundObj>> m_sound_map {};
    public:
        SDL();
        ~SDL();
        SDL(const SDL&) = delete;
        SDL(SDL &&) = delete;
        SDL& operator=(const SDL&) = delete;
        SDL& operator=(SDL &&) = delete;
        Arcade::Input event();
        void clear();
        void refresh();
        void draw(std::shared_ptr<Arcade::IObject> object);
        void drawTile(Arcade::Tile *tile);
        void drawText(Arcade::Text *text);
        void playSound(Arcade::Sound *sound);
    };
    extern "C" SDL *entry_point();
}