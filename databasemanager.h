#pragma once

#include <string>
#include <vector>

// Struktur przechowująca informacje o graczu
struct PlayerInfo {
    int             id;
    std::string     username;
    int             levels_completed;
};

class DatabaseManager {
public:
    // Inicjalizuje połączenie z bazą danych
    static bool init(const std::string& dbPath);

    // Logowanie użytkownika (sprawdza login i hasło)
    static bool authenticate(const std::string& user, const std::string& pass);

    // Tworzenie nowego użytkownika
    static bool createUser(const std::string& user, const std::string& pass);

    // Pobiera listę wszystkich graczy z bazy
    static std::vector<PlayerInfo> fetchAllPlayers();

    // Aktualizuje liczbę ukończonych poziomów dla gracza
    static bool updateLevels(int playerId, int newCount);
};
