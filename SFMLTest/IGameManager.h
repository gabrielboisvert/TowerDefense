#pragma once
#include <SFML/System/Clock.hpp>
#include "Renderer.h"


namespace TD {
	
	enum class GameState
	{
		MAIN_MENU,
		RUNNING,
		PAUSED,
		GAME_OVER,
		QUIT,
	};

	class IGameManager {
	protected:
		sf::Clock clock;
		float deltaTime;
		Renderer renderer;
		GameState currentState;
		
		void updateDeltaTime();

	public:
		IGameManager();

		const float getDeltaTime();
		
		virtual void update() = 0;
		
		virtual void init() = 0;

		void setWindow(TD::Window& window);
	};
}