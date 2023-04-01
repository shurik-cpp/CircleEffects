/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CircleScene.h"
#include <iostream>


USING_NS_CC;

Scene* CircleScene::createScene()
{
	return CircleScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool CircleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create("CloseNormal.png",
                                           "CloseSelected.png",
										   CC_CALLBACK_1(CircleScene::menuCloseCallback, this));

	if (closeItem == nullptr
		|| closeItem->getContentSize().width <= 0
		|| closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
	else {
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////

	auto label = Label::createWithTTF("Circle Effects", "fonts/Marker Felt.ttf", 24);
	if (label == nullptr) {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
	else {
        // position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }


	const float CIRCLE_RADIUS = 10;
	const int SEGMENTS = 150; // количество сегментов из которых рисуется кружок
	const Vec2 WINDOW_CENTER(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);


	int objectsRadius = 30; // начальный радиус расположения кружков
	int rowsCount = 5; // количество рядов
	int distanceBetweenCircles = 3; // растояние между кружками

	auto orange = Color3B::ORANGE;
	int r = orange.r;
	int g = orange.g;
	int b = orange.b;

	for (size_t i = 0; i < rowsCount; ++i)
	{
		int objectsCount = (objectsRadius * 2 + distanceBetweenCircles) / (CIRCLE_RADIUS - distanceBetweenCircles); // начальное количество кружков

		for (size_t j = 0; j < objectsCount; ++j)
		{
			float angle = j * 2 * M_PI / objectsCount; // вычисляем угол между центрами кружков
			const float x = WINDOW_CENTER.x + objectsRadius * cos(angle); // вычисляем координату X центра кружка
			const float y = WINDOW_CENTER.y + objectsRadius * sin(angle); // вычисляем координату Y центра кружка
			Vec2 position(x, y);
			// отрисовываем кружок с координатами (x, y)
			auto circle = DrawNode::create();
			std::cout << "Circle number: " << circles.size() << std::endl;
			std::cout << "Color: (" << r << ", " << g << ", " << b << ")\n";
			circle->drawSolidCircle(position, CIRCLE_RADIUS, 360, SEGMENTS, Color4F(Color3B(r, g, b)));
			circles.push_back(circle);
			this->addChild(circle);
			// TODO: Уже тут можно использовать random
			g = (g + 5 < 255) ? g + 5 : 127;
		}

		objectsRadius += CIRCLE_RADIUS * 2 + distanceBetweenCircles;
	}



    return true;
}


void CircleScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
