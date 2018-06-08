#ifndef _UNIT
#define _UNIT
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
class Unit :public Sprite {
private:
	static int _id; //the id count of the whole building group
protected:
	bool _isBuilding;
	char _type;
public:
	virtual ui::LoadingBar* getHP() = 0;
	//set and get the Id for the building
	int getIdCount() { return _id; }
	void addIdCount() { _id++; }
	virtual bool isBuilding() { return _isBuilding; }
	virtual void setIsBuilding(bool character = false) { _isBuilding = character; }
	virtual int getCampID() = 0;
	virtual void setCampID(int) = 0;
	virtual bool judgeAttack(Vec2 pos) = 0;
	virtual int getAttack() = 0;
	virtual void setType(char type) { _type = type; }
	virtual char getType() { return _type; }
	virtual void moveTo(Vec2 pos){}
	virtual Vec2 getTiledPosition() { return { 0,0 }; }
	virtual void setTiledPosition(Vec2 pos) {}
};



#endif // !_UNIT

