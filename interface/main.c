#include "raylib.h"

int main()
{
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        // Update

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, MAROON);

        EndDrawing();
    }

    
    CloseWindow();
    

    return 0;
}
