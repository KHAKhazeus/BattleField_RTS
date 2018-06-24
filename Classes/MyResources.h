#ifndef _MY_RESOURCES
#define _MY_RESOURCES
#include "cocos2d.h"
class Money :public cocos2d::Label {
private:
	int _money ;
	Label* _label;
public:
	virtual bool init() override;
	//to judge if you can afford it
	bool checkMoney(const int cost)const;
	void spendMoney(const int cost);
	//to get the value of the _money
	int getMoney()const;
	//update the value of the _money
	void increaseMoney(int amount);
	//update the display of the _money
	void updateMoneyDisplay();
	//schedule update function
	void updateMoney(float dt);
	CREATE_FUNC(Money);
};

class Power :public cocos2d::Label {
private:
	int _power;
	Label* _label;
public:
	virtual bool init() override;
	//to judge if you can afford it
	bool checkPower(const int cost)const;
	void spendPower(const int cost);
	//to get the value of the _power
	int getPower()const;
	//update the value of the _power
	void increasePower(int amount);
	//update the display of the _power
	void updatePowerDisplay();
	//schedule update function
	void updatePower(float dt);
	CREATE_FUNC(Power);
};
#endif // !_MY_RESOURCES

