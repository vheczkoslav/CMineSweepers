#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include <time.h>
#include <malloc.h>
#define sprite_count 30
#define ifdbg if(DEBUG == true)


int g = BEFORE;

SDL_Texture** load_textures(int len, SDL_Renderer* renderer) {
    SDL_Texture** textures = malloc(len * sizeof(SDL_Texture*));
    SDL_Surface *tmp;
    for(int i = 0; i < len; i++){
        if(i < 26){
            char ind[2];
            sprintf(ind, "%d", i + 1);
            char path[64] = "sprites/";
            strcat(path, ind);
            char ending[16] = ".png";
            strcat(path, ending);
            tmp = IMG_Load(path);
            if(tmp == NULL){
                printf("Unable to load image[%d]: %s\n", i, path);
                exit(-1);
            }
            textures[i] = SDL_CreateTextureFromSurface(renderer, tmp);
            continue;
        }
        else{
            switch(i){
                case 26:
                    tmp = IMG_Load("sprites/bomb.png");
                    if(tmp == NULL){
                        printf("Unable to load image[%d]\n", i);
                        exit(-1);
                    }
                    textures[i] = SDL_CreateTextureFromSurface(renderer, tmp);
                    continue;
                case 27:
                    tmp = IMG_Load("sprites/flag.png");
                    if(tmp == NULL){
                        printf("Unable to load image[%d]\n", i);
                        exit(-1);
                    }
                    textures[i] = SDL_CreateTextureFromSurface(renderer, tmp);
                    continue;
                case 28:
                    tmp = IMG_Load("sprites/0.png");
                    if(tmp == NULL){
                        printf("Unable to load image[%d]\n", i);
                        exit(-1);
                    }
                    continue;
                case 29:
                    tmp = IMG_Load("sprites/hidden.png");
                    if(tmp == NULL){
                        printf("Unable to load image[%d]\n", i);
                        exit(-1);
                    }
                    textures[i] = SDL_CreateTextureFromSurface(renderer, tmp);
                    continue;
            }
        }
    }
    free(tmp);
    return textures;
}

