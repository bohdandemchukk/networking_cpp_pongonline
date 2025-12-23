#ifndef DEBUGUTIL_H
#define DEBUGUTIL_H

#include <SFML/Graphics.hpp>

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& v) {
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2u& v) {
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}


#endif // DEBUGUTIL_H