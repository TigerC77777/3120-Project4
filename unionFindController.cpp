#include <cstdlib>
#include <ctime>
#include "unionFindModel.h"
#include "unionFindView.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static int width = 0;
static int height = 0;
// Union-Find vectors
static std::vector<int> mazeParents;
static std::vector<int> mazeRanks;
static std::vector<bool> mazeWalls;
// Breadth-First Search vectors
static std::vector<int> pathCells;
static std::vector<int> animPathCells;
static std::vector<std::vector<int>> adjList;

void initializeVectors(int width, int height)
{
	mazeParents.reserve(width * height);
	for (int i = 0; i < width * height; i++)
		mazeParents.push_back(i);
    mazeRanks.reserve(width * height);
    for (int r = 0; r < width * height; r++)
        mazeRanks.push_back(0);

    int wallAmt = (width - 1) * height + (height - 1) * width;
	mazeWalls.reserve(wallAmt);
	for (int j = 0; j < wallAmt; j++)
		mazeWalls.push_back(true);
    mazeWalls[0] = true;

    // Intentionally don't push anything here
    pathCells.reserve(width * height);
    animPathCells.reserve(width * height);
}

int main(int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        return SDL_APP_FAILURE;

    if (!SDL_CreateWindowAndRenderer("SDL3 Line Drawing", 900, 800, 0, &window, &renderer))
        return SDL_APP_FAILURE;

    if (argc < 3)
        return SDL_APP_FAILURE;

    width = atoi(argv[1]);
    height = atoi(argv[2]);
    initializeVectors(width, height);
    srand(time(NULL));

    SDL_Event event;
    bool running = true;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                return SDL_APP_SUCCESS;

            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_ESCAPE)
                    return SDL_APP_SUCCESS;
            }
        }

        // DRAWING

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        // Draw the outside box.
        RenderOutsideBox(renderer, width, height);
        // Draw the maze walls.
        RenderMazeWalls(renderer, width, height, mazeWalls);
        // Draw the path through the maze.
        RenderBFSPath(renderer, width, height, animPathCells);
        SDL_RenderPresent(renderer);

        // LOGIC

        // Remove walls until there is a path through the maze.
        if (find(0, mazeParents) != find(width * height - 1, mazeParents))
        {
            // Pick a random wall, if it exists, remove the wall and unite the cells it divides.
            int wallToRemove = rand() % mazeWalls.size();
            if (mazeWalls[wallToRemove])
            {
                mazeWalls[wallToRemove] = false;
                std::pair<int,int> cellsToUnite = findCellsByWall(wallToRemove, width, height);
                if (find(cellsToUnite.first, mazeParents) != find(cellsToUnite.second, mazeParents))
                    unite(cellsToUnite.first, cellsToUnite.second, mazeParents, mazeRanks);

                // 0.2 second delay between wall removals.
                SDL_Delay(200);
            }
        }
        else
        {
            // Construct the adjacency list, then perform breadth-first search through the constructed maze to find the shortest path.
            adjList.clear();
            adjList.resize(width * height);
            for (int a = 0; a < width * height; a++)
            {
                int row = a / width;
                int col = a % width;

                int verticalWall = findWallByCells(a, a + 1, width, height);
                if (col < width - 1 && verticalWall != -1 && !mazeWalls[verticalWall])
                {
                    adjList[a].push_back(a + 1);
                    adjList[a + 1].push_back(a);
                }
                int horizWall = findWallByCells(a, a + width, width, height);
                if (row < height - 1 && horizWall != -1 && !mazeWalls[horizWall])
                {
                    adjList[a].push_back(a + width);
                    adjList[a + width].push_back(a);
                }
            }
            breadthFirstSearch(adjList, pathCells, 0, width * height - 1);

            // Controls creating the animation of the pathfinding.
            if (animPathCells.size() != pathCells.size())
            {
                animPathCells.push_back(pathCells[animPathCells.size()]);
                // 0.2 second delay between path additions.
                SDL_Delay(200);
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

/*
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        return SDL_APP_FAILURE;

    if (!SDL_CreateWindowAndRenderer("SDL3 Line Drawing", 900, 800, 0, &window, &renderer))
        return SDL_APP_FAILURE;

    if (argc < 3)
        return SDL_APP_FAILURE;    

    width = atoi(argv[1]);
    height = atoi(argv[2]);

    //FindModel model;
    //FindView view;
    model.initializeVectors(width, height);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.key == SDLK_ESCAPE)
            return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Draw the outside box.
    view.RenderOutsideBox(renderer, width, height);
    view.RenderMazeWalls(renderer, width, height, model.mazeWalls);

    
    //for (int i = 0; i <= width; i++)
        //SDL_RenderLine(renderer, 100 + i * 20, 100, 100 + i * 20, 100 + height * 20);
    //for (int j = 0; j <= height; j++)
        //SDL_RenderLine(renderer, 100, 100 + j * 20, 100 + width * 20, 100 + j * 20);
    
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    // SDL will clean up the window/renderer for us.
}
*/

