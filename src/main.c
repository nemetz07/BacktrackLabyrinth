#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include "map.h"

int main(int argc, char *argv[]) {
    SDL_Event ev;
    int winHeight, winWidth;        //ablak magassága és szélessége

    colorMap=mapMake(MAPFILE, &mapHeight, &mapWidth);   //a színeket tároló tömb inicializálása(fájlból)

    calcWin(mapHeight,mapWidth, &winWidth, &winHeight); //kiszámolja mekkora ablakot kell nyitni

    wallMap=getSpace(mapHeight,mapWidth);                           //a labirintus elemeit alkotó négyzetek
    gridPos(wallMap, mapHeight, mapWidth, winHeight, winWidth);     //pozícióját tároló tömb incializálása

    int quit=0;     //kilépéshez szükséges változó
    int click=0;    //egérkattintáshoz szükséges változó
    int colorNum=1; //default mód: pályaszerkesztés
    int i,j;        //segédváltozók
    mouseRow=1;
    mouseCol=1;     //egér pozícióját tárolják

    SDL_Init(SDL_INIT_VIDEO);      //SDL inicializálása
    screen=SDL_SetVideoMode(winWidth+MENU, winHeight, 0, SDL_ANYFORMAT);    //ablak nyitás
    if (!screen) {
        fprintf(stderr, "Nem sikerult megnyitni az ablakot!\n");
        exit(1);
    }
    SDL_WM_SetCaption("Labirintus", "Labirintus");

    drawMenu(screen, winWidth);     //kirajzolja a menüt

    while(!quit){
        drawBorder(screen,colorMap,mapHeight,mapWidth);         //keret kirajzolása
        drawGrid(wallMap,colorMap,mapHeight,mapWidth,screen);   //ablak kirajzolása
        SDL_WaitEvent(&ev);         //várunk inputot a felhasználótól

        switch(ev.type){
            case SDL_QUIT:
                quit=1;
                break;
            case SDL_KEYDOWN:
                switch(ev.key.keysym.sym){
                    case SDLK_ESCAPE:       //kilépés
                        quit=1;
                        break;
                    case SDLK_F5:          //ablak törlése
                        clearGrid(colorMap,mapHeight,mapWidth);
                        break;
                    case SDLK_F8:       //labirintus manuális betöltése fájlból
                        colorMap=mapMake(MAPFILE, &mapHeight, &mapWidth);
                        break;
                    case SDLK_SPACE:        //backtrack algoritmus indítása
                        deletePath(colorMap);
                        if(!backtrack(mouseRow,mouseCol,colorMap)){
                            colorMap[mouseRow][mouseCol]=2;
                            stringColor(screen,(winWidth)/2,winHeight/2,"Nem tudtam eljutni a sajthoz!:(",0xFFFFFFFF);
                            SDL_Flip(screen);
                            SDL_Delay(2000);
                        }
                        colorMap[mouseRow][mouseCol]=2;     //egér bejelölése a labirintusban
                        drawGrid(wallMap,colorMap,mapHeight,mapWidth,screen);
                        break;
                    case SDLK_r:
                        deletePath(colorMap);
                        break;
                    case SDLK_DOWN:             //ablak növelése
                        if(mapHeight>=50)
                            break;
                        free(wallMap[0]);
                        free(wallMap);
                        saveToFile(colorMap,mapHeight,mapWidth);
                        addRow(mapWidth);
                        colorMap=mapMake(MAPFILE, &mapHeight, &mapWidth);
                        calcWin(mapHeight,mapWidth, &winWidth, &winHeight);
                        wallMap=getSpace(mapHeight,mapWidth);
                        gridPos(wallMap, mapHeight, mapWidth, winHeight, winWidth);
                        drawScreen(screen, winHeight, winWidth);
                        break;
                    case SDLK_UP:               //ablak csökkentése
                        if(mapHeight<=15 || winHeight<=225)
                            break;
                        free(wallMap[0]);
                        free(wallMap);
                        saveToFile(colorMap,mapHeight,mapWidth);
                        delRow(colorMap, mapHeight, mapWidth);
                        colorMap=mapMake(MAPFILE, &mapHeight, &mapWidth);
                        calcWin(mapHeight,mapWidth, &winWidth, &winHeight);
                        wallMap=getSpace(mapHeight,mapWidth);
                        gridPos(wallMap, mapHeight, mapWidth, winHeight, winWidth);
                        drawScreen(screen, winHeight, winWidth);
                        break;
                    case SDLK_RIGHT:            //ablak növelése
                        if(mapWidth>=50)
                            break;
                        free(wallMap[0]);
                        free(wallMap);
                        addColumn(colorMap,mapHeight,mapWidth);
                        colorMap=mapMake(MAPFILE, &mapHeight, &mapWidth);
                        calcWin(mapHeight,mapWidth, &winWidth, &winHeight);
                        wallMap=getSpace(mapHeight,mapWidth);
                        gridPos(wallMap, mapHeight, mapWidth, winHeight, winWidth);
                        drawScreen(screen, winHeight, winWidth);
                        break;
                    case SDLK_LEFT:             //ablak csökkentése
                        if(mapWidth<=10)
                            break;
                        free(wallMap[0]);
                        free(wallMap);
                        delColumn(colorMap,mapHeight,mapWidth);
                        colorMap=mapMake(MAPFILE, &mapHeight, &mapWidth);
                        calcWin(mapHeight,mapWidth, &winWidth, &winHeight);
                        wallMap=getSpace(mapHeight,mapWidth);
                        gridPos(wallMap, mapHeight, mapWidth, winHeight, winWidth);
                        drawScreen(screen, winHeight, winWidth);
                        break;
                    case SDLK_1:            //pályaszerkesztõ mód
                        colorNum=1;
                        break;
                    case SDLK_2:            //egér elhelyezése
                        colorNum=2;
                        break;
                    case SDLK_3:            //sajt elhelyezése
                        colorNum=3;
                        break;
                    case SDLK_F10:          //labirintus mentése
                        if(saveToFile(colorMap,mapHeight,mapWidth)){
                            stringColor(screen,2,122,"Saved!",0x4AB4FFFF);
                            SDL_Flip(screen);
                            SDL_Delay(1000);
                            stringColor(screen,2,122,"Saved!",0x000000FF);
                            SDL_Flip(screen);
                        }
                        else{
                            stringColor(screen,2,122,"Not Saved!",0x4AB4FFFF);
                            SDL_Flip(screen);
                            SDL_Delay(1000);
                            stringColor(screen,2,122,"Not Saved!",0x000000FF);
                            SDL_Flip(screen);
                        }
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:       //rajzolás a labirintusba kurzorral
                for(i=0;i<mapHeight;++i){
                    for(j=0;j<mapWidth;++j){
                        if(ev.button.x<wallMap[i][j].x2 && ev.button.x>wallMap[i][j].x1 && ev.button.y>wallMap[i][j].y1 && ev.button.y<wallMap[i][j].y2){
                            if(colorNum==2 && ev.button.button==SDL_BUTTON_LEFT){
                                delMouse(mapHeight,mapWidth,colorMap);
                                mouseRow=i;         //egér pozíciójának lementése változóba
                                mouseCol=j;
                            }
                            if(colorNum==3 && ev.button.button==SDL_BUTTON_LEFT)
                                delCheese(mapHeight,mapWidth,colorMap);
                            if(ev.button.button==SDL_BUTTON_RIGHT){     //jobb egérgombbal lehet törölni a labirintusból
                                click=2;
                                if(colorMap[i][j]!=2 && colorMap[i][j]!=3)
                                    colorMap[i][j]=0;
                            }
                            else{
                                click=1;
                                colorMap[i][j]=colorNum;
                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                click=0;
                break;
            case SDL_MOUSEMOTION:
                if(click){
                    for(i=0;i<mapHeight;++i){
                        for(j=0;j<mapWidth;++j){
                            if(ev.motion.x<wallMap[i][j].x2 && ev.motion.x>wallMap[i][j].x1 && ev.motion.y>wallMap[i][j].y1 && ev.motion.y<wallMap[i][j].y2){
                                if(click==2 && colorMap[i][j]!=2 && colorMap[i][j]!=3)
                                    colorMap[i][j]=0;
                                else
                                    colorMap[i][j]=colorNum;
                            }
                        }
                    }
                }
                break;
        }
    }

    SDL_Quit();

    return 0;
}
