#pragma once

enum class GameState {
    Welcome,     // ten ekran
    Auth,        // logowanie / gra jako gość
    Options,     // opcje z suwakami (w przyszłości)
    ScoreTable,  // tabela wyników
    Credits,     // credits
    Playing      // rozgrywka
};
