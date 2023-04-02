#include "Circles.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

USING_NS_CC;


inline int Circles::GetRandom(const int min, const int max) {
	static bool do_once = true;
	if (do_once) {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		do_once = false;
	}

	return std::rand() % (max - min + 1) + min;
}

Circles::Circles()
{
	Init();
}

Circles::Circles(const Vec2& centerPosition)
	: _centerPosition(centerPosition)
{
	Init();
}

const std::vector<cocos2d::Sprite*>& Circles::GetObjects()
{
	return circles;
}

void Circles::Init()
{
	//auto orange = Color3B::ORANGE;
	for (size_t i = 0; i < rowsCount; ++i)
	{
		int objectsCount = (objectsRadius * 2 + distanceBetweenCircles) / (circleRadius - distanceBetweenCircles); // начальное количество кружков
		if (i == rowsCount - 1)
			objectsCount = 7;
		for (size_t j = 0; j < objectsCount; ++j)
		{
			float angle = j * 2 * M_PI / objectsCount; // вычисляем угол между центрами кружков
			const float x = _centerPosition.x + objectsRadius * cos(angle); // вычисляем координату X центра кружка
			const float y = _centerPosition.y + objectsRadius * sin(angle); // вычисляем координату Y центра кружка
			Vec2 position(x, y);
			// отрисовываем кружок с координатами (x, y)
			auto circle = Sprite::create("circle.png");
			std::cout << "Circle number: " << circles.size() << std::endl;
			std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
			//std::cout << "Color: (" << r << ", " << g << ", " << b << ")\n";
			circle->setPosition(position);
			circles.push_back(circle);
		}

		objectsRadius += circleRadius * 2 + distanceBetweenCircles;
	}
}

void Circles::Tick()
{
	for (auto& circle : circles) {
		r = GetRandom(130, 205);

		if (!GetRandom(0, 10)) {
			b = g = r + GetRandom(0, 3);
		}
		else {
			g = r - 50;
			b = 0;
		}
		Vec2 position = circle->getPosition();
		circle->setColor(Color3B(r, g, b));
	}
}
