#define _USE_MATH_DEFINES

#include "Game.h"
#include <iostream>
#include <fstream>
#include <cmath>


Game::Game(const std::string& config)
{
	init(config);
}

// starts game
void Game::run()
{
	// some systems should funciton while paused (rendering, user unput)
	while (m_running)
	{
		m_entities.update();

		if (!m_paused)
		{
			// pause this systems
			sEnemySpawner();
			sMovement();		
			sCollision();
			sLifeSpan();

			// increment current frame if game is not paused
			m_currentFrame++;
		}

		sUserInput();
		sRender();
	}
}

// reads and stores configuration from config file
void Game::init(const std::string& path)
{
	// read config file
	std::ifstream fin(path);
	// error handle input filestream before reading stream
	if (!fin)
	{
		std::cerr << "Could not load font!\n";
		exit(-1);
	};

	// iterate through filestream
	std::string temp;
	while (fin >> temp)
	{
		// window configuration
		if (temp == "Window")
		{
			int width;
			int height;
			int fLimit;
			int fullscreen;
			fin >> width >> height >> fLimit >> fullscreen;
			fullscreen ? 
				m_window.create(sf::VideoMode(width, height), "Game02", sf::Style::Fullscreen)
				:
				m_window.create(sf::VideoMode(width, height), "Game02");
			// frame limit 
			m_window.setFramerateLimit(fLimit);
		}
		else if (temp == "Font")
		{
			std::string fontFilePath;
			int size;
			int r, g, b;
			fin >> fontFilePath >> size >> r >> g >> b;

			// attempt to load font from file
			if (!m_font.loadFromFile(fontFilePath))
			{
				// if can't load font, print an error to the error console and exit
				std::cerr << "Could not load font!\n";
				exit(-1);
			}
			m_textSize = size;
			m_color = sf::Color(r, g, b);
		} 
		else if (temp == "Player")
		{
			int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; 
			float S;
			fin >> SR >> CR >> S;
			fin >> FR >> FG >> FB >> OR >> OG >> OB >> OT >> V;
			m_playerConfig = { SR, CR, FR, FG, FB, OR, OG, OB, OT, V, S };
		}
		else if (temp == "Enemy")
		{
			int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; 
			float SMIN, SMAX;
			fin >> SR >> CR;
			fin >> SMIN >> SMAX;
			fin >> OR >> OG >> OB >> OT;
			fin >> VMIN >> VMAX;
			fin >> L >> SI;
			m_enemyConfig = { SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI, SMIN, SMAX };
		}
		else
		{
			// bullet config
			int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; 
			float S;
			fin >> SR >> CR;
			fin >> S;
			fin >> FR >> FG >> FB;
			fin >> OR >> OG >> OB;
			fin >> OT >> V >> L;
			m_bulletConfig = { SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L, S };
		}
		//std::cout << temp << std::endl;
	}
	//spawn player
	spawnPlayer();
}

// pauses the game
void Game::setPaused(bool paused)
{
	m_paused = paused;
}

