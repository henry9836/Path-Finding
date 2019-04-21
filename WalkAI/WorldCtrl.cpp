#include "WorldCtrl.h"

using namespace std;

/*

WORLD SYBMOLS:

░ - Ground
X - Destination
O - AI
█ - Wall
▓ - Explored
+ - Path

*/

//DrawWorld outputs Symbols to represent areas of the world onto the screen
int DrawWorld(vector<vector<WORLD>> m_world) {
	for (size_t i = 0; i < m_world.size(); i++)
	{
		for (size_t j = 0; j < m_world.at(i).size(); j++)
		{
			switch (m_world.at(i).at(j))
			{
			case WORLD::GROUND: {
				wcout << L"░";
				break;
			}
			case WORLD::WALL: {
				wcout << L"█";
				break;
			}
			case WORLD::DEST: {
				wcout << L"X"; //⚐ \u2690
				break;
			}
			case WORLD::AI: {
				wcout << L"O"; //♘ \u2658
				break;
			}
			case WORLD::EXPLORED: {
				wcout << L"▓"; 
				break;
			}
			case WORLD::PATH: {
				wcout << L"+"; 
				break;
			}
			default: {
				wcout << L"ERROR DID NOT FIND VALUE OF " << static_cast<int>(m_world.at(i).at(j)) << L" in DRAWWORLD" << endl;
				abort();
				break;
			}
			}
		}
		wcout << L'\n';
	}

	return 0;
}
