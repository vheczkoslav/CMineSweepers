#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

void init_tiles();
void free_tiles();
void fillBombs();
int neighborBombs(int x, int y);
SDL_Window* init_win();
char* rtStateMsg();
void mouseHandle(int x, int y);
SDL_Texture** load_textures(int len, SDL_Renderer* renderer);
void render(SDL_Renderer* renderer, SDL_Texture** textures);

typedef struct{
    short x; // there is no need for x, y to be int  given that their value will not exceed 29 (30)
    short y;
    bool bomb;
    bool found;
    bool show; // DEBUG PURPOSES ONLY
}Tile;

enum GameState {BEFORE, PLAY, BOMBED};
enum GameState g = BEFORE;

int GAME_SIZE = 0;
bool DEBUG = 0;
Tile** tiles;
