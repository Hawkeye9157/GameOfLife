#include "raylib.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <vector>
#include <time.h>


/* Assignment Details
	Underpopulation: Any live cell with fewer than two live neighbors dies.

	Survival: Any live cell with two or three live neighbors continues living.

	Overpopulation: Any live cell with more than three live neighbors dies.

	Reproduction: Any dead cell with exactly three live neighbors becomes alive.

	White == alive
	Black == dead
*/
const int screenWidth = 1000;
const int screenHeight = 1000;

const int gridWidth = 80;
const int gridHeight = 80;

const float cellWidth = screenWidth / gridWidth;
const float cellHeight = screenHeight / gridHeight;

using Cells = std::vector<bool>;

bool ReadCell(int x, int y, const Cells& cells) {
	if (x < 0) x = gridWidth - 1;
	else if (x > gridWidth - 1) x = 0;
	if (y < 0) y = gridHeight - 1;
	else if (y > gridHeight - 1) y = 0;
	return cells[x + (y * gridWidth)];
}
void WriteCell(int x, int y,bool value, Cells& cells) {
	cells[x + (y * gridWidth)] = value;
}
void RandomizeCells(Cells& cells) {
	for (int i = 0; i < cells.size(); i++) {
		cells[i] = GetRandomValue(0, 1);
	}
}
int CountLiveNeighbors(int x, int y,const Cells& cells) {
	int count = 0;
	if (ReadCell(x - 1, y - 1, cells)) count++;
	if (ReadCell(x - 0, y - 1, cells)) count++;
	if (ReadCell(x + 1, y - 1, cells)) count++;
	if (ReadCell(x - 1, y - 0, cells)) count++;
	if (ReadCell(x + 1, y - 0, cells)) count++;
	if (ReadCell(x - 1, y + 1, cells)) count++;
	if (ReadCell(x - 0, y + 1, cells)) count++;
	if (ReadCell(x + 1, y + 1, cells)) count++;

	return count;
}

int main ()
{
	SetRandomSeed((unsigned int)time(NULL));
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(screenWidth, screenHeight, "Game of Life");
	SetTargetFPS(60);

	Cells current(gridWidth * gridHeight);
	Cells next(gridWidth * gridHeight);

	RandomizeCells(current);
	while (!WindowShouldClose()) {
		
		std::fill(next.begin(),next.end(),false);

		if (GetKeyPressed() == KEY_SPACE) {
			RandomizeCells(current);
		}

		BeginDrawing();
		ClearBackground(BLACK);

		for (int y = 0; y < gridHeight; y++) {
			for (int x = 0; x < gridWidth; x++) {
				int count = CountLiveNeighbors(x, y, current);
				bool isAlive = ReadCell(x, y, current);
				if (isAlive) {
					if (count >= 2 && count <= 3) {
						WriteCell(x, y, true, next);
					}
					else {
						WriteCell(x, y, false, next);
					}
				}
				else {
					if (count == 3) {
						WriteCell(x, y, true, next);
					}
					else {
						WriteCell(x, y, false, next);
					}
				}
			}
		}
		for (int y = 0; y < gridHeight; y++) {
			for (int x = 0; x < gridWidth; x++) {
				if (ReadCell(x, y, next)) {
					DrawRectangle(x * cellWidth, y * cellHeight,
						cellWidth, cellHeight, WHITE);
				}
				else {
					DrawRectangle(x * cellWidth, y * cellHeight,
						cellWidth, cellHeight, BLACK);
				}
			}
		}

		DrawText("SPACE: Randomizer", 10, 10, 20, RED);
		DrawFPS(10, 40);
		EndDrawing();

		current = next;
	}
	CloseWindow();
	return 0;
}
