#include "SFML/Graphics.hpp"
#include <iostream>
#include "model/Game.h"

#include <iostream>

int main()
{
	Game game("gameSpecifications/config.txt");
	game.run();
}