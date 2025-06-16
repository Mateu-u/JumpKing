#include <SFML/Graphics.hpp>
#include "GameState.h"

class OptionsScreen {
public:
    OptionsScreen(const sf::Font& font, const sf::Vector2f& windowSize);

    // Obsługa eventów: zmienia stan na Welcome po kliknięciu 'Wróć'
    void handleEvent(const sf::Event& ev, GameState& state, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

 //   float getGameVolume() const  { return gameVolume; }
  //  float getMusicVolume() const { return musicVolume; }
    const std::string& getBackgroundPath() const { return backgroundPath; }

private:
    struct Button {
        sf::RectangleShape box;
        sf::Text            label;
    };
    struct Slider {
        sf::RectangleShape track;
        sf::RectangleShape thumb;
        sf::Text            label;
        float               value = 50.f; // 0–100
    };

    const sf::Font&      font;
    sf::Vector2f         winSize;

    // podgląd tła
    sf::Texture          bgPreviewTex;
    sf::Sprite           bgPreview;
    std::string          backgroundPath = "";

    // suwaki
    Slider               gameSlider;
    Slider               musicSlider;
    bool                 draggingGame  = false;
    bool                 draggingMusic = false;

    // przyciski
    Button               loadBgBtn;
    Button               backBtn;
    int                  hoveredBtn = -1;

    void createButton(Button& b, const std::string& text, const sf::Vector2f& pos);
    void updateHover(const sf::Vector2f& mp);
    void updateSliders(const sf::Vector2f& mp);
};
