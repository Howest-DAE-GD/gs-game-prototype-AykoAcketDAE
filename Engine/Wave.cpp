#include "Wave.h"

Wave::Wave()
{
	for (int index{}; index < 10 ;++index)
	{
		m_Numbers[index] = new Texture{ std::to_string(index),"Font.ttf",24,Color4f{0.9,0.9,0.9,1} };
	}
	WaveTexture = new Texture{ "Wave: ","Font.ttf",24,Color4f{0.9,0.9,0.9,1}};
	PointsTexture = new Texture{ "Score: ","Font.ttf",24,Color4f{0.9,0.9,0.9,1} };
}

void Wave::Update(float elapsedSec)
{
	if (EnemyCount == 0)
	{
		++WaveCount;
		SpawnAmount = 25 + 10 * (WaveCount / 2);
	}
}

void Wave::Draw() const
{
	DrawWave();
	DrawPoints();
}

void Wave::DrawWave() const
{
	WaveTexture->Draw(Point2f{ 760,500 });
	m_Numbers[WaveCount / 10]->Draw(Point2f{ 850,500 });
	m_Numbers[WaveCount % 10]->Draw(Point2f{ 870,500 });
}

void Wave::DrawPoints() const
{
	PointsTexture->Draw(Point2f{ 340,500 });

	m_Numbers[(points / 10000) % 10]->Draw(Point2f{ 430,500 });
	m_Numbers[(points / 1000) % 10]->Draw(Point2f{ 450,500 });							//
	m_Numbers[(points / 100) % 10]->Draw(Point2f{ 470,500 });				//
	m_Numbers[(points / 10) % 10]->Draw(Point2f{ 490,500 });		//3
	m_Numbers[0]->Draw(Point2f{ 510,500 });		//0
}



void  Wave::GetEnemyCount(int enemycount)
{
	if (enemycount < EnemyCount) points += 10;
	EnemyCount = enemycount;
}

void Wave::GetScore()
{
	points += 10;
}

int Wave::GetWaveCount()
{
	return WaveCount;
}

int Wave::SetSpawnCount()
{
	return SpawnAmount;
}
