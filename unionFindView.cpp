#include "unionFindView.h"

void RenderOutsideBox(SDL_Renderer *renderer, int width, int height)
{
    SDL_RenderLine(renderer, OFFSET, OFFSET, OFFSET + width * CELL_SIZE, OFFSET);
    SDL_RenderLine(renderer, OFFSET + width * CELL_SIZE, OFFSET, OFFSET + width * CELL_SIZE, OFFSET + (height - 1) * CELL_SIZE);
    SDL_RenderLine(renderer, OFFSET, OFFSET + CELL_SIZE, OFFSET, OFFSET + height * CELL_SIZE);
    SDL_RenderLine(renderer, OFFSET, OFFSET + height * CELL_SIZE, OFFSET + width * CELL_SIZE, OFFSET + height * CELL_SIZE);
}

void RenderMazeWalls(SDL_Renderer *renderer, int width, int height, std::vector<bool> mazeWalls)
{
    for (int i = 0; i < mazeWalls.size(); i++)
    {
        if (mazeWalls[i])
        {
            // Horizontal walls
            if (i >= (width - 1) * height)
            {
                int xPos = OFFSET + ((i - (width - 1) * height) % width) * CELL_SIZE;
                int yPos = OFFSET + (((i - (width - 1) * height) / width) + 1) * CELL_SIZE;
                SDL_RenderLine(renderer, xPos, yPos, xPos + CELL_SIZE, yPos);
            }
            // Vertical walls
            else
            {
                int xPos = OFFSET + (i % (width - 1) + 1) * CELL_SIZE;
                int yPos = OFFSET + (i / (width - 1)) * CELL_SIZE;
                SDL_RenderLine(renderer, xPos, yPos, xPos, yPos + CELL_SIZE);
            }
        }
    }
}

void RenderBFSPath(SDL_Renderer *renderer, int width, int height, std::vector<int> pathCells)
{
    if (pathCells.size() <= 0)
        return;

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 160);
    for (int p = 0; p < pathCells.size(); p++)
    {
        int cellNumber = pathCells[p];
        float pathBoxXOffset = OFFSET + (cellNumber % width) * CELL_SIZE + 4;
        float pathBoxYOffset = OFFSET + (cellNumber / width) * CELL_SIZE + 4;
        SDL_FRect cellBox = { pathBoxXOffset, pathBoxYOffset, PATH_BOX_SIZE, PATH_BOX_SIZE };
        SDL_RenderFillRect(renderer, &cellBox);
    }

    // Debug code which draws a box over every cell in the maze
    /*for (int p = 0; p < width * height; p++)
    {
        float pathBoxXOffset = OFFSET + (p % width) * CELL_SIZE + 2;
        float pathBoxYOffset = OFFSET + (p / width) * CELL_SIZE + 2;
        SDL_FRect cellBox = { pathBoxXOffset, pathBoxYOffset, PATH_BOX_SIZE, PATH_BOX_SIZE };
        SDL_RenderFillRect(renderer, &cellBox);
    }*/
}