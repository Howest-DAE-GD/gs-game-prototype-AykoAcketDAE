#pragma once
#include "BaseGame.h"
#include "Vector2f.h"
#include "vector"
#include "Bullet.h"
#include "Enemy.h"
#include "Time.h"
#include "Wave.h"

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw()const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	
	Vector2f m_PlayerLocation;
	Vector2f m_DeltaLocation;
	Vector2f m_Direction;
	float m_Speed;
	float m_ElapsedSec;
	Circlef m_Player;


	float m_Health{ 100 };
	bool m_IsHit{false};
	float m_HitTime{ 0 };
	float m_Time{0};

	Time* m_TimerPtr{};

	std::vector<Bullet*> m_ArrBulletPtr;
	const float m_MaxTimeBullet{ 0.5f };
	float m_TimeSinceShoot;
	Vector2f m_MousePos;

	std::vector<Enemy*> m_ArrEnemyPtr;

	int m_AmountEnemies;
	std::vector<std::vector<Point2f>> m_Maze;

	Texture* m_Deathscreen;

	Wave* m_Wave;
	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void MoveInput();
	void collision(float elapsedSec);
	void DrawEnemies() const;
	void UpdateEnemies(float elapsedSec);

	void CreateBullet();

	void DrawHealth() const;


};