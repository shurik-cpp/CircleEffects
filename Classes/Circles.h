#ifndef CIRCLES_H
#define CIRCLES_H

#include "cocos2d.h"
#include <iostream>

class Circles
{
public:
	Circles();
	Circles(const cocos2d::Vec2& centerPosition);

	const std::vector<cocos2d::Sprite*>& GetObjects() const;

	void Tick();

private:
	cocos2d::Vec2 _centerPosition;
	float circleRadius = 10;

	int locationRadius = 30; // начальный радиус расположения кружков
	int rowsCount = 5; // количество рядов
	int distanceBetweenCircles = 0; // растояние между кружками

	//auto orange = Color3B::ORANGE;
	int r = 183; //orange.r;
	int g = 133; //orange.g;
	int b = 0; //orange.b;

	std::vector<cocos2d::Sprite*> circles;

	inline int GetRandom(const int min, const int max) const;
	int GetObjectCount(const float objectRadius, const float locationRadius) const;
	void Init();
};

#endif // CIRCLES_H
