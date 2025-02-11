#include "Game.h"
#include <cmath> // Used for abs()

// Bunch of variables RoB has made global in the name of making his code work quickly
const Vector3 mapSize = { 16, 8, 16 };
float worldNormalX;
float worldNormalZ;
float texUcoord;
float texVcoord;

Color colorFromPosition;
float worldYNormalFromCol;
float worldYPos;

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
    UnloadImage(heightmapImage);             // Unload heightmap image from RAM, already uploaded to VRAM
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
    camPos = { 7.0f, 2.0f, 0.0f };
    camera.position = camPos;     // Camera position
    camera.target = { 0.0f, 0.0f, -2300.0f };          // Camera looking at point
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
    DrawModel(heightmapModel, mapPosition2, 4.0f, GREEN);
    DrawModel(*player.getModel(), player.getPositon(), 1.0f, player.getColour());
    DrawModel(*enemy.getModel(), enemy.getPositon(), 1.0f, enemy.getColour());
    for (int i = 0; i < player.getBulletMax(); i++)
    {
        DrawModel(*player.getBulletModel(i), player.getBulletPositon(i), 0.5f, BLUE);
        DrawBoundingBox(player.getBulletHitBox(i), RED);
    }
    //DrawBoundingBox(player.getHitbox(), RED);
    DrawBoundingBox(enemy.getHitbox(), GREEN);
    


    DrawGrid(20, 1.0f);

    EndMode3D();

    DrawText(TextFormat("PLAYER Z POSITION: %f", player.getPositon().z), 10, 430, 10, RED);
    DrawText(TextFormat("SCORE: %i", score), 10, 70, 25, RED);
    DrawFPS(10, 10);

    //DrawText((TextFormat("XPos: %f, YPos: %f, ZPos: %f", player.getPositon().x, player.getPositon().y, player.getPositon().z)), 10, 10, 32, GREEN);
    //DrawText((TextFormat("NormalX: %f, NormalZ: %f", worldNormalX, worldNormalZ)), 10, 45, 32, ORANGE);
    //DrawText((TextFormat("TexU: %f, TexV: %f", texUcoord, texVcoord)), 10, 90, 32, PURPLE);
    //DrawText((TextFormat("World Y Normal: %f", worldYNormalFromCol)), 10, 135, 32, BROWN);
    //DrawText((TextFormat("World Y Pos: %f", worldYPos)), 10, 170, 32, SKYBLUE);

    EndDrawing();
}

void Game::update()
{
    gamepadUpdate();
    inputControl();
    player.updateZPos(camPos.z - 5.0f);
    player.update();
    mapMove(); // Repos terrain meshes based on camera X (distance/z) pos

    // RoB'S HEIGHT MAP COLLISION STUFF STARTS HERE
    // Get Normalised Coord
     worldNormalX = (player.getPositon().x + abs(mapPosition.x)) / mapSize.x;
     worldNormalZ = (player.getPositon().z + abs(mapPosition.z)) / mapSize.z;
     texUcoord = worldNormalX * heightmapImage.width;
     texVcoord = worldNormalZ * heightmapImage.height;

    // Clampity clamp (make this a helper function?) 0.001f - just to be sure we don't get OOBounds error
    if (texUcoord > heightmapImage.height - 0.001f) texUcoord = heightmapImage.height - 0.001f;
    if (texUcoord < 0) texUcoord = 0;

    if (texVcoord > heightmapImage.width - 0.001f) texVcoord = heightmapImage.width - 0.001f;
    if (texVcoord < 0) texVcoord = 0;

     colorFromPosition = GetImageColor(heightmapImage, texUcoord, texVcoord);
     worldYNormalFromCol = colorFromPosition.r / 255.0f;
     worldYPos = worldYNormalFromCol * mapSize.y;

    if (player.getPositon().y <= worldYPos)
    {
        player.collision(true);
    }
    else
    {
        player.collision(false);
    }
    // RoB's HEIGHT MAP COLLISION STUFF ENDS HERE

    player.updateBullet();
    camera.position = camPos;
    checkCollisions(player.getHitbox(), enemy.getHitbox());
    UpdateCamera(&camera, CAMERA_PERSPECTIVE);

}

void Game::loadAssets()
{
    heightmapImage = LoadImage("ASSETS/heightmapWider.png");     // Load heightmap image (RAM)
    heightmapTexture = LoadTextureFromImage(heightmapImage);        // Convert image to texture (VRAM)

    
    heightmapMesh = GenMeshHeightmap(heightmapImage, mapSize); // Generate heightmap mesh (RAM and VRAM)
    heightmapModel = LoadModelFromMesh(heightmapMesh);                  // Load model from generated mesh
    heightmapModel.transform = MatrixRotateXYZ({ DEG2RAD * 270.0f, DEG2RAD * 270.0f, DEG2RAD * 270.0f });

    heightmapModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = heightmapTexture; // Set map diffuse texture
    mapPosition = { 35.0f, 0.0f, -70.0f };           // Define model position
    mapPosition2 = { 35.0f, 0.0f, -130.0f };

    

    *player.getModel() = LoadModel("ASSETS/RS/animTest.glb");
    *enemy.getModel() = LoadModel("ASSETS/RS/bugProto01.glb");
    enemy.getModel()->transform = MatrixRotateXYZ({ 0, DEG2RAD * 90.0f, 0 });
    for (int i = 0; i < player.getBulletMax(); i++)
    {
        *player.getBulletModel(i) = LoadModel("ASSETS/RS/bulletProto.glb");
    }

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
        camPos.z += camDirection;
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
        camPos.z += camDirection;
    }
    
    Command* command = Input::getInstance()->handleInput();
    if (command)
    {
        command->execute(&player);
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
        player.move(WEST);
        camPos.x -= 0.1f;
    }
    if (IsKeyDown(KEY_RIGHT) || rightStickX > 0)
    {
        player.move(EAST);
        camPos.x += 0.1f;
    }
    

    if (IsKeyPressed(KEY_ENTER))
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

void Game::checkCollisions(BoundingBox t_a, BoundingBox t_b)
{
    int collide = 0;

    player.collision(CheckCollisionBoxes(t_a, t_b));   

    for (int i = 0; i < player.getBulletMax(); i++)
    {
        if (CheckCollisionBoxSphere(enemy.getHitbox(), player.getBulletPositon(i), 1.0f))
        {
            collide = 1;
            enemy.collision(true);
            player.despawnBullet(i);
            score += 10;
        }
    }

    if (collide != 1)
    {
        enemy.collision(false);
    }
}

void Game::mapMove()
{
    //{ 35.0f, 0.0f, -70.0f }; 
    float newMapX = mapPosition.x;
    float newMapX2 = mapPosition2.x;

    if (player.getPositon().z < -74.0f)
    {
        mapPosition2 = { 35.0f, 0.0f, -70.0f };
        mapPosition = { 35.0f, 0.0f, -130.0f };
        camPos.z = -9.2f;
        player.resetToOrigin();
    }

    if (camPos.x < newMapX + 40.0f)
    {
       // mapPosition2.x = newMapX - 50.0f;
    }
}