// handles movement of all entities
void Game::sMovement()
{
	// make player velocity (0,0) on every frame so it does not move automatically
	m_player->cTransform->velocity = { 0,0 };
	
	// save player x and y position for ease of access
	float playerX = m_player->cTransform->pos.x;
	float playerY = m_player->cTransform->pos.y;
	// store player radius for ease of access
	float playerR = m_player->cShape->circle.getRadius();

	// update velocity for player based on user input
	// RECALL (0,0) is upper left corner
	// movement up
	if (m_player->cInput->up && playerY - playerR > 0)
	{
		// movement up and left, direction <-1,-1>
		if (m_player->cInput->left && playerX - playerR > 0)
		{
			m_player->cTransform->velocity += Vec2(-1, -1).normalize() * m_playerConfig.S;
		}
		// movement up and right, direction <1,-1>
		else if (m_player->cInput->right && playerX + playerR < m_window.getSize().x)
		{
			m_player->cTransform->velocity += Vec2(1, -1).normalize() * m_playerConfig.S;
		}
		// movement up only, direction <0,-1>
		else
		{
			m_player->cTransform->velocity += Vec2(0, -1).normalize() * m_playerConfig.S;
		}
	}
	// movement down 
	else if (m_player->cInput->down && playerY + playerR < m_window.getSize().y)
	{
		// movement down and left, direction <-1,1>
		if (m_player->cInput->left && playerX - playerR > 0)
		{
			m_player->cTransform->velocity += Vec2(-1,1).normalize() * m_playerConfig.S;
		} 
		// movement down and right, direction <1,1>
		else if (m_player->cInput->right && playerX + playerR < m_window.getSize().x)
		{
			m_player->cTransform->velocity += Vec2(1, 1).normalize() * m_playerConfig.S;
		}
		// movement down only, direction <0,1>
		else
		{
			m_player->cTransform->velocity += Vec2(0, 1).normalize() * m_playerConfig.S;
		}
	}
	// movement left only, direction <-1,0>
	else if (m_player->cInput->left && playerX - playerR > 0)
	{
		m_player->cTransform->velocity += Vec2(-1, 0).normalize() * m_playerConfig.S;
	} 
	// movement right only, direction <1,0>
	else if (m_player->cInput->right && playerX + playerR < m_window.getSize().x)
	{
		m_player->cTransform->velocity += Vec2(1, 0).normalize() * m_playerConfig.S;
	}

	// update x and y position of entities by velocity x and y 
	for (auto e : m_entities.getEntities())
	{
		// entity radius
		float radius = e->cShape->circle.getRadius();

		e->cTransform->pos += e->cTransform->velocity;

		// handle out of bounds for entities
		// out of bounds y positon < 0
		if ( e->cTransform->pos.y - radius < 0)
		{
			e->cTransform->velocity.y *= -1;
		}
		// out of bounds y position > window height
		if (e->cTransform->pos.y + radius > m_window.getSize().y)
		{
			e->cTransform->velocity.y *= -1;
		} 
		// out of bounds x position < 0
		if (e->cTransform->pos.x - radius < 0)
		{
			e->cTransform->velocity.x *= -1;
		}
		// out of bounds x position > window width
		if (e->cTransform->pos.x + radius > m_window.getSize().x)
		{
			e->cTransform->velocity.x *= -1;
		}
	}
}

// handles user input
void Game::sUserInput()
{
	//		 handle user input here
	//		 sUserInput only sets player's input component variable here.
	//		 no implemention regarding player's movement logic here.
	//		 movement system will read variables set in this function.

	sf::Event event;
	while (m_window.pollEvent(event))
	{
		// this event triggers when the window is closed
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		// this event is triggered when a key is pressed
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W Key Pressed\n";
				m_player->cInput->up = true;
				break;
			case sf::Keyboard::S:
				std::cout << "S Key Pressed\n";
				m_player->cInput->down = true;
				break;
			case sf::Keyboard::A:
				std::cout << "A Key Pressed\n";
				m_player->cInput->left = true;
				break;
			case sf::Keyboard::D:
				std::cout << "D Key Pressed\n";
				m_player->cInput->right = true;
				break;
			case sf::Keyboard::P:
				std::cout << "P Key Pressed\n";
				if (m_paused)
				{
					setPaused(false);
				} 
				else 
				{
					setPaused(true);
				}
				break;
			default: break;
			}
		}

		// this event is triggered when a key is released
		if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				std::cout << "W Key Released\n";
				m_player-> cInput->up = false;
				break;
			case sf::Keyboard::S:
				std::cout << "S Key Released\n";
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::A:
				std::cout << "A Key Released\n";
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::D:
				std::cout << "D Key Released\n";
				m_player->cInput->right = false;
				break;
			default: break;
			}
		}

		// this event is triggered when mouse is pressed
		if (event.type == sf::Event::MouseButtonPressed)
		{
			// left mouse click spawns bullet
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				std::cout << "Left mouse Button Clicked at (" 
					<< event.mouseButton.x << "," << event.mouseButton.y << ")"
					<< std::endl;
				spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
			}
			// TODO: trigger special weapon
			// right mouse click spawns special weapon
			if (event.mouseButton.button == sf::Mouse::Right)
			{
				std::cout << "Right Mouse Button Clicked at (" 
					<< event.mouseButton.x 
					<< "," << event.mouseButton.y << ")"
					<< std::endl;
			}
		}
	}

}

// handle lifespan of entities
void Game::sLifeSpan()
{
	for (auto e : m_entities.getEntities())
	{
		if (e->cLifespan != nullptr)
		{
			e->cLifespan->remaining--;
			if (e->cLifespan->remaining < 0)
			{
				e->destroy();
			}
		}
	}
}

// spawns enemy every m_enemyConfig.SI (spawn interval) frames
void Game::sEnemySpawner()
{
	if (m_currentFrame - m_lastEnemySpawnTime == m_enemyConfig.SI) {
		spawnEnemy();
	}
}

