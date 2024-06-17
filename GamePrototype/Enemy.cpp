#include "pch.h"
#include "Enemy.h"



Enemy::Enemy(Vector2f pos, std::vector<std::vector<Point2f>> walls, float range)

{
	m_Pos = pos;
	m_Maze = walls;

}

Enemy::~Enemy()
{

}

void Enemy::Update(float elapsedSec)
{
	if(isAlive)
	{
		m_BulletTimer += elapsedSec;
		PathFinding(elapsedSec);

		if (utils::IsPointInCircle(m_PlayerPos.ToPoint2f(), Circlef{ m_Pos.ToPoint2f(),300 }) and m_BulletTimer > 0.5)
		{
			m_Bullets.push_back(new Bullet{ Direction,m_PlayerPos,600 });
			m_BulletTimer = 0;
		}
		for (int index{}; index < m_Bullets.size(); ++index)
		{
			if (m_Bullets[index]->GetTime() > 0.5)
			{
				m_Bullets.erase(m_Bullets.begin());
			}
		}

		for (int index{}; index < m_Maze.size(); ++index)
		{
			for (int index2{}; index2 < m_Maze[index].size() - 1; ++index2)
			{
				if (utils::IsOverlapping(m_Maze[index][index2], m_Maze[index][index2 + 1], Circlef{ m_Pos.ToPoint2f() ,10 }))
				{
					m_Pos -= (Direction.Normalized() - Vector2f{ m_Maze[index][index2]-m_Maze[index][index2 + 1] }.Normalized()) * 300 * elapsedSec;
					

				}
				for (int idx{}; idx < m_Bullets.size(); ++idx)
				{
					if (utils::IsOverlapping(m_Maze[index][index2], m_Maze[index][index2 + 1], Circlef{ m_Bullets[idx]->GetPos().x,m_Bullets[idx]->GetPos().y,4 }))
					{
						m_Bullets.erase(m_Bullets.begin());
					}

				}
			}

		}
	}
}

void Enemy::Draw() const
{
	if (isAlive)
	{
		utils::SetColor(Color4f{ 1,0,0,1 });
		utils::FillEllipse(m_Pos.x, m_Pos.y, 10, 10);
	}

}

void Enemy::PathFinding(float elapsedSec)
{
	Direction =  m_PlayerPos- m_Pos;
	if(utils::IsPointInCircle(m_PlayerPos.ToPoint2f(), Circlef{ m_Pos.ToPoint2f() ,1000 }))
	{
		m_Pos += Direction.Normalized() * elapsedSec * 200;
	}
}

void Enemy::Collision(Point2f bulletpos)
{

	if(utils::IsPointInCircle(bulletpos, Circlef{ m_Pos.ToPoint2f() ,10 })) 
	{
		isAlive = false;
		isDead = true;
	}
}

void Enemy::InterCollision(Vector2f enemyCenter,float elapsedSec)
{
	Vector2f collisionDirection{ m_Pos - enemyCenter };

	if (utils::IsOverlapping(Circlef{ enemyCenter.ToPoint2f(),10 }, Circlef{ m_Pos.ToPoint2f() ,10 }))
	{
		m_Pos += collisionDirection.Normalized() * elapsedSec * 200;
	};
}

void Enemy::GetPlayerLocation(Point2f pos)
{

	m_PlayerPos = Vector2f{ -pos.x+450,-pos.y+300 };
}

Vector2f Enemy::GetEnemyPos()
{
	return m_Pos;
}

bool Enemy::GiveHealth()
{
	if (isAlive)
	{
		return false;
	}
	else if(isDead)
	{
		isDead = false;
		return true;
	}
	else
	{
		return false;
	}
}

