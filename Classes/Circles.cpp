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
	Tick(CircleEffects::RANDOM_COLOR);
}

std::vector<cocos2d::Sprite*> Circles::GetObjects() const
{
	std::vector<cocos2d::Sprite*> result;
	for (const auto& row : circles)
		for (const auto& it : row)
			result.emplace_back(it.sprite);
	return result;
}

void Circles::Init()
{
	auto spriteSize = Sprite::create(spriteFileName)->getContentSize().width;
	float spriteRadius = (spriteSize + distanceBetweenCircles) / 2;

	int circleCount = 0;
	for (size_t i = 0; i < rowsCount; ++i)
	{
		int objectsCount = GetObjectCount(spriteRadius, rowRadius);

		if (i == rowsCount - 1)
			objectsCount = lastRowCount;

		std::vector<SingleCircle> circlesInRow;
		for (size_t j = 0; j < objectsCount; ++j)
		{
			float angle = j * 2 * M_PI / objectsCount; // вычисляем угол между центрами кружков
			const float x = _centerPosition.x + rowRadius * cos(angle); // вычисляем координату X центра кружка
			const float y = _centerPosition.y + rowRadius * sin(angle); // вычисляем координату Y центра кружка
			Vec2 position(x, y);
			// отрисовываем кружок с координатами (x, y)
			auto circle = Sprite::create(spriteFileName);
			std::cout << "Circle number: " << circleCount << std::endl;
			std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
			circle->setPosition(position);
			//circle->setColor(Color3B(255, 190, 0));

			circlesInRow.emplace_back(SingleCircle{false, static_cast<bool>(GetRandom(0, 1)), circle});
			circleCount++;
		}

		circles.emplace_back(circlesInRow);
		rowRadius += spriteRadius * 2 + distanceBetweenCircles;
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
	this->Tick(_effect);
}

void Circles::Tick(const CircleEffects& effect)
{
	switch (effect) {
		case CircleEffects::RANDOM_COLOR:
		{
			int red, green, blue;
			for (size_t i = 0; i < circles.size(); ++i) {
				auto& row = circles[i];
				for (auto& circle : row) {
					red = GetRandom(130, 255);

					if (!GetRandom(0, 5)) {
						auto colorValue = red + GetRandom(0, 25);
						blue = green = (colorValue) > 255 ? red : colorValue;
					}
					else {
						green = red - 50;
						blue = 0;
					}
					circle.sprite->setColor(Color3B(red, green, blue));
				}
			}
		}
		break;
		case CircleEffects::SMOOTH_RANDOM_COLOR:
		{
			for (size_t i = 0; i < circles.size(); ++i) {
				auto& row = circles[i];
				for (auto& circle : row) {
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

					const int green = red - 30;
					const int blue = green;
					circle.sprite->setColor(Color3B(red, green, blue));
				}
			}
		}
		break;
		case CircleEffects::ROTATE_CCV:
		{
			for (size_t i = 0; i < circles.size(); ++i) {
				auto& row = circles[i];
				for (auto& circle : row) {
					Vec2 position = circle.sprite->getPosition();
					position.rotate(_centerPosition, 0.001);
					circle.sprite->setPosition(position);
				}
			}
		}
		break;
		case CircleEffects::ROTATE_CV:
		{
			for (size_t i = 0; i < circles.size(); ++i) {
				auto& row = circles[i];
				for (auto& circle : row) {
					Vec2 position = circle.sprite->getPosition();
					position.rotate(_centerPosition, -0.001);
					circle.sprite->setPosition(position);
				}
			}
		}
		break;
		case CircleEffects::DIFFERENT_ROTATION:
		{
			float rotateDirection = 0.0001;
			for (size_t i = 0; i < circles.size(); ++i) {
				auto& row = circles[i];
				for (auto& circle : row) {
					Vec2 position = circle.sprite->getPosition();
					position.rotate(_centerPosition, rotateDirection);
					circle.sprite->setPosition(position);
				}
				rotateDirection *= -1;
			}
		}
		break;
		case CircleEffects::SMOOTH_OPACITY:
		{
			for (size_t i = 0; i < circles.size(); ++i) {
				auto& row = circles[i];
				for (auto& circle : row) {
					int opacity = circle.sprite->getOpacity();
					const int randomMax = GetRandom(0, (opacity > 200 || opacity < 150) ? 15 : 8);
					int minValue = 0;
					int maxValue = 255;
					// для внутренних рядов
					if (i < circles.size() / 3) {
						minValue = (255 / 3) * 2;
					}
					// для средних рядов
					else if (i < (circles.size() / 3) * 2) {
						minValue = 255 / 3;
					}
					// для наружних рядов
					else {
						minValue = 0;
						maxValue = (255 / 3) * 2;
					}
					// для самого наружнего кольца
					if (i == circles.size() - 1) {
						minValue = 0;
						maxValue = 255 / 4;
					}

					if (opacity + randomMax >= maxValue && opacity - randomMax <= minValue)
						continue;

					if (opacity + randomMax >= maxValue)
						circle.opacityIncrement = false;
					else if (opacity - randomMax <= minValue)
						circle.opacityIncrement = true;

					if (circle.opacityIncrement)
						opacity += GetRandom(0, randomMax);
					else
						opacity -= GetRandom(0, randomMax);

					circle.sprite->setOpacity(opacity);
				}
			}
		}
		break;

		default:
		break;
	}

}