// handles collisions between entities
void Game::sCollision()
{
	// handles collisions between entities.
	// uses the collision radius, NOT the shape radius. 

	// collision between bullet and enemy.
	// call spawnSmallEnemies(enemy) when bullet collides with enemy.
	// then destroy both bullet and enemy.
	for (auto b : m_entities.getEntities("bullet"))
	{
		for (auto e : m_entities.getEntities("enemy"))
		{
			float distanceX = b->cTransform->pos.x - e->cTransform->pos.x;
			float distanceY = b->cTransform->pos.y - e->cTransform->pos.y;

			float collisionSum = b->cCollision->radius + e->cCollision->radius;
		
			if (distanceX*distanceX + distanceY*distanceY < collisionSum * collisionSum)
			{
				std::cout << "collision detected" << std::endl;

				b->destroy();
				// spawn small enemies
				spawnSmallEnemies(e);
				// increment score since enemy was destroyed
				m_score += e->cScore->score;
				e->destroy();
			}
		}
	}

	// collisions between enemy and player.
	// if collision detected, player re spawns at center of window.
	for (auto e : m_entities.getEntities("enemy"))
	{
		float distanceX = m_player->cTransform->pos.x - e->cTransform->pos.x;
		float distanceY = m_player->cTransform->pos.y - e->cTransform->pos.y;

		float collisionSum = m_player->cCollision->radius + e->cCollision->radius;

		if (distanceX * distanceX + distanceY * distanceY < collisionSum * collisionSum)
		{
			std::cout << "collision detected player" << std::endl;

			m_player->cTransform->pos = 
			{ 
				m_window.getSize().x / 2.0f, m_window.getSize().y / 2.0f 
			};
			//m_score -= e->cScore->score;
		}
	}
}


// draws entities to window
void Game::sRender()
{
	m_window.clear();

	// draw score to window.
	// TODO: encapsulate text configurations somewhere else. ONLY draw here.
	std::string stringScore = "Score: " + std::to_string(m_score);
	sf::Text score(stringScore, m_font, m_textSize);
	score.setFillColor(m_color);
	m_window.draw(score);

	// draw entities
	for (auto e : m_entities.getEntities())
	{
		// set position of the shape based on entity's transform->pos
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);

		// set the rotation of the shape based on the entity's transform->angle
		e->cTransform->angle += 1.0f;
		//e->cShape->circle.setRotation(e->cTransform->angle);

		// draw the entity's sf::CircleShape
		m_window.draw(e->cShape->circle);
	}

	m_window.display();
}

// spawn main player
void Game::spawnPlayer()
{
	// create every entity by calling EntityManager.addEntity(tag)
	// This returns a std::shared_ptr<Entity>, so use 'auto' to save typing
	auto entity = m_entities.addEntity("player");

	// Give this entity a Transform so it spwans at middle of window, no velocity and 0 for angle.
	float xPos = m_window.getSize().x / 2.0f;
	float yPos = m_window.getSize().y / 2.0f;
	entity->cTransform = std::make_shared<CTransform>(Vec2(xPos, yPos), Vec2(0.0f, 0.0f), 0.0f);

	// entity's shape is given properties specified in config file
	entity->cShape = std::make_shared<CShape>(m_playerConfig.SR, m_playerConfig.V,
		sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
		sf::Color(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB), m_playerConfig.OT);

	// add collision component to player with collision radius (CR)
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);

	// add an input component to the player so user can move it with keyboard
	entity->cInput = std::make_shared<CInput>();

	// We want this Entity to be our player, set Game's player variable to be this entity
	m_player = entity;
}

