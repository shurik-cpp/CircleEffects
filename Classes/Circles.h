#ifndef CIRCLES_H
#define CIRCLES_H

#include "cocos2d.h"
#include <iostream>

enum class CircleEffects {
	RANDOM,
	SMOOTH_RANDOM,
	SMOOTH_OPACITY,
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

	int locationRadius = 35; // начальный радиус расположения кружков
	int rowsCount = 10; // количество рядов
	int distanceBetweenCircles = 1; // растояние между кружками
	int lastRowCount = 0; // количество кружков в последнем ряду

	struct SingleCircle {
		explicit SingleCircle(bool _colorIncrement, bool _opacityIncrement, cocos2d::Sprite* _sprite)
			: colorIncrement(_colorIncrement)
			, opacityIncrement(_opacityIncrement)
			, sprite(_sprite)
		{}
		bool colorIncrement;
		bool opacityIncrement;
		cocos2d::Sprite* sprite;
	};

	std::vector<SingleCircle> circles;

	inline int GetRandom(const int min, const int max) const;
	int GetObjectCount(const float objectRadius, const float locationRadius) const;
	void Init();
};

#endif // CIRCLES_H
