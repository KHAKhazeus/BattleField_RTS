#include "PathArithmetic.h"

const int DISTANCE = 10;

void PathArithmetic::initPathArithmetic(TiledMap* tiled_map, Vec2 from, Vec2 to) {
	map_width = tiled_map->getTiledMap()->getMapSize().width;
	map_height = tiled_map->getTiledMap()->getMapSize().height; 
	for (int i = 0; i < map_width; i++) {
		for (int j = 0; j < map_height; j++) {
			TiledMap::_grid_Vector.at(i).at(j)->setG(0);
			TiledMap::_grid_Vector.at(i).at(j)->setH(0);
			TiledMap::_grid_Vector.at(i).at(j)->setF(0);
			TiledMap::_grid_Vector.at(i).at(j)->setFlag(0);
			TiledMap::_grid_Vector.at(i).at(j)->setParent(nullptr);
		}
	}
	start_point = TiledMap::_grid_Vector.at(from.x).at(from.y);
	terminal_point = TiledMap::_grid_Vector.at(to.x).at(to.y);
	close_list.push_back(start_point);
	addSurroundGrid(start_point);
}

void PathArithmetic::addSurroundGrid(Grid* grid) {
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) {
				continue;
			}
			auto current_x = grid->getX() + i;
			auto current_y = grid->getY() + j;
			if (current_x >= 0 && current_x < map_width && current_y >= 0 && current_y < map_height
				&& checkGrid(TiledMap::_grid_Vector.at(current_x).at(current_y))) {
				if (TiledMap::_grid_Vector.at(current_x).at(current_y)->getFlag() == OPEN) {
					if (TiledMap::_grid_Vector.at(current_x).at(current_y)->getG() > 
						calculateEuclideanDistance(TiledMap::_grid_Vector.at(current_x).at(current_y), grid)){
						TiledMap::_grid_Vector.at(current_x).at(current_y)->setG(calculateEuclideanDistance(TiledMap::_grid_Vector.at(current_x).at(current_y), grid));
						TiledMap::_grid_Vector.at(current_x).at(current_y)->setF(TiledMap::_grid_Vector.at(current_x).at(current_y)->getG() + TiledMap::_grid_Vector.at(current_x).at(current_y)->getH());
						TiledMap::_grid_Vector.at(current_x).at(current_y)->setParent(grid);
					}
				}
				else {
					TiledMap::_grid_Vector.at(current_x).at(current_y)->setG(calculateEuclideanDistance
					(TiledMap::_grid_Vector.at(current_x).at(current_y), grid));
					TiledMap::_grid_Vector.at(current_x).at(current_y)->setH(calculateManhattanDistance
					(TiledMap::_grid_Vector.at(current_x).at(current_y), terminal_point));
					TiledMap::_grid_Vector.at(current_x).at(current_y)->setF(
						TiledMap::_grid_Vector.at(current_x).at(current_y)->getG() + TiledMap::_grid_Vector.at(current_x).at(current_y)->getH());
					TiledMap::_grid_Vector.at(current_x).at(current_y)->setParent(grid);
					TiledMap::_grid_Vector.at(current_x).at(current_y)->setFlag(OPEN);
					open_list.push_back(TiledMap::_grid_Vector.at(current_x).at(current_y));
				}
			}
		}
	}
}


int PathArithmetic::calculateEuclideanDistance(Grid* grid_next, Grid* grid_now){
	if (grid_next->getX() == grid_now->getX() || grid_next->getY() == grid_now->getY()){
		return grid_now->getG() + 10;
	}
	return grid_now->getG() + 14;
}

int PathArithmetic::calculateManhattanDistance(Grid* grid_next, Grid* terminal){
	return (abs(grid_next->getX() - terminal->getX()) + 
		abs(grid_next->getY() - terminal->getY())) * DISTANCE;
}

bool PathArithmetic::checkGrid(Grid* grid){
	if (grid->getFlag() == CLOSE){
		return false;
	}
	else if (!grid->isPass()){
		return false;
	}
	else{
		return true;
	}
}


void PathArithmetic::findPath() {
	while (terminal_point->getFlag() != CLOSE && !open_list.empty()) {
		auto next_grid = open_list[0];
		for (auto p : open_list) {
			if (p->getF() < next_grid->getF()) {
				next_grid = p;
			}
		}
		next_grid->setFlag(CLOSE);
		close_list.push_back(next_grid);
		addSurroundGrid(next_grid);
		std::vector<Grid*>::iterator iter = find(open_list.begin(), open_list.end(), next_grid);
		if (iter != open_list.end())
		{
			open_list.erase(iter);
		}
	}
}

std::vector<Vec2> PathArithmetic::getPath() {
	Grid* grid = terminal_point;
	while (grid->getParent() != start_point) {
		path.push_back(Vec2(grid->getX(), grid->getY()));
		grid = grid->getParent();
	}
	std::reverse(path.begin(), path.end());
	return path;
}