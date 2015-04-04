#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "game.h"
#include "main.h"

using namespace std;

/**
 * Handles main window input events
 */
void eventHandler() {
	sf::Event event;
	bool doLoop = true;
	game &g = game::getInstance();

	while (doLoop) {

		if (window.pollEvent(event)) {
			switch (event.type) {
				case sf::Event::Closed:
					doLoop = false;
					window.close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Right) {
						g.player.go(character::dRight);
					}
					if (event.key.code == sf::Keyboard::Left) {
						g.player.go(character::dLeft);
					}
					if (event.key.code == sf::Keyboard::LControl) {
						g.player.fire();
					}
					break;
				case sf::Event::KeyReleased:
					if (event.key.code == sf::Keyboard::Right)
						g.player.stop();
					if (event.key.code == sf::Keyboard::Left)
						g.player.stop();
					if (event.key.code == sf::Keyboard::LControl)
						g.player.holdFire();
					break;
			}
		}
	}
}

int main(int argc, char** argv) {
	// create window
	window.create(sf::VideoMode::getDesktopMode(), "MUL 2015");
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	game &g = game::getInstance();
	sf::Clock frameClock;

	// create window event handler thread
	sf::Thread eventThread(&eventHandler);
	eventThread.launch();

	// init player sprite
	g.player.init(sf::milliseconds(75));

	g.player.setPosition(
			sf::VideoMode::getDesktopMode().width / 2 - 100,
			sf::VideoMode::getDesktopMode().height / 2 - 125
			);

	// main loop
	while (window.isOpen()) {
		sf::Time frameTime = frameClock.restart();

		// animate
		g.player.animate(frameTime);

		// draw
		window.clear(sf::Color::White);

		if (g.player.getDirection() == character::dRight) {
			window.draw(g.player.sprite);
			window.draw(g.player.gun);
		} else {
			window.draw(g.player.gun);
			window.draw(g.player.sprite);
		}
		window.display();
	}

	// wait to terminate event thread
	eventThread.wait();

	return 0;
}

