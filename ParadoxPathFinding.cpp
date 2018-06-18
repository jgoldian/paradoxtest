// ParadoxPathFinding.cpp : main project file.

#include <queue>
#include <map>
#include <math.h>
#include "Node.h"

using namespace System;
using namespace std;

inline int Map::ConvertXYToInt(int x, int y)
{
	return x + (_mapWidth * y);
}

Map::Map(int mapWidth, int mapHeight, int targetX, int targetY)
{
	_mapHeight = mapHeight;
	_mapWidth = mapWidth;
	_targetX = targetX;
	_targetY = targetY;
}

int Node::ConvertToInt()
{
	return _mapLocation;
}

Node::Node()
{
}

Node::Node(int X, int Y, int prevNode, int cost, Map *map)
{
	_x = X;
	_y = Y;
	_previousNode = prevNode;
	_costToNode = cost;
	_map = map;
	_mapLocation = map->ConvertXYToInt(_x, _y);
}

int Node::OptimisticDistanceToGoal() const
{
	int xDistance = Math::Abs(_map->_targetX - _x);
	int yDistance = Math::Abs(_map->_targetY - _y);
	return xDistance + yDistance;
}


struct CompareNode {
	bool operator()(Node const & p1, Node const & p2) {
		// return "true" if "p1" is ordered before "p2", for example:
		int p1Cost = p1.CostToNode() + p1.OptimisticDistanceToGoal();
		int p2Cost = p2.CostToNode() + p2.OptimisticDistanceToGoal();
		return p1Cost > p2Cost;
	}
};

int FindPath(const int nStartX, const int nStartY,
	const int nTargetX, const int nTargetY,
	const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
	int* pOutBuffer, const int nOutBufferSize)
{
	Map currentMap(nMapWidth, nMapHeight, nTargetX, nTargetY);

	Node *end = nullptr;
	Node start(nStartX, nStartY, -1, 0, &currentMap);
	// The STL priority queue requires the underlying structure to be exposed as well as a compare node structure to define the priority
	// While STL is probably not the correct choice in real development making a priority queue by hand for this didn't seem like the correct choice
	priority_queue<Node, vector<Node>, CompareNode> queue;
	map<int, Node> exploredNodes;
	queue.push(start);	
	
	while (!queue.empty())
	{		
		Node n = queue.top();
		queue.pop();

		if (exploredNodes.find(n.ConvertToInt()) != exploredNodes.end() || n.CostToNode() > nOutBufferSize)
		{
			continue;
		}
		exploredNodes[n.ConvertToInt()] = n;

		if (n.GetX() == nTargetX && n.GetY() == nTargetY)
		{
			end = &n;
			break;
		}		

		int leftNode = currentMap.ConvertXYToInt(n.GetX() - 1, n.GetY());
		int rightNode = currentMap.ConvertXYToInt(n.GetX() + 1, n.GetY());
		int downNode = currentMap.ConvertXYToInt(n.GetX(), n.GetY() - 1);
		int upNode = currentMap.ConvertXYToInt(n.GetX(), n.GetY() + 1);

		// Check left
		if (n.GetX() > 0 && 
			exploredNodes.find(leftNode) == exploredNodes.end() &&
			pMap[leftNode])
		{
			Node newNode(n.GetX() - 1, n.GetY(), n.ConvertToInt(), n.CostToNode() + 1, &currentMap);
			queue.push(newNode);
		}

		// Check down
		if (n.GetY() > 0 &&
			exploredNodes.find(downNode) == exploredNodes.end() &&
			pMap[downNode])
		{
			Node newNode(n.GetX(), n.GetY() - 1, n.ConvertToInt(), n.CostToNode() + 1, &currentMap);
			queue.push(newNode);
		}

		// Check right
		if (n.GetX() + 1 <nMapWidth  &&
			exploredNodes.find(rightNode) == exploredNodes.end() &&
			pMap[rightNode])
		{
			Node newNode(n.GetX() + 1, n.GetY(), n.ConvertToInt(), n.CostToNode() + 1, &currentMap);
			queue.push(newNode);
		}

		// Check up
		if (n.GetY() + 1 < nMapHeight &&
			exploredNodes.find(upNode) == exploredNodes.end() &&
			pMap[upNode])
		{
			Node newNode(n.GetX(), n.GetY() + 1, n.ConvertToInt(), n.CostToNode() + 1, &currentMap);
			queue.push(newNode);
		}
	}

	if (end != nullptr)
	{
		int currentNode = end->ConvertToInt();
		for (int i = end->CostToNode() - 1; i >= 0; i--)
		{
			pOutBuffer[i] = currentNode;
			currentNode = exploredNodes[currentNode].PreviousNode();
		}

		return end->CostToNode();
	}

	return -1;
}

int tests()
{
	{	
		unsigned char pMap[] = { 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1 };
		int pOutBuffer[12];
		FindPath(0, 0, 1, 2, pMap, 4, 3, pOutBuffer, 12);
		if (pOutBuffer[0] != 1 || pOutBuffer[1] != 5 || pOutBuffer[2] != 9)
			return -1;
	}

	int output = 0;
	unsigned char pMap[] = { 0, 0, 1, 0, 1, 1, 1, 0, 1 };
	int pOutBuffer[7];
	output = FindPath(2, 0, 0, 2, pMap, 3, 3, pOutBuffer, 7);
	if (output != -1)
		return -1;

	unsigned char pMap2[] = { 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 0, 1,
		1, 1, 1, 1, 1, 1, 0, 1, };
	int pOutBuffer2[20];

	for (int i = 0; i < 13; i++)
	{
		output = FindPath(5, 0, 5, 3, pMap2, 8, 5, pOutBuffer2, i);
		if (output != -1)
			return -1;
	}
		
	output = FindPath(5, 0, 5, 3, pMap2, 8, 5, pOutBuffer2, 20);
	if (output == -1)
		return -1;
    return 0;
}
