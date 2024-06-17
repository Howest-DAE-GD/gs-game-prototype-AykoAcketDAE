#pragma once
#include "Texture.h"

class Time
{
public:
	Time();
	~Time();

	void Draw() const;
	void Update(float elapsedSec);

	void DrawSeconds() const;
	void DrawMinutes() const;
private:
	Texture* m_Numbers[11]{};
	float m_Time{0};
};

