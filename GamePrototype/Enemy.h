#pragma once
#include "Vector2f.h"
#include "utils.h"
#include "vector"
#include "Bullet.h"

class Enemy
{
public:
	Enemy(Vector2f pos ,std::vector<std::vector<Point2f>> walls,float range,float speedMod);
	Enemy(const Enemy& rhs) ;
	Enemy& operator=(const Enemy& rhs);

	void Update(float elapsedSec);
	void Draw() const;

	void PathFinding(float elapsedSec);
	bool Collision(Point2f bulletpos);
	void InterCollision(Vector2f enemyCenter, float elapsedSec);
	void GetPlayerLocation(Point2f pos);

	Vector2f GetEnemyPos();
	

	std::vector<std::vector<Point2f>> m_Maze{};
	std::vector<Bullet*> m_Bullets{};
	Vector2f m_Pos{};
	Vector2f m_PlayerPos{};
	float m_Range{};
	Vector2f m_DeltaLocation{};
	Vector2f Direction{};
	float m_BulletTimer{};

	// raycast detection
	bool isAlive{ true };
	bool isDead{false};
	float m_IsFast{0};
};

