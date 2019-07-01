#include "map.h"

int** mapMake(char* fileName, int *mapHeight, int *mapWidth){       //Fájlból beolvassa a pályát
    FILE *mapFile;
    mapFile=fopen(fileName,"r");
    if(!mapFile){
        fprintf(stderr,"Nem sikerult megnyitni a palyat!");
        exit(1);
    }

    int wallY=1;
    int wallX=0;
    char temp[400+1];
    char s[2]=" ";
    char *token;
    int i,j;

    fgets(temp,400,mapFile);

    token=strtok(temp,s);

    while(token!=NULL){
        ++wallX;
        token=strtok(NULL,s);
    }

    while(fgets(temp,400,mapFile))
        ++wallY;

    int **worldMap=(int**)malloc(wallY*sizeof(int*));
    worldMap[0]=(int*)malloc(wallY*wallX*sizeof(int));
    for(i=1;i<wallY;++i)
        worldMap[i]=worldMap[i-1]+wallX;

    fseek(mapFile,0,SEEK_SET);

    i=0;

    while(fgets(temp,400,mapFile)){
        j=0;
        token=strtok(temp,s);
        while(token!=NULL && j<wallX){
            sscanf(token,"%d",&(worldMap[i][j]));
            token=strtok(NULL,s);
            ++j;
        }
        ++i;
    }

    fclose(mapFile);

    *mapHeight=wallY;
    *mapWidth=wallX-1;

    return worldMap;
}
