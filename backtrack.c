#include "map.h"

/*A visszal�p�ses algoritmus implement�ci�ja (rekurzi�val)*/
int backtrack(int row, int col, int** colorMap){

    /*L�p�senk�nti k�vet�s*/

    drawGrid(wallMap,colorMap,mapHeight,mapWidth,screen);     //p�lya kirajzol�sa
    SDL_Delay(25);                                           //l�p�sek k�z�tti k�sleltet�s


    /*Le�ll�si felt�tel: ha a jelenlegi poz�ci� mellett van a sajt, meg�rkezt�nk*/
    if(colorMap[row+1][col]==3 || colorMap[row-1][col]==3 || colorMap[row][col+1]==3 || colorMap[row][col-1]==3){
        colorMap[row][col]=4;
        return 1;
    }

    if(colorMap[row-1][col]==0){    //Ha l�phet�nk felfele
        colorMap[row][col]=4;       //megjel�lj�k helyes vonalnak
        if(backtrack(row-1,col,colorMap))   //ha megtal�ltuk a sajtot ezen az �tvonalon visszat�r�nk 1-gyel
            return 1;
        else{
            colorMap[row][col]=0;    //vissza�ll�tjuk �resre
            colorMap[row-1][col]=5;  //megjel�lj�k rossz �tvonalnak
        }
    }
    if(colorMap[row][col+1]==0){    //Ha l�phet�nk jobbra
        colorMap[row][col]=4;
        if(backtrack(row,col+1,colorMap))
            return 1;
        else{
            colorMap[row][col]=0;
            colorMap[row][col+1]=5;
        }
    }
    if(colorMap[row+1][col]==0){    //Ha l�phet�nk lefele
        colorMap[row][col]=4;
        if(backtrack(row+1,col,colorMap))
            return 1;
        else{
            colorMap[row][col]=0;
            colorMap[row+1][col]=5;
        }
    }
    if(colorMap[row][col-1]==0){    //Ha l�phet�nk balra
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
