#include "Game.h"
#include <cmath> // Used for abs()

bool g_renderWireDebug = false;
bool g_render2DDebug = false;

Game::Game() : score(0), activeMap(1), state(GameState::GAMEPLAY)
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
    srand(time(nullptr));

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
    //ToggleFullscreen();
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

    m_terrainTileCollection[m_tileCurrent].makeTileCurrent(true); // Sets tile position in world
    m_terrainTileCollection[m_tileNext].makeTileCurrent(false); // Sets tile position to 'next'
    
    // std::cout << "Furniture is set to: " << m_terrainTileCollection[0].getFurniture()[0].m_inPlay << "\n";

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
    m_terrainTileCollection.push_back(Tile(ASSET_HEIGHTMAP_02, ASSET_FURNITUREMAP_02, ASSET_TILE_MODEL_01, GULLY_DIFFUSE_02));
    m_terrainTileCollection.push_back(Tile(ASSET_HEIGHTMAP_03, ASSET_FURNITUREMAP_03, ASSET_TILE_MODEL_02, GULLY_DIFFUSE_03));
    m_terrainTileCollection.push_back(Tile(ASSET_HEIGHTMAP_04, ASSET_FURNITUREMAP_04, ASSET_TILE_MODEL_03, GULLY_DIFFUSE_04));
    // m_terrainTileCollection.push_back(Tile(ASSET_HEIGHTMAP_01, ASSET_FURNITUREMAP_01, ASSET_TILE_MODEL_01, GULLY_DIFFUSE_01));

    for (int i = 0; i < MAX_SWARMERS; i++)
    {
        Vector3 pos = m_terrainTileCollection[m_tileCurrent].getSwarmerPos(i);
        swarmer[i].spawn(pos, pos.x + 5.0f, pos.x - 5.0f);
    }

    gameFont = LoadFont("ASSETS/2D/Font/BuzzzFont.ttf");

    fogOpacity = WHITE;
    fogOpacity.a = 0;
    fogVignette = LoadTexture("ASSETS/2D/Fog/OrangeVignette.png");
    fogBar = LoadTexture("ASSETS/2D/UI/FogBar.png");
    fogGradient = LoadTexture("ASSETS/2D/UI/FogGradient.png");

    gradientSource = { 0, 0, (float)fogGradient.width, (float)fogGradient.height};
    gradientDest = { SCREEN_WIDTH - 30, 370, (float)fogGradient.width + 10, (float)fogGradient.height};

    healthBar = LoadTexture("ASSETS/2D/UI/HealthBar.png");

    bill = LoadTexture("ASSETS/2D/Crosshair/crosshair.png");
    source = { 0.0f, 0.0f, (float)bill.width, (float)bill.height };
    billUp = { 0.0f, 1.0f, 0.0f };
    size = { source.width / source.height, 1.0f };
    origin = Vector2Scale(size, 0.5f);
        
    player.init();
    billPositionStatic = { 2.0f,2.0f,3.0f };

    for (int i = 0; i < MAX_SWARMERS; i++)
    {
        swarmer[i].init();
    }
    //RS: Can we get rid of this now?
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

    m_terrainTileCollection[m_tileCurrent].makeFeederSeekPlayer(true, player);

    bgm = LoadMusicStream("ASSETS/Audio/Music/hiveMindSet.wav");
    SetMusicVolume(bgm, 0.1);
    //PlayMusicStream(bgm);
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

    Image skyboxImage = LoadImage("ASSETS/3D/Skybox/skybox_cloudy.png"); // <-------------------------------------------------------- HERE -----------------------------------------------------------
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

    for (int i = 0; i < MAX_SWARMERS; i++)
    {
        swarmer[i].render();
        swarmer[i].renderBoom(camera);
    }

    /*for (int i = 0; i < maxStreetFurniture; i++)
    {
        streetF[i].render();
    }*/
    
    if (g_renderWireDebug)
    {
        //DrawSphereWires(Vector3{ 0.0f, 0.0f, 0.0f }, 0.25f, 8, 8, ORANGE); // Marks origin.
        //DrawSphereWires(Vector3{ 0.0f, 4.0f, 0.0f }, 0.25f, 8, 8, ORANGE);
        //DrawSphereWires(Vector3{ 0.0f, 8.0f, 0.0f }, 0.25f, 8, 8, ORANGE);
        //DrawSphereWires(Vector3{ 0.0f,0.0f,-64.0f }, 0.25f, 6, 6, RED);
        //DrawSphereWires(Vector3{ 0.0f,4.0f,-64.0f }, 0.25f, 6, 6, RED);
        //DrawSphereWires(Vector3{ 0.0f,8.0f,-64.0f }, 0.25f, 6, 6, RED);
        //DrawSphereWires(Vector3{ 0.0f,2.0f, 0.0f  }, 0.25f, 6, 6, BLUE);
        //DrawSphereWires(Vector3{ 0.0f,2.0f, -64.0f}, 0.25f, 6, 6, BLUE);

        DrawSphereWires(heightMapBounds.min, 2.5f, 6, 6, GREEN);
        DrawSphereWires(heightMapBounds.max, 2.5f, 6, 6, PURPLE);

        DrawLine3D(g_furnCollisionItem, g_furnCollisionPlyr, PURPLE);

        //DrawSphere(objectPlacementTest, 2.0f, ORANGE);

        DrawGrid(20, 1.0f);
    }
    
    EndMode3D();

    if (g_render2DDebug)
    {      
        DrawFPS(10, 30);
        DrawTexture(fogVignette, 0, 0, fogOpacity);

        //DrawTexture(fogGradient, SCREEN_WIDTH - 45, 155, WHITE);
        //DrawTextureRec(fogGradient, gradientSource, { SCREEN_WIDTH - 45, 155 }, WHITE);

        //DrawText(TextFormat("PLAYER Z POSITION: %f", player.getPosition().z), 10, 430, 10, RED);
        //DrawText(TextFormat("PLAYER Y POSITION: %f", player.getPosition().y), 10, 440, 10, RED);
        //DrawText(TextFormat("PLAYER X POSITION: %f", player.getPosition().x), 10, 450, 10, RED);
        //DrawText(TextFormat("SCORE: %i", score), 10, 70, 25, RED);

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
    DrawRectangleRec(player.getHealthBar(), player.getHealthBarColour());
    DrawTexture(healthBar, 0, 1000, WHITE);

    DrawTexture(fogVignette, 0, 0, fogOpacity);
    //DrawTexture(fogGradient, SCREEN_WIDTH - 45, 155, WHITE);
    //DrawTextureRec(fogGradient, gradientSource, { SCREEN_WIDTH - 45, 155 }, WHITE);
    DrawTexturePro(fogGradient, gradientSource, gradientDest, {(float)fogGradient.width / 2.0f, (float)fogGradient.height / 2.0f }, 180.0f, WHITE);
    DrawTexture(fogBar, SCREEN_WIDTH - 60, 100, WHITE);
  
    DrawText(TextFormat("DIFF IN LIMITS: %f", diffBetweenLimits), 10, 440, 10, RED);
    //DrawText(TextFormat("SCORE: %i", score), 10, 70, 25, RED);
    DrawTextEx(gameFont, TextFormat("SCORE: %i", score), { (SCREEN_WIDTH / 2.0f) - 150, 20 }, 25, 5, WHITE);
    

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
    }
    else
    {
        DrawRectangleRec(player.getHealthBar(), player.getHealthBarColour());
        DrawTexture(healthBar, 0, 1000, WHITE);
        DrawTextEx(gameFont, TextFormat("SCORE: %i", score), { (SCREEN_WIDTH / 2.0f) - 150, 20 }, 25, 5, WHITE);
        DrawTexturePro(fogGradient, gradientSource, gradientDest, { (float)fogGradient.width / 2.0f, (float)fogGradient.height / 2.0f }, 180.0f, WHITE);
        DrawTexture(fogBar, SCREEN_WIDTH - 60, 100, WHITE);        
    }


    EndDrawing();
}

