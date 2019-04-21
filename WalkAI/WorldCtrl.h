#pragma once
#include <iostream>
#include <vector>

const int MOVECOST = 10;
const int DIAMOVECOST = 14;

enum class WORLD
{
	GROUND,
	WALL,
	DEST,
	AI,
	EXPLORED,
	PATH,
};

class gridPOS
{
	public:
		int x = 0;
		int y = 0;
		int h = 0;
		int g = 0;
		int f = 9999;
		int ParentX = 0;
		int ParentY = 0;
};

using namespace std;

int DrawWorld(vector<vector<WORLD>> m_world);
