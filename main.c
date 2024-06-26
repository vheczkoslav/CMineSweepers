#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <time.h>
#define sprite_count 12
#define ifdbg if(DEBUG == true)

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
                textures[11] = SDL_CreateTextureFromSurface(renderer, tmp);
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

void mouseHandle(int x, int y, SDL_Window* sw, int signal, bool* show) {
    int rx = x / TILE_SIZE, ry = y / TILE_SIZE;
    ifdbg printf("X: %d, Y: %d\n", rx, ry);
    if(!show){}
    else {
        if (signal == 0) { // L_CLICK
            if (g == BEFORE) {
                g++;
                SDL_SetWindowTitle(sw, rtStateMsg());
            }
            if (g == PLAY) {
                if (tiles[ry][rx].bomb) {
                    g++;
                    SDL_SetWindowTitle(sw, rtStateMsg());
                    showBombs();
                } else {
                    tiles[ry][rx].state = FOUND;
                    if (tiles[ry][rx].nei_bom == 0) showEmpty(rx, ry);
                }
            } else {
                g = 0;
                SDL_SetWindowTitle(sw, rtStateMsg());
                init_tiles();
            }
        } else { // R_CLICK
            if (g != BOMBED) {
                if (tiles[ry][rx].state != FLAG) {
                    tiles[ry][rx].state = FLAG;
                    if (tiles[ry][rx].bomb) bombsFound++;
                } else {
                    tiles[ry][rx].state = NOT_FOUND;
                    if (tiles[ry][rx].bomb) bombsFound--;
                }
            }
            if (bombsToFind == bombsFound) {
                g++;
                SDL_SetWindowTitle(sw, rtStateMsg());
            }
        }
    }
    ifdbg printf("BombsToFind: %d\nBombsFound: %d\n", bombsToFind, bombsFound);
}

/** Show all adjacent "empty" tiles when clicked. Trivial BFS used */
void showEmpty(int rx, int ry){
    tiles[ry][rx].state = FOUND;
    if(ry > 0 && tiles[ry - 1][rx].state == NOT_FOUND && tiles[ry - 1][rx].nei_bom == 0){ // T
        showEmpty(rx, ry - 1);
    }
    if(ry < TILE_ROWS - 1 && tiles[ry + 1][rx].state == NOT_FOUND && tiles[ry + 1][rx].nei_bom == 0){ // D
        showEmpty(rx, ry + 1);
    }
    if(rx > 0 && tiles[ry][rx - 1].state == NOT_FOUND && tiles[ry][rx - 1].nei_bom == 0){ // L
        showEmpty(rx - 1, ry);
    }
    if(rx < TILE_COLS - 1 && tiles[ry][rx + 1].state == NOT_FOUND && tiles[ry][rx + 1].nei_bom == 0){ // R
        showEmpty(rx + 1, ry);
    }
    if(rx > 0 && ry > 0 && tiles[ry - 1][rx - 1].state == NOT_FOUND && tiles[ry - 1][rx - 1].nei_bom == 0){ // TL
        showEmpty(rx - 1, ry - 1);
    }
    if(rx < TILE_COLS - 1 && ry > 0 && tiles[ry - 1][rx + 1].state == NOT_FOUND && tiles[ry - 1][rx + 1].nei_bom == 0){ // TR
        showEmpty(rx + 1, ry - 1);
    }
    if(rx > 0 && ry < TILE_ROWS - 1 && tiles[ry + 1][rx - 1].state == NOT_FOUND && tiles[ry + 1][rx - 1].nei_bom == 0){ // DL
        showEmpty(rx - 1, ry + 1);
    }
    if(rx < TILE_COLS - 1 && ry < TILE_ROWS - 1 && tiles[ry + 1][rx + 1].state == NOT_FOUND && tiles[ry + 1][rx + 1].nei_bom == 0){ // DR
        showEmpty(rx + 1, ry + 1);
    }
    // Second part of function is handling the border "empty tiles"
    if(ry > 0 && tiles[ry - 1][rx].state == NOT_FOUND){ // T
        tiles[ry - 1][rx].state = FOUND;
    }
    if(ry < TILE_ROWS - 1 && tiles[ry + 1][rx].state == NOT_FOUND){ // D
        tiles[ry + 1][rx].state = FOUND;
    }
    if(rx > 0 && tiles[ry][rx - 1].state == NOT_FOUND){ // L
        tiles[ry][rx - 1].state = FOUND;
    }
    if(rx < TILE_COLS - 1 && tiles[ry][rx + 1].state == NOT_FOUND){ // R
        tiles[ry][rx + 1].state = FOUND;
    }
    if(rx > 0 && ry > 0 && tiles[ry - 1][rx - 1].state == NOT_FOUND){ // TL
        tiles[ry - 1][rx - 1].state = FOUND;
    }
    if(rx < TILE_COLS - 1 && ry > 0 && tiles[ry - 1][rx + 1].state == NOT_FOUND){ // TR
        tiles[ry - 1][rx + 1].state = FOUND;
    }
    if(rx > 0 && ry < TILE_ROWS - 1 && tiles[ry + 1][rx - 1].state == NOT_FOUND){ // DL
        tiles[ry + 1][rx - 1].state = FOUND;
    }
    if(rx < TILE_COLS - 1 && ry < TILE_ROWS - 1 && tiles[ry + 1][rx + 1].state == NOT_FOUND){ // DR
        tiles[ry + 1][rx + 1].state = FOUND;
    }
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

    WIN_SIZE_X = TILE_COLS * TILE_SIZE;
    WIN_SIZE_Y = TILE_ROWS * TILE_SIZE;

    int window_x = (SDL_DM.w / 2) - (WIN_SIZE_X / 2);
    int window_y = (SDL_DM.h / 2) - (WIN_SIZE_Y / 2);

    return SDL_CreateWindow(rtStateMsg(), window_x, window_y, WIN_SIZE_X, WIN_SIZE_Y, SDL_WINDOW_SHOWN);
}

