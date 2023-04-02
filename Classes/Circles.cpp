#include "Circles.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

USING_NS_CC;


inline int Circles::GetRandom(const int min, const int max) const {
	static bool do_once = true;
	if (do_once) {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		do_once = false;
	}

	return std::rand() % (max - min + 1) + min;
}

int Circles::GetObjectCount(const float objectRadius, const float locationRadius) const {
	// длина окружности, на которой будут размещаться объекты
	float circumference = locationRadius * 2 * M_PI;
	return circumference / (objectRadius * 2);
}

Circles::Circles()
{
	Init();
}

Circles::Circles(const Vec2& centerPosition)
	: _centerPosition(centerPosition)
{
	Init();
	_effect = CircleEffects::RANDOM;
	Tick();
}

const std::vector<cocos2d::Sprite*>& Circles::GetObjects() const
{
	return circles;
}

void Circles::Init()
{
	auto sprite = Sprite::create("circle.png");
	float spriteRadius = (sprite->getTextureRect().getMaxX() + distanceBetweenCircles) / 2;

	for (size_t i = 0; i < rowsCount; ++i)
	{
		int objectsCount = GetObjectCount(spriteRadius, locationRadius);

		if (i == rowsCount - 1)
			objectsCount = 7;

		for (size_t j = 0; j < objectsCount; ++j)
		{
			float angle = j * 2 * M_PI / objectsCount; // вычисляем угол между центрами кружков
			const float x = _centerPosition.x + locationRadius * cos(angle); // вычисляем координату X центра кружка
			const float y = _centerPosition.y + locationRadius * sin(angle); // вычисляем координату Y центра кружка
			Vec2 position(x, y);
			// отрисовываем кружок с координатами (x, y)
			auto circle = Sprite::create("circle.png");
			std::cout << "Circle number: " << circles.size() << std::endl;
			std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
			circle->setPosition(position);
			circles.push_back(circle);
		}

		locationRadius += spriteRadius * 2 + distanceBetweenCircles;
	}
}

void Circles::SetCenterPosition(const cocos2d::Vec2 position)
{
	_centerPosition = position;
}

void Circles::SetEffects(const CircleEffects& effect)
{
	_effect = effect;
}

void Circles::Tick()
{
	switch (_effect) {
		case CircleEffects::RANDOM:
			for (auto& circle : circles) {
				r = GetRandom(130, 205);

				if (!GetRandom(0, 100)) {
					b = g = r + GetRandom(0, 3);
				}
				else {
					g = r - 50;
					b = 0;
				}
				circle->setColor(Color3B(r, g, b));
			}
		break;
		case CircleEffects::SMOOTH_RANDOM:
			for (auto& circle : circles) {
				int rr = GetRandom(-5, 5);
				int newRed = r + rr;
				if (newRed > 205)
					r = 205;
				else if (newRed < 130)
					r = 130;
				else
					r = newRed;

				g = r - 50;
				b = 0;
				circle->setColor(Color3B(r, g, b));
			}
		break;
		case CircleEffects::ROTATE:
			for (auto& circle : circles) {
				Vec2 position = circle->getPosition();
				position.rotate(_centerPosition, 0.001);
				circle->setPosition(position);
			}
		break;


		default:
		break;
	}

}
