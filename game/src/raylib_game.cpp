#include "raylib.h"
#include "Car.h"
#include <cmath>
#include <iostream>
#include <vector>

#define BGCOLOR (Color){128, 128, 128}
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TOP_LIMIT_Y 100
#define CAR_WIDTH 60
#define CAR_HEIGHT 130
#define BOX_WIDTH 80
#define BOX_HEIGHT 80

using namespace std;

void initBoxes(vector<Rectangle>& boxes, Texture2D boxTexture, Rectangle boxRec, int level);
void initParkRect();

int main(){
    srand(time(0));

    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Extreme Car Parking - Selçuk Öz");
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetTargetFPS(60);

    Image car_image = LoadImage("resources/car.png");
    ImageRotateCW(&car_image);
    ImageResize(&car_image, CAR_WIDTH, CAR_HEIGHT);
    Texture2D car_texture = LoadTextureFromImage(car_image);
    Rectangle car_texture_rec = {0, 0, CAR_WIDTH, CAR_HEIGHT};

    Image exit_image = LoadImage("resources/exit.png");
    Texture2D exit_texture = LoadTextureFromImage(exit_image);
    Rectangle exit_rec = {0, 0, (float) exit_texture.width, (float) exit_texture.height};
    Rectangle exitBounds = { 
        SCREEN_WIDTH - exit_texture.width * 1.5f, 
        exit_texture.height / 2.0f, 
        (float) exit_texture.width,
        (float) exit_texture.height
    };

    Image box_image = LoadImage("resources/box.png");
    ImageResize(&box_image, BOX_WIDTH, BOX_HEIGHT);
    Texture2D box_texture = LoadTextureFromImage(box_image);
    Rectangle box_rec = {0, 0, (float) box_texture.width, (float) box_texture.height};

    UnloadImage(car_image);
    UnloadImage(exit_image);
    UnloadImage(box_image);

    const float startX = 1000;
    const float startY = 400;
    Car car(startX, startY);

    int fade = 0;
    bool crashed = false;

    float totalTime = 0;

    Vector2 mousePoint = {0.0f, 0.0f};

    int level = 1;
    vector<Rectangle> boxes;
    Rectangle parkRect;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, exitBounds))
        {   
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) break;
        }
        
        BeginDrawing();
        ClearBackground( GRAY );
        initBoxes(boxes, box_texture, box_rec, level);

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
                car.getX() < CAR_WIDTH || 
                car.getY() < TOP_LIMIT_Y + CAR_WIDTH || 
                car.getX() > (SCREEN_WIDTH - CAR_WIDTH) || 
                car.getY() - (CAR_HEIGHT / 2) > (SCREEN_HEIGHT - CAR_HEIGHT)
            ){
                crashed = true;
            }

            // kutuya vurdu
            Rectangle carCurrentRect = {car.getX() - CAR_WIDTH / 2, car.getY() - CAR_HEIGHT / 2, CAR_WIDTH, CAR_HEIGHT};
            for (auto & r : boxes) {
                if(CheckCollisionRecs(carCurrentRect, r)){
                    crashed = true;
                }
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

        DrawRectangle(0, 0, SCREEN_WIDTH, 100, WHITE);
        DrawTextureRec(exit_texture, exit_rec, (Vector2){ exitBounds.x, exitBounds.y }, WHITE);

        initBoxes(boxes, box_texture, box_rec, level);

        if(crashed){
            DrawText("You Crashed!", 25, 25, 25, RED);
            DrawText("Press ENTER to restart", 25, 50, 25, RED);
        }

        DrawText(TextFormat("%d", (int) totalTime), SCREEN_WIDTH/2, 20, 50, BLACK);
        DrawText(TextFormat("Level %d", level), SCREEN_WIDTH/2 - 40, 65, 30, BLACK);
        
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

void initBoxes(vector<Rectangle>& boxes, Texture2D boxTexture, Rectangle boxRec, int level){
    bool _ = boxes.empty();
    Rectangle box;

    // top boxes
    for(float i = 0; i <= SCREEN_WIDTH - BOX_WIDTH; i += BOX_WIDTH){
        box = {i, TOP_LIMIT_Y, BOX_WIDTH, BOX_HEIGHT};
        DrawTextureRec(boxTexture, boxRec, (Vector2){i, TOP_LIMIT_Y}, WHITE);
        boxes.push_back(box);
    }


}