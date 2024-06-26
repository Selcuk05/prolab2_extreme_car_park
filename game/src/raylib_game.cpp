#include "raylib.h"
#include "Car.h"
#include "Level.h"
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

#define CONT_WIDTH 250
#define CONT_HEIGHT 90

#define PARK_RECT_WIDTH 70
#define PARK_RECT_HEIGHT 140

#define STAR_WIDTH 30
#define STAR_HEIGHT 30

#define SUCCESS_COLLISION_PERCENTAGE 70
#define ONE_STAR_TIME 20
#define TWO_STAR_TIME 15
#define THREE_STAR_TIME 10

#define FADE_FRAMES 100

using namespace std;

void initBoxes(vector<Rectangle>& boxes, Texture2D boxTexture, Rectangle boxRec, int level);
void initContainers(vector<Rectangle>& containers, Texture2D contTexture, Rectangle contRec, int level);
Rectangle initParkRect(int level);

static bool DEBUG_MODE = false;

int main(){
    srand(time(0));

    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Extreme Car Parking - Selçuk Öz");
    InitAudioDevice();
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    SetTargetFPS(60);

    Music music = LoadMusicStream("resources/cant_tell_me_nothin.mp3");
    SetMusicVolume(music, 0.3f);
    bool musicPaused = false;
    PlayMusicStream(music);

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

    Texture2D sound_texture = LoadTexture("resources/sound.png");
    Texture2D soundoff_texture = LoadTexture("resources/soundoff.png");
    Rectangle sound_rec = {0, 0, (float) sound_texture.width, (float) sound_texture.height};
    Rectangle sound_bounds = { 
        SCREEN_WIDTH - sound_texture.width * 3.0f, 
        sound_texture.height / 2.0f, 
        (float) sound_texture.width,
        (float) sound_texture.height
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
    UnloadImage(container_image);
    UnloadImage(star_image);

    int level = 1;
    bool levelComplete = false;
    Level levels[3] = {Level(1000, 300, 270), Level(100, 300, 270), Level(850, 600, 315)};

    float startX = 0;
    float startY = 0;
    float startR = 0;
    Car car(startX, startY);

    int fade = 0;
    bool crashed = false;

    float totalTime = 0;

    vector<Rectangle> boxes;
    vector<Rectangle> containers;
    Rectangle parkRect;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        Vector2 mousePoint = GetMousePosition();
        
        UpdateMusicStream(music);
        
        if (IsKeyPressed(KEY_SPACE))
        {
            StopMusicStream(music);
            PlayMusicStream(music);
        }

        if (CheckCollisionPointRec(mousePoint, exitBounds))
        {   
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) break;
        }

        if (CheckCollisionPointRec(mousePoint, sound_bounds))
        {   
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                musicPaused = !musicPaused;
                if (musicPaused) PauseMusicStream(music);
                else ResumeMusicStream(music);
            }
        }

        if (CheckCollisionPointRec(mousePoint, {SCREEN_WIDTH/2+50, 30, 50, 50}))
        {   
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) DEBUG_MODE = !DEBUG_MODE;
        }

        BeginDrawing();
        ClearBackground( GRAY );

        if(level == 4){
            Rectangle rec = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            DrawRectangleRec(rec, DARKGRAY);
            DrawText("Congrulations!", SCREEN_WIDTH/3, SCREEN_HEIGHT/3, 60, GREEN);

            DrawTextureRec(exit_texture, exit_rec, (Vector2){ exitBounds.x, exitBounds.y }, WHITE);

            float i;

            DrawText("Level 1: ", SCREEN_WIDTH/3, SCREEN_HEIGHT/3 + 80, 30, GREEN);
            for(i = 0; i < levels[0].getStars(); i++){
                DrawTextureRec(star_texture, star_rec, (Vector2){ SCREEN_WIDTH/3 + 140 + 60 * i, SCREEN_HEIGHT/3 + 80 }, WHITE);
            }

            DrawText("Level 2: ", SCREEN_WIDTH/3, SCREEN_HEIGHT/3 + 140, 30, GREEN);
            for(i = 0; i < levels[1].getStars(); i++){
                DrawTextureRec(star_texture, star_rec, (Vector2){ SCREEN_WIDTH/3 + 140 + 60 * i, SCREEN_HEIGHT/3 + 140 }, WHITE);
            }

            DrawText("Level 3: ", SCREEN_WIDTH/3, SCREEN_HEIGHT/3 + 200, 30, GREEN);
            for(i = 0; i < levels[2].getStars(); i++){
                DrawTextureRec(star_texture, star_rec, (Vector2){ SCREEN_WIDTH/3 + 140 + 60 * i, SCREEN_HEIGHT/3 + 200 }, WHITE);
            }

            EndDrawing();
            continue;
        }

        Level lvl = levels[level-1];
        if(startX != lvl.spawnX || startY != lvl.spawnY || startR != lvl.spawnR){
            startX = lvl.spawnX;
            startY = lvl.spawnY;
            startR = lvl.spawnR;
            car.reinstate(startX, startY, startR);
        }

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
            
            float prevX = car.getX();
            float prevY = car.getY();
            float prevR = car.getRotation();

            car.move();

            // duvara vurdu
            if(
                car.getX() < CAR_WIDTH || 
                car.getY() < TOP_LIMIT_Y + CAR_WIDTH || 
                car.getX() > (SCREEN_WIDTH - CAR_WIDTH) || 
                car.getY() - (CAR_HEIGHT / 2) > (SCREEN_HEIGHT - CAR_HEIGHT)
            ){
                crashed = true;
                car.reinstate(prevX, prevY, prevR);
                if(DEBUG_MODE) cout << "duvar" << endl;
            }

            float currentRotation = car.getRotation();
            bool vertical = 
            (currentRotation < -135 && currentRotation > -225)
                || 
            (currentRotation > 135 && currentRotation < 225)
                ||
            (currentRotation > -45 && currentRotation < 45)
                ||
            (currentRotation > 315);

            // kutuya vurdu
            Rectangle carCurrentRect = {
                vertical ? car.getX() - CAR_HEIGHT / 2 : car.getX() - CAR_WIDTH / 2, 
                vertical ? car.getY() - CAR_WIDTH / 2 : car.getY() - CAR_HEIGHT / 2, 
                vertical ? (float) CAR_HEIGHT : (float) CAR_WIDTH, 
                vertical ? (float) CAR_WIDTH : (float) CAR_HEIGHT
            };
            if(DEBUG_MODE) DrawRectangleRec(carCurrentRect, RED);
            vector<Rectangle> allCrashables;
            allCrashables.insert(allCrashables.end(), boxes.begin(), boxes.end());
            allCrashables.insert(allCrashables.end(), containers.begin(), containers.end());
            for (Rectangle & r : allCrashables) {
                if(CheckCollisionRecs(carCurrentRect, r)){
                    Rectangle coll = GetCollisionRec(carCurrentRect, r);
                    if(DEBUG_MODE) cout << coll.width * coll.height << endl;
                    if(DEBUG_MODE) DrawRectangleRec(coll, BLUE);
                    crashed = true;
                    car.reinstate(prevX, prevY, prevR);
                    if(DEBUG_MODE) cout << "Kutu" << endl;
                }
            }

            // sarı kutuya girdi
            if(CheckCollisionRecs(carCurrentRect, parkRect)){
                Rectangle coll = GetCollisionRec(carCurrentRect, parkRect);
                if(DEBUG_MODE) DrawRectangleRec(coll, RED);
                if(100 * coll.width * coll.height >= SUCCESS_COLLISION_PERCENTAGE * parkRect.width * parkRect.height){
                    levelComplete = true;
                    if(DEBUG_MODE) cout << "Sari kutu" << endl;
                }
            }
        }

        if(crashed && IsKeyPressed(KEY_ENTER)){
            crashed = false;
            car.reinstate(startX, startY, startR);
            fade = FADE_FRAMES;
            totalTime = 0;
        }else if(levelComplete && IsKeyPressed(KEY_ENTER)){
            levelComplete = false;
            level += 1;
            car.reinstate(startX, startY, startR);
            fade = FADE_FRAMES;
            totalTime = 0;
        }


        Rectangle car_rec = {
            .x = car.getX(),
            .y = car.getY(),
            .width = CAR_HEIGHT,
            .height = CAR_WIDTH
        };
        Vector2 car_origin = {
            .x = CAR_HEIGHT/2,
            .y = CAR_WIDTH/2,
        };
        DrawTexturePro(car_texture, car_texture_rec, car_rec, car_origin, car.getRotation(), WHITE);

        DrawRectangle(0, 0, SCREEN_WIDTH, 100, DARKGRAY);
        DrawTextureRec(exit_texture, exit_rec, (Vector2){ exitBounds.x, exitBounds.y }, WHITE);
        DrawTextureRec(musicPaused ? soundoff_texture : sound_texture, sound_rec, (Vector2){ sound_bounds.x, sound_bounds.y }, WHITE);

        if(crashed){
            DrawText("You Crashed!", 25, 25, 25, RED);
            DrawText("Press ENTER to restart", 25, 50, 25, RED);
        }

        if(levelComplete){
            DrawText("Level Complete!", 25, 25, 25, GREEN);
            DrawText("Press ENTER to continue", 25, 50, 25, GREEN);
            if(totalTime <= ONE_STAR_TIME && TWO_STAR_TIME < totalTime){
                DrawTextureRec(star_texture, star_rec, (Vector2){ 400, 30 }, WHITE);
                levels[level-1].setStars(1);
            } else if(totalTime <= TWO_STAR_TIME && THREE_STAR_TIME < totalTime){
                DrawTextureRec(star_texture, star_rec, (Vector2){ 400, 30 }, WHITE);
                DrawTextureRec(star_texture, star_rec, (Vector2){ 450, 30 }, WHITE);
                levels[level-1].setStars(2);
            } else if(totalTime <= THREE_STAR_TIME){
                DrawTextureRec(star_texture, star_rec, (Vector2){ 400, 30 }, WHITE);
                DrawTextureRec(star_texture, star_rec, (Vector2){ 450, 30 }, WHITE);
                DrawTextureRec(star_texture, star_rec, (Vector2){ 500, 30 }, WHITE);
                levels[level-1].setStars(3);
            }
        }

        DrawText(TextFormat("%d", (int) totalTime), SCREEN_WIDTH/2, 30, 50, WHITE);
        DrawText(TextFormat("Level %d", level), SCREEN_WIDTH * 2.2 / 3, 40, 30, WHITE);
        
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
    UnloadTexture(sound_texture);
    UnloadTexture(soundoff_texture);
    UnloadMusicStream(music);
    CloseAudioDevice(); 
    CloseWindow();
    return 0;
}

