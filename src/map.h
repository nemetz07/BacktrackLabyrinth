#include <SDL.h>

#define MAP_H

#define PIXELPERWALL 15         //egy kocka oldalának nagysága pixelben
#define MAPFILE "map2.ini"       //a labirintus fájl neve
#define MENU 120                //menü szélessége pixelben

typedef struct Wall{
    int x1, y1;
    int x2, y2;
}Wall;

Wall **wallMap;
int **colorMap;
SDL_Surface *screen;
int mapHeight, mapWidth;
int mouseRow;
int mouseCol;

Wall** getSpace(int mapHeight, int mapWidth);

void deletePath(int** colorMap);

void gridPos(Wall **wallMap, int mapHeight, int mapWidth, int winHeight, int winWidth);

Uint32 pickColor(int numColor);

void delMouse(int mapHeight, int mapWidth, int **colorMap);

void delCheese(int mapHeight, int mapWidth, int **colorMap);

int saveToFile(int **colorMap, int mapHeight, int mapWidth);

int** mapMake(char* fileName, int *mapHeight, int *mapWidth);

void drawGrid(Wall **wallMap, int **colorMap, int mapHeight, int mapWidth, SDL_Surface *screen);

void clearGrid(int **colorMap, int mapHeight, int mapWidth);

void drawBorder(SDL_Surface *screen, int **colorMap, int mapHeight, int mapWidth);

void drawMenu(SDL_Surface *screen, int winWidth);

void calcWin(int mapHeight, int mapWidth, int *winWidth, int *winHeight);

void drawScreen(SDL_Surface *screen, int winHeight, int winWidth);

void addRow(int mapWidth);

void delRow(int **colorMap, int mapHeight, int mapWidth);

void addColumn(int **colorMap, int mapHeight, int mapWidth);

void delColumn(int **colorMap, int mapHeight, int mapWidth);
