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
						g.player.go(Character::dRight);
						stepSound.setLoop(true);
						stepSound.play();
					}
					if (event.key.code == sf::Keyboard::Left) {
						g.player.go(Character::dLeft);
						stepSound.setLoop(true);
						stepSound.play();
					}
					if (event.key.code == sf::Keyboard::LControl) {
						g.player.fire();
						shotSound.setLoop(true);
						shotSound.play();
					}
					if (event.key.code == sf::Keyboard::Space) {
						explosionSound.play();
					}
					if (event.key.code == sf::Keyboard::F1) {
						g.player.setWeapon(g.weapons["machinegun"]);
						gunloadSound.play();
						if (shotSound.getStatus() == sf::Sound::Playing) {
							shotSound.setBuffer(g.sounds[g.player.weapon.name]);
							shotSound.play();
						} else {
							shotSound.setBuffer(g.sounds[g.player.weapon.name]);
						}
					}
					if (event.key.code == sf::Keyboard::F2) {
						g.player.setWeapon(g.weapons["laser"]);
						gunloadSound.play();
						if (shotSound.getStatus() == sf::Sound::Playing) {
							shotSound.setBuffer(g.sounds[g.player.weapon.name]);
							shotSound.play();
						} else {
							shotSound.setBuffer(g.sounds[g.player.weapon.name]);
						}
					}
					break;
				case sf::Event::KeyReleased:
					if (event.key.code == sf::Keyboard::Right &&
							g.player.getDirection() == Character::dRight) {
						g.player.stop();
						stepSound.setLoop(false);
					}
					if (event.key.code == sf::Keyboard::Left &&
							g.player.getDirection() == Character::dLeft) {
						g.player.stop();
						stepSound.setLoop(false);
					}
					if (event.key.code == sf::Keyboard::LControl) {
						g.player.holdFire();
						shotSound.setLoop(false);
					}
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
	g.player.init(sf::milliseconds(10), 5);

	g.player.setPosition(
			sf::VideoMode::getDesktopMode().width / 2 - 100,
			sf::VideoMode::getDesktopMode().height / 2 - 125
			);

	// prepare sounds
	shotSound.setBuffer(g.sounds[g.player.weapon.name]);
	shotSound.setVolume(50);
	stepSound.setBuffer(g.sounds["step"]);
	explosionSound.setBuffer(g.sounds["explosion"]);
	gunloadSound.setBuffer(g.sounds["gunload"]);
	gunloadSound.setVolume(50);

	// play background sounds
	g.music.at("horor")->play();
	g.music.at("drums")->play();

	// main loop
	while (window.isOpen()) {
		sf::Time frameTime = frameClock.restart();

		// animate
		g.player.animate(frameTime);

		// draw
		window.clear(sf::Color::White);

		if (g.player.getDirection() == Character::dRight) {
			window.draw(g.player.sprite);
			window.draw(g.player.weapon.sprite);
		} else {
			window.draw(g.player.weapon.sprite);
			window.draw(g.player.sprite);
		}
		window.display();
	}

	// wait to terminate event thread
	eventThread.wait();

	return 0;
}

