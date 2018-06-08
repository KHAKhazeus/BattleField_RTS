#ifndef _MOUSERECT
#define _MOUSERECT
#include "cocos2d.h"
USING_NS_CC;

//Class of mouse rect selection
class MouseRect : public cocos2d::DrawNode
{
public:
	CREATE_FUNC(MouseRect);

	//Start point and end point
	Vec2 start, end;

	virtual void update(float delta);

	void reset();
};

#endif _MOUSERECT