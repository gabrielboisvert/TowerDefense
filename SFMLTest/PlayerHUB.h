#pragma once
#include "HUBWindow.h"
#include <vector>

namespace TD {
	class PlayerHUB {
	public:
		std::vector<HUBWindow*> windows;

		void clearWindow();
	};
}