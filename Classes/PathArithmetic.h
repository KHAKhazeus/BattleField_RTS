#ifndef __MX2__PathArithmetic__
#define __MX2__PathArithmetic__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "Grid.h"
#include "TiledMap.h"
#include <algorithm>

#define OPEN 1
#define CLOSE 2
USING_NS_CC;


class PathArithmetic : public cocos2d::Ref
{
private:
	int map_width;
	int map_height;
	Grid * start_point;
	Grid * terminal_point;
	std::vector<Grid *> open_list;
	std::vector<Grid *> close_list;
	std::vector<Vec2> path;

	int calculateEuclideanDistance(Grid* grid_next, Grid* grid_now);

	int calculateManhattanDistance(Grid* grid_next, Grid* grid_now);

	bool checkGrid(Grid* grid);

	void addSurroundGrid(Grid* grid);

public:
	//create pathfinder
	static PathArithmetic* create() {
		PathArithmetic* p = new PathArithmetic();
		if (p) {
			p->autorelease();
		}
		else {
			CC_SAFE_DELETE(p);
		}
		return p;
	}
	//initialize pathfinder
	void initPathArithmetic(TiledMap* tiled_map, Vec2 from, Vec2 to);
	//search path
	void findPath();
	//return path
	std::vector<Vec2> getPath();
};


#endif