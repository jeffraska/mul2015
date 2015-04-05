#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "game.h"

#ifndef MAIN_H
#define	MAIN_H

sf::Mutex windowMutex;
sf::RenderWindow window;

sf::SoundBuffer shot;
sf::SoundBuffer step;
sf::Sound shotSound;
sf::Sound stepSound;

#endif	/* MAIN_H */

