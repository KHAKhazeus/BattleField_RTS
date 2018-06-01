#include "MyResources.h"
#include "GameScene.h"
bool Money::init() {
	_money = 10000;
	char temp[50];
	sprintf(temp, "%d", _money);
	_label =  Label::create(temp, "fonts/arial.ttf", 32);
	this->addChild(_label);
	return _label;
}

bool Money::checkMoney(const int cost)const {
	if (_money >= cost) {
		return true;
	}
	return false;
}

void Money::updateMoneyDisplay() {
	char temp[50];
	sprintf(temp, "%d", _money);
	_label->setString(temp);
}

int Money::getMoney()const {
	return _money;
}

void Money::spendMoney(const int cost) {
	_money -= cost;
	updateMoneyDisplay();
}

void Money::increaseMoney(int amount) {
	//TODO
	//if(building is true)
	_money += amount;
	updateMoneyDisplay();
}

void Money::updateMoney(float dt) {
	//TODO the number of MineToMoney
	int amount = static_cast<GameScene*>(this->getParent())->getVectorMine().size() * 50;
	increaseMoney(amount);
}

bool Power::init() {
	_power = 0;
	char temp[30];
	sprintf(temp, "%d", _power);
	_label = Label::create(temp, "fonts/arial.ttf", 32);
	this->addChild(_label);
	return _label;
}
bool Power::checkPower(const int cost)const {
	if (_power >= cost) {
		return true;
	}
	return false;
}

void Power::updatePowerDisplay() {
	char temp[30];
	sprintf(temp, "%d", _power);
	_label->setString(temp);
}

void Power::spendPower(const int cost) {
	_power -= cost;
	updatePowerDisplay();
}

int Power::getPower()const  {
	return _power;
}

void Power::increasePower(int amount) {
	//TODO if the Powerstation is built
	
	_power += amount;
	updatePowerDisplay();
}



