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

// Bunch of variables RoB has made global in order to get skybox in quickly - can probably throw this in header!
Mesh cube;
Model skybox;

Game::Game() : score(0), activeMap(1)
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

    // Skybox memory management
    //UnloadShader(skybox.materials[0].shader);
    //UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    ///UnloadModel(skybox);        // Unload skybox model
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

    // BEGIN SKYBOX INIT ----------------------------------------------------------------------------------
    // RS: Should most of the following be in loadAssets()?
    // Load skybox model
    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model skybox = LoadModelFromMesh(cube);

    // Load skybox shader and set required locations
    // NOTE: Some locations are automatically set at shader loading
    skybox.materials[0].shader = LoadShader(TextFormat("ASSETS/shaders/glsl%i/skybox.vs", GLSL_VERSION),
        TextFormat("ASSETS/shaders/glsl%i/skybox.fs", GLSL_VERSION));

    int cubeMapMat = MATERIAL_MAP_CUBEMAP;
    SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), &cubeMapMat, SHADER_UNIFORM_INT);

    // Load cubemap shader and setup required shader locations
    Shader shdrCubemap = LoadShader(TextFormat("ASSETS/shaders/glsl%i/cubemap.vs", GLSL_VERSION),
        TextFormat("ASSETS/shaders/glsl%i/cubemap.fs", GLSL_VERSION));

    int cubeMapVal = 0;
    SetShaderValue(shdrCubemap, GetShaderLocation(shdrCubemap, "equirectangularMap"), &cubeMapVal, SHADER_UNIFORM_INT);

    char skyboxFileName[256] = { 0 };

    Image skyboxImage = LoadImage("ASSETS/3D/Skybox/Skybox_example.png");
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(skyboxImage, CUBEMAP_LAYOUT_AUTO_DETECT);    // CUBEMAP_LAYOUT_PANORAMA
    UnloadImage(skyboxImage);
    // CONCLUDE SKYBOX INIT -----------------------------------------------------------------------------

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
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
    
    player.init();

    for (int i = 0; i < MAX_MUSHROOMS; i++)
    {
        mushroom[i].init();
        if (i != mushroomOnMap)
        {
            mushroom[i].spawn({ 2.0f, 1.0f, -90.0f });
        }
        mushroom[i].spawnEnemy();
    }
    mushroom[0].playerDetected(true);
}

void Game::render()
{
    BeginDrawing();

    ClearBackground({ 22, 22, 31, 255 });

    BeginMode3D(camera);

    // SKYBOX STUFF STARTS
    // We are inside the cube, we need to disable backface culling!
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(skybox, Vector3{ 0, 0, 0 }, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
    // SKYBOX STUFF ENDS

    DrawModel(heightmapModel, mapPosition, 4.0f, { 147, 204, 147, 255 });
    DrawModel(heightmapModel, mapPosition2, 4.0f, { 147, 204, 147, 255 });
    
    player.render();
    for (int i = 0; i < MAX_MUSHROOMS; i++)
    {
        mushroom[i].render();
    }

    DrawGrid(20, 1.0f);

    EndMode3D();

    DrawText(TextFormat("PLAYER Z POSITION: %f", player.getPositon().z), 10, 430, 10, RED);
    DrawText(TextFormat("SCORE: %i", score), 10, 70, 25, RED);
    DrawFPS(10, 30);

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
    for (int i = 0; i < MAX_MUSHROOMS; i++)
    {
        mushroom[i].update();
    }
    mapMove(); // Repos terrain meshes based on camera X (distance/z) pos
    /*camPos.z -= 6;
    camPos.y = 10;*/

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
    checkCollisions(player.getHitbox(), mushroom[mushroomOnMap].getEnemyHitbox());
    UpdateCamera(&camera, CAMERA_PERSPECTIVE);

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

    if (IsKeyDown(KEY_UP))
    {
        player.move({0, -1, 0});
        camPos.y += 0.1f;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        player.move({0,1,0});
        camPos.y -= 0.1f;
    }
    if (IsKeyDown(KEY_LEFT))
    {
        player.move({-1,0,0});
        camPos.x -= 0.1f;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        player.move({1,0,0});
        camPos.x += 0.1f;
    }

    Vector3 normVelocity = Vector3Normalize({ rightStickX, rightStickY, 0 });

    player.move(normVelocity);
    camPos += normVelocity * Vector3{ 0.1, -0.1, 0.1 };

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
        if (CheckCollisionBoxSphere(mushroom[mushroomOnMap].getEnemyHitbox(), player.getBulletPositon(i), 1.0f))
        {
            collide = 1;
            mushroom[mushroomOnMap].setCollisions(true);
            player.despawnBullet(i);
            score += 10;
        }
    }

    if (collide != 1)
    {
        mushroom[mushroomOnMap].setCollisions(false);
    }
}

void Game::mapMove()
{
    //{ 35.0f, 0.0f, -70.0f }; 
    float newMapX = mapPosition.x;
    float newMapX2 = mapPosition2.x;

    if (player.getPositon().z < -74.0f && activeMap == 1)
    {
        mapPosition2 = { 35.0f, 0.0f, -70.0f };
        mapPosition = { 35.0f, 0.0f, -130.0f };
        activeMap = 2;
        camPos.z = -9.2f;
        player.resetToOrigin();
        mushroomOnMap = 0;
        mushroom[0].spawn({2.0f, 1.0f, -30.0f});
        mushroom[0].spawnEnemy();
        mushroom[0].playerDetected(true);

        mushroom[1].spawn({ 2.0f, 1.0f, -90.0f });
        mushroom[1].spawnEnemy();
        mushroom[1].playerDetected(false);
    }

    if (player.getPositon().z < -74.0f && activeMap == 2)
    {
        mapPosition = { 35.0f, 0.0f, -70.0f };
        mapPosition2 = { 35.0f, 0.0f, -130.0f }; 
        activeMap = 1;
        camPos.z = -9.2f;
        player.resetToOrigin();
        mushroomOnMap = 1;

        mushroom[1].spawn({ 2.0f, 1.0f, -30.0f });
        mushroom[1].spawnEnemy();
        mushroom[1].playerDetected(true);

        mushroom[0].spawn({ 2.0f, 1.0f, -90.0f });
        mushroom[0].spawnEnemy();
        mushroom[0].playerDetected(false);
    }

    
    
}