// spawn enemy at random position
void Game::spawnEnemy()
{
	// make sure enemy is spawned properly with the m_enemyConfig variables. 
	// the enemy must be spawned completely within the bounds of the window

	auto entity = m_entities.addEntity("enemy");

	// enemy radius, save for ease of access
	float radius = m_enemyConfig.SR;

	// initialize rand() with current time as seed
	srand(time(NULL));

	// give enemy Entity a Transform with random x and y position within the screen.
	// posX is in range 0 + enemy.Radius < x < window.SizeX - enemy.Radius.
	// posY is in range 0 + enemy.Radius < y < window.SizeY - enemy.Radius.
	// utilized formula r = min + ( rand() % (1+max-min) ), where r is random number in [min,max]
	float posX = radius + fmod(rand(), 1 + (m_window.getSize().x - radius - radius));
	float posY = radius + fmod(rand(), 1 + (m_window.getSize().y - radius - radius));


	// initial scalar speed for enemy is range between [m_enemyConfig.SMIN, m_enemyConfig.SMAX].
	float speed = m_enemyConfig.SMIN + fmod(rand(), 1 + (m_enemyConfig.SMAX - m_enemyConfig.SMIN));
	
	// initial direction vector will be random between <1,1>, <1,-1>, <-1,-1>, <-1,1>.
	float directionX = rand() % 2 ? -1 : 1;
	float directionY = rand() % 2 ? -1 : 1;
	entity->cTransform = std::make_shared<CTransform>(Vec2(posX, posY), Vec2(directionX,directionY).normalize() * speed, 0.0f);

	// give enemy random number of vertices between [m_enemyConfig.VMIN, m_enemyConfig.VMAX].
	int vertices = m_enemyConfig.VMIN + fmod(rand(), 1 + m_enemyConfig.VMAX - m_enemyConfig.VMIN);
	// give enemy outline color and thicknes specified in config file, and shape radius.
	// give enemy random fill color.
	int randomR = 0 + (rand() % (1 + 255));
	float randomG = 0 + (rand() % (1 + 255));
	float randomB = 0 + (rand() % (1 + 255));
	entity->cShape = std::make_shared<CShape>(m_enemyConfig.SR, vertices,
		sf::Color(randomR, randomG, randomB), sf::Color(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB), m_enemyConfig.OT);

	// add collision component to enemy entity with collision radius specified in config file
	entity->cCollision = std::make_shared<CCollision>(m_enemyConfig.CR);

	// give enemy a score component
	entity->cScore = std::make_shared<CScore>(entity->cShape->circle.getPointCount());
	//std::cout << entity->cShape->circle.getPointCount() << std::endl;

	// record when the most recent enemy was spawned
	m_lastEnemySpawnTime = m_currentFrame;
}

// spawns small enemies when large enemy gets destroyed
void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	// # of vertices of entity
	float points = entity->cShape->circle.getPointCount();
	// small enemies travel outward in intervals of 360 / # of vertices of entity
	float angleIncrement = 360.0 / points;

	// Inside this loop, 'angle' represents the current division degree
	for (float angle = 0.0; angle < 360.0; angle += angleIncrement) {
		// small enemies have radius half of original enemy
		float radius = entity->cShape->circle.getRadius() / 2.0f;
		// get speed of entity 
		float speed = entity->cTransform->velocity.length();

		// add small enemy to entity manager
		auto smallEnemy = m_entities.addEntity("enemy");

		// give small enemy a shape, transform, collision, score, lifespan component

		smallEnemy->cShape = std::make_shared<CShape>(radius, points,
			entity->cShape->circle.getFillColor(), entity->cShape->circle.getOutlineColor(),
			entity->cShape->circle.getOutlineThickness());
		// convert angle to radians, will be used to find velocity
		float angle_radians = angle * (M_PI / 180);
		double yDirection = std::sin(angle_radians);
		double xDirection = std::cos(angle_radians);
		smallEnemy->cTransform = std::make_shared<CTransform>(
			entity->cTransform->pos, Vec2(xDirection * speed, yDirection * speed), 0);
		smallEnemy->cCollision = std::make_shared<CCollision>(radius);
		// score for small enemies is N * 200, where N is number of points
		smallEnemy->cScore = std::make_shared<CScore>(2 * points);
		smallEnemy->cLifespan = std::make_shared<CLifespan>(m_enemyConfig.L);
	}
}

// spawns bullet with direction from entity to target
void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target)
{
	auto bullet = m_entities.addEntity("bullet");
	// specify bullet velocity.
	// first find direction vector between entity pos and target pos, then scale it to desired speed S.
	// also give bullet initial position same as entity.
	Vec2 velVector = entity->cTransform->pos.direction(target).normalize() * m_bulletConfig.S; 
	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, velVector, 0);

	bullet->cShape = std::make_shared<CShape>(m_bulletConfig.SR, m_bulletConfig.CR,
		sf::Color(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB), 
		sf::Color(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB), m_bulletConfig.OT);

	// add collision component to bullet
	bullet->cCollision = std::make_shared<CCollision>(m_bulletConfig.CR);

	// give bullet a life span component, with lifespan value specified in config file
	bullet->cLifespan = std::make_shared<CLifespan>(m_bulletConfig.L);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
}

