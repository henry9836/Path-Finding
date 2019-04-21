#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include "WorldCtrl.h"
#include "ConsoleControls.h"

using namespace std;

vector<vector<WORLD>> m_world; //2D vector (y,x)
vector<gridPOS> closedList; //explored positions
vector<gridPOS> path_to_success; //solution path
vector<gridPOS> openList; //open list

gridPOS Des;
gridPOS currentSquare;
gridPOS bestArea;

int bestScore = 9999;
int worldYSize = 0;
int worldXSize = 0;
bool DEBUG_MODE = false;

int GetPosValue(gridPOS &m_gridPos) {
	int f = 0;
	m_gridPos.g = MOVECOST;
	m_gridPos.h = ((MOVECOST * (abs(Des.x - m_gridPos.x))) + (MOVECOST * (abs(Des.y - m_gridPos.y))));
	if (m_gridPos.x != currentSquare.x && m_gridPos.y != currentSquare.y) { //Are we going in a diagonal?
		m_gridPos.g = DIAMOVECOST;
	}
	m_gridPos.f = m_gridPos.g + m_gridPos.h;
	return m_gridPos.f;
}

int FindPath() {
	bool foundpath = false;

	Console_gotoXY(0, worldYSize + 1);
	wcout << L"Attempting to find a path..." << endl;
	while (!foundpath) {
		bestScore = 9999;

		// Look for the lowest F cost square on the open list. We refer to this as the current square.

		for (unsigned i = 0; i < openList.size(); i++) 
		{
			if (openList.at(i).f < bestScore) {
				currentSquare = openList.at(i);
				bestScore = currentSquare.f;
			}
		}

		if (openList.size() <= 0) {
			foundpath = true;
			break;
		}

		if (currentSquare.x == Des.x && currentSquare.y == Des.y) {
			foundpath = true;
			break;
		}

		// Switch it to the closed list.

		closedList.push_back(currentSquare);

		for (unsigned i = 0; i < openList.size(); i++)
		{
			if (openList.at(i).x == currentSquare.x && openList.at(i).y == currentSquare.y) {
				openList.erase(openList.begin()+i);
			}
		}

		// For each of the 8 squares adjacent to this current square
		for (size_t m_y = 0; m_y < 3; m_y++) //y
		{
			for (size_t m_x = 0; m_x < 3; m_x++) //x
			{
				gridPOS m_gridPos;
				m_gridPos.y = ((currentSquare.y - 1) + m_y);
				m_gridPos.x = ((currentSquare.x - 1) + m_x);

				if ((m_gridPos.y >= 0 && m_gridPos.y < worldYSize) && (m_gridPos.x >= 0 && m_gridPos.x < worldXSize)) { //Ignore any out of bounds

					//If it is not walkable or if it is on the closed list, ignore it. Otherwise do the following.

					bool isOnClosedList = false;

					for (unsigned i = 0; i < closedList.size(); i++)
					{
						if (closedList.at(i).x == m_gridPos.x && closedList.at(i).y == m_gridPos.y) {
							isOnClosedList = true;
						}
					}

					if (m_world.at(m_gridPos.y).at(m_gridPos.x) != WORLD::WALL && !isOnClosedList) {

						//Eye candy

						if (m_world.at(m_gridPos.y).at(m_gridPos.x) != WORLD::WALL && m_world.at(m_gridPos.y).at(m_gridPos.x) != WORLD::AI && m_world.at(m_gridPos.y).at(m_gridPos.x) != WORLD::DEST) {
							m_world.at(m_gridPos.y).at(m_gridPos.x) = WORLD::EXPLORED;
						}

						//If it isn’t on the open list, add it to the open list. Make the current square the parent of this square. Record the F, G, and H costs of the square.

						bool isOnOpenList = false;

						for (unsigned i = 0; i < openList.size(); i++)
						{
							if (openList.at(i).x == m_gridPos.x && openList.at(i).y == m_gridPos.y) {
								isOnOpenList = true;
							}
						}

						m_gridPos.f = GetPosValue(m_gridPos);

						if (!isOnOpenList) {
							m_gridPos.ParentX = currentSquare.x;
							m_gridPos.ParentY = currentSquare.y;
							openList.push_back(m_gridPos);
						}

						//If it is on the open list already, check to see if this path to that square is better, using G cost as the measure. A lower G cost means that this is a better path. If so, change the parent of the square to the current square, and recalculate the G and F scores of the square.

						else{
							if (m_gridPos.g < currentSquare.g) {
								m_gridPos.ParentX = currentSquare.x;
								m_gridPos.ParentY = currentSquare.y;
								m_gridPos.f = GetPosValue(m_gridPos);
							}
						}

					}
				
				}

			}

		}

		DrawWorld(m_world);

		Console_Clear();

	}

	if (openList.size() <= 0) {
		wcout << L"I give up ¯\\_(�)_/¯" << endl << L"No Possible Path" << endl;
	}
	else {
		//while not at start trace backwards from end of closed list
		bool traced = false;
		gridPOS tracer = closedList.at(closedList.size()-1);
		while (!traced) {
			if (tracer.x == closedList.at(0).x && tracer.y == closedList.at(0).y) {
				traced = true;
				break;
			}
			m_world.at(tracer.y).at(tracer.x) = WORLD::PATH;
			tracer.x = tracer.ParentX;
			tracer.y = tracer.ParentY;
			for (unsigned i = 0; i < closedList.size(); i++)
			{
				if (closedList.at(i).x == tracer.x && closedList.at(i).y == tracer.y) {
					tracer = closedList.at(i);
				}
			}
			DrawWorld(m_world);
			Console_Clear();
		}
		Console_Clear();
		DrawWorld(m_world);
	}
	
	return 0;
}

