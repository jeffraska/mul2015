#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "game.h"
#include "main.h"
#include "particle.h"

using namespace std;

void renderThread() {
	cout << "Rendering thread created" << endl;
	sf::Clock frameClock;
	game &g = game::getInstance();

	sf::Text score;
	

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

	/*ParticleSystem fire(window.getSize());
	//fire.setPosition(500, 500);
	fire.setGravity(0, -7);
	fire.setParticleSpeed(25);
	fire.fuel(200);
	fire.setDissolutionRate(4);

	fire.setDistribution();
	fire.setDissolve();*/

	sf::Time interestRateTime;
	sf::Time dollarsTextsTime;
	//sf::Time a;

	// scrolling map view
	gameView = window.getView();
	fixedView = window.getView();
	//gameView.move(-500, 0);
	window.setView(gameView);

	// GUI
	score.setFont(g.fonts["Jose"]);
	score.setColor(sf::Color::Black);
	score.setCharacterSize(72);

	char scoreStr[10];

	lives.setFont(g.fonts["Jose"]);
	lives.setString("100");
	lives.setColor(sf::Color::Black);
	lives.setPosition(fixedView.getCenter().x - (lives.getLocalBounds().width), 0);
	lives.setCharacterSize(72);

	float ground = 800;
	
	//generate ground
	g.genGround(window.getSize(),1);

	while (window.isOpen()) {
		sf::Time frameTime = frameClock.restart();

		// animate
		g.player.animate(frameTime, ground);
		//fire.update(static_cast<float>(20) / 1000);
		for (auto i = 0; i < g.enemies.size(); i++)
		{
			g.enemies[i].animate(frameTime, ground);
		}
		for (auto i = 0; i < g.shots.size(); i++)
		{
			g.shots[i]->animate(frameTime);
		}

		// draw
		window.clear(sf::Color::White);

		// Zvyšování úrokù
		/*interestRateTime += frameTime;
		if (interestRateTime.asSeconds() > 1)
		{
			interestRateTime = sf::microseconds(interestRateTime.asMicroseconds() % frameTime.asMicroseconds());

			if (g.dollars > 0)
			{
				g.dollars += static_cast<float>(g.dollars) * 0.01;	// úrok 1% na bìžném úètu
			} else if (g.dollars < 0)
			{
				g.dollars += static_cast<float>(g.dollars) * 0.1;	// úrok 10% pøi dluhu
			}
		}*/

		/*a += frameTime;
		if (a.asMilliseconds() > 50)
		{
			a = sf::microseconds(a.asMicroseconds() % frameTime.asMicroseconds());
			fire.fuel(200);
		}*/

		//fire.setPosition(g.player.getPosition() + sf::Vector2f(100, 50));

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

		/*
			Vykreslit pozadí
			*/

		//window.draw(fire);

		// Draw enemies
		for (int i = 0; i < g.enemies.size(); i++)
		{
			window.draw(g.enemies[i].sprite);
		}

		// Draw Player
#pragma region
		if (g.player.getDirection() == Character::dRight) {
			window.draw(g.player.sprite);
			window.draw(g.player.weapon.sprite);
		}
		else {
			window.draw(g.player.weapon.sprite);
			window.draw(g.player.sprite);
		}
#pragma endregion player sprite
	
		// Draw Ground
		for (int k = 0; k < g.ground.size(); k++){
			window.draw(g.ground[k].sprite);
		}

		// Draw Shots
		for (int i = 0; i < g.shots.size(); i++)
		{
			window.draw(g.shots[i]->sprite);
		}

		// animate dolars texts
		dollarsTextsTime += frameTime;
		if (dollarsTextsTime.asMilliseconds() > 20)
		{
			dollarsTextsTime = sf::microseconds(dollarsTextsTime.asMicroseconds() % frameTime.asMicroseconds());

			for (vector<sf::Text>::iterator i = g.dollarsTexts.begin(); i != g.dollarsTexts.end(); ++i)
			{
				i->setPosition(i->getPosition() + sf::Vector2f(0, -10));
				i->setScale(i->getScale() + sf::Vector2f(0.1, 0.1));
				sf::Color c = i->getColor();
				c.a -= 15;	// 255 musí mýt dìlitelné tímto èíslem
				i->setColor(c);
				if (c.a == 0)
				{
					g.dollarsTexts.erase(i);
					--i;
				}
			}
		}

		// render dollar texts
		for (int i = 0; i < g.dollarsTexts.size(); i++)
		{
			window.draw(g.dollarsTexts[i]);
		}

		// render explosions
		for (vector<AnimatedSprite>::iterator i = g.explosions.begin(); i != g.explosions.end(); ++i)
		{
			i->update(frameTime);
			window.draw(*i);
		}

		// Draw GUI
#pragma region

		window.setView(fixedView);

		if (g.dollars < -9999999)
		{
			sprintf_s(scoreStr, 10, "$    -moc");
		} else if (g.dollars > 99999999)
		{
			sprintf_s(scoreStr, 10, "$     moc");
		}
		else
		{
			sprintf_s(scoreStr, 10, "$%8d", static_cast<int>(g.dollars));
		}
		score.setString(scoreStr);
		window.draw(score);

		lives.setString(to_string(g.player.lives));
		window.draw(lives);

		window.setView(gameView);

#pragma endregion GUI

		window.display();
		sf::sleep(sf::milliseconds(5));
	}
	cout << "Rendering thread stop" << endl;
}

