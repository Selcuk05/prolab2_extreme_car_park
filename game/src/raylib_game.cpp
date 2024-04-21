#include "raylib.h"
#include "Car.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#define BGCOLOR (Color){128, 128, 128}
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TOP_LIMIT_Y 100
#define CAR_WIDTH 60
#define CAR_HEIGHT 130
#define BOX_WIDTH 80
#define BOX_HEIGHT 80
#define CONT_WIDTH 250
#define CONT_HEIGHT 90
#define STAR_WIDTH 30
#define STAR_HEIGHT 30
#define SUCCESS_COLLISION_PERCENTAGE 75
#define ONE_STAR_TIME 20
#define TWO_STAR_TIME 15
#define THREE_STAR_TIME 10

using namespace std;

void initBoxes(vector<Rectangle>& boxes, Texture2D boxTexture, Rectangle boxRec, int level);
void initContainers(vector<Rectangle>& containers, Texture2D contTexture, Rectangle contRec, int level);
Rectangle initParkRect(int level);

int main(){
    // random for car movement deviation
    srand(time(0));

    // window/game/raylib settings
    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Extreme Car Parking - Selçuk Öz");
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetTargetFPS(60);

    // images and textures
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

    Image container_image = LoadImage("resources/container.png");
    ImageResize(&container_image, CONT_WIDTH, CONT_HEIGHT);
    Texture2D container_texture = LoadTextureFromImage(container_image);
    Rectangle container_rec = {0, 0, (float) container_texture.width, (float) container_texture.height};

    Image star_image = LoadImage("resources/star.png");
    ImageResize(&star_image, STAR_WIDTH, STAR_HEIGHT);
    Texture2D star_texture = LoadTextureFromImage(star_image);
    Rectangle star_rec = {0, 0, (float) star_texture.width, (float) star_texture.height};

    UnloadImage(car_image);
    UnloadImage(exit_image);
    UnloadImage(box_image);

    int level = 1;
    bool levelComplete = false;
    Vector2 levelSpawns[3] = {{1000, 300}, {}, {}};

    float startX = levelSpawns[0].x;
    float startY = levelSpawns[0].y;
    Car car(startX, startY);

    int fade = 0;
    bool crashed = false;

    float totalTime = 0;

    Vector2 mousePoint = {0.0f, 0.0f};

    vector<Rectangle> boxes;
    vector<Rectangle> containers;
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
        initContainers(containers, container_texture, container_rec, level);
        Rectangle parkRect = initParkRect(level);

        if(!crashed && !levelComplete){
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

            // duvara vurdu
            if(
                car.getX() < CAR_WIDTH || 
                car.getY() < TOP_LIMIT_Y + CAR_WIDTH || 
                car.getX() > (SCREEN_WIDTH - CAR_WIDTH) || 
                car.getY() - (CAR_HEIGHT / 2) > (SCREEN_HEIGHT - CAR_HEIGHT)
            ){
                crashed = true;
                cout << "duvar" << endl;
            }

            // kutuya vurdu
            Rectangle carCurrentRect = {car.getX() - CAR_WIDTH / 2, car.getY() - CAR_HEIGHT / 2, CAR_WIDTH, CAR_HEIGHT};
            vector<Rectangle> allCrashables;
            allCrashables.insert(allCrashables.end(), boxes.begin(), boxes.end());
            allCrashables.insert(allCrashables.end(), containers.begin(), containers.end());
            for (auto & r : allCrashables) {
                if(CheckCollisionRecs(carCurrentRect, r)){
                    crashed = true;
                }
            }

            // sarı kutuya girdi
            if(CheckCollisionRecs(carCurrentRect, parkRect)){
                Rectangle coll = GetCollisionRec(carCurrentRect, parkRect);
                if(100 * coll.width * coll.height >= SUCCESS_COLLISION_PERCENTAGE * parkRect.width * parkRect.height){
                    levelComplete = true;
                }
            }
        }

        if(!crashed && !levelComplete){
            car.move();
        }

        if(crashed && IsKeyPressed(KEY_ENTER)){
            crashed = false;
            car.reinstate(startX, startY);
            fade = 120;
            totalTime = 0;
        }

        if(levelComplete && IsKeyPressed(KEY_ENTER)){
            levelComplete = false;
            level += 1;
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

        DrawRectangle(0, 0, SCREEN_WIDTH, 100, DARKGRAY);
        DrawTextureRec(exit_texture, exit_rec, (Vector2){ exitBounds.x, exitBounds.y }, WHITE);

        if(crashed){
            DrawText("You Crashed!", 25, 25, 25, RED);
            DrawText("Press ENTER to restart", 25, 50, 25, RED);
        }

        if(levelComplete){
            DrawText("Level Complete!", 25, 25, 25, GREEN);
            DrawText("Press ENTER to continue", 25, 50, 25, GREEN);
            if(totalTime <= ONE_STAR_TIME && TWO_STAR_TIME < totalTime){
                DrawTextureRec(star_texture, star_rec, (Vector2){ 400, 30 }, WHITE);
            } else if(totalTime <= TWO_STAR_TIME && THREE_STAR_TIME < totalTime){
                DrawTextureRec(star_texture, star_rec, (Vector2){ 400, 30 }, WHITE);
                DrawTextureRec(star_texture, star_rec, (Vector2){ 450, 30 }, WHITE);
            } else if(totalTime <= THREE_STAR_TIME){
                DrawTextureRec(star_texture, star_rec, (Vector2){ 400, 30 }, WHITE);
                DrawTextureRec(star_texture, star_rec, (Vector2){ 450, 30 }, WHITE);
                DrawTextureRec(star_texture, star_rec, (Vector2){ 500, 30 }, WHITE);
            }
        }

        DrawText(TextFormat("%d", (int) totalTime), SCREEN_WIDTH/2, 20, 50, WHITE);
        DrawText(TextFormat("Level %d", level), SCREEN_WIDTH/2, 65, 30, WHITE);
        
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
    UnloadTexture(box_texture);
    UnloadTexture(exit_texture);
    UnloadTexture(star_texture);
    UnloadTexture(container_texture);
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

    if(level == 1){
        for(float i = SCREEN_WIDTH / 2; i <= SCREEN_WIDTH - BOX_WIDTH; i += BOX_WIDTH){
            box = {i, SCREEN_HEIGHT / 2 + TOP_LIMIT_Y, BOX_WIDTH, BOX_HEIGHT};
            DrawTextureRec(boxTexture, boxRec, (Vector2){i, SCREEN_HEIGHT / 2 + TOP_LIMIT_Y}, WHITE);
            boxes.push_back(box);
        }
    }

    if(level == 2){
        return;
    }

    if(level == 3){
        return;
    }
}

void initContainers(vector<Rectangle>& containers, Texture2D contTexture, Rectangle contRec, int level){
    bool _ = containers.empty();
    Rectangle container;

    if(level == 1){
        for(float i = TOP_LIMIT_Y + 80; i <= SCREEN_HEIGHT/1.7; i += CONT_HEIGHT){
            container = {100, i, CONT_WIDTH, CONT_HEIGHT};
            DrawTextureRec(contTexture, contRec, (Vector2){100, i}, WHITE);
            containers.push_back(container);
        }
    }

    if(level == 2){
        return;
    }

    if(level == 3){
        return;
    }
}

Rectangle initParkRect(int level){
    Rectangle pr;

    if(level == 1){
        pr = {100, SCREEN_HEIGHT - 160, 70, 140};    
    }

    if(level == 2){
        Rectangle pr = {500, 500, 70, 140};
    }

    if(level == 3){
        Rectangle pr = {500, 500, 70, 140};
    }

    DrawRectangleRec(pr, YELLOW);
    return pr;
}