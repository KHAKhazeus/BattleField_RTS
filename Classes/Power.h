#ifndef _POWER
#define _POWER
#include "cocos2d.h"
class Power :public cocos2d::LabelBMFont {
private:
	int _power;
public:
	virtual bool init() override;
	//to judge if you can afford it
	bool checkPower(int cost)const;
	void spendPower(int cost);
	//to get the value of the _money
	int getPower();
	//update the value of the _money
	void increasePower(int amount);
	CREATE_FUNC(Power);

};
#endif // !_POWER

