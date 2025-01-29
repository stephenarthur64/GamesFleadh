#include "Game.h"

Game::Game()
{
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
    camera.position = { 8.0f, 2.0f, 0.0f };     // Camera position
    camera.target = { -230.0f, 0.0f, 0.0f };          // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };              // Camera up vector (rotation towards target)
    camera.fovy = 90.0f;                                    // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;                 // Camera projection type
    
    loadAssets();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
}

void Game::render()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);

    DrawModel(heightmapModel, mapPosition, 1.0f, WHITE);
    DrawModel(heightmapModel, mapPosition2, 1.0f, WHITE);
    DrawModel(*player.getModel(), {newCamX - 5.0f, player.getPositon().y, player.getPositon().z }, 1.0f, GREEN);

    DrawGrid(20, 1.0f);

    EndMode3D();

    DrawFPS(10, 10);

    EndDrawing();
}

void Game::update()
{
    inputControl();
    camera.position = { newCamX, 2.0f, 0.0f };

    UpdateCamera(&camera, CAMERA_PERSPECTIVE);
}

void Game::loadAssets()
{
    heightmapImage = LoadImage("ASSETS/heightmap4.png");     // Load heightmap image (RAM)
    heightmapTexture = LoadTextureFromImage(heightmapImage);        // Convert image to texture (VRAM)

    heightmapMesh = GenMeshHeightmap(heightmapImage, { 16, 8, 16 }); // Generate heightmap mesh (RAM and VRAM)
    heightmapModel = LoadModelFromMesh(heightmapMesh);                  // Load model from generated mesh

    heightmapModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = heightmapTexture; // Set map diffuse texture
    mapPosition = { -8.0f, 0.0f, -8.0f };           // Define model position
    mapPosition2 = { -23.0f, 0.0f, -8.0f };

    UnloadImage(heightmapImage);             // Unload heightmap image from RAM, already uploaded to VRAM

    *player.getModel() = LoadModel("ASSETS/RS/bugProto01.glb");
}

void Game::inputControl()
{
    if (IsKeyDown(KEY_W))
    {
        newCamX -= 0.1f;
    }
    if (IsKeyDown(KEY_S))
    {
        newCamX += 0.1f;
    }


    if (IsKeyDown(KEY_UP))
    {
        player.move(NORTH);
    }
    if (IsKeyDown(KEY_DOWN))
    {
        player.move(SOUTH);
    }
    if (IsKeyDown(KEY_LEFT))
    {
        player.move(EAST);
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        player.move(WEST);
    }
}