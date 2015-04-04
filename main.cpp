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
					if (event.key.code == sf::Keyboard::Right)
						g.player.play(g.playerRight);
					if (event.key.code == sf::Keyboard::Left)
						g.player.play(g.playerLeft);
					break;
				case sf::Event::KeyReleased:
					if (event.key.code == sf::Keyboard::Right)
						g.player.stop();
					if (event.key.code == sf::Keyboard::Left)
						g.player.stop();
					break;
			}
		}
	}
}

int main(int argc, char** argv) {
	// create window
	window.create(sf::VideoMode(800, 600), "MUL 2015");
	window.setVerticalSyncEnabled(true);
	game &g = game::getInstance();
	sf::Clock frameClock;

	// create window event handler thread
	sf::Thread eventThread(&eventHandler);
	eventThread.launch();

	// init player sprite
	g.player.setFrameTime(sf::milliseconds(50));
	g.player.setAnimation(g.playerRight);
	g.player.stop();
	g.player.setScale(4, 4);

	// main loop
	while (window.isOpen()) {
		sf::Time frameTime = frameClock.restart();

		// animate
		g.player.update(frameTime);

		// draw
		window.clear(sf::Color::White);
		window.draw(g.player);
		window.display();
	}

	// wait to terminate event thread
	eventThread.wait();

	return 0;
}

