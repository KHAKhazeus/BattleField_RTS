#ifndef _UNIT_MANAGER
#define _UNIT_MANAGER
#include "cocos2d.h"
#include "TiledMap.h"
USING_NS_CC;
class UnitManager :public cocos2d::Ref {
public:
	virtual bool init();
	CREATE_FUNC(UnitManager);

	//
	void checkWinOrLose();
};


#endif // !_UNIT_MANAGER

