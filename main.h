#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "game.h"

#ifndef MAIN_H
#define	MAIN_H

sf::Mutex windowMutex;
sf::RenderWindow window;

sf::View gameView;
sf::View fixedView;

#endif	/* MAIN_H */
