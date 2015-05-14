#include <dirent.h>
#include <iostream>
#include "game.h"
#include "GroundPanel.h"

// sf::Music and sf::SoundBuffer must be created separately in different threads
sf::Mutex OpenALMutex;

game::game() {
	sf::Thread texT(&game::loadTextures, this);
	texT.launch();
	sf::Thread sndT(&game::loadSounds, this);
	sndT.launch();
	sf::Thread musT(&game::loadMusic, this);
	musT.launch();
	sf::Thread fontT(&game::loadFonts, this);
	fontT.launch();

	texT.wait();
	sndT.wait();
	musT.wait();
	fontT.wait();
}

game::~game() {
	for (int i = 0; i < music.size(); i++)
	{
		music.begin()->second->stop();
		delete music.begin()->second;
		music.erase(music.begin());
	}
}

void game::init() {
	// prepare player animation
	player.setTexture(
		Character::tCharacterWalk,
		Character::dRight,
		textures.at("player"),
		sf::IntRect(0, 0, 200, 250),
		10
		);

	player.setTexture(
		Character::tCharacterWalk,
		Character::dLeft,
		textures.at("player"),
		sf::IntRect(0, 250, 200, 250),
		10
		);

	// prepare tank animation
	tankTemplate.setTexture(
		Character::tCharacterWalk,
		Character::dRight,
		textures["tank"],
		sf::IntRect(0, 0, 450, 175),
		10
		);

	tankTemplate.setTexture(
		Character::tCharacterWalk,
		Character::dLeft,
		textures["tank"],
		sf::IntRect(0, 175, 450, 175),
		10
		);

	Weapon machinegun(
		"machinegun",							// name (require textures/<name>.png, textures/<name>shot.png, sounds/<name>.ogg)
		sf::Vector2f(40, 100),				// offset
		sf::IntRect(0, 0, 200, 100),		// walk right sprite coordinates
		10,									// number of walk right sprites
		sf::IntRect(0, 100, 200, 100),		// walk left sprite coordinates
		10,									// number of walk left sprites
		sf::IntRect(0, 200, 200, 100),		// fire right sprite coordinates
		10,									// number of fire right sprites
		sf::IntRect(0, 300, 200, 100),		// fire left sprite coordinates
		10,									// number of fire right sprites
		sf::IntRect(0, 0, 27, 34),			// shot right sprite coordinates
		1,									// number of shot right sprites
		sf::IntRect(0, 34, 27, 34),			// shot left sprite coordinates
		1,									// number of shot left sprites
		15,									// speed of shot
		10,									// shot price in $
		sf::milliseconds(100),				// shot rate
		1500,								// max shot distance
		50									// shot damage
		);

	Weapon laser(
		"laser",							// name (require textures/<name>.png, textures/<name>shot.png, sounds/<name>.ogg)
		sf::Vector2f(40, 100),				// offset
		sf::IntRect(0, 0, 200, 100),		// walk right sprite coordinates
		10,									// number of walk right sprites
		sf::IntRect(0, 100, 200, 100),		// walk left sprite coordinates
		10,									// number of walk left sprites
		sf::IntRect(0, 200, 200, 100),		// fire right sprite coordinates
		10,									// number of fire right sprites
		sf::IntRect(0, 300, 200, 100),		// fire left sprite coordinates
		10,									// number of fire left sprites
		sf::IntRect(0, 0, 80, 34),			// shot right sprite coordinates
		10,									// number of shot right sprites
		sf::IntRect(0, 34, 80, 34),			// shot left sprite coordinates
		10,									// number of shot left sprites
		20,									// speed of shot
		20,									// shot price in $
		sf::milliseconds(200),				// shot rate
		2000,								// max shot distance
		75									// shot damage
		);

	Weapon tankcanon(
		"tankcanon",						// name (require textures/<name>.png, textures/<name>shot.png, sounds/<name>.ogg)
		sf::Vector2f(-50, -10),				// offset
		sf::IntRect(0, 0, 0, 0),			// walk right sprite coordinates
		1,									// number of walk right sprites
		sf::IntRect(0, 0, 0, 0),			// walk left sprite coordinates
		1,									// number of walk left sprites
		sf::IntRect(0, 0, 0, 0),			// fire right sprite coordinates
		1,									// number of fire right sprites
		sf::IntRect(0, 0, 0, 0),			// fire left sprite coordinates
		1,									// number of fire right sprites
		sf::IntRect(0, 0, 27, 34),			// shot right sprite coordinates
		1,									// number of shot right sprites
		sf::IntRect(0, 34, 27, 34),			// shot left sprite coordinates
		1,									// number of shot left sprites
		25,									// speed of shot
		0,									// shot price in $
		sf::milliseconds(2000),				// shot rate
		2000,								// max shot distance,
		100									// shot damage
		);

	explosionAnimation.setSpriteSheet(textures["explosion"]);
	explosionAnimation.addXFrames(sf::IntRect(0, 0, 320, 240), 10);

	player.weapon = weapons["machinegun"];
	tankTemplate.weapon = weapons["tankcanon"];

	explosionSound.setBuffer(sounds["explosion"]);
}

