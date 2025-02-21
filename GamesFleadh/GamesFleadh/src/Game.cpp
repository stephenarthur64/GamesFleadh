#include "Game.h"
#include <cmath> // Used for abs()

Game::Game() : score(0), activeMap(1)
{
    leftStickX = 0.0f;
    leftStickY = 0.0f;
    rightStickX = 0.0f;
    rightStickY = 0.0f;
    leftTrigger = 0.0f;
    rightTrigger = 0.0f;
    billSpeed = 0.2f;
}

Game::~Game()
{
    UnloadTexture(heightmapTexture);     // Unload texture
    UnloadModel(heightmapModel);         // Unload model
    UnloadImage(heightmapImage);         // Unload heightmap image from RAM, already uploaded to VRAM
    
    UnloadShader(skybox.materials[0].shader);// Skybox memory management
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(skybox);
    
    UnloadMusicStream(bgm);
    CloseAudioDevice();
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
    ToggleFullscreen();
    InitAudioDevice();

    // Define our custom camera to look into our 3d world
    camera = { 0 };
    camPos = { 0.0f, 4.0f, -2.0f };             // Determines player's starting position!
    camera.position = camPos;                   // Camera position
    camera.target = { 0.0f, 0.0f, -2300.0f };   // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };           // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                        // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;     // Camera projection type

    loadAssets();
    
    m_tileCurrent = 0; // Initialise index for m_terrainTileCollection
    m_tileNext = 1;

    m_terrainTileCollection[m_tileCurrent].tileIsCurrent(true); // Sets tile position in world
    m_terrainTileCollection[m_tileNext].tileIsCurrent(false); // Sets tile position to 'next'
    
    std::cout << "Furniture is set to: " << m_terrainTileCollection[0].getFurniture()[0].m_inPlay << "\n";

    setupSkybox();
    
    gamepadInit();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    m_gameIsBeginning = false;
}

void Game::loadAssets()
{
    //imgPlacementTest = LoadImage("ASSETS/2D/Heightmaps/test1_EnemyPlacement01001RS.png");

    //heightmapImage = LoadImage("ASSETS/2D/Heightmaps/test1_3xWider_halfDark4_Rot_halfDark3.png");
    //heightmapTexture = LoadTextureFromImage(heightmapImage);        

    // There should be a line below for every tile in the game (currently has a duplicate tile)
    m_terrainTileCollection.push_back(Tile(ASSET_HEIGHTMAP_01, ASSET_FURNITUREMAP_01, ASSET_TILE_MODEL_01, GULLY_DIFFUSE_01a));
    m_terrainTileCollection.push_back(Tile(ASSET_HEIGHTMAP_01, ASSET_FURNITUREMAP_01, ASSET_TILE_MODEL_01, GULLY_DIFFUSE_01));

    

    healthBar = LoadTexture("ASSETS/2D/UI/HealthBar.png");

    bill = LoadTexture("ASSETS/2D/Crosshair/crosshair.png");
    source = { 0.0f, 0.0f, (float)bill.width, (float)bill.height };
    billUp = { 0.0f, 1.0f, 0.0f };
    size = { source.width / source.height, 1.0f };
    origin = Vector2Scale(size, 0.5f);
        
    player.init();
    billPositionStatic = { 2.0f,2.0f,3.0f };

    for (int i = 0; i < maxSwarmer; i++)
    {
        swarmer[i].init();
    }
    
    // placeObjectsFromImage(imgPlacementTest);

    /*for (int i = 0; i < maxStreetFurniture; i++)
    {
        streetF[i].init();
    }*/

    /*for (int i = 0; i < MAX_MUSHROOMS; i++)
    {
        mushroom[i].init();
        if (i != mushroomOnMap)
        {
            mushroom[i].spawn({ -1.0f, 2.0f, -79.0f });
        }
        mushroom[i].spawnFeeder();
    }
    mushroom[0].playerDetected(true, player.getPosition());*/

    bgm = LoadMusicStream("ASSETS/Audio/Music/hiveMindSet.wav");
    SetMusicVolume(bgm, 0.2);
   // PlayMusicStream(bgm);
}

