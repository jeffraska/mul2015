#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "game.h"

#ifndef MAIN_H
#define	MAIN_H

sf::Mutex windowMutex;
sf::RenderWindow window;

sf::Sound shotSound;
sf::Sound stepSound;
sf::Sound explosionSound;
sf::Sound gunloadSound;

#endif	/* MAIN_H */

