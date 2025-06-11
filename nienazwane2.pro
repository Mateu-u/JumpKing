TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    SQL/sqlite3.c \
    databasemanager.cpp \
    main.cpp \
    platformgenerator.cpp \
    player.cpp

# SFML
INCLUDEPATH += "C:/SFML-2.5.1/include" \
               "C:/Users/CP24/Documents/nienazwane2/SQL"
LIBS        += -L"C:/SFML-2.5.1/lib"
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

HEADERS += \
    databasemanager.h \
    platformgenerator.h \
    player.h
