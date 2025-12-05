#ifndef VIEW_H
#define VIEW_H

#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_timer.h>
#include <vector>

const int OFFSET = 100;
const int CELL_SIZE = 20;
const float PATH_BOX_SIZE = 12;

void RenderOutsideBox(SDL_Renderer *renderer, int width, int height);
void RenderMazeWalls(SDL_Renderer *renderer, int width, int height, std::vector<bool> mazeWalls);
void RenderBFSPath(SDL_Renderer *renderer, int width, int height, std::vector<int> pathCells);

#endif