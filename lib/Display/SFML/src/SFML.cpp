#include "SFML.hpp"
#include "Exception.hpp"
#include <memory>

extern "C" Arcade::SFML *Arcade::entry_point()
{
    return new Arcade::SFML;
}

std::map<Arcade::Color, sf::Color> colormap {
        {Arcade::Color::BLACK, sf::Color::Black},
        {Arcade::Color::RED, sf::Color::Red},
        {Arcade::Color::GREEN, sf::Color::Green},
        {Arcade::Color::YELLOW, sf::Color::Yellow},
        {Arcade::Color::BLUE, sf::Color::Blue},
        {Arcade::Color::MAGENTA, sf::Color::Magenta},
        {Arcade::Color::CYAN, sf::Color::Cyan},
        {Arcade::Color::WHITE, sf::Color::White},
};

Arcade::SFML::SFML()
{
    m_window.create(sf::VideoMode(1600, 1000), "Arcade - SFML");
    m_window.setFramerateLimit(60);
    m_window.setKeyRepeatEnabled(true);
    if (!m_font.loadFromFile("assets/font.ttf"))
        throw Arcade::exception("Failed to load font");
}

Arcade::SFML::~SFML()
{
    m_window.close();
}

Arcade::Input Arcade::SFML::event()
{
    sf::Event ev{};
    Arcade::Input input = NIL;

    while (m_window.pollEvent(ev)) {
        if (ev.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, ev.size.width, ev.size.height);
            m_window.setView(sf::View(visibleArea));
        }
        else if (ev.type == sf::Event::Closed)
            input = ESCAPE;
        else if (ev.type == sf::Event::KeyPressed) {
            if (ev.key.code == sf::Keyboard::Up)
                input = UP;
            else if (ev.key.code == sf::Keyboard::Down)
                input = DOWN;
            else if (ev.key.code == sf::Keyboard::Left)
                input = LEFT;
            else if (ev.key.code == sf::Keyboard::Right)
                input = RIGHT;
            else if (ev.key.code == sf::Keyboard::Enter)
                input = ENTER;
            else if (ev.key.code == sf::Keyboard::Space)
                input = SPACE;
            else if (ev.key.code == sf::Keyboard::Escape)
                input = ESCAPE;
            else if (ev.key.code == sf::Keyboard::R)
                input = RESET;
            else if (ev.key.code == sf::Keyboard::M)
                input = MENU;
            else if (ev.key.code == sf::Keyboard::N)
                input = N;
            else if (ev.key.code == sf::Keyboard::P)
                input = P;
            else if (ev.key.code == sf::Keyboard::I)
                input = I;
            else if (ev.key.code == sf::Keyboard::O)
                input = O;
        }
    }
    return input;
}

void Arcade::SFML::refresh()
{
    m_window.display();
}

void Arcade::SFML::clear()
{
    m_window.clear(sf::Color::Black);
}

void Arcade::SFML::draw(std::shared_ptr<Arcade::IObject> object)
{
    if (dynamic_cast<Arcade::DynamicTile*>(object.get()) != nullptr)
        drawTile(dynamic_cast<Arcade::DynamicTile *>(object.get())->getActualTile());
    else if (dynamic_cast<Arcade::Tile*>(object.get()) != nullptr)
        drawTile(dynamic_cast<Arcade::Tile*>(object.get()));
    else if (dynamic_cast<Arcade::Text*>(object.get()) != nullptr)
        drawText(dynamic_cast<Arcade::Text *>(object.get()));
    else if (dynamic_cast<Arcade::Sound*>(object.get()) != nullptr)
        playSound(dynamic_cast<Arcade::Sound*>(object.get()));
}

void Arcade::SFML::drawTile(Arcade::Tile *tile)
{
    if (m_texture_map.find(tile->getPath()) == m_texture_map.end()) {
        m_texture_map[tile->getPath()] = std::make_shared<sf::Texture>();
        if (!m_texture_map[tile->getPath()]->loadFromFile(tile->getPath()))
            throw Arcade::MissingAsset("SFML Failed to load a texture");
    }
    std::shared_ptr<sf::Sprite> sprite = std::make_shared<sf::Sprite>(*m_texture_map[tile->getPath()]);
    sprite->setPosition((tile->getPosition().first * Arcade::getTileSize()) + Arcade::getTileSize() / 2, (tile->getPosition().second * Arcade::getTileSize()) + Arcade::getTileSize() / 2);
    sprite->setOrigin(Arcade::getTileSize() / 2, Arcade::getTileSize() / 2);
    sprite->setRotation(tile->getRotation());

    m_window.draw(*sprite);
}

void Arcade::SFML::drawText(Arcade::Text *text)
{
    std::shared_ptr<sf::Text> txtobj = std::make_shared<sf::Text>();
    txtobj->setFont(m_font);
    txtobj->setString(text->getText());
    txtobj->setPosition(text->getPosition().first * Arcade::getTileSize(), text->getPosition().second * Arcade::getTileSize());
    txtobj->setCharacterSize(30);
    txtobj->setFillColor(colormap[text->getColor()]);

    m_window.draw(*txtobj);
}

void Arcade::SFML::playSound(Arcade::Sound *sound)
{
    if (m_sound_map.find(sound->getSound()) == m_sound_map.end()) {
        m_sound_map[sound->getSound()].first = std::make_shared<sf::SoundBuffer>();
        if (!m_sound_map[sound->getSound()].first->loadFromFile(sound->getSound()))
            throw Arcade::MissingAsset("SFML Failed to load a sound");
        m_sound_map[sound->getSound()].second = std::make_shared<sf::Sound>(*m_sound_map[sound->getSound()].first);
    }
    m_sound_map[sound->getSound()].second->play();
}