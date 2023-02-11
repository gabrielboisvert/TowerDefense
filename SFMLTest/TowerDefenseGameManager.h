#pragma once
#include "IGameManager.h"
#include "GameMap.h"
#include "EnemyArmy.h"
#include "Player.h"
#include "InputScanner.h"

namespace TD {
	class TowerDefenseGameManager : public IGameManager {
	private:
		TowerDefenseGameManager();
	
	public:
		TD::Player player;
		TD::EnemyArmy enemyArmy;
		TD::GameMap map;

		TD::InputScanner* scanner;
		std::string mapChoosed;

		TD::ITower* choosedTower;

		TD::HUBButton* upgradeButton;
		TD::ITower* towerToUpgrade;

		sf::Time currentWaveTimer;
		sf::Text* warmupText;

		static TowerDefenseGameManager instance;
		
		void init() override;

		void update() override;

		void updateState();

		void render();

		static TowerDefenseGameManager& getInstance();

		TD::Renderer& getRenderer();

		void setState(TD::GameState state);
		
		void setInputeScanner(InputScanner& inputeScanner);

		bool isValidPosition(const sf::Rect<float>& hitBox);

		TD::HUBWindow* createHub(const char* spritePath, sf::Vector2f pos, sf::Vector2f size, unsigned int layer);

		TD::HUBButton* createButton(const char* spritePath[3], TD::HUBWindow* hubWindow, sf::Rect<float> rect, std::function<bool()> action, unsigned int hoverAlpha = 255);

		//Main menu
		void initMainMenuKey();
		void initMainMenuButton();

		//Game running
		void initGameMenuKey();
		void initGameMenuButton();

		//Pause menu
		void initPauseMenuKey();
		void initPauseMenuButton();

		//GameOver menu
		void initGameOverMenuKey();
		void initGameOverMenuButton();

		~TowerDefenseGameManager();
	};
}