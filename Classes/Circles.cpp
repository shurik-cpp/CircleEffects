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
	Tick(CircleEffects::RANDOM);
}

std::vector<cocos2d::Sprite*> Circles::GetObjects() const
{
	std::vector<cocos2d::Sprite*> result;
	for (const auto& it : circles)
		result.emplace_back(it.sprite);
	return result;
}

void Circles::Init()
{
	auto sprite = Sprite::create("circle.png");
	float spriteRadius = (sprite->getTextureRect().getMaxX() + distanceBetweenCircles) / 2;

	for (size_t i = 0; i < rowsCount; ++i)
	{
		int objectsCount = GetObjectCount(spriteRadius, locationRadius);

		if (i == rowsCount - 1)
			objectsCount = 15;

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
			circle->setColor(Color3B(255, 190, 0));
			// TODO: разделить общий массив на массивы по locationRadius
			circles.push_back(SingleCircle{false, circle});
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

void Circles::Tick(const CircleEffects& effect)
{
	switch (effect) {
		case CircleEffects::RANDOM:
		{
			int red, green, blue;
			for (auto& circle : circles) {
				red = GetRandom(130, 205);

				if (!GetRandom(0, 100)) {
					blue = green = red + GetRandom(0, 15);
				}
				else {
					green = red - 50;
					blue = 0;
				}
				circle.sprite->setColor(Color3B(red, green, blue));
			}
		}
		break;
		case CircleEffects::SMOOTH_RANDOM:
		{
			for (auto& circle : circles) {
				int red = circle.sprite->getColor().r;
				if (red >= 200)
					circle.colorIncrement = false;
				else if (red <= 90)
					circle.colorIncrement = true;

				const int randomMax = GetRandom(0, (red > 150 || red < 110) ? 15 : 5);
				if (circle.colorIncrement)
					red += GetRandom(0, randomMax);
				else
					red -= GetRandom(0, randomMax);

				const int green = red - GetRandom(50, 70);
				const int blue = GetRandom(0, 10);
				circle.sprite->setColor(Color3B(red, green, blue));
			}
		}
		break;
		case CircleEffects::ROTATE:
			for (auto& circle : circles) {
				Vec2 position = circle.sprite->getPosition();
				position.rotate(_centerPosition, 0.001);
				circle.sprite->setPosition(position);
			}
		break;


		default:
		break;
	}

}
