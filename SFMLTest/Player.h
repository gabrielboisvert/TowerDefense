#pragma once
#include "PlayerHUB.h"

namespace TD {
	class Player {
	public:
		PlayerHUB hub;
		unsigned int money;
		unsigned int baseHealth;
		unsigned int score;
		sf::Text* lifeText;
		sf::Text* cashText;
		sf::Text* scoreText;

		Player();

		void updateLife(int amount);

		void updateMoney(int amount);

		void updateScore(unsigned int amount);

		void setLifeText(sf::Text* txt);

		void setCashText(sf::Text* txt);

		void setScoreText(sf::Text* txt);

		template <typename T> T* buyTower();

		void reset();

		void clear();
	};
}