void initBoxes(vector<Rectangle>& boxes, Texture2D boxTexture, Rectangle boxRec, int level){
    boxes.clear();
    Rectangle box;

    // top boxes
    for(float i = 0; i <= SCREEN_WIDTH - BOX_WIDTH; i += BOX_WIDTH){
        box = {i, TOP_LIMIT_Y, BOX_WIDTH, BOX_HEIGHT};
        if(DEBUG_MODE) DrawRectangleRec(box, BLUE);
        DrawTextureRec(boxTexture, boxRec, (Vector2){i, TOP_LIMIT_Y}, WHITE);
        boxes.push_back(box);
    }

    if(level == 1){
        for(float i = SCREEN_WIDTH / 2; i <= SCREEN_WIDTH - BOX_WIDTH; i += BOX_WIDTH){
            box = {i, SCREEN_HEIGHT / 2 + TOP_LIMIT_Y, BOX_WIDTH, BOX_HEIGHT};
            if(DEBUG_MODE) DrawRectangleRec(box, BLUE);
            DrawTextureRec(boxTexture, boxRec, (Vector2){i, SCREEN_HEIGHT / 2 + TOP_LIMIT_Y}, WHITE);
            boxes.push_back(box);
        }
    }

    if(level == 2){
        for(float i = SCREEN_HEIGHT / 1.8; i <= SCREEN_HEIGHT - 80; i += BOX_HEIGHT){
            box = {700, i, BOX_WIDTH, BOX_HEIGHT};
            if(DEBUG_MODE) DrawRectangleRec(box, BLUE);
            DrawTextureRec(boxTexture, boxRec, (Vector2){700, i}, WHITE);
            boxes.push_back(box);
        }

        for(float i = SCREEN_HEIGHT / 1.8; i <= SCREEN_HEIGHT - 80; i += BOX_HEIGHT){
            box = {1000, i, BOX_WIDTH, BOX_HEIGHT};
            if(DEBUG_MODE) DrawRectangleRec(box, BLUE);
            DrawTextureRec(boxTexture, boxRec, (Vector2){1000, i}, WHITE);
            boxes.push_back(box);
        }
        return;
    }

    if(level == 3){
        for(float i = 625 + CONT_HEIGHT; i <= SCREEN_WIDTH - BOX_WIDTH * 4; i += BOX_WIDTH){
            box = {i, 400, BOX_WIDTH, BOX_HEIGHT};
            if(DEBUG_MODE) DrawRectangleRec(box, BLUE);
            DrawTextureRec(boxTexture, boxRec, (Vector2){i, 400}, WHITE);
            boxes.push_back(box);
        }

        box = {625 + CONT_HEIGHT + 3 * BOX_WIDTH, 320, BOX_WIDTH, BOX_HEIGHT};
        if(DEBUG_MODE) DrawRectangleRec(box, BLUE);
        DrawTextureRec(boxTexture, boxRec, (Vector2){625 + CONT_HEIGHT + 3 * BOX_WIDTH, 320}, WHITE);
        boxes.push_back(box);

        for(float i = SCREEN_WIDTH / 3; i <= SCREEN_WIDTH - BOX_WIDTH * 8; i += BOX_WIDTH){
            box = {i, TOP_LIMIT_Y + BOX_WIDTH, BOX_WIDTH, BOX_HEIGHT};
            if(DEBUG_MODE) DrawRectangleRec(box, BLUE);
            DrawTextureRec(boxTexture, boxRec, (Vector2){i, TOP_LIMIT_Y + BOX_WIDTH}, WHITE);
            boxes.push_back(box);
        }
        return;
    }
}

