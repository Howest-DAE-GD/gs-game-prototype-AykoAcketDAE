#pragma once
#include "Vector2f.h"
class Bullet 
{
public:
	Bullet(Vector2f mousePos, Vector2f playerPos,float speed);

	void Update(float elapsedSec);
	void Draw() const;

	float GetTime();
	void DeletePos();
	Point2f GetPos();

private:
	Vector2f m_StartPos;
	Vector2f m_Direction;
	Vector2f m_Pos2;
	float m_Speed;
	float m_Time;
};

