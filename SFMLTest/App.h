#pragma once
#include "TowerDefenseGameManager.h"
#include "Window.h"
#include "InputScanner.h"

namespace TD {
	class App {
	private:
		Window m_window;
		TowerDefenseGameManager& m_gameManager = TD::TowerDefenseGameManager::getInstance();
		InputScanner m_inputeScanner;
	public:
		void init();
		void run();
	};
}