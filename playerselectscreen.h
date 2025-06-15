#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "databasemanager.h"

class PlayerSelectScreen {
public:
    explicit PlayerSelectScreen(const sf::Font& font);
    void handleEvent(const sf::Event& event, GameState& state);
    void draw(sf::RenderWindow& window);

    // po wybraniu: id wybranego gracza
    int  getSelectedPlayerId() const { return selectedId; }

private:
    const sf::Font& font;
    std::vector<sf::Text> entries;
    std::vector<PlayerInfo> infos;
    int selected = 0;
    int selectedId = -1;

    void updateEntries();
    void updateHighlight();
};
