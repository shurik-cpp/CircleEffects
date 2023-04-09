#ifndef CIRCLES_H
#define CIRCLES_H

#include "cocos2d.h"
#include <iostream>

enum class CircleEffects {
	RANDOM_COLOR,
	SMOOTH_RANDOM_COLOR,
	SMOOTH_OPACITY,
	ROTATE_CV,
	ROTATE_CCV,
	DIFFERENT_ROTATION
};

class Circles
{
public:
	Circles();
	Circles(const cocos2d::Vec2& centerPosition);

	std::vector<cocos2d::Sprite*> GetObjects() const;

	void Tick(const CircleEffects& effect);
	void Tick();

	void SetCenterPosition(const cocos2d::Vec2 position);
	void SetRowsCount(const size_t rows);
	void SetDistance(const size_t distance);
	void SetSpeed(const float delta);
	void SetColor(const cocos2d::Color3B color);
	void SetEffects(const CircleEffects& effect);

private:
	cocos2d::Vec2 _centerPosition;
	CircleEffects _effect;

	std::string spriteFileName = "circle_small.png"; // имя файла спрайта
	int rowRadius = 30; // начальный радиус расположения кружков
	int rowsCount = 15; // количество рядов
	float distanceBetweenCircles = 0.3; // растояние между кружками
	int lastRowCount = 0; // количество кружков в последнем ряду
	float rotateAngle = 45; // угол доворота всего массива

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

	// [ряды в окружности][кружки]
	std::vector<std::vector<SingleCircle>> circles;

	inline int GetRandom(const int min, const int max) const;
	int GetObjectCount(const float objectRadius, const float locationRadius) const;
	void Init();
};

#endif // CIRCLES_H
