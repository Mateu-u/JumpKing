TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

# ─── Źródła ────────────────────────────────────────────────────────────────
SOURCES += \
    SQL/sqlite3.c \
    databasemanager.cpp \
    main.cpp \
    platformgenerator.cpp \
    player.cpp \
    LoginScreen.cpp \
    MenuScreen.cpp \
    PlayerSelectScreen.cpp \
    CreditsScreen.cpp

# ─── Nagłówki ───────────────────────────────────────────────────────────────
HEADERS += \
    databasemanager.h \
    player.h \
    platformgenerator.h \
    GameState.h \
    LoginScreen.h \
    MenuScreen.h \
    PlayerSelectScreen.h \
    CreditsScreen.h

# ─── Ścieżki do nagłówków ───────────────────────────────────────────────────
INCLUDEPATH += \
    "C:/SFML-2.5.1/include" \
    $$PWD/SQL

# ─── Biblioteki SFML ────────────────────────────────────────────────────────
LIBS += -L"C:/SFML-2.5.1/lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d \
            -lsfml-graphics-d \
            -lsfml-network-d \
            -lsfml-system-d \
            -lsfml-window-d
} else {
    LIBS += -lsfml-audio \
            -lsfml-graphics \
            -lsfml-network \
            -lsfml-system \
            -lsfml-window
}

# ─── Deployment: kopiowanie bazy do folderu z exe ───────────────────────────
win32 {
    QMAKE_POST_LINK += $$quote(\
        if not exist \"$$OUT_PWD\\SQL\" mkdir \"$$OUT_PWD\\SQL\" && \
        copy /Y \"$$PWD\\SQL\\Database.sqlite\" \"$$OUT_PWD\\SQL\\Database.sqlite\"\
    )
}