void game::newEnemy(float x, float y)
{
	enemies.push_back(tankTemplate);
	enemies.back().setPosition(x, y);
}

void game::genGround(sf::Vector2u windowSize, int direction){
	/*int newX = lastPosition.x;
	int genY;
	int genX;

	// clear
	ground.clear();
	//last position
	ground.push_back(lastPosition);

	//generate new positions
	for (int i = 0; i < windowSize.x; i += genX){
		genY = rand() % (windowSize.y ) - 10;
		genX = rand() % (windowSize.x / 2) - 50;

		ground.push_back(sf::Vector2i(newX, genY));
		newX += genX;
		ground.push_back( sf::Vector2i(newX, genY));
	}
	return ground.at(ground.size()-1);*/

	int j = 100;
	int genTexType;
	int lives;
	sf::Vector2f lastPosition;


	for (int i = 0; i < j; i++){
		GroundPanel panel;
		lives = 0;
		genTexType = randomGenerator();

		if (genTexType == 5){
			lives = 5;
		}

		if (i == 0){
			if (ground.size() == 0){
				lastPosition.x = 0;
				lastPosition.y = 740;
				panel.init(lastPosition, genTexType, lives);
			}
			else {
				lastPosition = ground.at(ground.size() - 1).getPosition();
				panel.init(lastPosition, genTexType, lives);
			}
		}
		else {
			panel.init(sf::Vector2f(lastPosition.x + 300, lastPosition.y), genTexType, lives);
		}
		
		panel.sprite.setTexture(textures["ground" + panel.getSType()]);
		panel.sprite.setPosition(panel.getPosition());
		lastPosition = panel.position;
		ground.push_back(panel);
	}
}

int game::randomGenerator(){
	double val = (double)rand() / RAND_MAX;

	if (val < 0.71)       //  floor 71%
		return 3;
	else if (val < 0.79)  //  8%
		return 1;
	else if (val < 0.87)  //  8%
		return 2;
	else if (val < 0.95)  //  8%
		return 4;
	else
		return 5;  //  5%
}

void createGround(){

}

