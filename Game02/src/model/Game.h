#pragma once

#include "Entity.h"
#include "EntityManager.h"

#include "SFML/Graphics.hpp"

struct PlayerConfig	{ int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig	{ int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
	sf::RenderWindow	m_window;					// the window we will draw to
	EntityManager		m_entities;					// vector of entities to maintain
	sf::Font			m_font;						// the font we will use to draw score to screen
	sf::Color			m_color;					// color of text used to draw score to screen
	int					m_textSize;					// size of text used to draw score to screen
	PlayerConfig		m_playerConfig;
	EnemyConfig			m_enemyConfig;
	BulletConfig		m_bulletConfig;
	int					m_score;
	int					m_currentFrame = 0;
	int					m_lastEnemySpawnTime = 0;
	bool				m_paused = false;			// whether we update game logic
	bool				m_running = true;			// whether the game is running


	std::shared_ptr<Entity> m_player;


	void init(const std::string& path);				// initialize GameState with config file
	void setPaused(bool paused);					// pause the game

	void sMovement();								// System: Entity position / movement update
	void sUserInput();								// System: User Input
	void sLifeSpan();								// System: Lifespan
	void sRender();									// System: Render / Drawing
	void sEnemySpawner();							// System: Spanws Enemies
	void sCollision();								// System: Collisions

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& target);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:

	Game(const std::string& config);				// constructor, takes in game config

	void run();

};