int main(int argc, char** argv) {
	// create window
	window.create(sf::VideoMode::getDesktopMode(), "MUL 2015"/*, sf::Style::Fullscreen*/);
	window.setVerticalSyncEnabled(true);
	window.setKeyRepeatEnabled(false);
	window.setActive(false);
	game &g = game::getInstance();
	

	//sf::Sound shotSound;
	sf::Sound stepSound;
	sf::Sound explosionSound;
	sf::Sound gunloadSound;

	// create rendering thread
	sf::Thread renderThread(&renderThread);
	renderThread.launch();

	// init character sprites
	g.player.init(sf::milliseconds(10), 10);
	g.tankTemplate.init(sf::milliseconds(10), 5, 1000);
	

	g.player.setPosition(
		50,
		//sf::VideoMode::getDesktopMode().width / 2 - 100,
		sf::VideoMode::getDesktopMode().height / 2 - 125
		);

	g.tankTemplate.go(Character::dLeft);
	//g.tankTemplate.stop();
	//g.tankTemplate.fire();

	g.newEnemy(1500, 500);
	g.newEnemy(3500, 500);
	g.newEnemy(4000, 500);
	
	

	// prepare sounds
	stepSound.setBuffer(g.sounds["step"]);
	gunloadSound.setBuffer(g.sounds["gunload"]);
	gunloadSound.setVolume(50);

	// play background sounds
	g.music.at("horor")->play();
	g.music.at("drums")->play();

	// main loop
	sf::Event event;
	bool doLoop = true;
	float endPos = (window.getSize().x);
	float startPos = 0.;
	int collision = 0;

	while (window.isOpen()) {
		if (window.waitEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				doLoop = false;
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
				{
					doLoop = false;
					window.close();
					break;
				}
				if (event.key.code == sf::Keyboard::Right) {
					window.setKeyRepeatEnabled(true);
					if (stepSound.getLoop() != true){
						g.player.go(Character::dRight);
						stepSound.setLoop(true);
						stepSound.play();
					}
					
					if (g.player.getPosition().x > endPos-300){
						gameView.move(500, 0);
						endPos += 500;
						startPos += 500;
					}

					collision = g.groundCollision(g.player.getPosition(), 1);
					switch (collision){
					case 1: //ground
						break;
					case 2: //barrier
						if (g.barrier){
							g.player.stop();
							stepSound.setLoop(false);
							window.setKeyRepeatEnabled(false);
							break;
						}
					case 3: //fire
						if (g.barrier == true){
							g.player.lives--;
						}
						break;
					case 4://flower
						if (g.barrier == true){
							//nejakej zvuk
						}
						break;
					/*default://dira
						g.player.setPosition(g.player.getPosition().x, collision);
						break;*/
					}
				}
				if (event.key.code == sf::Keyboard::Left) {
					window.setKeyRepeatEnabled(true);
					if (stepSound.getLoop() != true){
						g.player.go(Character::dLeft);
						stepSound.setLoop(true);
						stepSound.play();
					}

					if (g.player.getPosition().x < startPos+300){
						gameView.move(500*-1, 0);
						endPos -= 500;
						startPos -= 500;
					}

					collision = g.groundCollision(g.player.getPosition(), -1);
					switch (collision){
					case 1: //rovina
						break;
					case 2: //prekazka
						if (g.barrier){
							g.player.stop();
							stepSound.setLoop(false);
							window.setKeyRepeatEnabled(false);
							break;
						}
					case 3: //fire
						if (g.barrier == true){
							g.player.lives--;
						}
						break;
					case 4://flower
						if (g.barrier == true){
							//nejakej zvuk
						}
						break;
					}
				}
				if (event.key.code == sf::Keyboard::LControl) {
					g.player.fire();
				}
				if (event.key.code == sf::Keyboard::Space ||
					event.key.code == sf::Keyboard::Up)
				{
					g.player.jump();
				}
				if (event.key.code == sf::Keyboard::F1) {
					g.player.setWeapon(g.weapons["machinegun"]);
					gunloadSound.play();
				}
				if (event.key.code == sf::Keyboard::F2) {
					g.player.setWeapon(g.weapons["laser"]);
					gunloadSound.play();
				}
				break;
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::Right &&
					g.player.getDirection() == Character::dRight) {
					g.player.stop();
					stepSound.setLoop(false);

					if (g.player.getPosition().x > endPos - 300){
						gameView.move(500, 0);
						endPos += 500;
						startPos += 500;
					}

					collision = g.groundCollision(g.player.getPosition(),1);
					switch (collision){
					case 1: //rovina
						break;
					case 2: //prekazka
						if (g.barrier){
							g.player.stop();
							stepSound.setLoop(false);
							window.setKeyRepeatEnabled(false);
							break;
						}
					case 3: //fire
						if (g.barrier == true){
							g.player.lives--;
						}
						break;
					case 4://flower
						if (g.barrier == true){
							//nejakej zvuk
						}
						break;
					}
					window.setKeyRepeatEnabled(false);
				}
				if (event.key.code == sf::Keyboard::Left &&
					g.player.getDirection() == Character::dLeft) {
					g.player.stop();
					stepSound.setLoop(false);

					if (g.player.getPosition().x < startPos + 300){
						gameView.move(500 * -1, 0);
						endPos -= 500;
						startPos -= 500;
					}

					collision = g.groundCollision(g.player.getPosition(), -1);
					switch (collision){
					case 1: //rovina
						break;
					case 2: //prekazka
						if (g.barrier){
							g.player.stop();
							stepSound.setLoop(false);
							window.setKeyRepeatEnabled(false);
							break;
						}
					case 3: //fire
						if (g.barrier == true){
							g.player.lives--;
						}
						break;
					case 4: // flower
						if (g.barrier == true){
							//nejakej zvuk
						}
						break;
					}
					window.setKeyRepeatEnabled(false);
				}
				if (event.key.code == sf::Keyboard::LControl) {
					g.player.holdFire();
				}
				break;
			}
		}
	}

	// wait to terminate render thread
	renderThread.wait();

	return 0;
}