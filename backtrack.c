#include "map.h"

/*A visszalépéses algoritmus implementációja (rekurzióval)*/
int backtrack(int row, int col, int** colorMap){

    /*Lépésenkénti követés*/

    drawGrid(wallMap,colorMap,mapHeight,mapWidth,screen);     //pálya kirajzolása
    SDL_Delay(25);                                           //lépések közötti késleltetés


    /*Leállási feltétel: ha a jelenlegi pozíció mellett van a sajt, megérkeztünk*/
    if(colorMap[row+1][col]==3 || colorMap[row-1][col]==3 || colorMap[row][col+1]==3 || colorMap[row][col-1]==3){
        colorMap[row][col]=4;
        return 1;
    }

    if(colorMap[row-1][col]==0){    //Ha léphetünk felfele
        colorMap[row][col]=4;       //megjelöljük helyes vonalnak
        if(backtrack(row-1,col,colorMap))   //ha megtaláltuk a sajtot ezen az útvonalon visszatérünk 1-gyel
            return 1;
        else{
            colorMap[row][col]=0;    //visszaállítjuk üresre
            colorMap[row-1][col]=5;  //megjelöljük rossz útvonalnak
        }
    }
    if(colorMap[row][col+1]==0){    //Ha léphetünk jobbra
        colorMap[row][col]=4;
        if(backtrack(row,col+1,colorMap))
            return 1;
        else{
            colorMap[row][col]=0;
            colorMap[row][col+1]=5;
        }
    }
    if(colorMap[row+1][col]==0){    //Ha léphetünk lefele
        colorMap[row][col]=4;
        if(backtrack(row+1,col,colorMap))
            return 1;
        else{
            colorMap[row][col]=0;
            colorMap[row+1][col]=5;
        }
    }
    if(colorMap[row][col-1]==0){    //Ha léphetünk balra
        colorMap[row][col]=4;
        if(backtrack(row,col-1,colorMap))
            return 1;
        else{
            colorMap[row][col]=0;
            colorMap[row][col-1]=5;
        }
    }

    return 0;
}