int game::groundCollision(sf::Vector2f playerPosition, int direction){
	/*for (int i = 0; i < ground.size(); i++){
		//dodelat
		if (playerPosition.x = ground.at(i).x){ //narazim na zmenu terenu
			if (playerPosition.y < ground.at(i).y){//prekazka
				return 2;
			}
			else if (playerPosition.y > ground.at(i).y){ // dira
				return ground.at(i).y;
			}
		}
		return 1; //rovina
	}*/
	int j = 0;;

	for (int i = 0; i < ground.size()-1; i++){
		if (direction == 1){
			if (playerPosition.x + player.sprite.getAnimation()->getFrame(0).width > ground[i].getPosition().x  &&
				playerPosition.x + player.sprite.getAnimation()->getFrame(0).width < ground[i + 1].getPosition().x){
				j = i;
			}
			else if (i == (ground.size() - 2)){
				j = i + 1;
			}
		}
		else {
			if (playerPosition.x + player.sprite.getAnimation()->getFrame(0).width > ground[i].getPosition().x  &&
				playerPosition.x < ground[i + 1].getPosition().x){
				j = i;
			}
			else if (i == (ground.size() - 2)){
				j =i + 1;
			}
		}
		
		if (j != 0) {
			switch (ground[j].getType()) {
			case 1: //barrier
			case 2: //barrier
				if ((playerPosition.y + player.sprite.getAnimation()->getFrame(0).height) == (ground[j].getPosition().y + 60))
					barrier = true;
				else
					barrier = false;
				///DODELAT ZATARAS
				return 2;
			case 3: //ground
				barrier = false;
				return 1;
			case 4: //fire
				if ((playerPosition.y + player.sprite.getAnimation()->getFrame(0).height) == (ground[j].getPosition().y + 60))
					barrier = true;
				else
					barrier = false;
				return 3;
			case 5: //flower
				if ((playerPosition.y + player.sprite.getAnimation()->getFrame(0).height) == (ground[j].getPosition().y + 60)){
					barrier = true;
					if (ground[j].getLives() != 0){
						player.lives++;
						ground[j].lives--;
					}
					if (ground[j].getLives() == 0) {
						ground[j].sprite.setTexture(textures["ground3"]);
					}
				}
				else
					barrier = false;
				return 4;
			}
		}
		
	}
}

map<string, string> game::getFiles(string dir, string filetype) {
	map<string, string> files;

	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == nullptr) {
		cout << "Error (" << errno << ") opening " << dir << endl;
		return files;
	}

	while ((dirp = readdir(dp)) != nullptr) {
		string f(dirp->d_name);

		if (f.rfind(filetype) == (f.size() - filetype.length())) {
			files.insert(std::pair<string, string>(f.substr(0, f.length() - filetype.length()), dir + f));
		}
	}
	closedir(dp);

	return files;
}

void game::loadTextures() {
	map<string, string> files = getFiles("textures/", ".png");

	for (map<string, string>::iterator i = files.begin(); i != files.end(); i++) {
		cout << "Opening " << i->second << endl;

		sf::Texture texture;
		texture.loadFromFile(i->second);
		texture.setSmooth(true);
		textures.insert(std::pair<string, sf::Texture>(i->first, texture));
	}
}

void game::loadSounds() {
	map<string, string> files = getFiles("sounds/", ".ogg");

	for (map<string, string>::iterator i = files.begin(); i != files.end(); i++) {
		cout << "Opening " << i->second << endl;

		OpenALMutex.lock();
		sf::SoundBuffer sound;
		OpenALMutex.unlock();
		if (!sound.loadFromFile(i->second))
		{
			cerr << "Cannot open " << i->second << endl;
			continue;
		}

		sounds.insert(std::pair<string, sf::SoundBuffer>(i->first, sound));
	}
}

void game::loadMusic() {
	map<string, string> files = getFiles("music/", ".ogg");

	for (map<string, string>::iterator i = files.begin(); i != files.end(); ++i) {
		cout << "Opening " << i->second << endl;

		sf::Music *m;
		OpenALMutex.lock();
		m = new sf::Music();
		OpenALMutex.unlock();
		if (!m->openFromFile(i->second))
		{
			delete m;
			cerr << "Cannot open " << i->second << endl;
			continue;
		}
		m->setLoop(true);
		music.insert(std::pair<string, sf::Music*>(i->first, m));
	}
}

void game::loadFonts() {
	map<string, string> files = getFiles("textures/", ".ttf");

	for (map<string, string>::iterator i = files.begin(); i != files.end(); i++) {
		cout << "Opening " << i->second << endl;

		sf::Font f;

		if (!f.loadFromFile(i->second))
		{
			cerr << "Cannot open " << i->second << endl;
			continue;
		}

		fonts.insert(std::pair<string, sf::Font>(i->first, f));
	}
}