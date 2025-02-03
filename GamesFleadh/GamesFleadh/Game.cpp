#include "Game.h"

Game::Game() : score(0)
{
    leftStickX = 0.0f;
    leftStickY = 0.0f;
    rightStickX = 0.0f;
    rightStickY = 0.0f;
    leftTrigger = 0.0f;
    rightTrigger = 0.0f;
}

Game::~Game()
{
    UnloadTexture(heightmapTexture);     // Unload texture
    UnloadModel(heightmapModel);         // Unload model
}

void Game::run()
{
    init();

    while (!WindowShouldClose())
    {
        update();
        render();
    }
}

void Game::init()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Games Fleadh 2025");

    // Define our custom camera to look into our 3d world
    camera = { 0 };
    camPos = { 8.0f, 2.0f, 0.0f };
    camera.position = camPos;     // Camera position
    camera.target = { -230.0f, 0.0f, 0.0f };          // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
    camera.fovy = 90.0f;                                    // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                 // Camera projection type
    
    loadAssets();
    gamepadInit();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
}

void Game::render()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    DrawModel(heightmapModel, mapPosition, 4.0f, WHITE);
    DrawModel(heightmapModel, mapPosition2, 1.0f, WHITE);
    DrawModel(*player.getModel(), {camPos.x - 5.0f, player.getPositon().y, player.getPositon().z }, 1.0f, player.getColor());
    DrawModel(*enemy.getModel(), enemy.getPositon(), 1.0f, enemy.getColour());
    DrawModel(*player.getBulletModel(), player.getBulletPositon(), 0.5f, BLUE);
    DrawBoundingBox(player.getHitbox(), RED);
    DrawBoundingBox(enemy.getHitbox(), GREEN);
    DrawBoundingBox(player.getBulletHitBox(), RED);


    DrawGrid(20, 1.0f);

    EndMode3D();

    DrawText(TextFormat("DETECTED BUTTON: %i", GetGamepadButtonPressed()), 10, 430, 10, RED);
    DrawText(TextFormat("SCORE: %i", score), 10, 70, 25, RED);
    DrawFPS(10, 10);

    EndDrawing();
}

void Game::update()
{
    gamepadUpdate();
    inputControl();
    player.updateBullet();
    camera.position = camPos;
    checkCollisions(player.getHitbox(), enemy.getHitbox());
    UpdateCamera(&camera, CAMERA_PERSPECTIVE);

}

void Game::loadAssets()
{
    heightmapImage = LoadImage("ASSETS/heightmapWider.png");     // Load heightmap image (RAM)
    heightmapTexture = LoadTextureFromImage(heightmapImage);        // Convert image to texture (VRAM)

    heightmapMesh = GenMeshHeightmap(heightmapImage, { 16, 8, 16 }); // Generate heightmap mesh (RAM and VRAM)
    heightmapModel = LoadModelFromMesh(heightmapMesh);                  // Load model from generated mesh

    heightmapModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = heightmapTexture; // Set map diffuse texture
    mapPosition = { -8.0f, 0.0f, -32.0f };           // Define model position
    mapPosition2 = { -23.0f, 0.0f, -8.0f };

    UnloadImage(heightmapImage);             // Unload heightmap image from RAM, already uploaded to VRAM

    *player.getModel() = LoadModel("ASSETS/RS/cube.glb");
    *enemy.getModel() = LoadModel("ASSETS/RS/bugProto01.glb");
    *player.getBulletModel() = LoadModel("ASSETS/RS/bulletProto.glb");

    player.setHitBox();
    enemy.setHitBox();
}

void Game::inputControl()
{
    if (IsKeyDown(KEY_W) || leftStickY < 0)
    {
        camDirection = 0.0f;
        if (leftStickY < 0)
        {
           camDirection -= camSpeed * (-leftStickY);
        }
        else
        {
            camDirection -= camSpeed;
        }
        player.updateHitBox(camDirection);
        camPos.x += camDirection;
    }
    if (IsKeyDown(KEY_S) || leftStickY > 0)
    {
        if (leftStickY > 0)
        {
            camDirection = camSpeed * (-leftStickY);
        }
        else
        {
            camDirection = camSpeed;
        }
        player.updateHitBox(camDirection);
        camPos.x += camDirection;
    }


    if (IsKeyDown(KEY_UP) || rightStickY < 0)
    {
        player.move(NORTH);
        camPos.y += 0.1f;
    }
    if (IsKeyDown(KEY_DOWN) || rightStickY > 0)
    {
        player.move(SOUTH);
        camPos.y -= 0.1f;
    }
    if (IsKeyDown(KEY_LEFT) || rightStickX < 0)
    {
        player.move(EAST);
        camPos.z += 0.1f;
    }
    if (IsKeyDown(KEY_RIGHT) || rightStickX > 0)
    {
        player.move(WEST);
        camPos.z -= 0.1f;
    }

    if (IsKeyDown(KEY_ENTER))
    {
        player.shootBullet();
    }
}

void Game::gamepadInit()
{
    TextFindIndex(TextToLower(GetGamepadName(gamepad)), PS_ALIAS);

    // Set axis deadzones
    leftStickDeadzoneX = 0.1f;
    leftStickDeadzoneY = 0.1f;
    rightStickDeadzoneX = 0.1f;
    rightStickDeadzoneY = 0.1f;
    leftTriggerDeadzone = -0.9f;
    rightTriggerDeadzone = -0.9f;
}

void Game::gamepadUpdate()
{

    if (IsGamepadAvailable(gamepad))
    {
        // Get axis values
        leftStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
        leftStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);
        rightStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X);
        rightStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y);
        leftTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER);
        rightTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER);

        // Calculate deadzones
        if (leftStickX > -leftStickDeadzoneX && leftStickX < leftStickDeadzoneX) leftStickX = 0.0f;
        if (leftStickY > -leftStickDeadzoneY && leftStickY < leftStickDeadzoneY) leftStickY = 0.0f;
        if (rightStickX > -rightStickDeadzoneX && rightStickX < rightStickDeadzoneX) rightStickX = 0.0f;
        if (rightStickY > -rightStickDeadzoneY && rightStickY < rightStickDeadzoneY) rightStickY = 0.0f;
        if (leftTrigger < leftTriggerDeadzone) leftTrigger = -1.0f;
        if (rightTrigger < rightTriggerDeadzone) rightTrigger = -1.0f;
    }
}

void Game::gamepadControl()
{
    
}

void Game::checkCollisions(BoundingBox t_a, BoundingBox t_b)
{
    player.collision(CheckCollisionBoxes(t_a, t_b));   

    
    if (CheckCollisionBoxSphere(enemy.getHitbox(), player.getBulletPositon(), 1.0f))
    {
        enemy.collision(true);
        player.despawnBullet();
        score += 10;
    }
    else
    {
        enemy.collision(false);
    }
}
