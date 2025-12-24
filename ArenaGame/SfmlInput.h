#ifndef SFMLINPUT_H
#define SFMLINPUT_H

#include "Types.h"
#include <SFML/Graphics.hpp>

class SfmlInput
{
public:
	Input sample(int clientId);
};

#endif // SFMLINPUT_H

