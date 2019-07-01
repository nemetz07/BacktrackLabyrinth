#include <SDL.h>
#include <SDL_gfxPrimitives.h>

#include "map.h"

Wall** getSpace(int mapHeight, int mapWidth){       //Lefoglalja a paraméterül kapott méretû kétdimenziós tömböt
    int i;

    Wall **wallMap=(Wall**)malloc(mapHeight*sizeof(Wall*));
    wallMap[0]=(Wall*)malloc(mapHeight*mapWidth*sizeof(Wall));
    for(i=1;i<mapHeight;++i)
        wallMap[i]=wallMap[i-1]+mapWidth;

    return wallMap;
}

void deletePath(int** colorMap){        //kitörli az egér útvonalát (zöld)
    int j,i;

    for(i=0;i<mapHeight;++i){
        for(j=0;j<mapWidth;++j){
            if(colorMap[i][j]==4 || colorMap[i][j]==5)
                colorMap[i][j]=0;
        }
    }
}

void gridPos(Wall **wallMap, int mapHeight, int mapWidth, int winHeight, int winWidth){     //A pályát alkotó négyzetek helyének kiszámolása
    int i,j;

    for(i=0;i<mapHeight;++i){
        for(j=0;j<mapWidth;++j){
            wallMap[i][j].x1=j*(winWidth/mapWidth)+MENU;
            wallMap[i][j].y1=i*(winHeight/mapHeight);
            wallMap[i][j].x2=(j+1)*(winWidth/mapWidth)+MENU;
            wallMap[i][j].y2=(i+1)*(winHeight/mapHeight);
        }
    }
}

Uint32 pickColor(int numColor){             //Visszaadja a kívánt színt
    Uint32 color;

    switch(numColor){
        case 0: color=0x000000FF; break; //black
        case 1: color=0x888888FF; break; //gray
        case 2: color=0xFFFFFFFF; break; //white
        case 3: color=0xFFFF00FF; break; //yellow
        case 4: color=0x00FF00FF; break; //green
        case 5: color=0xFF0000FF; break; //red
    }

    return color;
}

void delMouse(int mapHeight, int mapWidth, int **colorMap){     //egér előző pozíciójának törlése
    int i,j;

    for(i=0;i<mapHeight;++i){
        for(j=0;j<mapWidth;++j){
            if(colorMap[i][j]==2)
                colorMap[i][j]=0;
        }
    }
}

void delCheese(int mapHeight, int mapWidth, int **colorMap){    //sajt előző pozíciójának törlése
    int i,j;

    for(i=0;i<mapHeight;++i){
        for(j=0;j<mapWidth;++j){
            if(colorMap[i][j]==3)
                colorMap[i][j]=0;
        }
    }
}

int saveToFile(int **colorMap, int mapHeight, int mapWidth){        //Elmenti a labirintust fájlba, szöveges formátumban
    FILE *mapFile=fopen(MAPFILE,"w");
    if(!mapFile){
        fprintf(stderr,"Nem sikerult megnyitni a fajlt!");
        return 0;
    }

    int i,j;

    for(i=0;i<mapHeight;++i){
        for(j=0;j<mapWidth;++j){
            fprintf(mapFile,"%d ",colorMap[i][j]);
        }
        if(i==mapHeight-1)
            break;
        fprintf(mapFile,"\n");
    }

    fclose(mapFile);
    return 1;
}

void drawGrid(Wall **wallMap, int **colorMap, int mapHeight, int mapWidth, SDL_Surface *screen){    //Kirajzolja a labirintust
    int i, j;
    Uint32 color;

    for(i=0;i<mapHeight;++i){
        for(j=0;j<mapWidth;++j){
            color=pickColor(colorMap[i][j]);
            boxColor(screen,wallMap[i][j].x1,wallMap[i][j].y1,wallMap[i][j].x2,wallMap[i][j].y2,color);
        }
    }
    SDL_Flip(screen);
}

void clearGrid(int **colorMap, int mapHeight, int mapWidth){        //Törli a labirintust
    int i,j;

    for(i=0;i<mapHeight;++i){
        for(j=0;j<mapWidth;++j){
            colorMap[i][j]=0;
        }
    }
}

void drawBorder(SDL_Surface *screen, int **colorMap, int mapHeight, int mapWidth){      //Megrajzolja a labirintus keretét
    int i,j;

    for(i=0;i<mapHeight;++i){
        for(j=0;j<mapWidth;++j){
            if(i==0 || j==0 || i==(mapHeight-1) || j==(mapWidth-1))
                colorMap[i][j]=1;
        }
    }
}