void init_tiles(){
    tiles = malloc(sizeof(Tile*) * TILE_ROWS);
    for(int i = 0; i < TILE_ROWS; i++){
        tiles[i] = malloc(sizeof(Tile) * TILE_COLS);
        for(int j = 0; j < TILE_COLS; j++){
            tiles[i][j].state = NOT_FOUND;
            tiles[i][j].x = j;
            tiles[i][j].y = i;
            tiles[i][j].bomb = false;
        }
    }
    fillBombs();
    for(int i = 0; i < TILE_ROWS; i++)
        for(int j = 0; j < TILE_COLS; j++)
            tiles[i][j].nei_bom = neighborBombs(j, i);
}

void fillBombs() {
    int tilesCount = 0; int bombCount = 0;
    if(GAME_SIZE == 1) tilesCount = 81, bombCount = 10, bombsToFind = bombCount;
    if(GAME_SIZE == 2) tilesCount = 256, bombCount = 40, bombsToFind = bombCount;
    if(GAME_SIZE == 3) tilesCount = 480, bombCount = 99, bombsToFind = bombCount;
    int* freeTiles = malloc(sizeof(int) * tilesCount); // 1D representation index representation of 2D index of Tile

    for(int i = 0; i < tilesCount; i++){
        freeTiles[i] = i;
    }
    /** filling random tiles with Bombs section */
    srand(time(0)); // generates new seed for random tile index
    for(int i = 0; i < bombCount; i++){
        int rand_index = rand() % (tilesCount), rand_tile = freeTiles[rand_index];
        int x = rand_tile % TILE_COLS;
        int y = rand_tile / TILE_COLS;
        if(tiles[y][x].bomb != true) tiles[y][x].bomb = true;
        if(rand_index == tilesCount - 1){} // To make the print start only from the index
        else{
            for(int j = rand_index; j < tilesCount - 1; j++) {
                //ifdbg printf("Before tiles[%d] = %d, tiles[%d] = %d\n", j, freeTiles[j], j + 1, freeTiles[j + 1]);
                freeTiles[j] = freeTiles[j + 1];
                //ifdbg printf("After tiles[%d] = %d, tiles[%d] = %d\n", j, freeTiles[j], j + 1, freeTiles[j + 1]);
            }
            //ifdbg printf("Cycle no: %d\n", i);
        }
        tilesCount--;
    }
    free(freeTiles);
}

void free_tiles(){
    for(int i = 0; i < TILE_ROWS; i++){
        free(tiles[i]);
    }
    free(tiles);
}

