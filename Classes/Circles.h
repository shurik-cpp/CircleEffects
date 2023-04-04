#ifndef CIRCLES_H
#define CIRCLES_H

#include "cocos2d.h"
#include <iostream>

enum class CircleEffects {
	RANDOM,
	SMOOTH_RANDOM,
	ROTATE,
};

class Circles
{
public:
	Circles();
	Circles(const cocos2d::Vec2& centerPosition);

	std::vector<cocos2d::Sprite*> GetObjects() const;

	void Tick(const CircleEffects& effect);

	void SetCenterPosition(const cocos2d::Vec2 position);
	void SetRowsCount(const size_t rows);
	void SetDistance(const size_t distance);
	void SetSpeed(const float delta);
	void SetColor(const cocos2d::Color3B color);
	void SetEffects(const CircleEffects& effect);

private:
	cocos2d::Vec2 _centerPosition;
	CircleEffects _effect;

	int locationRadius = 40; // начальный радиус расположения кружков
	int rowsCount = 11; // количество рядов
	int distanceBetweenCircles = 0; // растояние между кружками

	struct SingleCircle {
		explicit SingleCircle(bool _colorIncrement, cocos2d::Sprite* _sprite)
			: colorIncrement(_colorIncrement)
			, sprite(_sprite)
		{}
		bool colorIncrement;
		cocos2d::Sprite* sprite;
	};

	std::vector<SingleCircle> circles;

	inline int GetRandom(const int min, const int max) const;
	int GetObjectCount(const float objectRadius, const float locationRadius) const;
	void Init();
};

#endif // CIRCLES_H
