#ifndef NORMALIZEUTIL_H
#define NORMALIZEUTIL_H

#include <SFML/Graphics.hpp>

sf::Vector2f normalize(sf::Vector2f v) {
	float len{ sqrt(v.x * v.x + v.y * v.y) };

	if (len != 0.f) v /= len;

	return v;
}

#endif // NORMALIZEUTIL_H