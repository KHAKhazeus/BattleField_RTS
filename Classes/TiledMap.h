  #pragma once
#include "cocos2d.h"
#include "Grid.h"
#include "Unit.h"
#include "BattleField_RTS.h"
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
	static void removeMapGrid(Vec2 pos, int fix_modelf);
	//@@The type of param is Vec2
	//@@param 1 is the old position 
	//@@param 2 is the new position
	//The old Grid _pass change to true, on the contrary the new Grid .... to false
	static void updateMapGrid(Vec2 old,Vec2 newPos);
	//@@param1 is Vec2 refered to the position of the Unit
	//@@param2 is the Id of the Unit
	//This function just for the unit who occupies only one tiled
	//The _pass change to false
	static void newMapGrid(Vec2 pos,int id);
	//@@param1 is Vec2 refered to the Anchor of the Unit
	//@@param2 is the Id of the Unit
	//@@param3 is the range of the Unit
	//@@param4 is the FIX_MODLE
	//This function just for the unit who occupies multiple tiled
	//The _pass change to false
	static void newMapGrid(Vec2 pos, int id, int range,int fix_modle = FIX_SQUARE);

	//@@The type of param is Vec2
	//Get UnitId
	static int getUnitIdByPosition(Vec2);
	//@@The type of param is Vec2
	static bool checkMapGrid(Vec2);
	//Check if the place is collidable
	static bool checkPass(Vec2 pos);
	static bool checkPass(int x, int y);
	//Check if the field can be built
	static bool checkBuilt(Vec2 pos,int range);
	//@@param 1 is the positon of your building  *TiledMap positon
	//@@paran 2 is the field range of your building
	//@@param 3 is the fix_model
	static void setUnpass(Vec2 pos, int range,int fix_model);

	static void setUnpass(Vec2 Pos);

	static void setPass(Vec2 Pos);
	//Check the boundary
	static bool checkBoundary(Vec2 pos);
	static bool checkCreate(Vec2 pos);

	//_idAndUnit_Map API

	//@@param1 is the Id of the Unit
	//@@param2 is the Pointer to the Unit
	//This function push the pair<id,Sprite*> into _idAndUnit_Map
	static void newMapId(int id, Unit* unit);
	//@@param is the Id of the Unit
	//This function remove the pair<id,Sprite*> from the _idAndUnit_Map
	static void removeMapId(int id);
	//@@param is the Id of the Unit
	//The return value is the Pointer to the Unit
	static Unit* getUnitById(int id);
	//@@param the Id of the Unit
	//To check the Id if has been removed already
	static bool checkUnitId(int id);

	Vec2 changeOPGL(Vec2 pos);

	//_selected_Vector API
	//@@param the Pointer to the Unit
	static void newVectorUnit(Unit *);
	//@@clear up the vector
	static void clearUp();
	//To check if the vector is empty
	static int checkSize();
	//To get the Pointer to the SelectedVector
	static std::vector<Unit*>* getSelectedVector();



	//turn the coordinate of OpenGL to TileMap
	//The return value is the coordinate of tho position in TileMap
	Vec2 tileCoordForPosition(Vec2 position);
	Vec2 locationForTilePos(Vec2 pos);

	TMXLayer* getCollLayer(){ return _collidable; }
	//All the grid in the map
	static std::vector<Vector<Grid*>>_grid_Vector;
	//find free grid near
	Vec2 findFreeNear(Vec2 position);

	static void setMapFlagLost() { _map_flag = LOSTTEMP; }
	static void setMapFlagSnow() { _map_flag = SNOWMAP; }
	
	
	CREATE_FUNC(TiledMap);
private:
	//The pair first is Gird* which can be gotten by _grid_Vector
	//The pair second is the Id of Unit
	static std::map<Grid*, int> _gridAndId_Map;
	//The pair first is the Id of the Unit
	//The pair second is the Pointer to the Unit
	static std::map<int, Unit*> _idAndUnit_Map;
	
	static std::vector<Unit*> _select_Vector;

	//The Pointer to the Map
	TMXTiledMap* _tiled_Map;
	//The Pointer to the collidable Layer
	TMXLayer* _collidable;
	//The flag to select map
	static int _map_flag;
};

 // !_TILEDMAP

