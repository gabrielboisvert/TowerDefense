#pragma once
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Shape.hpp>

namespace TD {
	class Sprite : public sf::Sprite {
	public:
		typedef std::uint8_t Layer;
		bool drawShape;
		sf::Shape* shape;

		Sprite();
		Sprite(Layer layer);
		void setShape(sf::Shape* shape) { this->shape = shape; };
		~Sprite();
	private:
		Layer layer;
	};
}