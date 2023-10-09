#pragma once

#include "Vec2.h"
#include "SFML/Graphics.hpp"

/*
* Components store data only, no logic.
* Components are assigned to Entities.
* An Entity will hold at least one of these components.
*/

// stores data regarding where an entity is and where it is going
class CTransform
{
public:
	Vec2 pos		= { 0.0,0.0 };
	Vec2 velocity	= { 0.0, 0.0 };
	float angle		= 0;

	CTransform(const Vec2 &p, const Vec2& v, float a)
		: pos(p), velocity(v), angle(a) {}

};

// stores an sfml circle shape
class CShape
{
public:
	sf::CircleShape circle;

	CShape(float radius, int points, const sf::Color& fill,
		const sf::Color& outline, float thickness) :
		circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}
};

// stores radius of collision of an entity
class CCollision
{
public: 
	float radius = 0;
	CCollision(float r)
		: radius(r) {}
};

/*
* stores the score of an entity.
* big entities have score of
* small entities have score of
*/
class CScore
{
public:
	int score = 0;
	CScore(int s)
		: score(s) {}
};

// stores the lifespan of component
class CLifespan
{
public:
	int remaining	= 0;		// amount of lifespan remaining on the entity
	int total		= 0;		// the total initial amount of lifespan
	CLifespan(int total)
		: remaining(total), total(total) {}
};

// if entity has CInput component, user can control entity with keyboard keys
class CInput
{
public:
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;
	bool shoot = false;

	CInput() {}
};