int CreateWorld() {
	Console_Clear();
	bool doneCreating = false;
	bool placing = true;
	char keyPressed = NULL;
	int currentY = 0;
	int currentX = 0;
	int stagePlace = 0; //0 - AI, 1 - Dest, 2 - Walls
	while (!doneCreating) {
		placing = true;
		Console_Clear();
		DrawWorld(m_world);
		Console_gotoXY(0, worldYSize + 1);
		switch (stagePlace)
		{
		case 0: {
			wcout << L"AI Placement Mode, WASD and Enter";
			break;
		}
		case 1: {
			wcout << L"Destination Placement Mode, WASD and Enter";
			break;
		}
		default: {
			wcout << L"Wall Placement Mode Press ESC to continue, WASD and Enter to place walls";
			break;
		}
		}
		
		while (placing) {
			keyPressed = NULL;
			keyPressed = _getch();
			Console_gotoXY(currentX, currentY);

			if (keyPressed == 224) { //Stop Arrow Keys from inputing twice and breaking the game
				keyPressed = _getch();
			}

			if (keyPressed == 27) { //Escape build mode
				switch (stagePlace)
				{
				case 0: {
					break;
				}
				case 1: {
					break;
				}
				default: {
					placing = false;
					doneCreating = true;
					break;
				}
				}
				
			}

			Beep(1000, 100);

			if (keyPressed == 119 || keyPressed == 87 || keyPressed == 72) { //Up keys
				if (currentY > 0) {
					currentY--;
				}
			}

			else if (keyPressed == 80 || keyPressed == 115 || keyPressed == 83) { //Down keys
				if (currentY < worldYSize-1) {
					currentY++;
				}
			}

			else if (keyPressed == 97 || keyPressed == 65 || keyPressed == 75) { //Left keys
				if (currentX > 0) {
					currentX--;
				}
			}

			else if (keyPressed == 100 || keyPressed == 68 || keyPressed == 77) { //Right keys
				if (currentX < worldXSize-1) {
					currentX++;
				}
			}

			else if (keyPressed == 13 || keyPressed == 101 || keyPressed == 69 || keyPressed == 32) { //Enter, e, E, Space
				if (m_world.at(currentY).at(currentX) == WORLD::GROUND) {
					switch (stagePlace)
					{
					case 0: {
						m_world.at(currentY).at(currentX) = WORLD::AI;
						currentSquare.x = currentX;
						currentSquare.y = currentY;
						gridPOS AI;
						AI.x = currentSquare.x;
						AI.y = currentSquare.y;
						AI.f = GetPosValue(AI);
						openList.push_back(AI);
						stagePlace = 1;
						break;
					}
					case 1: {
						m_world.at(currentY).at(currentX) = WORLD::DEST;
						Des.x = currentX;
						Des.y = currentY;
						stagePlace = 2;
						break;
					}
					default: {
						m_world.at(currentY).at(currentX) = WORLD::WALL;
						break;
					}
					}
					placing = !placing;
				}
			}
		}
		
	}

	return 0;
}

int main() {
	_setmode(_fileno(stdout), _O_WTEXT); //let there be unicode!

	wcout << L"A* Search" << endl << L"~ Henry Oliver" << endl;

	wcout << L"Please input World Size X: " << endl;
	cin >> worldXSize;

	while (worldXSize < 1)
	{
		wcout << L"Please a valid input World Size X (More than 0): " << endl;
		cin >> worldXSize;
	}


	wcout << L"Please input World Size Y: " << endl;
	cin >> worldYSize;

	while (worldYSize < 1)
	{
		wcout << L"Please a valid input World Size Y (More than 0): " << endl;
		cin >> worldYSize;
	}

	m_world.resize(worldYSize);

	for (signed i = 0; i < worldYSize; i++)
	{
		for (signed j = 0; j < worldXSize; j++)
		{
			m_world.at(i).push_back(WORLD::GROUND);
		}
	}

	DrawWorld(m_world);
	CreateWorld();
	FindPath();

	system("pause");

	return 0;
}