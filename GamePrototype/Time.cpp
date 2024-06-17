#include "pch.h"
#include "Time.h"

Time::Time()
{
	for (int index{}; index < 10; ++index)
	{
		m_Numbers[index] = new Texture{ std::to_string(index),"Font.ttf",24,Color4f{0.9,0.9,0.9,1} };
	}
	m_Numbers[10] = new Texture{ ":","Font.ttf",24,Color4f{0.9,0.9,0.9,1}};
}

Time::~Time()
{
	for (int index{}; index < 10; ++index)
	{
		delete m_Numbers[index];
	}
}

void Time::Draw() const
{
	DrawSeconds();
	DrawMinutes();
}

void Time::Update(float elapsedSec)
{
	m_Time += elapsedSec;


}

void Time::DrawSeconds() const
{
	m_Numbers[(int(m_Time) % 60) % 10]->Draw(Point2f{ 67,500 });
	m_Numbers[(int(m_Time) % 60)/10]->Draw(Point2f{50,500});
}

void Time::DrawMinutes() const
{
	m_Numbers[10]->Draw(Point2f{ 45,505 });
	m_Numbers[(int(m_Time) / 60)%10]->Draw(Point2f{ 28,500 });
	m_Numbers[(int(m_Time) / 600) % 10]->Draw(Point2f{ 11,500 });
}
