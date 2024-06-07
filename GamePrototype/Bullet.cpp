#include "pch.h"
#include "Bullet.h"
#include "utils.h"

Bullet::Bullet(Vector2f mousePos, Vector2f playerPos, float speed)
	:m_StartPos{playerPos}
	,m_Direction{}
	,m_Speed{speed}
{
	m_Time = 0;
	m_Pos2 = playerPos;
	m_Direction = mousePos;
	m_Direction.x -= 450;
	m_Direction.y -= 300;
	m_Pos2 += m_Direction.Normalized() * 20;
}

void Bullet::Update(float elapsedSec)
{
	m_Time += elapsedSec;
	m_StartPos += m_Direction.Normalized()* elapsedSec * m_Speed;
	m_Pos2 += m_Direction.Normalized() * elapsedSec * m_Speed;
	
	//m_StartPos += m_Direction.Normalized() *m_Speed* elapsedSec;
}

void Bullet::Draw() const
{
	utils::SetColor(Color4f{ 1,1,0,1 });
	utils::DrawLine(m_StartPos.ToPoint2f(), m_Pos2.ToPoint2f(), 4);
	//utils::FillEllipse(Point2f{ m_StartPos.x ,m_StartPos.y }, 5, 5);
}

float Bullet::GetTime()
{
	return m_Time;
}

void Bullet::DeletePos()
{
	m_StartPos.x = 9999999;
	m_StartPos.y = 9999999;
	m_Pos2.x = 9999999;
	m_Pos2.y = 9999999;
	m_Direction.x = 0;
	m_Direction.y = 0;
}

Point2f Bullet::GetPos()
{
	return m_Pos2.ToPoint2f();
}
