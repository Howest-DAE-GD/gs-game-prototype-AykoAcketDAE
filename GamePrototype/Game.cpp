#include "pch.h"
#include "Game.h"
#include <iostream>
#include "utils.h"
#include "SVGParser.h"

/*
imposter | consumption
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

	m_PlayerLocation = Vector2f{ GetViewPort().width / 2,GetViewPort().height / 2 };
	m_Player = Circlef(Point2f{ m_Direction.ToPoint2f()}, 10);
	m_Speed = 250;
	m_Direction = Vector2f{0,0};
	m_TimeSinceShoot = 0;
	SVGParser::GetVerticesFromSvgFile("drawing.svg", m_Maze);
	for (int index{}; index < 50; ++index)
	{
		m_ArrEnemyPtr.push_back(new Enemy{ /*Vector2f{float(rand() % 5000 - 2500) ,float(rand() % 5000 - 2500)}*/Vector2f{200,300},m_Maze,200});
	}
	
}

void Game::Cleanup( )
{
}

void Game::Update( float elapsedSec )
{
	UpdateEnemies(elapsedSec);

	m_DeltaLocation = m_Direction;
	m_ElapsedSec = elapsedSec;
	
	MoveInput();
	
	CreateBullet();
	

	collision();

	for (int index{}; index < m_ArrBulletPtr.size(); ++index)
	{
		m_ArrBulletPtr[index]->Update(elapsedSec);

		if (m_ArrBulletPtr[index]->GetTime() > m_MaxTimeBullet)
		{
			m_ArrBulletPtr.erase(m_ArrBulletPtr.begin());
		}
	}
	m_TimeSinceShoot += elapsedSec;
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

	utils::SetColor(Color4f{ 0,1,0,1 });
	utils::FillEllipse(Point2f{450,300},10,10);
	
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
	//std::cout << "MOUSEBUTTONDOWN event: ";
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

void Game::collision()
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
	for (int index{}; index < m_ArrEnemyPtr.size(); ++index)
	{
		m_ArrEnemyPtr[index]->Update(elapsedSec);
		for (int idx{}; idx < m_ArrBulletPtr.size(); ++idx)
		{
			m_ArrEnemyPtr[index]->Collision(m_ArrBulletPtr[idx]->GetPos());
		}
		m_ArrEnemyPtr[index]->GetPlayerLocation(m_Direction.ToPoint2f());
	}
}
void Game::CreateBullet()
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	const bool SpaceBar { bool(pStates[SDL_SCANCODE_SPACE]) };

	if (SpaceBar and m_TimeSinceShoot > 1.f)
	{
	//	std::cout << "Space\n";
		m_ArrBulletPtr.push_back(new Bullet{ m_MousePos,Vector2f{-m_Direction.x+450,-m_Direction.y+300},600.f });
		m_TimeSinceShoot = 0;
	}
}