void Game::update()
{
    UpdateMusicStream(bgm);
    gamepadUpdate();
    inputControl();
    if (state == GameState::GAMEPLAY)
    {
        player.updateZPos(camPos.z - playerZOffsetFromCamera);
        player.faceCrosshair(billPositionRotating);

        for (int i = 0; i < MAX_SWARMERS; i++)
        {
            swarmer[i].checkDistanceFromPlayer(player.getPosition());
            swarmer[i].update();
        }


        mapMove(); // Repositions terrain meshes based on camera X (distance/z) pos

        for (Tile& item : m_terrainTileCollection)
        {
            item.update(player.getPosition());
        }

        player.updateBullet();
        camera.position = camPos;
        checkCollisions();
        player.update(camera.position, billPositionRotating);
    }
    else if (state == GameState::TITLE)
    {
        camera.target = player.getPosition();
    }
    cameraMove();
    UpdateCamera(&camera, CAMERA_PERSPECTIVE);
    //fogVisibility();
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
        player.setAuto(autoScroll);
        std::cout << "Good god.";
    }

    /*if (IsKeyPressed(KEY_Z))
    {
        player.collision(true);
    }*/

    //if (IsKeyReleased(KEY_X))
    //{
    //    std::cout << "\nPlacing objects.\n";
    //    // placeObjectsFromImage(imgPlacementTest);
    //}
    if (IsKeyPressed(KEY_ENTER) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        player.shootBullet(billPositionRotating);
    }

    if (IsKeyReleased(KEY_BACKSPACE))
    {
        std::cout << "Is this even working?";
        player.addHealth(10);
    }

    if (IsKeyReleased(KEY_F3))
    {
        std::cout << "\nToggling 2D Elements!\n";
        g_render2DDebug = !g_render2DDebug;
    }

    if (IsKeyReleased(KEY_F4))
    {
        std::cout << "\nToggling Debug Wireframes!\n";
        g_renderWireDebug = !g_renderWireDebug;
    }



    Vector3 normVelocity = Vector3Normalize({ leftStickX, leftStickY, 0 });

    player.move(normVelocity);
    camPos += normVelocity * Vector3{ 0.1, -0.1, 0.1 };

    crosshairMove();
    billPositionRotating.z = player.getPosition().z - 3.0f;

    if (player.isAuto())
    {// RS: How are we not doing this stuff with GETFRAMETIME(), are we barbarians?
        camPos.z += -0.12f;
    }

    if (m_reboundCounter > 0)
    {// RS: Shit be fucked up that I have to put this in INPUT.
        float frameTime = GetFrameTime();
        m_reboundCounter -= frameTime;
        camPos -= M_REBOUND_DIRECTION * m_reboundForce * frameTime;
    }

    Command* command = nullptr;
    if ((leftStickX == 0 && rightStickX == 0 && leftStickY == 0 && rightStickY == 0) && !IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2))
    {
        command = new NoInputCommand;
    }
    else
    {
        command = Input::getInstance()->handleInput();
    }
    if (command)
    {
        command->execute(&player);
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
    billPositionRotating.x = Clamp(billPositionRotating.x, player.getPosition().x - 5.0f, player.getPosition().x + 5.0f);
    billPositionRotating.y = Clamp(billPositionRotating.y, player.getPosition().y - 2.0f, player.getPosition().y + 2.5f);
}

