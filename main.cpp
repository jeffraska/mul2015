#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "game.h"
#include "main.h"
#include "particle.h"

using namespace std;

void renderThread() {
	sf::Clock frameClock;
	game &g = game::getInstance();

	/*vector<int> terrain;
	terrain.push_back(0);

	srand(time(NULL));
	for (int i = 0; i < 10; i++)
	{
	int random = rand() % 100 - 50;
	if (random > 30)
	terrain.push_back(terrain[i] + 1);
	else if (random < -30)
	terrain.push_back(terrain[i] - 1);
	else
	terrain.push_back(terrain[i]);
	}*/

	/*sf::Shader shader;
	if (sf::Shader::isAvailable())
	{
	const string fragmentShader = "uniform sampler2D texture; void main() { vec4 ref = texture2D(texture, gl_TexCoord[0].xy) * gl_Color; gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) - ref; gl_FragColor.a = ref.a;	}";
	shader.loadFromMemory(fragmentShader, sf::Shader::Fragment);
	shader.setParameter("texture", sf::Shader::CurrentTexture);
	}*/

	ParticleSystem fire(window.getSize());
	fire.setShape(Shape::SQUARE);
	fire.setPosition(500, 500);
	fire.setGravity(0, 0.5);
	fire.setParticleSpeed(50);
	fire.fuel(100);
	fire.setDissolutionRate(5);

	fire.setDistribution();
	fire.setDissolve();

	while (window.isOpen()) {
		sf::Time frameTime = frameClock.restart();

		// animate
		g.player.animate(frameTime);
		fire.update(static_cast<float>(20) / 1000);

		// draw
		window.clear(sf::Color::White);

		/*for (int i = 0; i < terrain.size() - 1; i++)
		{
		//sf::RectangleShape line(sf::Vector2f()
		int w = 100;
		int h = 50;
		sf::Vertex line[] = {
		sf::Vertex(sf::Vector2f(i * w, terrain[i] * h + window.getSize().y / 2 + 125), sf::Color::Black),
		sf::Vertex(sf::Vector2f((i + 1) * w, terrain[i + 1] * h + window.getSize().y / 2 + 125), sf::Color::Black)
		};

		window.draw(line, 2, sf::Lines);
		}*/

		if (g.player.getDirection() == Character::dRight) {
			window.draw(g.player.sprite);
			window.draw(g.player.weapon.sprite);
		}
		else {
			window.draw(g.player.weapon.sprite);
			window.draw(g.player.sprite);
		}

		window.draw(fire);

		window.display();
		sf::sleep(sf::milliseconds(5));
	}
}

int main(int argc, char** argv) {
	// create window
	window.create(sf::VideoMode::getDesktopMode(), "MUL 2015"/*, sf::Style::Fullscreen*/);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	window.setActive(false);
	game &g = game::getInstance();

	sf::Sound shotSound;
	sf::Sound stepSound;
	sf::Sound explosionSound;
	sf::Sound gunloadSound;

	// create rendering thread
	sf::Thread renderThread(&renderThread);
	renderThread.launch();

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
	sf::Event event;
	bool doLoop = true;

	while (window.isOpen()) {
		if (window.waitEvent(event)) {
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
					}
					else {
						shotSound.setBuffer(g.sounds[g.player.weapon.name]);
					}
				}
				if (event.key.code == sf::Keyboard::F2) {
					g.player.setWeapon(g.weapons["laser"]);
					gunloadSound.play();
					if (shotSound.getStatus() == sf::Sound::Playing) {
						shotSound.setBuffer(g.sounds[g.player.weapon.name]);
						shotSound.play();
					}
					else {
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

	// wait to terminate render thread
	renderThread.wait();

	return 0;
}