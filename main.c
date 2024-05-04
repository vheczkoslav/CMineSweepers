#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#define sprite_count 12
#define tile_size 16

int g = BEFORE;

SDL_Texture** load_textures(int len, SDL_Renderer* renderer) {
    SDL_Texture** textures = malloc(len * sizeof(SDL_Texture*));
    SDL_Surface *tmp;
    for(int i = 0; i < len; i++){
        switch(i){
            case 0:
                tmp = IMG_Load("sprites/hidden.png");
                textures[0] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
            case 1:
                tmp = IMG_Load("sprites/bomb.png");
                textures[1] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
            case 2:
                tmp = IMG_Load("sprites/flag.png");
                textures[2] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
            case 3:
                tmp = IMG_Load("sprites/one.png");
                textures[3] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
            case 4:
                tmp = IMG_Load("sprites/two.png");
                textures[4] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
            case 5:
                tmp = IMG_Load("sprites/three.png");
                textures[5] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
            case 6:
                tmp = IMG_Load("sprites/four.png");
                textures[6] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
            case 7:
                tmp = IMG_Load("sprites/five.png");
                textures[7] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
            case 8:
                tmp = IMG_Load("sprites/six.png");
                textures[8] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
            case 9:
                tmp = IMG_Load("sprites/seven.png");
                textures[9] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
            case 10:
                tmp = IMG_Load("sprites/eight.png");
                textures[10] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
            case 11:
                tmp = IMG_Load("sprites/found.png");
                textures[10] = SDL_CreateTextureFromSurface(renderer, tmp);
                if( tmp == NULL )
                {
                    printf( "Unable to load image[%d]\n", i);
                    exit(-1);
                }
                break;
        }
    }
    free(tmp);
    return textures;
}

void mouseHandle(int x, int y, SDL_Window* sw, int signal) {
    int rx = x / 16, ry = y / 16;
    /*if(signal == 0){
        if(g == BEFORE){
            g++;
            SDL_SetWindowTitle(sw, rtStateMsg());
        }
        if(g == PLAY){
            if(tiles[y][x].bomb){
                g++;
                SDL_SetWindowTitle(sw, rtStateMsg());
            }
            else{

            }
        }
        else{
            showBombs();
        }
    }
    else{

    }*/
}

