#include "pch.h"
#include "Game.h"
#include "SVGParser.h"
#include "utils.h"
#include <iostream>

/*
recursion | losing
*/


Game::Game( const Window& window ) 
	:BaseGame{ window }
	, m_PlayerLocation{}
	, m_DeltaLocation{}
	, m_Speed{}
	, m_Direction{}
	, m_TimeSinceShoot{}
	, m_MousePos{}
	, m_ArrBulletPtr{}
	, m_Maze{}
	, m_AmountEnemies{}
	, m_ArrEnemyPtr{}
	
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
	
{
	std::cout << ((rand() % 50) / 100.f) + 1.f;

	m_PlayerLocation = Vector2f{ GetViewPort().width / 2,GetViewPort().height / 2 };
	m_Player = Circlef(Point2f{ m_Direction.ToPoint2f()}, 10);
	m_Speed = 250;
	m_Direction = Vector2f{0,0};
	m_TimeSinceShoot = 0;
	m_Wave = new Wave{};
	
	SVGParser::GetVerticesFromSvgFile("drawing.svg", m_Maze);
	for (int index{}; index < m_Wave->SetSpawnCount(); ++index)
	{
		m_ArrEnemyPtr.push_back(new Enemy{ Vector2f{float(rand() % 2000 - 1000) ,float(rand() % 2000 - 1000)},m_Maze,5000, ((rand() % 50)/100.f)+1.f });
	}
	m_TimerPtr = new Time();
	m_Deathscreen = new Texture{ "You Died","Font.ttf",100,Color4f{0.9,0.9,0.9,1} };
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	if(m_Health > 0)
	{
		UpdateEnemies(elapsedSec);

		m_DeltaLocation = m_Direction;
		m_ElapsedSec = elapsedSec;

		MoveInput();

		


		collision(elapsedSec);

		for (int index{}; index < m_ArrEnemyPtr.size(); ++index)
		{
			for (int index1{}; index1 < m_ArrEnemyPtr.size(); ++index1)
			{
				if (index1 != index)
				{
					m_ArrEnemyPtr[index]->InterCollision(m_ArrEnemyPtr[index1]->GetEnemyPos(), elapsedSec);
				}
			}
		}
		
		for (int index{}; index < m_ArrBulletPtr.size(); ++index)
		{
			m_ArrBulletPtr[index]->Update(elapsedSec);

			if (m_ArrBulletPtr[index]->GetTime() > m_MaxTimeBullet)
			{
				m_ArrBulletPtr.erase(m_ArrBulletPtr.begin());
			}
		}
		m_TimeSinceShoot += elapsedSec;
		if (m_IsHit)
		{
			m_HitTime += elapsedSec;
			if (m_HitTime > 1.f)
			{
				m_IsHit = false;
				m_HitTime = 0;
			}
		}
		m_Time += elapsedSec;
	

		m_TimerPtr->Update(elapsedSec);
	}
	
}

void Game::Draw( ) const 
{
	ClearBackground( );

	

	
	glPushMatrix();
	{
		
		glTranslatef(m_Direction.x,m_Direction.y,0);
		DrawEnemies();
		for (int index{}; index < m_ArrBulletPtr.size(); ++index)
		{
			m_ArrBulletPtr[index]->Draw();
		}
		utils::SetColor(Color4f{ 1,1,1,1 });
		for (int index{}; index < m_Maze.size(); ++index)
		{
			utils::DrawPolygon(m_Maze[index], false, 3);
		}
		
	}
	glPopMatrix();

	utils::SetColor(Color4f{ 0,0,1,1 });
	utils::FillEllipse(Point2f{450,300},10,10);

	DrawHealth();
	m_TimerPtr->Draw();
	m_Wave->Draw();
	if (m_Health <= 0)
	{
		m_Deathscreen->Draw(Point2f{200,250});
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
	
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;

}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	m_MousePos = Vector2f{ float(e.x),float(e.y) };
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		CreateBullet();
		break;
	
	}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const 
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::MoveInput()
{
	Vector2f Dir{};

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	const bool isLeft{ bool(pStates[SDL_SCANCODE_A]) };
	const bool isRight{ bool(pStates[SDL_SCANCODE_D]) };
	const bool isUp{ bool(pStates[SDL_SCANCODE_W]) };
	const bool isDown{ bool(pStates[SDL_SCANCODE_S]) };


	m_Direction.x -= float(isRight - isLeft) * m_Speed * m_ElapsedSec;
	m_Direction.y -= float(isUp - isDown) * m_Speed * m_ElapsedSec;
	m_Player = Circlef(Point2f{ -m_Direction.x+450,-m_Direction.y+300 }, 10);
}