void initContainers(vector<Rectangle>& containers, Texture2D contTexture, Rectangle contRec, int level){
    containers.clear();
    Rectangle container;

    if(level == 1){
        float cont1x = 230;
        container = {cont1x, SCREEN_HEIGHT-CONT_WIDTH, CONT_HEIGHT, CONT_WIDTH};
        if(DEBUG_MODE) DrawRectangleRec(container, BLUE);
        DrawTexturePro(
            contTexture, 
            contRec, 
            {cont1x, SCREEN_HEIGHT-CONT_WIDTH, CONT_WIDTH, CONT_HEIGHT}, 
            {CONT_HEIGHT/2-45, CONT_WIDTH/2-35}, 
            90, 
            WHITE
        );
        containers.push_back(container);

        for(float i = TOP_LIMIT_Y + 80; i <= SCREEN_HEIGHT/4; i += CONT_HEIGHT){
            container = {100, i, CONT_WIDTH, CONT_HEIGHT};
            if(DEBUG_MODE) DrawRectangleRec(container, BLUE);
            DrawTextureRec(contTexture, contRec, (Vector2){100, i}, WHITE);
            containers.push_back(container);
        }
    }

    if(level == 2){
        for(float i = TOP_LIMIT_Y + 80; i <= SCREEN_HEIGHT/2; i += CONT_HEIGHT){
            container = {200, i, CONT_WIDTH, CONT_HEIGHT};
            if(DEBUG_MODE) DrawRectangleRec(container, BLUE);
            DrawTextureRec(contTexture, contRec, (Vector2){200, i}, WHITE);
            containers.push_back(container);
        }
        return;
    }

    if(level == 3){
        float cont1x = 625;
        container = {cont1x, SCREEN_HEIGHT-CONT_WIDTH-25, CONT_HEIGHT, CONT_WIDTH};
        if(DEBUG_MODE) DrawRectangleRec(container, BLUE);
        DrawTexturePro(
            contTexture, 
            contRec, 
            {cont1x, SCREEN_HEIGHT-CONT_WIDTH-25, CONT_WIDTH, CONT_HEIGHT}, 
            {CONT_HEIGHT/2-45, CONT_WIDTH/2-35}, 
            90, 
            WHITE
        );
        containers.push_back(container);

        for(float i = SCREEN_HEIGHT - 3 * CONT_HEIGHT; i <= SCREEN_HEIGHT - CONT_HEIGHT; i += CONT_HEIGHT){
            container = {50, i-20, CONT_WIDTH, CONT_HEIGHT};
            if(DEBUG_MODE) DrawRectangleRec(container, BLUE);
            DrawTextureRec(contTexture, contRec, (Vector2){50, i-20}, WHITE);
            containers.push_back(container);
        }
        return;
    }
}

Rectangle initParkRect(int level){
    Rectangle pr;

    if(level == 1){
        pr = {100, SCREEN_HEIGHT - 160, PARK_RECT_WIDTH, PARK_RECT_HEIGHT};    
    }

    if(level == 2){
        pr = {SCREEN_WIDTH / 1.5, SCREEN_HEIGHT - 160, PARK_RECT_WIDTH, PARK_RECT_HEIGHT};
    }

    if(level == 3){
        pr = {100, 220, PARK_RECT_WIDTH, PARK_RECT_HEIGHT};
    }

    DrawRectangleRec(pr, YELLOW);
    return pr;
}