void Game::reboundZ(Vector3 t_impactPoint)
{
    std::cout << "Rebound triggered.\n";
    m_reboundCounter = m_reboundCountMax;
    // m_reboundDirection = Vector3Normalize(m_position - t_impactPoint);
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

      //  SetGamepadVibration(gamepad, 1.0f, 1.0f, 100.0f);
    }
}

void Game::checkCollisions()
{
    int collide = 0;

    player.enemyCollision(false); // RS: At the start of the collision check, set collision to false.

    for (int i = 0; i < MAX_SWARMERS; i++)
    {
        if (CheckCollisionBoxSphere(swarmer[i].getHitbox(), player.getPosition(), 2.0f))
        {
            if (swarmer[i].getPosition().x < player.getPosition().x)
            {
                player.handleInput(EVENT_HIT_L);
            }
            if (swarmer[i].getPosition().x > player.getPosition().x)
            {
                player.handleInput(EVENT_HIT_R);
            }
            player.hitSound(1);
            player.enemyCollision(true);
            swarmer[i].collision(true);
            swarmer[i].handleInput(EVENT_ATTACK);
        }
    }

    for (int i = 0; i < player.getBulletMax(); i++)
    {
        if (m_terrainTileCollection[m_tileCurrent].checkFeederBulletCollision(player.getBulletPositon(i), 1.0f))
        {// Feeder collision set to true in Furniture (follow if statement above)
            player.despawnBullet(i);
            reduceFog();
            score += 10;
        }

        for (int j = 0; j < MAX_SWARMERS; j++)
        {
            if (CheckCollisionBoxSphere(swarmer[j].getHitbox(), player.getBulletPositon(i), 1.0f))
            {
                swarmer[j].collision(true);
                reduceFog();
                player.despawnBullet(i);
                score += 10;
            }
        }
    }

    if (m_terrainTileCollection[m_tileCurrent].isColliding(player.getPosition() + PLAYER_COLLISION_OFFSET_LATERAL))
    {// Colliding with terrain on the right
        player.worldCollision(true);
        player.handleInput(EVENT_COLLIDE_R);
        player.hitSound(0);
        player.rebound(player.getPosition() + PLAYER_COLLISION_OFFSET_LATERAL);
    }

    if (m_terrainTileCollection[m_tileCurrent].isColliding(player.getPosition() - PLAYER_COLLISION_OFFSET_LATERAL))
    {// Colliding with terrain on the left
        player.worldCollision(true);
        player.handleInput(EVENT_COLLIDE_L);
        player.hitSound(0);
        player.rebound(player.getPosition() - PLAYER_COLLISION_OFFSET_LATERAL);
    }

    if (m_terrainTileCollection[m_tileCurrent].isColliding(player.getPosition() + PLAYER_COLLISION_OFFSET_FRONT))
    {// Colliding with terrain in front
        player.worldCollision(true);
        player.hitSound(0);
        //reboundZ(PLAYER_COLLISION_OFFSET_FRONT - camPos);
    }

    // m_terrainTileCollection[m_tileCurrent].checkFurnitureItemsCollision(player.getHitbox()); // Deprecated, if we're just doing radius checks.

    m_collisionData = m_terrainTileCollection[m_tileCurrent].checkBoundsFurnitureItemsCollision(player.getPosition(), player.getCollisionRadius(), player.getHitbox());

    if (m_collisionData.collision)
    {
        std::cout << "Hitting a mushroom!\n\n";
        player.hitSound(0);
        player.enemyCollision(true);
        player.reboundFurniture(m_collisionData);
    }
    else
    {
        player.setAuto(autoScroll);
    }

    //if (m_terrainTileCollection[m_tileCurrent].checkRadialFurnitureItemsCollision(player.getPosition(), player.getCollisionRadius()))
    //{
    //    //std::cout << "Is this calling?\n\n";
    //    player.hitSound(0);
    //    //player.rebound(player.getPosition() - PLAYER_COLLISION_OFFSET_LATERAL);
    //    player.enemyCollision(true);
    //    player.reboundFurniture(g_lastFurnitureCollision);
    //}

    if (m_terrainTileCollection[m_tileCurrent].checkMudBombPlayerCollision(player.getHitbox()))
    {
        if (mudBombPosition < player.getPosition().x)
        {
            player.handleInput(EVENT_HIT_L);
        }
        if (mudBombPosition > player.getPosition().x)
        {
            player.handleInput(EVENT_HIT_R);
        }
        player.hitSound(1);
        player.poisonPlayer(true);
    }
}