void drawMenu(SDL_Surface *screen, int winWidth){                //Megrajzolja a menüt
    stringColor(screen,2,2,"1 - Map Edit",0xFFFFFFFF);
    stringColor(screen,2,12,"2 - Mouse",0xFFFFFFFF);
    stringColor(screen,2,22,"3 - Cheese",0xFFFFFFFF);
    stringColor(screen,2,42,"RM - Delete",0xFFFFFFFF);
    stringColor(screen,2,62,"F5 - Clear All",0xFFFFFFFF);
    stringColor(screen,2,72,"F8 - Load",0xFFFFFFFF);
    stringColor(screen,2,82,"F10 - Save",0xFFFFFFFF);
    stringColor(screen,2,102,"Space - Start",0xFFFFFFFF);
    stringColor(screen,2,112,"R - Reset",0xFFFFFFFF);
}

void calcWin(int mapHeight,int mapWidth, int *winWidth, int *winHeight){        //Kiszámolja mekkora ablakot kell kirajzolni
    *winWidth=mapWidth*PIXELPERWALL;
    *winHeight=mapHeight*PIXELPERWALL;
}

void drawScreen(SDL_Surface *screen, int winHeight, int winWidth){          //Megnyitja az ablakot
    screen=SDL_SetVideoMode(winWidth+MENU,winHeight,0,SDL_ANYFORMAT);
    drawMenu(screen,winHeight);
}

void addRow(int mapWidth){          //Hozzáad egy sort a pályához
    int i;

    FILE *mapFile=fopen(MAPFILE,"r+");
    if(!mapFile){
        fprintf(stderr,"Nem sikerult megnyitni a fajlt!\n");
    }

    fseek(mapFile,-(mapWidth*2),SEEK_END);

    fprintf(mapFile,"9 ");

    for(i=1;i<mapWidth-1;++i)
        fprintf(mapFile,"0 ");

    fprintf(mapFile,"9 ");

    fprintf(mapFile,"\n");

    for(i=0;i<mapWidth;++i)
        fprintf(mapFile,"9 ");

    fclose(mapFile);
}

void delRow(int **colorMap, int mapHeight, int mapWidth){       //Elvesz egy sort
    int i,j;

    FILE *mapFile=fopen(MAPFILE,"w");
    if(!mapFile){
        fprintf(stderr,"Nem sikerult megnyitni a fajlt!\n");
    }

    for(i=0;i<mapHeight-2;++i){
        for(j=0;j<mapWidth;++j)
            fprintf(mapFile,"%d ",colorMap[i][j]);
        fprintf(mapFile,"\n");
    }

    for(i=0;i<mapWidth;++i)
        fprintf(mapFile,"9 ");

    fclose(mapFile);
}

void addColumn(int **colorMap, int mapHeight, int mapWidth){        //Hozzáad egy oszlopot
    int i,j;

    FILE *mapFile=fopen(MAPFILE,"w");
    if(!mapFile){
        fprintf(stderr,"Nem sikerult megnyitni a fajlt!\n");
    }

    for(i=0;i<mapWidth+1;++i)
        fprintf(mapFile,"9 ");

    fprintf(mapFile,"\n");

    for(i=1;i<mapHeight-1;++i){
        for(j=0;j<mapWidth-1;++j){
            fprintf(mapFile,"%d ",colorMap[i][j]);
        }
        fprintf(mapFile,"0 9 \n");
    }

    for(i=0;i<mapWidth+1;++i)
        fprintf(mapFile,"9 ");

    fclose(mapFile);
}

void delColumn(int **colorMap, int mapHeight, int mapWidth){        //Elvesz egy oszlopot
    int i,j;

    FILE *mapFile=fopen(MAPFILE,"w");
    if(!mapFile){
        fprintf(stderr,"Nem sikerult megnyitni a fajlt!\n");
    }

    for(i=0;i<mapWidth-1;++i)
        fprintf(mapFile,"9 ");

    fprintf(mapFile,"\n");

    for(i=1;i<mapHeight-1;++i){
        for(j=0;j<mapWidth-2;++j)
            fprintf(mapFile,"%d ",colorMap[i][j]);
        fprintf(mapFile,"9 \n");
    }

    for(i=0;i<mapWidth-1;++i)
        fprintf(mapFile,"9 ");

    fclose(mapFile);
}

