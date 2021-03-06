#include "LoadingScene.h"
#include "MenuScene.h"
std::shared_ptr<SocketServer> LoadingScene::_socket_server;
std::shared_ptr<SocketClient> LoadingScene::_socket_client;



Scene* LoadingScene::createScene() {
	auto scene = Scene::create();
	auto loadingScene = LoadingScene::create();
	scene->addChild(loadingScene);
	return scene;
}

bool LoadingScene::init() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	_background = Sprite::create("Menu/MainBackground.png");
	_background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(_background);

	_loadingBar = LoadingBar::create("Menu/SliderProgress.png");
	_loadingBar->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.05));
	this->addChild(_loadingBar,99);
	_loadingBar->setDirection(LoadingBar::Direction::LEFT);
	_loadingBar->setPercent(100);

	auto loadingBarBackground = Sprite::create("Menu/SliderTrack.png");
	loadingBarBackground->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.05));
	this->addChild(loadingBarBackground,2);

	_count = 0;

	

	return true;
}


void LoadingScene::loadingCallback() {
	_count++;
	_loadingBar->setPercent((_count / 8.0) * 100);
	//log("%d", _count);
	if(_count == 15) {
		auto gameScene = MenuScene::createScene();
		auto gameSceneAnimate = TransitionFade::create(1.0f, gameScene);
		Director::getInstance()->replaceScene(gameSceneAnimate);
	}
}

Animation* LoadingScene::getAnimateByName(std::string animName, float delay, int animNum) {
	Animation* animation = Animation::create();
	for (unsigned int i = 0; i <= animNum; i++) {
		// get the picture name
		std::string frameName = animName;
		frameName.append(StringUtils::format("%d", i)).append(".png");
		// add the picture to spriteframe
		animation->addSpriteFrameWithFile(frameName.c_str());
	}
	// set the properties of the animation
	animation->setDelayPerUnit(delay);
	// reset the animate
	animation->setRestoreOriginalFrame(true);
	// return the animate
	return animation;
}

void LoadingScene::onEnter() {
	// Ԥ������Ч preloading sound effect
	Scene::onEnter();

	SimpleAudioEngine::getInstance()->preloadEffect(CONSTRUCTION);
	SimpleAudioEngine::getInstance()->preloadEffect(BUILD);
	SimpleAudioEngine::getInstance()->preloadEffect(SOLDIER);
	SimpleAudioEngine::getInstance()->preloadEffect(TANK);
	SimpleAudioEngine::getInstance()->preloadEffect(DOG);
	SimpleAudioEngine::getInstance()->preloadEffect(FIGHT);
	SimpleAudioEngine::getInstance()->preloadEffect(LOST);
	SimpleAudioEngine::getInstance()->preloadEffect(TANKBULLET);
	SimpleAudioEngine::getInstance()->preloadEffect(EXPLODE);

	auto textureCache = Director::getInstance()->getTextureCache();
	auto AnimateCache = AnimationCache::getInstance();
	AnimateCache->addAnimation(getAnimateByName("tankBase/tankbuilding_", 0.1f, 23), "tankBase");
	AnimateCache->addAnimation(getAnimateByName("warfactoryblue/warfactory_", 0.1f, 23), "tankBaseBlue");
	textureCache->addImageAsync("unit/base_28.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	textureCache->addImageAsync("unit/base_27.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	AnimateCache->addAnimation(getAnimateByName("soldierBase/soldierbase_", 0.1f, 23), "soldierBase");
	AnimateCache->addAnimation(getAnimateByName("soldierbaseblue/SoldierBase_", 0.1f, 23), "soldierBaseBlue");
	textureCache->addImageAsync("unit/building_1.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	AnimateCache->addAnimation(getAnimateByName("powerPlant/PowerBuilt_", 0.1f, 24), "powerPlant");
	AnimateCache->addAnimation(getAnimateByName("powerPlantBlue/powerPlant_", 0.1f, 23), "powerPlantBlue");
	textureCache->addImageAsync("unit/building_2.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	AnimateCache->addAnimation(getAnimateByName("moneyMine/MinetoMoney_", 0.1f, 24), "moneyMine");
	AnimateCache->addAnimation(getAnimateByName("mineblue/Mine_", 0.1f, 23), "moneyMineBlue");
	textureCache->addImageAsync("unit/building_3.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	AnimateCache->addAnimation(getAnimateByName("dogAttack/dog_attack", 0.2f, 6), "dogAttack");
	textureCache->addImageAsync("unit/building_4.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	AnimateCache->addAnimation(getAnimateByName("dogRun/dog", 0.2f, 5), "dogRun");
	textureCache->addImageAsync("unit/FighterUnit.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	AnimateCache->addAnimation(getAnimateByName("soldierRun/soldier", 0.2f, 7), "soldierRun");
	AnimateCache->addAnimation(getAnimateByName("blueSoldierRun/blueSoldier", 0.2f, 6), "soldierRunBlue");
	textureCache->addImageAsync("unit/FighterUnit_1.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	AnimateCache->addAnimation(getAnimateByName("tank/tank", 0.2f, 7), "tankRun");
	AnimateCache->addAnimation(getAnimateByName("blueTank/blueTank", 0.2f, 4), "tankRunBlue");
	textureCache->addImageAsync("unit/FighterUnit_2.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	textureCache->addImageAsync("unit/FighterUnit_3.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	AnimateCache->addAnimation(getAnimateByName("explode1/explode", 0.1f, 30), "explode");
	textureCache->addImageAsync("soldierAttack/bullet.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	textureCache->addImageAsync("dogRun/dog0.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	textureCache->addImageAsync("soldierRun/soldierstand.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	textureCache->addImageAsync("blueSoldierRun/blueSoldierStand.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
	textureCache->addImageAsync("tank/tankBullet.png", CC_CALLBACK_0(LoadingScene::loadingCallback, this));
}