void Game::collision(float elapsedSec)
{
	for (int index{}; index < m_Maze.size(); ++index)
	{
		for (int index2{}; index2 < m_Maze[index].size() - 1; ++index2)
		{
			if (utils::IsOverlapping(m_Maze[index][index2], m_Maze[index][index2 + 1], m_Player))
			{
				m_Direction = m_DeltaLocation;

			}
			for (int idx{}; idx < m_ArrBulletPtr.size(); ++idx)
			{
				if (utils::IsOverlapping(m_Maze[index][index2], m_Maze[index][index2 + 1], Circlef{ m_ArrBulletPtr[idx]->GetPos().x,m_ArrBulletPtr[idx]->GetPos().y,4}))
				{
					m_ArrBulletPtr.erase(m_ArrBulletPtr.begin());
				}
			}
		}
		
	}
	for (int idx{}; idx < m_ArrEnemyPtr.size(); ++idx)
	{
		if (utils::IsOverlapping(Circlef{ m_ArrEnemyPtr[idx]->GetEnemyPos().ToPoint2f(),10 }, m_Player)and !m_IsHit)
		{
			m_Health -= 10.f;
			m_IsHit = true;
		};
		
	}
	
}
void Game::DrawEnemies() const
{
	for (int index{}; index < m_ArrEnemyPtr.size(); ++index)
	{
		m_ArrEnemyPtr[index]->Draw();
	}
}
void Game::UpdateEnemies(float elapsedSec)
{
	std::cout << "enemy count: " << m_ArrEnemyPtr.size() << std::endl;

	if (m_ArrEnemyPtr.size() == 0)
	{

		m_Wave->Update(elapsedSec);
		//if (m_Wave->GetWaveCount() % 5 == 0 and m_Wave->GetWaveCount() != 0)
		
		for (int index{}; index < m_Wave->SetSpawnCount(); ++index)
		{
			m_ArrEnemyPtr.push_back(new Enemy{ Vector2f{float(rand() % 2000 - 1000) ,float(rand() % 2000 - 1000)},m_Maze,500, ((rand() % 50) / 100.f) + 1.f });
		}
		m_Health = int(m_Health + 50);
		if (m_Health >= 100) m_Health = 100;
	}
	
	
	for (int index{}; index < m_ArrEnemyPtr.size(); ++index)
	{
		m_ArrEnemyPtr[index]->Update(elapsedSec);
		for (int idx{}; idx < m_ArrBulletPtr.size(); ++idx)
		{
			if(m_ArrEnemyPtr[index]->Collision(m_ArrBulletPtr[idx]->GetPos()))
			{
				m_ArrEnemyPtr[index] = m_ArrEnemyPtr[m_ArrEnemyPtr.size() - 1];
				m_ArrEnemyPtr.resize(m_ArrEnemyPtr.size() - 1);
				index = 0;
				m_Wave->GetScore();
			}

			
		}
		if (m_ArrEnemyPtr.empty()) break;
		m_ArrEnemyPtr[index]->GetPlayerLocation(m_Direction.ToPoint2f());
	}
}
void Game::CreateBullet()
{
	//const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	//const bool SpaceBar { bool(pStates[SDL_Sca]) };

	if (m_TimeSinceShoot > 1.f)
	{
	//	std::cout << "Space\n";
		m_ArrBulletPtr.push_back(new Bullet{ m_MousePos,Vector2f{-m_Direction.x+450,-m_Direction.y+300},600.f });
		m_TimeSinceShoot = 0;
	}
}

void Game::DrawHealth() const
{
	utils::SetColor(Color4f{ 1,0,0,1 });
	utils::FillRect(Rectf{ 100,550,700,25 });
	utils::SetColor(Color4f{ 0,1,0,1 });
	utils::FillRect(Rectf{ 100,550,700 * (m_Health / 100.f),25 });
}