char* rtStateMsg(){
    if(!DEBUG){
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
    else{
        return "DEBUG ENABLED";
    }
}

SDL_Window* init_win(){
    int WIN_SIZE_X, WIN_SIZE_Y = 0;
    SDL_DisplayMode SDL_DM;
    SDL_GetCurrentDisplayMode(0, &SDL_DM);
    switch (GAME_SIZE) {
        case 1:
            // 9x9tiles x 16x16px
            WIN_SIZE_X = 144;
            WIN_SIZE_Y = 144;
            break;
        case 2:
            // 16x16tiles x 16x16px
            WIN_SIZE_X = 256;
            WIN_SIZE_Y = 256;
            break;
        case 3:
            // 16x30tiles x 16x16px
            WIN_SIZE_X = 480;
            WIN_SIZE_Y = 256;
            break;
    }
    int window_x = (SDL_DM.w / 2) - (WIN_SIZE_X / 2);
    int window_y = (SDL_DM.h / 2) - (WIN_SIZE_Y / 2);

    return SDL_CreateWindow(rtStateMsg(), window_x, window_y, WIN_SIZE_X, WIN_SIZE_Y, SDL_WINDOW_SHOWN);
}

void init_tiles(){
    switch(GAME_SIZE){
        case 1:
            tiles = malloc(sizeof(Tile*) * 9);
            for(int i = 0; i < 9; i++){
                tiles[i] = malloc(sizeof(Tile) * 9);
                for(int j = 0; j < 9; j++){
                    tiles[i][j].state = NOT_FOUND;
                    tiles[i][j].x = j;
                    tiles[i][j].y = i;
                    tiles[i][j].bomb = false;
                }
            }
            break;
        case 2:
            tiles = malloc(sizeof(Tile*) * 16);
            for(int i = 0; i < 16; i++){
                tiles[i] = malloc(sizeof(Tile) * 16);
                for(int j = 0; j < 16; j++){
                    tiles[i][j].state = NOT_FOUND;
                    tiles[i][j].x = j;
                    tiles[i][j].y = i;
                    tiles[i][j].bomb = false;
                }
            }
            break;
        case 3:
            tiles = malloc(sizeof(Tile*) * 16);
            for(int i = 0; i < 16; i++){ // i is y (1st dim)
                tiles[i] = malloc(sizeof(Tile) * 30);
                for(int j = 0; j < 30; j++){ // j is x (2nd dim)
                    tiles[i][j].state = NOT_FOUND;
                    tiles[i][j].x = j;
                    tiles[i][j].y = i;
                    tiles[i][j].bomb = false;
                }
            }
            break;
    }
    fillBombs();
    switch(GAME_SIZE){
        case 1:
            for(int i = 0; i < 9; i++)
                for(int j = 0; j < 9; j++)
                    tiles[i][j].nei_bom = neighborBombs(j, i);
            break;
        case 2:
            for(int i = 0; i < 16; i++)
                for(int j = 0; j < 16; j++)
                    tiles[i][j].nei_bom = neighborBombs(j, i);
            break;
        case 3:
            for(int i = 0; i < 16; i++)
                for(int j = 0; j < 30; j++)
                    tiles[i][j].nei_bom = neighborBombs(j, i);
            break;
    }
}

void fillBombs() {
    int tilesCount = 0;
    int bombCount = 0;
    if(GAME_SIZE == 1) tilesCount = 81; bombCount = (int)tilesCount * 0.123; bombsToFind = bombCount;
    if(GAME_SIZE == 2) tilesCount = 256; bombCount = (int)tilesCount * 0.156; bombsToFind = bombCount;
    if(GAME_SIZE == 3) tilesCount = 480; bombCount = (int)tilesCount * 0.206; bombsToFind = bombCount;
    int* freeTiles = malloc(sizeof(int) * tilesCount); // 1D representation index representation of 2D index of Tile

    for(int i = 0; i < tilesCount; i++){
        freeTiles[i] = i;
    }
    // filling random tiles with Bombs section
    for(int i = 0; i < bombCount; i++){
        int rand_index = rand() % tilesCount, rand_tile = freeTiles[rand_index];
        int y = 0, x = 0, xlim = 0, ylim = 0; // y = row, x = column
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
        freeTiles = realloc(freeTiles, sizeof(int) * --tilesCount);
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
        case 3:
            for(int i = 0; i < 16; i++){
                free(tiles[i]);
            }
            break;
    }
    free(tiles);
}

void render(SDL_Renderer* renderer, SDL_Texture** textures, bool* show){
    int x = 0, y = 0;
    if(GAME_SIZE == 1) x = 9, y = 9;
    if(GAME_SIZE == 2) x = 16, y = 16;
    if(GAME_SIZE == 3) x = 30, y = 16;

    SDL_RenderClear(renderer);

    if(*show){
        for(int i = 0; i < y; i++){
            for(int j = 0; j < x; j++){
                SDL_Rect temp;
                temp.x = j * tile_size, temp.y = i * tile_size;
                temp.w = 16, temp.h = 16;
                if(!tiles[i][j].bomb){
                    switch(tiles[i][j].nei_bom){
                        case 0:
                            SDL_RenderCopy(renderer, textures[11], NULL, &temp);
                            break;
                        case 1:
                            SDL_RenderCopy(renderer, textures[3], NULL, &temp);
                            break;
                        case 2:
                            SDL_RenderCopy(renderer, textures[4], NULL, &temp);
                            break;
                        case 3:
                            SDL_RenderCopy(renderer, textures[5], NULL, &temp);
                            break;
                        case 4:
                            SDL_RenderCopy(renderer, textures[6], NULL, &temp);
                            break;
                        case 5:
                            SDL_RenderCopy(renderer, textures[7], NULL, &temp);
                            break;
                        case 6:
                            SDL_RenderCopy(renderer, textures[8], NULL, &temp);
                            break;
                        case 7:
                            SDL_RenderCopy(renderer, textures[9], NULL, &temp);
                            break;
                        case 8:
                            SDL_RenderCopy(renderer, textures[10], NULL, &temp);
                            break;
                    }
                }
                else{
                    SDL_RenderCopy(renderer, textures[1], NULL, &temp);
                }
            }
        }
    }
    else{
        for(int i = 0; i < y; i++){
            for(int j = 0; j < x; j++){
                if(tiles[i][j].state == NOT_FOUND) {
                    SDL_Rect temp;
                    temp.x = j * tile_size, temp.y = i * tile_size;
                    temp.w = 16, temp.h = 16;
                    SDL_RenderCopy(renderer, textures[0], NULL, &temp);
                }
                else if(tiles[i][j].state == FOUND && !tiles[i][j].bomb){
                    SDL_Rect temp;
                    temp.x = j * tile_size, temp.y = i * tile_size;
                    temp.w = 16, temp.h = 16;
                    switch(tiles[i][j].nei_bom){
                        case 0:
                            SDL_RenderCopy(renderer, textures[11], NULL, &temp);
                            break;
                        case 1:
                            SDL_RenderCopy(renderer, textures[3], NULL, &temp);
                            break;
                        case 2:
                            SDL_RenderCopy(renderer, textures[4], NULL, &temp);
                            break;
                        case 3:
                            SDL_RenderCopy(renderer, textures[5], NULL, &temp);
                            break;
                        case 4:
                            SDL_RenderCopy(renderer, textures[6], NULL, &temp);
                            break;
                        case 5:
                            SDL_RenderCopy(renderer, textures[7], NULL, &temp);
                            break;
                        case 6:
                            SDL_RenderCopy(renderer, textures[8], NULL, &temp);
                            break;
                        case 7:
                            SDL_RenderCopy(renderer, textures[9], NULL, &temp);
                            break;
                        case 8:
                            SDL_RenderCopy(renderer, textures[10], NULL, &temp);
                            break;
                    }
                }
                else if(tiles[i][j].state == FOUND && tiles[i][j].bomb){
                    SDL_Rect temp;
                    temp.x = j * tile_size, temp.y = i * tile_size;
                    temp.w = 16, temp.h = 16;
                    SDL_RenderCopy(renderer, textures[1], NULL, &temp);
                }
                else if(tiles[i][j].state == FLAG){
                    SDL_Rect temp;
                    temp.x = j * tile_size, temp.y = i * tile_size;
                    temp.w = 16, temp.h = 16;
                    SDL_RenderCopy(renderer, textures[2], NULL, &temp);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
}

short neighborBombs(int x, int y){
    int counter = 0;
    int xlim = 0, ylim = 0;
    switch(GAME_SIZE){
        case 1:
            xlim = 9, ylim = 9;
            break;
        case 2:
            xlim = 16, ylim = 16;
            break;
        case 3:
            xlim = 30, ylim = 16;
    }

    if (x > 0 && tiles[y][x - 1].bomb) counter++; // L
    if (x > 0 && y > 0 && tiles[y - 1][x - 1].bomb) counter++; // TL
    if (y > 0 && tiles[y - 1][x].bomb) counter++; // T
    if (x < xlim - 1 && y > 0 && tiles[y - 1][x + 1].bomb) counter++; // TR
    if (x < xlim - 1 && tiles[y][x + 1].bomb) counter++; // R
    if (x < xlim - 1 && y < ylim - 1 && tiles[y + 1][x + 1].bomb) counter++; // BR
    if (y < ylim - 1 && tiles[y + 1][x].bomb) counter++; // B
    if (x > 0 && y < ylim - 1 && tiles[y + 1][x - 1].bomb) counter++; // BL
    return counter;
}

void showBombs() {
    int xlim, ylim;
    if (GAME_SIZE == 1) xlim = 9, ylim = 9;
    if (GAME_SIZE == 2) xlim = 16, ylim = 16;
    else xlim = 30, ylim = 16;
    for(int i = 0; i < ylim; i++){
        for(int j = 0; j < xlim; j++){
            if(tiles[i][j].bomb) tiles[i][j].state = FOUND;
        }
    }
}

void print_tiles(){
    int xlim = 0, ylim = 0;
    if(GAME_SIZE == 1){
        xlim = 9, ylim = 9;
    }
    if(GAME_SIZE == 2){
        xlim = 16, ylim = 16;
    }
    if(GAME_SIZE == 3){
        xlim = 30, ylim = 16;
    }
    for(int i = 0; i < ylim; i++){
        for(int j = 0; j < xlim; j++) {
            if(!tiles[i][j].bomb){
                printf("%hi", tiles[i][j].nei_bom);
            }
            else{
                printf("B");
            }
        }
        printf("\n");
    }
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

    SDL_Texture** sprites = load_textures(sprite_count, rnd);

    int running = 1;

    init_tiles();
    bool showAll = false;
    render(rnd, sprites, &showAll);
    while(running){
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
                        mouseHandle(mouseX, mouseY, win, 0);
                        render(rnd, sprites, &showAll);
                    }
                    if(SDL_BUTTON_RIGHT == evt.button.button){
                        int mouseX = 0, mouseY = 0;
                        SDL_GetMouseState(&mouseX, &mouseY);
                        mouseHandle(mouseX, mouseY, win, 1);
                    }
                    break;
                case SDL_KEYDOWN:
                    if(evt.key.keysym.sym == SDLK_LSHIFT){
                        if(DEBUG){
                            showAll = !showAll;
                            render(rnd, sprites, &showAll);
                        }
                    }
                    if(evt.key.keysym.sym == SDLK_LCTRL){
                        print_tiles();
                    }
            }
        }
    }

    SDL_Quit();
    free_tiles();

    for(int i = 0; i < sprite_count; i++) SDL_DestroyTexture(sprites[i]);
    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(rnd);
    return 0;
}