void Game::mapMove()
{
    if (player.getPosition().z > -64.0f - playerZOffsetFromCamera) return;

    m_tileCurrent = m_tileNext;

    while (m_tileNext == m_tileCurrent)
    {
        m_tileNext = rand() % m_terrainTileCollection.size();
        std::cout << "m_tileNext is " << m_tileNext << ".\n";
    }

    for (Tile& item : m_terrainTileCollection)
    {
        item.setInPlay(false);
    }

    m_terrainTileCollection[m_tileCurrent].makeTileCurrent(true);
    m_terrainTileCollection[m_tileNext].makeTileCurrent(false);

    m_terrainTileCollection[m_tileCurrent].makeFeederSeekPlayer(true, player);
    m_terrainTileCollection[m_tileNext].makeFeederSeekPlayer(false, player);

    for (int i = 0; i < MAX_SWARMERS; i++)
    {
        Vector3 pos = m_terrainTileCollection[m_tileCurrent].getSwarmerPos(i);
        swarmer[i].spawn(pos, 2, 0);
    }

    float mapLength = 64.0f;
    
    camPos.z = 0.0f;
}

void Game::cameraMove()
{
    float speed = 0.2f;

    Vector2 lowerLimit = player.getLowerLimit();
    Vector2 upperLimit = player.getUpperLimit();

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

    player.updateLimits(lowerLimit, upperLimit);
    diffBetweenLimits = upperLimit.x - lowerLimit.x;
}

void Game::fogVisibility()
{
    float heightPercent;
    const float MAX_HEIGHT = 427.0f;
    const int MAX_TICK = 700;

    gradientDest.height = EaseLinearInOut(fogTick, heightVal, MAX_HEIGHT, MAX_TICK);
    heightPercent = gradientDest.height / MAX_HEIGHT;

    fogOpacity.a = 255.0f * heightPercent;

    if (fogTick <= MAX_TICK)
    {
        fogTick++;
    }
    
}

void Game::reduceFog()
{
    fogTick -= 100;
    if (fogTick < 0)
    {
        fogTick = 0;
    }
}


