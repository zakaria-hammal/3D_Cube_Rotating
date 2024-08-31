#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#define WIDTH 1000
#define HEIGHT 1000
#define SCALE 200

typedef struct Point
{
    double x;
    double y;
    double z;
} Point;


double** dotMat(double** a, double** b)
{
    double** result;
    result = malloc(3 * sizeof(double*));

    for (int i = 0; i < 3; i++)
    {
        result[i] = malloc(3 * sizeof(double));
    }
    

    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
        
    }

    return result;
}

void drawCube(SDL_Renderer* r, Point* cube, double** a)
{

    for(int i = 0; i < 8; i++) 
    {
        cube[i].x = cube[i].x - WIDTH / 2;
        cube[i].y = cube[i].y - HEIGHT / 2;


        cube[i].x = a[0][0] * cube[i].x + a[0][1] * cube[i].y + a[0][2] * cube[i].z;
        cube[i].y = a[1][0] * cube[i].x + a[1][1] * cube[i].y + a[1][2] * cube[i].z;
        cube[i].z = a[2][0] * cube[i].x + a[2][1] * cube[i].y + a[2][2] * cube[i].z;

        cube[i].x = cube[i].x + WIDTH / 2;
        cube[i].y = cube[i].y + HEIGHT / 2;
    }

    for (int i = 0; i < 4; i++)
    {
        SDL_RenderDrawLine(r, cube[i].x, cube[i].y, cube[(i + 1) % 4].x, cube[(i + 1) % 4].y);
        SDL_RenderDrawLine(r, cube[i + 4].x, cube[i + 4].y, cube[((i + 1) % 4) + 4].x, cube[((i + 1) % 4) + 4].y);
        SDL_RenderDrawLine(r, cube[i].x, cube[i].y, cube[i + 4].x, cube[i + 4].y);
    }
    
    SDL_RenderPresent(r);
}

int main(int argc, char* argv[])
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Erreur de SDL_Init : %s", SDL_GetError());

        return EXIT_FAILURE;
    }

    SDL_Window* window = NULL;

    window = SDL_CreateWindow("Fenêtre", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if(window == NULL)
    {
        fprintf(stderr, "Erreur de SDL_CreateWindow : %s", SDL_GetError());

        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = NULL;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if(renderer == NULL)
    {
        fprintf(stderr, "Erreur de SDL_CreateRenderer : %s", SDL_GetError());

        return EXIT_FAILURE;
    }

    int quit = 0;

    SDL_Event e;

    Point cube[] = {
        {-1, 1, 1}, {1, 1, 1}, {1, -1, 1}, {-1, -1, 1}, {-1, 1, -1}, {1, 1, -1}, {1, -1, -1}, {-1, -1, -1}
    };

    double alpha = 0.001;

    double** rotationX = malloc(3 * sizeof(double*));

    for (int i = 0; i < 3; i++)
    {
        rotationX[i] = malloc(3 * sizeof(double));
    }
    
    rotationX[0][0] = 1;
    rotationX[0][1] = 0;
    rotationX[0][2] = 0;
    rotationX[1][0] = 0;
    rotationX[1][1] = cos(alpha);
    rotationX[1][2] = -sin(alpha);
    rotationX[2][0] = 0;
    rotationX[2][1] = sin(alpha);
    rotationX[2][2] = cos(alpha);

    double beta = 0.002;

    double** rotationY = malloc(3 * sizeof(double*));

    for (int i = 0; i < 3; i++)
    {
        rotationY[i] = malloc(3 * sizeof(double));
    }
    
    rotationY[0][0] = cos(beta);
    rotationY[0][1] = 0;
    rotationY[0][2] = sin(beta);
    rotationY[1][0] = 0;
    rotationY[1][1] = 1;
    rotationY[1][2] = 0;
    rotationY[2][0] = -sin(beta);
    rotationY[2][1] = 0;
    rotationY[2][2] = cos(beta);

    double gamma = 0.003;

    double** rotationZ = malloc(3 * sizeof(double*));

    for (int i = 0; i < 3; i++)
    {
        rotationZ[i] = malloc(3 * sizeof(double));
    }
    
    rotationZ[0][0] = cos(gamma);
    rotationZ[0][1] = -sin(gamma);
    rotationZ[0][2] = 0;
    rotationZ[1][0] = sin(gamma);
    rotationZ[1][1] = cos(gamma);
    rotationZ[1][2] = 0;
    rotationZ[2][0] = 0;
    rotationZ[2][1] = 0;
    rotationZ[2][2] = 1;

    double** rotation = dotMat(rotationZ, dotMat(rotationY, rotationX));    

    for (int i = 0; i < 8; i++)
    {
        cube[i].x = cube[i].x * SCALE + WIDTH / 2;
        cube[i].y = cube[i].y * SCALE + HEIGHT / 2;
        cube[i].z = cube[i].z * SCALE;
    }
    

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if(e.type == SDL_QUIT)
            {
                quit = 1;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        drawCube(renderer, cube, rotation);

        SDL_Delay(3);
        
    }
    
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}