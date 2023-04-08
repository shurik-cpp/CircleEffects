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

	Color4B blueSkyColor(Color3B(184, 237, 255));
	LayerColor* backgroundLayer = LayerColor::create(blueSkyColor);
	this->addChild(backgroundLayer, 0);

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

	auto label = Label::createWithTTF("Circle Effects", "fonts/Marker Felt.ttf", 14);
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



	const Vec2 WINDOW_CENTER(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
	auto c = new Circles(WINDOW_CENTER);
	circles = std::unique_ptr<Circles>(c);

	for (const auto& it : circles->GetObjects()) {
		this->addChild(it);
	}


	// Запускаем игровой цикл
	// Как только игровой цикл открывается, он вызывает функцию CircleScene::update(float dt)
	// у нас есть контроль над приоритетом, с которым программа обновления будет просматривать нашу функцию обновления.
	// По умолчанию, когда мы вызываем scheduleUpdate(), наша функция обновления будет вызываться для каждого кадра.
	// Если узел, который мы обновляем, не нужно обновлять каждый кадр, мы просто тратим впустую мощность процессора
	// (и время автономной работы). Если есть обновление с более низким приоритетом, мы можем использовать:
	// this->scheduleUpdateWithPriority(42);
	// Сначала вызывает все update(), для которых не установлен приоритет. Затем вызовет узел с наименьшим значением,
	// затем следующий с наибольшим и т.д.
	// (https://gamefromscratch.com/cocos2d-x-tutorial-series-game-loop-updates-and-action-handling/)
	this->scheduleUpdate();


	return true;
}

// Параметр, передаваемый для обновления — это значение, представляющее количество времени в секундах
// с момента последнего вызова функции обновления.
// Поэтому, если с момента последнего вызова обновления прошло 1/10 секунды, переданное значение будет равно 0.1
// В двух словах, при покадровом перемещении выражайте свои единицы в секундах,
// а затем умножайте их на дельту, переданную в функцию обновления.
void CircleScene::update(float delta) {
	static size_t counter = 0;
	if (counter > 50) {
		//circles->Tick(CircleEffects::SMOOTH_RANDOM_COLOR);
		counter = 0;
	}
	else {
		//circles->Tick(CircleEffects::SMOOTH_RANDOM_COLOR);
		circles->Tick(CircleEffects::SMOOTH_OPACITY);
		//circles->Tick(CircleEffects::ROTATE_CCV);
		//circles->Tick(CircleEffects::ROTATE_CV);
		//circles->Tick(CircleEffects::DIFFERENT_ROTATION);
	}
	counter++;
}


void CircleScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