void mouseHandle(pos p, SDL_Window* sw, int signal, bool* show) {
    int rx = p.x / TILE_SIZE, ry = p.y / TILE_SIZE, rz = p.z;
    pos rp = {rx, ry, rz};
    ifdbg printf("X: %d, Y: %d\n", rx, ry);
    if(!show){}
    else {
        if (signal == 0) { // L_CLICK
            if (g == BEFORE) {
                g++;
                SDL_SetWindowTitle(sw, rtStateMsg());
            }
            if (g == PLAY) {
                if (tiles[rz][ry][rx].bomb) {
                    g++;
                    SDL_SetWindowTitle(sw, rtStateMsg());
                    showBombs();
                } else {
                    tiles[rz][ry][rx].state = FOUND;
                    if (tiles[rz][ry][rx].nei_bom == 0) showEmpty(rp);
                }
            } else {
                g = 0;
                SDL_SetWindowTitle(sw, rtStateMsg());
                init_tiles();
            }
        } else { // R_CLICK
            if (g != BOMBED) {
                if (tiles[rz][ry][rx].state != FLAG) {
                    tiles[rz][ry][rx].state = FLAG;
                    if (tiles[rz][ry][rx].bomb) bombsFound++;
                } else {
                    tiles[rz][ry][rx].state = NOT_FOUND;
                    if (tiles[rz][ry][rx].bomb) bombsFound--;
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
void showEmpty(pos rp) {
    int x = rp.x;
    int y = rp.y;
    int z = rp.z;

    tiles[z][y][x].state = FOUND;

    // Check all 26 neighbors in 3D space
    for (int dz = -1; dz <= 1; dz++) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dz == 0 && dy == 0 && dx == 0) continue; // Skip the current tile

                int nz = z + dz;
                int ny = y + dy;
                int nx = x + dx;

                // Bounds checking
                if (nz >= 0 && nz < TILE_LEVELS && ny >= 0 && ny < TILE_ROWS && nx >= 0 && nx < TILE_COLS) {
                    if (tiles[nz][ny][nx].state == NOT_FOUND && tiles[nz][ny][nx].nei_bom == 0) {
                        showEmpty((pos){.x = nx, .y = ny, .z = nz});
                    } else if (tiles[nz][ny][nx].state == NOT_FOUND) {
                        tiles[nz][ny][nx].state = FOUND;
                    }
                }
            }
        }
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

void init_tiles() {
    tiles = malloc(sizeof(Tile**) * TILE_LEVELS);
    for (int h = 0; h < TILE_LEVELS; h++) {
        tiles[h] = malloc(sizeof(Tile*) * TILE_ROWS);
        for (int i = 0; i < TILE_ROWS; i++) {
            tiles[h][i] = malloc(sizeof(Tile) * TILE_COLS);
            for (int j = 0; j < TILE_COLS; j++) {
                tiles[h][i][j].state = NOT_FOUND;
                tiles[h][i][j].x = j;
                tiles[h][i][j].y = i;
                tiles[h][i][j].z = h;
                tiles[h][i][j].bomb = false;
                tiles[h][i][j].nei_bom = 0;
            }
        }
    }
    fillBombs();
    for (int h = 0; h < TILE_LEVELS; h++) {
        for (int i = 0; i < TILE_ROWS; i++) {
            for (int j = 0; j < TILE_COLS; j++) {
                pos p = { .x = j, .y = i, .z = h };
                tiles[h][i][j].nei_bom = neighborBombs(p);
            }
        }
    }
    //ifdbg print_tiles();
}

void fillBombs() {
    int tilesCount = 0; int bombCount = 0;
    if(GAME_SIZE == 1) tilesCount = 243, bombCount = 30, bombsToFind = bombCount;
    if(GAME_SIZE == 2) tilesCount = 1280, bombCount = 200, bombsToFind = bombCount;
    if(GAME_SIZE == 3) tilesCount = 3360, bombCount = 700, bombsToFind = bombCount;
    int* freeTiles = malloc(sizeof(int) * tilesCount); // 1D representation index representation of 2D index of Tile

    for(int i = 0; i < tilesCount; i++){
        freeTiles[i] = i;
    }
    /** filling random tiles with Bombs section */
    srand(time(0)); // generates new seed for random tile index
    for(int i = 0; i < bombCount; i++){
        int rand_index = rand() % (tilesCount), rand_tile = freeTiles[rand_index];
        //printf("%d, %d, ", i, rand_index);
        int z = rand_tile / (TILE_ROWS * TILE_COLS);
        int y = (rand_tile / TILE_COLS) % TILE_ROWS;
        int x = rand_tile % TILE_COLS;
        //printf("%d, %d, %d\n", x, y, z);
        if(tiles[z][y][x].bomb != true) tiles[z][y][x].bomb = true;
        else {i--; continue;}
        //printf("check ok\n");
        if(rand_index == tilesCount - 1){} // To make the print start only from the index
        else{
            for(int j = rand_index; j < tilesCount - 1; j++) {
                //ifdbg printf("Before tiles[%d] = %d, tiles[%d] = %d\n", j, freeTiles[j], j + 1, freeTiles[j + 1]);
                freeTiles[j] = freeTiles[j + 1];
                //ifdbg printf("After tiles[%d] = %d, tiles[%d] = %d\n", j, freeTiles[j], j + 1, freeTiles[j + 1]);
                //ifdbg printf("Cyclo no: %d, %d\n", i, j);
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

void render(SDL_Renderer* renderer, SDL_Texture** textures, bool* show, int currZ){
    SDL_RenderClear(renderer);
    if(*show == true){ // DEBUG!
        for(int i = 0; i < TILE_ROWS; i++){
            for(int j = 0; j < TILE_COLS; j++){
                SDL_Rect temp;
                temp.x = j * TILE_SIZE, temp.y = i * TILE_SIZE; // Z must NOT be used!
                temp.w = TILE_SIZE, temp.h = TILE_SIZE;
                if(!tiles[currZ][i][j].bomb){
                    SDL_RenderCopy(renderer, textures[tiles[currZ][i][j].nei_bom], NULL, &temp);
                }
                else{
                    SDL_RenderCopy(renderer, textures[27], NULL, &temp);
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
                if(tiles[currZ][i][j].state == NOT_FOUND) {
                    SDL_RenderCopy(renderer, textures[29], NULL, &temp);
                }
                else if(tiles[currZ][i][j].state == FOUND && !tiles[currZ][i][j].bomb){
                    SDL_RenderCopy(renderer, textures[tiles[currZ][i][j].nei_bom], NULL, &temp);
                }
                else if(tiles[currZ][i][j].state == FOUND && tiles[currZ][i][j].bomb){
                    SDL_RenderCopy(renderer, textures[27], NULL, &temp);
                }
                else if(tiles[currZ][i][j].state == FLAG){
                    SDL_RenderCopy(renderer, textures[28], NULL, &temp);
                }
            }
        }
    }
    SDL_RenderPresent(renderer);
}

short neighborBombs(pos p){
    static int iter = 0;
    int counter = 0;
    int x = p.x;
    int y = p.y;
    int z = p.z;

    for (int dz = -1; dz <= 1; dz++) {
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                if (dx == 0 && dy == 0 && dz == 0) continue; // Skip the current tile
                int nx = x + dx;
                int ny = y + dy;
                int nz = z + dz;

                if (nx >= 0 && nx < TILE_COLS && ny >= 0 && ny < TILE_ROWS && nz >= 0 && nz < TILE_LEVELS) {
                    if (tiles[nz][ny][nx].bomb) {
                        counter++;
                    }
                }
            }
        }
    }
    //ifdbg printf("Tile at (%d, %d, %d) has %d neighboring bombs.\n", x, y, z, counter);
    return counter;
}

void showBombs() {
    for(int h = 0; h < TILE_LEVELS; h++){
        for(int i = 0; i < TILE_ROWS; i++){
            for(int j = 0; j < TILE_COLS; j++){
                if(tiles[h][i][j].bomb) tiles[h][i][j].state = FOUND;
            }
        }
    }
}

void print_tiles(){
    for(int h = 0; h < TILE_LEVELS; h++){
        for(int i = 0; i < TILE_ROWS; i++){
            for(int j = 0; j < TILE_COLS; j++) {
                if(!tiles[h][i][j].bomb){
                    printf("%hi", tiles[h][i][j].nei_bom);
                }
                else{
                    printf("B");
                }
            }
            printf("\n");
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
            TILE_LEVELS = 3;
        break;
        case 2:
            TILE_ROWS = 16;
            TILE_COLS = 16;
            TILE_LEVELS = 5;
        break;
        case 3:
            TILE_ROWS = 16;
            TILE_COLS = 30;
            TILE_LEVELS = 7;
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
    int currentZ = TILE_LEVELS / 2;
    render(rnd, sprites, &showAll, currentZ);
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
                        pos p = {mouseX, mouseY, currentZ};
                        mouseHandle(p, win, 0, &showAll);
                        render(rnd, sprites, &showAll, currentZ);
                    }
                    if(SDL_BUTTON_RIGHT == evt.button.button){
                        SDL_GetMouseState(&mouseX, &mouseY);
                        pos p = {mouseX, mouseY, currentZ};
                        mouseHandle(p, win, 1, &showAll);
                        render(rnd, sprites, &showAll, currentZ);
                    }
                    break;
                case SDL_KEYDOWN:
                    if(evt.key.keysym.sym == SDLK_LSHIFT){
                        if(DEBUG){
                            showAll = !showAll;
                            if(showAll == true) printf("All showed!\n");
                            else printf("All hidden!\n");
                            render(rnd, sprites, &showAll, currentZ);
                        }
                    }
                    if(evt.key.keysym.sym == SDLK_LCTRL){
                        print_tiles();
                    }
                    if(evt.key.keysym.sym == SDLK_F5){
                        init_tiles();
                        render(rnd, sprites, &showAll, currentZ);
                    }
                    if(evt.key.keysym.sym == SDLK_q){ // get into upper level
                        if(currentZ > 0) currentZ--;
                        render(rnd, sprites, &showAll, currentZ);
                        ifdbg printf("%d\n", currentZ);
                    }
                    if(evt.key.keysym.sym == SDLK_e) {// get into lower level
                        if(currentZ < TILE_LEVELS - 1) currentZ++;
                        render(rnd, sprites, &showAll, currentZ);
                        ifdbg printf("%d\n", currentZ);
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
