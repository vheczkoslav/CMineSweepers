#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define BEFORE 0 // Game State
#define PLAY 1
#define BOMBED 2

#define NOT_FOUND 0 // Tile state
#define FOUND 1
#define FLAG 2

typedef struct{
    int x,
    y,
    z;
}pos;

void load_settings(const char* filename);
void init_tiles();
void free_tiles();
void fillBombs();
void showBombs();
short neighborBombs(pos p);
void showEmpty(pos rp);
SDL_Window* init_win();
char* rtStateMsg();
void mouseHandle(pos p, SDL_Window* sw, int signal, bool* show); // signal 0 = left click, 1 = right click
SDL_Texture** load_textures(int len, SDL_Renderer* renderer);
void render(SDL_Renderer* renderer, SDL_Texture** textures, bool* show, int currZ);
void print_tiles();

typedef struct{
    short x; // there is no need for x, y to be int  given that their value will not exceed 29 (30)
    short y;
    short z;
    bool bomb;
    short state; // not_found, found, flag
    short nei_bom; // neighboring bombs. It is determined in the beginning and then no longer needed.
}Tile;

/*typedef struct{
    int bombsToFind, bombsFound, GAME_SIZE;
    bool DEBUG;
    Tile** tiles;
}GAME_DATA;*/

int bombsToFind = 0, bombsFound = 0;
int GAME_SIZE = 0, TILE_COLS = 0, TILE_ROWS = 0, TILE_LEVELS = 0, TILE_SIZE = 0;
bool DEBUG = 0;
Tile*** tiles;
