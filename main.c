#include <stdio.h>
#include <stdlib.h>
#include "main.h"

void mouseHandle(int x, int y) {
    printf("X: %d, Y: %d\n", x, y);
    switch (GAME_SIZE) {
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
    }
}

char* rtGameState(){
    if(g == BEFORE){
        return "Minesweeper: click to start";
    }
    if(g == PLAY){
        return "Minesweeper: you are alive";
    }
    if(g == BOMBED){
        return "Minesweeper: press F5 to begin new game";
    }
}

SDL_Window* init_win(){
    int WIN_SIZE = 0;
    SDL_DisplayMode SDL_DM;
    SDL_GetCurrentDisplayMode(0, &SDL_DM);
    int monitor_width = SDL_DM.w;
    int monitor_height = SDL_DM.h;
    int window_x = (monitor_width - WIN_SIZE) / 2;
    int window_y = (monitor_height - WIN_SIZE) / 2;
    switch (GAME_SIZE) {
        case 1:
            WIN_SIZE = 216; // 9x9tiles x 24x24px
            break;
        case 2:
            WIN_SIZE = 384; // 16x16tiles x 24x24px
            break;
        case 3:
            WIN_SIZE = 720; // 30x30tiles x 24x24px
            break;
    }

    if(DEBUG == false){
        return SDL_CreateWindow(rtGameState(), window_x - WIN_SIZE / 2, window_y - WIN_SIZE / 2, WIN_SIZE, WIN_SIZE, SDL_WINDOW_SHOWN);
    }
    else{
        return SDL_CreateWindow("DEBUG ENABLED", window_x - WIN_SIZE / 2, window_y - WIN_SIZE / 2, WIN_SIZE, WIN_SIZE, SDL_WINDOW_SHOWN);
    }
}

void init_tiles(){
    switch(GAME_SIZE){
        case 1:
            tiles = malloc(sizeof(Tile*) * 9);
            for(int i = 0; i < 9; i++){
                tiles[i] = malloc(sizeof(Tile) * 9);
                for(int j = 0; j < 9; j++){
                    tiles[i][j].found = false;
                    tiles[i][j].x = j;
                    tiles[i][j].y = i;
                    tiles[i][j].bomb = false;
                    tiles[i][j].show = false;
                }
            }
            break;
        case 2:
            tiles = malloc(sizeof(Tile*) * 16);
            for(int i = 0; i < 16; i++){
                tiles[i] = malloc(sizeof(Tile) * 16);
                for(int j = 0; j < 16; j++){
                    tiles[i][j].found = false;
                    tiles[i][j].x = j;
                    tiles[i][j].y = i;
                    tiles[i][j].bomb = false;
                    tiles[i][j].show = false;
                }
            }
            break;
        case 3:
            tiles = malloc(sizeof(Tile*) * 16);
            for(int i = 0; i < 16; i++){
                tiles[i] = malloc(sizeof(Tile) * 30);
                for(int j = 0; j < 30; j++){
                    tiles[i][j].found = false;
                    tiles[i][j].x = j;
                    tiles[i][j].y = i;
                    tiles[i][j].bomb = false;
                    tiles[i][j].show = false;
                }
            }
            break;
    }
    fillBombs();
}

void fillBombs() {
    int tilesCount = 0;
    int bombCount = 0;
    if(GAME_SIZE == 1) tilesCount = 81; bombCount = (int)tilesCount * 0.123;
    if(GAME_SIZE == 2) tilesCount = 256; bombCount = (int)tilesCount * 0.156;
    if(GAME_SIZE == 3) tilesCount = 480; bombCount = (int)tilesCount * 0.206;
    int* freeTiles = malloc(sizeof(int) * tilesCount); // 1D representation index representation of 2D index of Tile

    for(int i = 0; i < tilesCount; i++){
        freeTiles[i] = i;
    }
    // filling random tiles with Bombs section
    for(int i = 0; i < bombCount; i++){
        int rand_index = rand() % tilesCount, rand_tile = freeTiles[rand_index];
        int y, x, xlim, ylim; // y = row, x = column
        if(GAME_SIZE == 1){
            xlim = 9; ylim = 9;
            x = rand_tile % xlim;
            y = rand_tile / xlim;
        }
        if(GAME_SIZE == 2){
            xlim = 16; ylim = 16;
            x = rand_tile % xlim;
            y = rand_tile / xlim;
        }
        if(GAME_SIZE == 3){
            xlim = 30; ylim = 16;
            x = rand_tile % ylim;
            y = rand_tile / xlim;
        }
        tiles[y][x].bomb = true;

        for(int j = rand_index; j < tilesCount - 1; j++) freeTiles[j] = freeTiles[j + 1];
        tilesCount--;
        freeTiles = realloc(freeTiles, sizeof(int) * tilesCount);
    }
    free(freeTiles);
}

void free_tiles(){
    switch(GAME_SIZE){
        case 1:
            for(int i = 0; i < 9; i++){
                free(tiles[i]);
            }
            break;
        case 2:
            for(int i = 0; i < 16; i++){
                free(tiles[i]);
            }
            break;
        case 3:
            for(int i = 0; i < 16; i++){
                free(tiles[i]);
            }
            break;
    }
    free(tiles);
}

int main(int argc, char** argv) {
    if(argc < 2 || argc > 3){
        printf("Incorrect amount of parameters! Look into README.md...\n");
        return -1;
    }
    else{
        GAME_SIZE = atoi(argv[1]);
        if(GAME_SIZE < 1 || GAME_SIZE > 3){
            printf("Incorrect map size/difficulty!\n");
            return -1;
        }
        if(argc == 3){
            if(strcmp("-d", argv[2]) == 0 || strcmp("--debug", argv[2]) == 0){
                DEBUG = 1;
            }
            else{
                printf("Incorrect voluntary parameter!\n");
                return -1;
            }
        }
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win;
    SDL_Renderer* rnd;
    SDL_Event evt;

    if(DEBUG == 1) printf("Debug on\n");
    win = init_win();
    rnd = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    init_tiles();
    int running = 1;
    bool firstRender = false;
    while(running){
        //if(firstRender == false) render(); firstRender = !firstRender;
        while(SDL_PollEvent( &evt ) != 0){
            switch(evt.type){
                case SDL_QUIT:
                    running = 0;
                    break;
                // Render will only happen after a click; There is no need to repeatedly re-draw everything again;
                case SDL_MOUSEBUTTONDOWN:
                    if(SDL_BUTTON_LEFT == evt.button.button){ // if (evt.button.type == ...)
                        int mouseX = 0, mouseY = 0;
                        SDL_GetMouseState(&mouseX, &mouseY);
                        mouseHandle(mouseX, mouseY);
                    }
                    break;
            }
        }
    }

    SDL_Quit();
    free_tiles();

    free(win);
    free(rnd);
    return 0;
}
