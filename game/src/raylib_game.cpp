#include "raylib.h"
#include <cmath>
#include "Car.h"
#include <iostream>

#define BGCOLOR (Color){128, 128, 128}
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define CAR_WIDTH 60
#define CAR_HEIGHT 130

int main(){
    srand(time(0));

    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Extreme Car Parking - Selçuk Öz");
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetTargetFPS(60);

    const float startX = 1000;
    const float startY = 400;
    Car car(startX, startY);

    Image car_image = LoadImage("resources/car.png");
    ImageRotateCW(&car_image);
    ImageResize(&car_image, CAR_WIDTH, CAR_HEIGHT);
    Texture2D car_texture = LoadTextureFromImage(car_image);
    UnloadImage(car_image);
    Rectangle car_texture_rec = {
        .x = 0,
        .y = 0,
        .width = CAR_WIDTH,
        .height = CAR_HEIGHT,
    };

    int fade = 0;
    bool crashed = false;
    float totalTime = 0;
    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        
        BeginDrawing();
        ClearBackground( GRAY );

        if(!crashed){
            totalTime += dt;

            if(IsKeyDown( KEY_UP ) || IsKeyDown(KEY_W)) {
                car.accelerate(dt);
            } else if(IsKeyDown( KEY_DOWN ) || IsKeyDown(KEY_S)) {
                car.deaccelerate(dt);
            } else {
                car.slowUntilStationary(dt);
            }

            if(IsKeyDown( KEY_LEFT ) || IsKeyDown(KEY_A)) {
                car.rotate(dt, 1);
            } else if(IsKeyDown( KEY_RIGHT ) || IsKeyDown(KEY_D)) {
                car.rotate(dt, -1);
            }

            car.move();

            // duvara vurdu
            if(
                car.getX() < CAR_WIDTH || car.getY() < CAR_WIDTH
                || 
                car.getX() - (CAR_WIDTH / 2) > (SCREEN_WIDTH - CAR_WIDTH) || car.getY() - (CAR_HEIGHT / 2) > (SCREEN_HEIGHT - CAR_HEIGHT)
            ){
                crashed = true;
            }
        }

        if(crashed && IsKeyPressed(KEY_ENTER)){
            crashed = false;
            car.reinstate(startX, startY);
            fade = 120;
            totalTime = 0;
        }

        Rectangle car_rec = {
            .x = car.getX(),
            .y = car.getY(),
            .width = CAR_HEIGHT,
            .height = CAR_WIDTH,
        };
        Vector2 car_origin = {
            .x = CAR_HEIGHT/2,
            .y = CAR_WIDTH/2,
        };
        DrawTexturePro(car_texture, car_texture_rec, car_rec, car_origin, car.getRotation(), WHITE);

        if(crashed){
            DrawText("You Crashed!", 300, 300, 50, BLACK);
            DrawText("Press ENTER to restart", 300, 350, 50, BLACK);
        }

        DrawText(TextFormat("%d", (int) totalTime), 50, 50, 50, BLACK);
        
        if(fade > 0 && !crashed){
            Rectangle rec = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            DrawRectangleRec(rec, Fade(BLACK, (float) fade / 100));
            EndDrawing();
            fade -= 1;
            continue;
        }

        EndDrawing();
    }

    UnloadTexture(car_texture);
    CloseWindow();

    return 0;
}