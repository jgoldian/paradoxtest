#pragma once

public struct Map
{
	int _targetX;
	int _targetY;

	Map(int mapWidth, int mapHeight, int targetX, int targetY);
	inline int ConvertXYToInt(int x, int y);

private:
	int _mapWidth;
	int _mapHeight;
};

public class Node
{
public:
	Node();
	Node(int X, int Y, int prevNode, int cost, Map *map);
	inline int ConvertToInt();	

	inline int OptimisticDistanceToGoal() const;	

	inline int GetX() const { return _x; }
	inline int GetY() const{ return _y; }
	inline int CostToNode() const{ return _costToNode; }
	inline int PreviousNode() const { return _previousNode; }

private:
	int _mapLocation;
	Map* _map;
	int _x;
	int _y;
	int _previousNode;
	int _costToNode;
};