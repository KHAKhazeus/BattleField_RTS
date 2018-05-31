#ifndef _TIlEDMAP
#define _TILEDMAP
#include "cocos2d.h"
#include "Grid.h"
USING_NS_CC;
class TiledMap : public cocos2d::Node {
public:
	bool init();
	//@@return value is the pointer to the TMXObjectGroup
	TMXObjectGroup * getObjectGroup(std::string);

	//@@return value is the pointer to the TXMtiledMap
	TMXTiledMap* getTiledMap();

	void setCollidableVector();
	//All the operations towards the map will change the boolean of the character _pass of Grid£¡
	//The _pass change to true	
	static void removeMapGrid(Grid *);
	//@@param 1 is the old position
	//@@param 2 is the new position
	//The old Grid _pass change to true, on the contrary the new Grid .... to false
	static void updateMapGrid(Grid *old,Grid *newPos);
	//The _pass change to false
	static void newMapGrid(Grid *,int id);
	//Get UnitId
	static int getUnitId(Grid *);
	//
	static bool checkMapGrid(Grid *);
	//
	//
	CREATE_FUNC(TiledMap);
private:
	static std::vector<Grid*>_collidable_Vector;
	static std::map<Grid*, int> _gridAndId_Map;
	TMXTiledMap* _tiled_Map;
	TMXLayer* _collidable;
};

#endif // !_TILEDMAP

