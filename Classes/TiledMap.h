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

	void setGridVector();
	//All the operations towards the map will change the boolean of the character _pass of Grid£¡
	//The _pass change to true	
	static void removeMapGrid(Vec2);
	//@@The type of param is Vec2
	//@@param 1 is the old position 
	//@@param 2 is the new position
	//The old Grid _pass change to true, on the contrary the new Grid .... to false
	static void updateMapGrid(Vec2 old,Vec2 newPos);
	//@@The type of first param is Vec2
	//The _pass change to false
	static void newMapGrid(Vec2,int id);
	//@@The type of param is Vec2
	//Get UnitId
	static int getUnitId(Vec2);
	//@@The type of param is Vec2
	static bool checkMapGrid(Vec2);
	//Check if the place is collidable
	static bool checkPass(Vec2 pos);
	//Check if the field can be built
	static bool checkBuilt(Vec2 pos,int range);
	//@@param 1 is the positon of your building  *TiledMap positon
	//@@paran 2 is the field range of your building
	static void setUnpass(Vec2 pos, int range);
	//Check the boundary
	static bool checkBoundary(Vec2 pos);
	//turn the coordinate of OpenGL to TileMap
	//The return value is the coordinate of tho position in TileMap
	Vec2 tileCoordForPosition(Vec2 position);

	TMXLayer* getCollLayer(){ return _collidable; }
	//
	CREATE_FUNC(TiledMap);
private:
	//All the grid in the map
	static std::vector<Vector<Grid*>>_grid_Vector;
	//The pair first is Gird* which can be gotten by _grid_Vector
	//The pair second is the Id of Unit
	static std::map<Grid*, int> _gridAndId_Map;
	TMXTiledMap* _tiled_Map;
	TMXLayer* _collidable;
};

#endif // !_TILEDMAP

