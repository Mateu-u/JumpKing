TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    SQL/sqlite3.c \
    databasemanager.cpp \
    main.cpp \
    optionsscreen.cpp \
    platformgenerator.cpp \
    player.cpp \
    WelcomeScreen.cpp \
    AuthScreen.cpp \
    ScoreTableScreen.cpp \
    CreditsScreen.cpp

HEADERS += \
    databasemanager.h \
    GameState.h \
    Player.h \
    PlatformGenerator.h \
    WelcomeScreen.h \
    AuthScreen.h \
    ScoreTableScreen.h \
    CreditsScreen.h \
    optionsscreen.h

INCLUDEPATH += \
    "C:/SFML-2.5.1/include" \
    $$PWD/SQL

LIBS += -L"C:/SFML-2.5.1/lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d \
            -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics \
            -lsfml-network -lsfml-system -lsfml-window
}

win32 {
    QMAKE_POST_LINK += $$quote(\
        if not exist \"$$OUT_PWD\\SQL\" mkdir \"$$OUT_PWD\\SQL\" && \
        copy /Y \"$$PWD\\SQL\\Database.sqlite\" \"$$OUT_PWD\\SQL\\Database.sqlite\" && \
        if not exist \"$$OUT_PWD\\debug\" mkdir \"$$OUT_PWD\\debug\" && \
        xcopy /Y /I /E \"$$PWD\\assets\\fonts\" \"$$OUT_PWD\\debug\"\
    )
}
