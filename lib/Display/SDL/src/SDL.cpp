#include "SDL.hpp"
#include <memory>

extern "C" Arcade::SDL *Arcade::entry_point()
{
    return new Arcade::SDL;
}

std::map<Arcade::Color, SDL_Color> colormap {
        {Arcade::Color::BLACK, {0, 0, 0, 0}},
        {Arcade::Color::RED, {255, 0, 0, 0}},
        {Arcade::Color::GREEN, {0, 255, 0, 0}},
        {Arcade::Color::YELLOW, {255, 255, 0, 0}},
        {Arcade::Color::BLUE, {0, 0, 255, 0}},
        {Arcade::Color::MAGENTA, {255, 0, 255, 0}},
        {Arcade::Color::CYAN, {0, 255, 255, 0}},
        {Arcade::Color::WHITE, {255, 255, 255, 0}},
};

Arcade::SDL::SDL() : m_window(nullptr), m_renderer(nullptr), m_font(nullptr)
{
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
        throw Arcade::exception("Error SDL_Init : " + std::string(SDL_GetError()));

    m_window = SDL_CreateWindow("Arcade - SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                1600, 1000, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (m_window == NULL)
        throw Arcade::exception("Error SDL_CreateWindow : " + std::string(SDL_GetError()));

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

    if (m_renderer == NULL)
        throw Arcade::exception("Error SDL_CreateRenderer : " + std::string(SDL_GetError()));

    if (TTF_Init() == -1)
        throw Arcade::exception("Error TTF_Init : " + std::string(TTF_GetError()));
    m_font = TTF_OpenFont("assets/font.ttf", 30);
    if (m_font == NULL)
        throw Arcade::exception("Error font not found");
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
        throw Arcade::exception("Error Mix_OpenAudio : " + std::string(Mix_GetError()));
}

Arcade::SDL::~SDL()
{
    m_texture_map.clear();
    m_sound_map.clear();
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    TTF_CloseFont(m_font);
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

Arcade::Input Arcade::SDL::event()
{
    SDL_Event event;
    Arcade::Input input = NIL;

    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT)
            input = ESCAPE;
        else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_LEFT)
                input = LEFT;
            else if (event.key.keysym.sym == SDLK_RIGHT)
                input = RIGHT;
            else if (event.key.keysym.sym == SDLK_UP)
                input = UP;
            else if (event.key.keysym.sym == SDLK_DOWN)
                input = DOWN;
            else if (event.key.keysym.sym == SDLK_RETURN)
                input = ENTER;
            else if (event.key.keysym.sym == SDLK_SPACE)
                input = SPACE;
            else if (event.key.keysym.sym == SDLK_ESCAPE)
                input = ESCAPE;
            else if (event.key.keysym.sym == SDLK_r)
                input = RESET;
            else if (event.key.keysym.sym == SDLK_m)
                input = MENU;
            else if (event.key.keysym.sym == SDLK_n)
                input = N;
            else if (event.key.keysym.sym == SDLK_p)
                input = P;
            else if (event.key.keysym.sym == SDLK_i)
                input = I;
            else if (event.key.keysym.sym == SDLK_o)
                input = O;
        }
    }
    return input;
}

void Arcade::SDL::refresh()
{
    SDL_RenderPresent(m_renderer);
}

void Arcade::SDL::clear()
{
    SDL_RenderClear(m_renderer);
}

void Arcade::SDL::draw(std::shared_ptr<Arcade::IObject> object)
{
    if (dynamic_cast<Arcade::DynamicTile*>(object.get()) != nullptr)
        drawTile(dynamic_cast<Arcade::DynamicTile *>(object.get())->getActualTile());
    else if (dynamic_cast<Arcade::Tile*>(object.get()) != nullptr)
        drawTile(dynamic_cast<Arcade::Tile *>(object.get()));
    else if (dynamic_cast<Arcade::Text*>(object.get()) != nullptr)
        drawText(dynamic_cast<Arcade::Text *>(object.get()));
    else if (dynamic_cast<Arcade::Sound*>(object.get()) != nullptr)
        playSound(dynamic_cast<Arcade::Sound*>(object.get()));
}

void Arcade::SDL::drawTile(Arcade::Tile *tile)
{
    if (m_texture_map.find(tile->getPath()) == m_texture_map.end())
        m_texture_map[tile->getPath()] = std::make_shared<SDLTextureObj>(tile->getPath(), m_renderer);
    m_texture_map[tile->getPath()]->setPosition(tile->getPosition().first, tile->getPosition().second);
    SDL_RenderCopyEx(m_renderer, m_texture_map[tile->getPath()]->m_img, nullptr, &m_texture_map[tile->getPath()]->m_rect, tile->getRotation(), &m_texture_map[tile->getPath()]->m_center, static_cast<SDL_RendererFlip>(SDL_FLIP_NONE));
}

void Arcade::SDL::drawText(Arcade::Text *text)
{
    std::shared_ptr<SDLTextureObj> tmpTexture = std::make_shared<SDLTextureObj>(*text, m_font, m_renderer);
    tmpTexture->setPosition(text->getPosition().first, text->getPosition().second);
    SDL_RenderCopy(m_renderer, tmpTexture->m_img, nullptr, &tmpTexture->m_rect);
}

void Arcade::SDL::playSound(Arcade::Sound *sound)
{
    if (m_sound_map.find(sound->getSound()) == m_sound_map.end())
        m_sound_map[sound->getSound()] = std::make_shared<SDLSoundObj>(sound->getSound());
    m_sound_map[sound->getSound()]->play();
}

Arcade::SDLTextureObj::SDLTextureObj(const std::string &path, SDL_Renderer *renderer)
    : m_img(NULL)
{
    m_img = IMG_LoadTexture(renderer, path.c_str());

    int w, h;
    SDL_QueryTexture(m_img, NULL, NULL, &w, &h);
    m_rect.h = h;
    m_rect.w = w;
    m_center.x = m_rect.w / 2;
    m_center.y = m_rect.h / 2;
}

Arcade::SDLTextureObj::SDLTextureObj(Arcade::Text text, TTF_Font *font, SDL_Renderer *renderer)
    : m_img(NULL)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.getText().c_str(), colormap[text.getColor()]);
    m_img = SDL_CreateTextureFromSurface(renderer, textSurface);
    int w, h;
    SDL_QueryTexture(m_img, NULL, NULL, &w, &h);
    m_rect.h = h;
    m_rect.w = w;
    SDL_FreeSurface(textSurface);
}

void Arcade::SDLTextureObj::setPosition(float x, float y)
{
    m_rect.x = x * Arcade::getTileSize();
    m_rect.y = y * Arcade::getTileSize();
}

Arcade::SDLTextureObj::~SDLTextureObj()
{
    SDL_DestroyTexture(m_img);
}