void render(SDL_Renderer* renderer, SDL_Texture** textures, bool* show){
    int x = 0, y = 0; // xlim, ylim !

    SDL_RenderClear(renderer);
    if(*show == true){ // DEBUG!
        for(int i = 0; i < TILE_ROWS; i++){
            for(int j = 0; j < TILE_COLS; j++){
                SDL_Rect temp;
                temp.x = j * TILE_SIZE, temp.y = i * TILE_SIZE;
                temp.w = TILE_SIZE, temp.h = TILE_SIZE;
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
        for(int i = 0; i < TILE_ROWS; i++){
            for(int j = 0; j < TILE_COLS; j++){
                SDL_Rect temp;
                temp.x = j * TILE_SIZE, temp.y = i * TILE_SIZE;
                temp.w = TILE_SIZE, temp.h = TILE_SIZE;
                if(tiles[i][j].state == NOT_FOUND) {
                    SDL_RenderCopy(renderer, textures[0], NULL, &temp);
                }
                else if(tiles[i][j].state == FOUND && !tiles[i][j].bomb){
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
                    SDL_RenderCopy(renderer, textures[1], NULL, &temp);
                }
                else if(tiles[i][j].state == FLAG){
                    SDL_RenderCopy(renderer, textures[2], NULL, &temp);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

short neighborBombs(int x, int y){
    int counter = 0;
    if (x > 0 && tiles[y][x - 1].bomb) counter++; // L
    if (x > 0 && y > 0 && tiles[y - 1][x - 1].bomb) counter++; // TL
    if (y > 0 && tiles[y - 1][x].bomb) counter++; // T
    if (x < TILE_COLS - 1 && y > 0 && tiles[y - 1][x + 1].bomb) counter++; // TR
    if (x < TILE_COLS - 1 && tiles[y][x + 1].bomb) counter++; // R
    if (x < TILE_COLS - 1 && y < TILE_ROWS - 1 && tiles[y + 1][x + 1].bomb) counter++; // BR
    if (y < TILE_ROWS - 1 && tiles[y + 1][x].bomb) counter++; // B
    if (x > 0 && y < TILE_ROWS- 1 && tiles[y + 1][x - 1].bomb) counter++; // BL
    return counter;
}

void showBombs() {
    for(int i = 0; i < TILE_ROWS; i++){
        for(int j = 0; j < TILE_COLS; j++){
            if(tiles[i][j].bomb) tiles[i][j].state = FOUND;
        }
    }
}

void print_tiles(){
    for(int i = 0; i < TILE_ROWS; i++){
        for(int j = 0; j < TILE_COLS; j++) {
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

void parseSettings(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char line[256];

    // Read and parse the TILE_SIZE line
    if (fgets(line, sizeof(line), file)) {
        char *equals_sign = strchr(line, '=');
        if (equals_sign != NULL) {
            int value = atoi(equals_sign + 1);
            if (value % 8 != 0) {
                printf("Incorrect tile size!\n");
                exit(-1);
            }
            TILE_SIZE = value;
            ifdbg printf("Parsed TILE_SIZE: %d\n", value);
        }
    }

    // Read and parse the GAME_SIZE line
    if (fgets(line, sizeof(line), file)) {
        char *equals_sign = strchr(line, '=');
        if (equals_sign != NULL) {
            int value = atoi(equals_sign + 1);
            if (value < 1 || value > 3) {
                printf("Incorrect game size!\n");
                exit(-1);
            }
            GAME_SIZE = value;
            ifdbg printf("Parsed GAME_SIZE: %d\n", value);
        }
    }

    // Read and parse the DEBUG line
    if (fgets(line, sizeof(line), file)) {
        char *equals_sign = strchr(line, '=');
        if (equals_sign != NULL) {
            int value = atoi(equals_sign + 1);
            if (value < 0 || value > 1) {
                printf("Invalid debug mode!\n");
                exit(-1);
            }
            DEBUG = value;
            ifdbg printf("Parsed DEBUG: %d\n", value);
        }
    }
    fclose(file);
}

int main(int argc, char** argv) {
    parseSettings("settings.txt");
    switch(GAME_SIZE){
        case 1:
            TILE_ROWS = 9;
            TILE_COLS = 9;
        break;
        case 2:
            TILE_ROWS = 16;
            TILE_COLS = 16;
        break;
        case 3:
            TILE_ROWS = 16;
            TILE_COLS = 30;
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
                    int mouseX = 0, mouseY = 0;
                    if(SDL_BUTTON_LEFT == evt.button.button){ // if (evt.button.type == ...)
                        SDL_GetMouseState(&mouseX, &mouseY);
                        mouseHandle(mouseX, mouseY, win, 0, &showAll);
                        render(rnd, sprites, &showAll);
                    }
                    if(SDL_BUTTON_RIGHT == evt.button.button){
                        SDL_GetMouseState(&mouseX, &mouseY);
                        mouseHandle(mouseX, mouseY, win, 1, &showAll);
                        render(rnd, sprites, &showAll);
                    }
                    break;
                case SDL_KEYDOWN:
                    if(evt.key.keysym.sym == SDLK_LSHIFT){
                        if(DEBUG){
                            showAll = !showAll;
                            if(showAll == true) printf("All showed!\n");
                            else printf("All hidden!\n");
                            render(rnd, sprites, &showAll);
                        }
                    }
                    if(evt.key.keysym.sym == SDLK_LCTRL){
                        print_tiles();
                    }
                    if(evt.key.keysym.sym == SDLK_F5){
                        init_tiles();
                        render(rnd, sprites, &showAll);
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
