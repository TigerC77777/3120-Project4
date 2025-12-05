#ifndef MODEL_H
#define MODEL_H

#include <SDL3/SDL.h>
#include <algorithm>
#include <queue>
#include <vector>

int find(int cell, std::vector<int> parents);
void unite(int i, int j, std::vector<int> &parents, std::vector<int> &ranks);
int findWallByCells(int a, int b, int width, int height);
std::pair<int, int> findCellsByWall(int w, int width, int height);
void breadthFirstSearch(std::vector<std::vector<int>> &adjList, std::vector<int> &pathCells, int start, int end);

#endif