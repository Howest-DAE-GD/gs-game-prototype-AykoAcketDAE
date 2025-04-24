#pragma once
#include "Texture.h"

class Wave
{
public:
	Wave();

	void Update(float elapsedSec);
	void Draw() const;

	void DrawWave() const;
	void DrawPoints() const;

	void GetEnemyCount(int enemycount);
	void GetScore();
	int GetWaveCount();
	int SetSpawnCount();
private:

	Texture* m_Numbers[11]{};
	int EnemyCount{};
	int WaveCount{1};
	Texture* WaveTexture;
	Texture* PointsTexture;

	int SpawnAmount{ 25 };
	int points{0};
};