void Game::setupSkybox()
{
    cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    skybox = LoadModelFromMesh(cube);

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

    Image skyboxImage = LoadImage("ASSETS/3D/Skybox/skyBox.png");
    skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(skyboxImage, CUBEMAP_LAYOUT_AUTO_DETECT);    // CUBEMAP_LAYOUT_PANORAMA
    UnloadImage(skyboxImage);
}

void Game::render()
{
    BeginDrawing();

    ClearBackground({ RAYWHITE });

    BeginMode3D(camera);

    // SKYBOX RENDER 
    rlDisableBackfaceCulling(); // We are inside the cube, we need to disable backface culling!
    rlDisableDepthMask();
    DrawModel(skybox, Vector3{ 0, 0, 0 }, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
    // SKYBOX RENDER ENDS

    player.render();

    DrawBillboardPro(camera, bill, source, billPositionRotating, billUp, size, origin, rotation, WHITE);

    

    for (Tile& tileToDraw : m_terrainTileCollection)
    {
        tileToDraw.render();
    }

    for (int i = 0; i < maxSwarmer; i++)
    {
        swarmer[0].render();
        swarmer[0].renderBoom(camera);
    }

    /*for (int i = 0; i < maxStreetFurniture; i++)
    {
        streetF[i].render();
    }*/
    
    //DrawSphereWires(Vector3{ 0.0f, 0.0f, 0.0f }, 0.25f, 8, 8, ORANGE); // Marks origin.
    //DrawSphereWires(Vector3{ 0.0f, 4.0f, 0.0f }, 0.25f, 8, 8, ORANGE);
    //DrawSphereWires(Vector3{ 0.0f, 8.0f, 0.0f }, 0.25f, 8, 8, ORANGE);
    //DrawSphereWires(Vector3{ 0.0f,0.0f,-64.0f }, 0.25f, 6, 6, RED);
    //DrawSphereWires(Vector3{ 0.0f,4.0f,-64.0f }, 0.25f, 6, 6, RED);
    //DrawSphereWires(Vector3{ 0.0f,8.0f,-64.0f }, 0.25f, 6, 6, RED);
    //DrawSphereWires(Vector3{ 0.0f,2.0f, 0.0f  }, 0.25f, 6, 6, BLUE);
    //DrawSphereWires(Vector3{ 0.0f,2.0f, -64.0f}, 0.25f, 6, 6, BLUE);
    
    //DrawSphereWires(heightMapBounds.min, 0.5f, 6, 6, GREEN);
    //DrawSphereWires(heightMapBounds.max, 0.5f, 6, 6, PURPLE);

    DrawSphere(objectPlacementTest, 2.0f, ORANGE);

    DrawGrid(20, 1.0f);
    EndMode3D();

    DrawRectangleRec(player.getHealthBar(), GREEN);
    DrawTexture(healthBar, 0, 1000, WHITE);
   
    DrawText(TextFormat("PLAYER Z POSITION: %f", player.getPosition().z), 10, 430, 10, RED);
    DrawText(TextFormat("PLAYER Y POSITION: %f", player.getPosition().y), 10, 440, 10, RED);
    DrawText(TextFormat("PLAYER X POSITION: %f", player.getPosition().x), 10, 450, 10, RED);
    DrawText(TextFormat("SCORE: %i", score), 10, 70, 25, RED);
    /*for (int i = 0; i < MAX_MUSHROOMS; i++)
    {
        if (mushroom[i].isActive())
        {
            DrawText(TextFormat("FEEDER KILLED: +%i SCORE", 10), 10, 90, 15, RED);
        }
    }
    if (swarmer[0].isActive())
    {
        DrawText(TextFormat("SWARMER KILLED: +%i SCORE", 10), 10, 90, 15, RED);
    }
    DrawFPS(10, 30);

    /*DrawText((TextFormat("PLAYER XPos: %f, YPos: %f, ZPos: %f", player.getPosition().x, player.getPosition().y, player.getPosition().z)), 10, 10, 32, GREEN);
    DrawText((TextFormat("NormalX: %f, NormalZ: %f", worldNormalX, worldNormalZ)), 10, 45, 32, ORANGE);
    DrawText((TextFormat("TexU: %f, TexV: %f", texUcoord, texVcoord)), 10, 90, 32, PURPLE);
    DrawText((TextFormat("World Y Normal: %f", worldYNormalFromCol)), 10, 135, 32, BROWN);
    DrawText((TextFormat("World Y Pos: %f", worldYPos)), 10, 170, 32, SKYBLUE);
    DrawText((TextFormat("CAMERA XPos: %f, YPos: %f, ZPos: %f", camPos.x, camPos.y, camPos.z)), 10, 202, 32, GREEN);
    //DrawText((TextFormat("Map 01 Position x %f, y %f, z %f", mapPosition.x, mapPosition.y, mapPosition.z)), 10, 247, 32, ORANGE);
    //DrawText((TextFormat("Map 02 Position x %f, y %f, z %f", mapPosition2.x, mapPosition2.y, mapPosition2.z)), 10, 280, 32, SKYBLUE);
    
    DrawText((TextFormat("BoundingBoxMin: x %f, y %f, z %f", heightMapBounds.min.x, heightMapBounds.min.y, heightMapBounds.min.z)), 10, 316, 32, GREEN);
    DrawText((TextFormat("BoundingBoxMax: x %f, y %f, z %f", heightMapBounds.max.x, heightMapBounds.max.y, heightMapBounds.max.z)), 10, 340, 32, PURPLE);*/

    EndDrawing();
}

void Game::update()
{
    UpdateMusicStream(bgm);
    gamepadUpdate();
    inputControl();
    player.updateZPos(camPos.z - playerZOffsetFromCamera);
    player.faceCrosshair(billPositionRotating);

    swarmer->checkDistanceFromPlayer(player.getPosition());
    swarmer->update();

    distanceStatic = Vector3Distance(camera.position, billPositionStatic);
    distanceStatic += 2.0f;
    distanceRotating = Vector3Distance(camera.position, billPositionRotating);
    /*for (int i = 0; i < MAX_MUSHROOMS; i++)
    {
        mushroom[i].update();
    }*/
    mapMove(); // Repositions terrain meshes based on camera X (distance/z) pos

    player.collision(m_terrainTileCollection[m_tileCurrent].isColliding(player.getPosition() + PLAYER_COLLISION_OFFSET_FRONT));
    player.collision(m_terrainTileCollection[m_tileCurrent].isColliding(player.getPosition() + PLAYER_COLLISION_OFFSET_LATERAL));
    player.collision(m_terrainTileCollection[m_tileCurrent].isColliding(player.getPosition() - PLAYER_COLLISION_OFFSET_LATERAL));
    
    m_terrainTileCollection[m_tileCurrent].checkFurnitureItemsCollision(player.getHitbox());
    // checkCollisions(player.getHitbox(), mushroom[mushroomOnMap].getEnemyHitbox());

    for (Tile& item : m_terrainTileCollection)
    {
        item.update(Vector3Zero()); // RS: THIS NEEDS TO BE CHANGED, TEMPORARY PARAMETER!
    }

    player.updateBullet();
    camera.position = camPos;
    
    player.update();
    cameraMove();
    UpdateCamera(&camera, CAMERA_PERSPECTIVE);

}

void Game::inputControl()
{
    if (IsKeyDown(KEY_W) || leftStickY < 0)
    {
        camDirection = 0.0f;
        if (leftStickY < 0)
        {
           //camDirection -= camSpeed * (-leftStickY);
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
            //camDirection = camSpeed * (-leftStickY);
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
    }
    if (IsKeyDown(KEY_DOWN))
    {
        player.move({0,1,0});
    }
    if (IsKeyDown(KEY_LEFT))
    {
        player.move({-1,0,0});
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        player.move({1,0,0});
    }

    if (IsKeyReleased(KEY_SPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1))
    {// RS: Toggle! Is nice, you like.
        autoScroll = !autoScroll;
    }

    if (IsKeyPressed(KEY_Z))
    {
        player.collision(true);
    }

    if (IsKeyReleased(KEY_X))
    {
        std::cout << "\nPlacing objects.\n";
        // placeObjectsFromImage(imgPlacementTest);
    }
    if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        player.shootBullet(billPositionRotating);
    }

    Vector3 normVelocity = Vector3Normalize({ leftStickX, leftStickY, 0 });

    player.move(normVelocity);
    camPos += normVelocity * Vector3{ 0.1, -0.1, 0.1 };

    crosshairMove();
    billPositionRotating.z = player.getPosition().z - 3.0f;

    if (autoScroll)
    {
        camPos.z += -0.1f;
    }
}

void Game::crosshairMove()
{
    if (IsKeyDown(KEY_I))
    {
        keyboardY = -1.0f;
    }
    else if (IsKeyDown(KEY_K))
    {
        keyboardY = 1.0f;
    }
    else
    {
        keyboardY = 0.0f;
    }

    if (IsKeyDown(KEY_J))
    {
        keyboardX = -1.0f;
    }
    else if (IsKeyDown(KEY_L))
    {
        keyboardX = 1.0f;
    }
    else
    {
        keyboardX = 0.0f;
    }

    billPositionRotating.x += billSpeed * (rightStickX + keyboardX);
    billPositionRotating.y += billSpeed * (- (rightStickY + keyboardY));
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

void Game::gameBegins()
{
    
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

        SetGamepadVibration(gamepad, 1.0f, 1.0f, 100.0f);
    }
}

void Game::checkCollisions(BoundingBox t_a, BoundingBox t_b)
{
    int collide = 0;

    player.collision(CheckCollisionBoxes(t_a, t_b));   

    /*for (int i = 0; i < player.getBulletMax(); i++)
    {
        if (CheckCollisionBoxSphere(mushroom[mushroomOnMap].getFeederHitbox(), player.getBulletPositon(i), 1.0f))
        {
            collide = 1;
            mushroom[mushroomOnMap].setCollisions(true);
            player.despawnBullet(i);
            score += 10;
        }
        if (CheckCollisionBoxSphere(swarmer[0].getHitbox(), player.getBulletPositon(i), 1.0f))
        {
            swarmer[0].collision(true);
            player.despawnBullet(i);
            score += 10;
        }
    }

    if (collide != 1)
    {
        mushroom[mushroomOnMap].setCollisions(false);
    }*/
}

void Game::mapMove()
{
    if (player.getPosition().z > -64.0f - playerZOffsetFromCamera) return;

    m_tileCurrent = m_tileNext;

    while (m_tileNext == m_tileCurrent)
    {
        m_tileNext = rand() % m_terrainTileCollection.size();
    }
    
    // std::cout << "Furniture is set to: " << m_terrainTileCollection[0].getFurniture()[0].m_inPlay << "\n";

    for (Tile& item : m_terrainTileCollection)
    {
        item.setInPlay(false);
    }

    m_terrainTileCollection[m_tileCurrent].tileIsCurrent(true);
    m_terrainTileCollection[m_tileNext].tileIsCurrent(false);

    float mapLength = 64.0f;

    /*mushroomOnMap = 1;
    mushroom[1].spawn({ -1.0f, 2.0f, -15.0f });
    mushroom[1].spawnFeeder();
    mushroom[1].playerDetected(true, player.getPosition());

    mushroom[0].spawn({ -1.0f, 2.0f, -mapLength - 15.0f});
    mushroom[0].spawnFeeder();
    mushroom[0].playerDetected(false, {0,0,0});
    */
    camPos.z = 0.0f;
}

void Game::cameraMove()
{
    float speed = 0.2f;

    if (player.getPosition().x < lowerLimit.x && camPos.x > player.getPosition().x)
    {
        camPos.x -= speed;
        lowerLimit.x -= speed;
        upperLimit.x -= speed;
    }
    if (player.getPosition().y < lowerLimit.y && camPos.y > player.getPosition().y)
    {
        camPos.y -= speed;
        lowerLimit.y -= speed;
        upperLimit.y -= speed;
    }

    if (player.getPosition().x > upperLimit.x && camPos.x < player.getPosition().x)
    {
        camPos.x += speed;
        upperLimit.x += speed;
        lowerLimit.x += speed;
    }
    if (player.getPosition().y > upperLimit.y && camPos.y < player.getPosition().y)
    {
        camPos.y += speed;
        upperLimit.y += speed;
        lowerLimit.y += speed;
    }
    camera.target = billPositionRotating;
    camera.target.z = billPositionRotating.z - 15.0f;
}


