#include "Circles.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>

USING_NS_CC;

inline float DegreesToRadians(float degree)
{
	return degree * (M_PI / 180);
}

inline float RadiansToDegrees(float radians)
{
	return radians * (180 / M_PI);
}

std::ostream& operator<<(std::ostream& stream, const Color3B& color) {
	const int r = color.r;
	const int g = color.g;
	const int b = color.b;
	stream << "Color3B(" << r << ", " << g << ", " << b << ")";
	return stream;
}

inline int Circles::GetRandom(const int min, const int max) const {
	static bool do_once = true;
	if (do_once) {
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		do_once = false;
	}

	return std::rand() % (max - min + 1) + min;
}

inline uint8_t AngleToWhite(const float angle) {
	const float maxAngle = M_PI;
	const float normalizedAngle = angle / maxAngle; // нормализуем угол к диапазону [0, 1]
	uint8_t white = static_cast<uint8_t>(std::abs(255 * (1 - normalizedAngle))); // вычисляем значение цвета
	return white;
}

inline Color3B GetColor(const uint8_t red, const uint8_t white) {
	float k = white / 255.0f;

	// Устанавливаем значения цвета
	uint8_t green = static_cast<uint8_t>(red * k);
	uint8_t blue = static_cast<uint8_t>(green * k);


	return Color3B(red, green, blue);
}


void TestAngleToWhite() {
	std::cout << "========================================\n";
	for (size_t angle = 0; angle <= 360; ++angle) {
		float rad = DegreesToRadians(angle);
		std::cout << "Degree: " << angle << ", rad: " << rad << ", white: " << static_cast<int>(AngleToWhite(rad)) << std::endl;
	}
}
void TestRedToGradient() {
	std::cout << "========================================\n";
	//for (int red = 0; red <= 255; ++red) {
		for (int white = 255; white >= 0; --white) {
			std::cout << "White = " << white << ", " << GetColor(255, white) << std::endl;
		}
		std::cout << "=================\n";
	//}
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
	TestAngleToWhite();
	TestRedToGradient();
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
		float lastCos = 1;
		int lastRed = 255;
		std::vector<SingleCircle> circlesInRow;
		for (size_t j = 0; j < objectsCount; ++j)
		{
			// вычисляем угол между центрами кружков
			float angleRad = j * 2 * M_PI / objectsCount;
			float angleDegree = RadiansToDegrees(angleRad);
			const float cos = std::cos(angleRad);
			const float sin = std::sin(angleRad);
			// вычисляем координату X центра кружка
			const float x = _centerPosition.x + rowRadius * cos;
			// вычисляем координату Y центра кружка
			const float y = _centerPosition.y + rowRadius * sin;
			Vec2 position(x, y);
			// отрисовываем кружок с координатами (x, y)
			auto circle = Sprite::create(spriteFileName);

			// задаем цвет кругляшу, в зависимости от угла его расположения
			int red;
			if (cos < lastCos)
				red = GetRandom(lastRed - 10, lastRed - 1);
			else
				red = GetRandom(lastRed + 1, lastRed + 10);
			// ограничиваем цветность
			if (red < 120)
				red = 120;
			else if (red > 255)
				red = 255;

			// TODO: вывести формулу зависимости green и blue от red...
			circle->setColor(GetColor(red, AngleToWhite(angleRad)));
			circle->setOpacity(GetRandom(150, 255));


			// сместим позицию кругляша, доворотом вектора на 45 град CCV относительно центра массива кругляшей
			position.rotate(_centerPosition, DegreesToRadians(rotateAngle));
			circle->setPosition(position);

			circlesInRow.emplace_back(SingleCircle{false, static_cast<bool>(GetRandom(0, 1)), circle});

			std::cout << "Circle number: " << circleCount << std::endl;
			std::cout << std::fixed << std::setprecision(2);
			std::cout << "Position: (" << position.x << ", " << position.y << ")\n";
			std::cout << "Angle: " << angleDegree << ", sin: " << sin << ", cos: " << cos << std::endl;
			circleCount++;
			lastRed = red;
			lastCos = cos;
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
					//minValue = maxValue / (i + 1);

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
					if (i > circles.size() - 1) {
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
