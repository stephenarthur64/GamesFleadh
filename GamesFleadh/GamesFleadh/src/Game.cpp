#include "Game.h"
#include <cmath> // Used for abs()

bool g_renderWireDebug = false;
bool g_render2DDebug = false;
bool g_testForMushrooms = true;

Game::Game() : score(0), activeMap(1), state(GameState::TITLE)
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

    while (!endGame && !WindowShouldClose())
    {
        update();
        render();
    }
}

void Game::init()
{
    // RUMBLE REQUIREMENT
    //------------------------------------------------------------------------------------
    // [7] ############## Connect to Controller ##########################################
    //------------------------------------------------------------------------------------
    // SDL_GameController* controller = NULL;
    // SDL_Joystick *joystick = NULL; (For flight Controller)
    if (SDL_NumJoysticks() > 0)
    {
        controller = SDL_GameControllerOpen(0);
        // joystick = SDL_GameControllerGetJoystick(controller);
        if (!controller)
        {
            TraceLog(LOG_ERROR, "SDL_GameControllerOpen(0) SDL_GameControllerGetJoystick(controller) : %s\n", SDL_GetError());
        }
        else {
            TraceLog(LOG_INFO, "********************** SDL_GameControllerOpen(0); ************************************");
        }
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Games Fleadh 2025");
    ToggleFullscreen();
    InitAudioDevice();
    HideCursor();

    // Define our custom camera to look into our 3d world
    camera = { 0 };
    camPos = { 0.0f, 2.0f, -2.0f };             // Determines player's starting position!
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
    darkenColour.a = 0;

    achievementManager.init();
    AchievementManager::addGoalToAchievement("First Kill", &score, 10);
    AchievementManager::addGoalToAchievement("Defender", &score, 50);
    AchievementManager::addGoalToAchievement("Soldier Bee", &score, 100);
    AchievementManager::addGoalToAchievement("Tiny Titan", &score, 250);
    AchievementManager::addGoalToAchievement("Hive Hero", &score, 300);
    AchievementManager::addGoalToAchievement("Bee-hemoth", &score, 400);
    AchievementManager::addGoalToAchievement("Vs Harvesters", &score, 500);
    AchievementManager::addGoalToAchievement("Jazz Fan", &score, 1000);

    AchievementManager::addGoalToAchievement("Tiny Traveller", &travelled, 3);
    AchievementManager::addGoalToAchievement("Long Way", &travelled, 5);
    AchievementManager::addGoalToAchievement("500 Miles", &travelled, 8);
    AchievementManager::addGoalToAchievement("Trench Run", &travelled, 15);
    AchievementManager::addGoalToAchievement("Spitfire", &travelled, 20);
    AchievementManager::addGoalToAchievement("12 Parsecs", &travelled, 30);
    AchievementManager::addGoalToAchievement("Tired Yet?", &travelled, 40);
    AchievementManager::addGoalToAchievement("True Dedication", &travelled, 50);


    //// RUMBLE REQUIREMENT
    //------------------------------------------------------------------------------------
        // [8] ##############  Poll SDL Events ###############################################
        //      GetTime() is a SDL Call
        //      SDL_GameControllerRumble(controller, 0, 0, rumble_duration);  // Stop rumbling
        //------------------------------------------------------------------------------------
    SDL_GameControllerUpdate();
    Uint32 startTime = GetTime() * 1000;
    Uint32 rumble_duration = 0; // Duration
    if (GetTime() * 1000 - startTime >= rumble_duration) {
        SDL_GameControllerRumble(controller, 0, 0, rumble_duration);  // Stop rumbling
        TraceLog(LOG_INFO, "Stopping Rumbleeeeeee");
    }
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
    fogOpacity.a = 255;
    fogVignette = LoadTexture("ASSETS/2D/Fog/OrangeVignette.png");
    fogBar = LoadTexture("ASSETS/2D/UI/FogBar.png");
    fogGradient = LoadTexture("ASSETS/2D/UI/FogGradient.png");
    scoreBack = LoadTexture("ASSETS/2D/UI/ScoreBox.png");

    healthBar = LoadTexture("ASSETS/2D/UI/HealthBarVertical.png");
    healthGradient = LoadTexture("ASSETS/2D/UI/HealthBarVerticalFill.png");

    countdown[2] = LoadTexture("ASSETS/2D/UI/3.png");
    countdown[1] = LoadTexture("ASSETS/2D/UI/2.png");
    countdown[0] = LoadTexture("ASSETS/2D/UI/1.png");

    countdownText = LoadTexture("ASSETS/2D/UI/RespawnText.png");
    darkenScreen = LoadTexture("ASSETS/2D/UI/DarkenLayer.png");

    logo = LoadTexture("ASSETS/2D/UI/titleFINAL.png");
    subtitle = LoadTexture("ASSETS/2D/UI/Play.png");

    controllerInputs = LoadTexture("ASSETS/2D/UI/Controller.png");
    keyboardInputs = LoadTexture("ASSETS/2D/UI/Keyboard.png");

    arrow = LoadTexture("ASSETS/2D/UI/LeftArrow.png");
    arrow2 = LoadTexture("ASSETS/2D/UI/RightArrow.png");

    difficulty[0] = LoadTexture("ASSETS/2D/UI/EasyDifficulty.png");
    difficulty[1] = LoadTexture("ASSETS/2D/UI/DefaultDifficulty.png");
    difficulty[2] = LoadTexture("ASSETS/2D/UI/HardDifficulty.png");

    leave = LoadTexture("ASSETS/2D/UI/QuitGame.png");

    sfxHover = LoadSound("ASSETS/Audio/SFX/UI/hoverSoundEffect.mp3");
    sfxSelect = LoadSound("ASSETS/Audio/SFX/UI/SelectionSoundEffect.m4a");
    SetSoundVolume(sfxHover, 0.4);
    SetSoundVolume(sfxSelect, 0.4);

    healthSource = { 0, 0, (float)healthGradient.width, (float)healthGradient.height };
    healthDest = { 57, SCREEN_HEIGHT / 2.0f, (float)healthGradient.width + 10, (float)healthGradient.height };

    gradientSource = { 0, 0, (float)fogGradient.width, (float)fogGradient.height};
    gradientDest = { SCREEN_WIDTH - 30, 370, (float)fogGradient.width + 10, (float)fogGradient.height};

    bill = LoadTexture("ASSETS/2D/Crosshair/HexagonCrosshair.png"); // ArrowCrosshair.png");
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

    titleScreenTrack = LoadMusicStream("ASSETS/Audio/SFX/Environment/environmentalAmbience.mp3");
    SetMusicVolume(titleScreenTrack, 0.4);
    PlayMusicStream(titleScreenTrack);
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



    for (Tile& tileToDraw : m_terrainTileCollection)
    {
        tileToDraw.render();
        tileToDraw.renderBoom(camera);
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

        DrawLine3D(player.getPosition(), m_collision.point, PURPLE);

        //DrawSphere(objectPlacementTest, 2.0f, ORANGE);

        DrawGrid(20, 1.0f);
    }

    DrawBillboardPro(camera, bill, source, billPositionRotating, billUp, size, origin, rotation, WHITE);
    //DrawBillboardPro(camera, bill, source, billPositionRotating + Vector3{0.0f, 0.0f, -5.0f}, billUp, size, origin, rotation, WHITE);
    //DrawBillboardPro(camera, bill, source, Vector3Lerp(player.getPosition(), billPositionRotating, 0.5f), billUp, size, origin, rotation, RED);

    EndMode3D();

    achievementManager.draw();

    if (gameOverTick > 0)
    {
        DrawTexture(darkenScreen, 0, 0, darkenColour);
        DrawTexture(countdownText, (SCREEN_WIDTH / 2.0f) - (countdownText.width / 2.0f), 200.0f, WHITE);
        DrawTexture(countdown[countdownRespawn], (SCREEN_WIDTH / 2.0f) - (countdown[countdownRespawn].width / 2.0f), (SCREEN_HEIGHT / 2.0f) - (countdown[countdownRespawn].height / 2.0f), WHITE);
        //DrawText(TextFormat("BUZZZ HAS FAILED!\n\nRESPAWNING....\n   %d", countdownRespawn), (SCREEN_WIDTH / 2.0f) - 150.0f, SCREEN_HEIGHT / 2.0f, 30, RED);
    }


    if (g_render2DDebug)
    {
        DrawFPS(10, 30);
        DrawTexture(fogVignette, 0, 0, fogOpacity);

        DrawText(TextFormat("CROSSHAIR X POSITION: %f", m_crosshairOnScreenPos.x), 10, 440, 10, RED);
        DrawText(TextFormat("CROSSHAIR Y POSITION: %f", m_crosshairOnScreenPos.y), 10, 450, 10, RED);

        if (g_testForMushrooms) DrawText("x", SCREEN_WIDTH - 32, SCREEN_HEIGHT - 32, 20, GREEN);

        /*DrawText(TextFormat("PLAYER Z POSITION: %f", player.getPosition().z), 10, 430, 10, RED);
        DrawText(TextFormat("PLAYER Y POSITION: %f", player.getPosition().y), 10, 440, 10, RED);
        DrawText(TextFormat("PLAYER X POSITION: %f", player.getPosition().x), 10, 450, 10, RED);*/
    }
    else
    {
        if (state == GameState::GAMEPLAY)
        {
            DrawTexture(scoreBack, (SCREEN_WIDTH / 2.0f) - (scoreBack.width / 2.0f), 40, WHITE);
            DrawTextEx(gameFont, TextFormat("%i", score), { (SCREEN_WIDTH / 2.0f) - (scoreBack.width / 2.0f) + 20, 55 }, 50, 5, Color {190, 190, 190, 255});
            DrawTextEx(gameFont, TextFormat("SCORE"), { (SCREEN_WIDTH / 2.0f) - (scoreBack.width / 2.0f) + 75, 130 }, 20, 5, Color{ 190, 190, 190, 255 });
            DrawTexturePro(healthGradient, healthSource, healthDest, { (float)healthGradient.width / 2.0f, (float)healthGradient.height / 2.0f }, 180.0f, player.getHealthBarColour());
            DrawTexture(healthBar, 30, 350.0f, WHITE);

            if (m_painShow)
            {
                Color fogVisiblity = Color{ 255, 255, 255, (unsigned char)m_painAlphaCur };
                DrawTexture(fogVignette, 0, 0, fogVisiblity);
            }
        }
        else if (state == GameState::TITLE)
        {
            DrawTexture(darkenScreen, 0, 0, WHITE);
            DrawTextureEx(logo, { (SCREEN_WIDTH / 2.0f) - (logo.width / 2.0f) + 220.0f, (SCREEN_HEIGHT / 2.0f) - (logo.height / 2.0f) },0.0f, 0.75f, WHITE);
            DrawTexture(subtitle, (SCREEN_WIDTH / 2.0f) - (subtitle.width / 2.0f), (SCREEN_HEIGHT / 2.0f), WHITE);
            DrawTexture(controllerInputs, SCREEN_WIDTH - (controllerInputs.width + 100.0f), SCREEN_HEIGHT / 2.0f, WHITE);
            DrawTexture(keyboardInputs, 100.0f, SCREEN_HEIGHT / 2.0f, WHITE);
            DrawTexture(difficulty[selectedDifficulty], (SCREEN_WIDTH / 2.0f) - (difficulty[selectedDifficulty].width / 2.0f), (SCREEN_HEIGHT / 2.0f) + 100.0f, WHITE);
            DrawTextureEx(arrow, { (SCREEN_WIDTH / 2.0f) - (difficulty[selectedDifficulty].width / 2.0f) - 50.0f, (SCREEN_HEIGHT / 2.0f) + arrowYOffset }, 0.0f, 1.0f, WHITE);
            DrawTextureEx(arrow2, { (SCREEN_WIDTH / 2.0f) - (difficulty[selectedDifficulty].width / 2.0f) + difficulty[selectedDifficulty].width + 10, (SCREEN_HEIGHT / 2.0f) + arrowYOffset }, 0.0f, 1.0f, WHITE);
            DrawTexture(leave, (SCREEN_WIDTH / 2.0f) - (leave.width / 2.0f), 800.0f, WHITE);
            DrawTextEx(gameFont, "Press (SPACE)/(RT) to select", { (SCREEN_WIDTH / 2.0f) - 150.0f, 1000.0f }, 20, 2, DARKGRAY);
        }
    }


    // DrawRectangleLines(SCREEN_WIDTH * 0.5 - m_upperLimit.x, SCREEN_HEIGHT * 0.5f - m_upperLimit.y, m_upperLimit.x, m_upperLimit.y, RED);

    EndDrawing();
}

void Game::update()
{
    // RUMBLE REQUIREMENT
    // Rumble goodnees
    SDL_GameControllerUpdate();
    Uint32 startTime = GetTime() * 1000;
    Uint32 rumble_duration = 0; // Duration
    if (GetTime() * 1000 - startTime >= rumble_duration) {
        SDL_GameControllerRumble(controller, 0, 0, rumble_duration);  // Stop rumbling
        TraceLog(LOG_INFO, "Stopping Rumbleeeeeee");
    }
    // Set Rumble ON 
    // Base on some event
    Rumble(controller, 0xFFFF, 0xFFFF, rumble_duration);

    painCountDown();

    gamepadUpdate();
    inputControl();
    if (state == GameState::GAMEPLAY)
    {
        UpdateMusicStream(bgm);
        achievementManager.checkForChanges();
        if (!(player.isAlive()))
        {
            gameOverTick++;
            if (gameOverTick > 0)
            {
                darkenScreenUpdate();
                countdownRespawn = ((180 - gameOverTick) / 60.0f);
            }
            if (gameOverTick > 180)
            {
                gameBegins();
            }
        }

        player.updateZPos(camPos.z - playerZOffsetFromCamera);
        player.faceCrosshair(billPositionRotating);
        healthBarUpdate();
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
        checkCollisions();
        player.update(camPos, billPositionRotating);
        camera.position = camPos;
    }
    else if (state == GameState::TITLE)
    {
        UpdateMusicStream(titleScreenTrack);
        camera.target = player.getPosition();
        for (Tile& item : m_terrainTileCollection)
        {
            item.update(player.getPosition());
        }
    }
    cameraMove();
    // UpdateCamera(&camera, CAMERA_PERSPECTIVE);
    //fogVisibility();

    // Continuously check if rumble should stop
    UpdateRumble(controller);
}

void Game::inputControl()
{
    if (!(player.isAlive()))
    {
        return;
    }

    if (state == GameState::TITLE)
    {
        if (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || IsKeyReleased(KEY_DOWN))
        {
            PlaySound(sfxHover);
            arrowYOffset = 265.0f;
        }
        else if (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_UP) || IsKeyReleased(KEY_UP))
        {
            PlaySound(sfxHover);
            arrowYOffset = 130.0f;
        }

        if (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) || 
            IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2) ||
            IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_UP) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) ||
            IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) ||
            IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_UP) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) ||
            IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT) ||
            IsKeyReleased(KEY_SPACE))
        {
            PlaySound(sfxSelect);
            if (arrowYOffset == 130.0f)
            {
                gameBegins();
            }
            else if (arrowYOffset == 265.0f)
            {
                endGame = true;
            }
        }

        if (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) || IsKeyReleased(KEY_RIGHT))
        {
            PlaySound(sfxHover);
            if (selectedDifficulty < 2)
            {
                selectedDifficulty++;
            }
            else
            {
                selectedDifficulty = 0;
            }
        }
        else if (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) || IsKeyReleased(KEY_LEFT))
        {
            PlaySound(sfxHover);
            if (selectedDifficulty > 0)
            {
                selectedDifficulty--;
            }
            else
            {
                selectedDifficulty = 2;
            }
        }
    }

    if (IsGamepadButtonReleased(0, GAMEPAD_BUTTON_MIDDLE_RIGHT) || IsKeyReleased(KEY_BACKSPACE))
    {
        state = GameState::TITLE;
    }

    //if (IsKeyDown(KEY_I) || leftStickY < 0)
    //{
    //    camDirection = 0.0f;
    //    if (leftStickY < 0)
    //    {
    //       //camDirection -= camSpeed * (-leftStickY);
    //    }
    //    else
    //    {
    //        camDirection -= camSpeed;
    //    }
    //    player.updateHitBox(camDirection);
    //    camPos.z += camDirection;
    //}
    //if (IsKeyDown(KEY_K) || leftStickY > 0)
    //{
    //    if (leftStickY > 0)
    //    {
    //        //camDirection = camSpeed * (-leftStickY);
    //    }
    //    else
    //    {
    //        camDirection = camSpeed;
    //    }
    //    player.updateHitBox(camDirection);
    //    camPos.z += camDirection;
    //}

    if (IsKeyDown(KEY_W))
    {
        player.move({0, -1 * m_keyboardMoveSensitivity, 0});
    }
    if (IsKeyDown(KEY_S))
    {
        player.move({0,1 * m_keyboardMoveSensitivity,0});
    }
    if (IsKeyDown(KEY_A))
    {
        player.move({-1 * m_keyboardMoveSensitivity,0,0});
    }
    if (IsKeyDown(KEY_D))
    {
        player.move({1 * m_keyboardMoveSensitivity,0,0});
    }

    if (IsKeyReleased(KEY_P) || IsKeyReleased(KEY_TAB))
    {
        // Dummy function to trigger breakpoint.
        std::cout << "This could trigger a breakpoint.";
    }

    //if (IsKeyReleased(KEY_F5) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1))
    //{// RS: Toggle! Is nice, you like.
    //    autoScroll = !autoScroll;
    //    player.setAuto(autoScroll);
    //    std::cout << "Good god.";
    //}

    if (IsKeyReleased(KEY_F1))
    {
        if (IsMusicStreamPlaying(bgm))
        {
            StopMusicStream(bgm);
        }
        else
        {
            PlayMusicStream(bgm);
        }
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
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || 
        IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_2) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_TRIGGER_1) ||
        IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_TRIGGER_1) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_TRIGGER_2) ||
        IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_UP) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT) ||
        IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_DOWN) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_LEFT_FACE_LEFT) ||
        IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_UP) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) ||
        IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN) || IsGamepadButtonReleased(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))
    {
        //Uint32 rumble_duration = 1000; // Duration
        //Rumble(controller, 0xFFFF, 0xFFFF, rumble_duration);
        Rumble(controller, 0xFFFF, 0xFFFF, 300);

        if (m_collision.hit)
        {
            player.handleInput(EVENT_SHOOT);
            player.shootBullet(m_collision.point);
        }
        else
        {
            player.handleInput(EVENT_SHOOT);
            player.shootBullet(billPositionRotating);
        }
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

    if (IsKeyReleased(KEY_F2))
    {
        std::cout << "\nToggling Pain Vignette!\n";
        m_showPainVignette = !m_showPainVignette;
    }

    Vector3 normVelocity = Vector3Normalize({ leftStickX, leftStickY, 0 });

    player.move(normVelocity);
    //camPos += normVelocity * Vector3{ 0.1, -0.1, 0.1 };

    if (state == GameState::GAMEPLAY)
    {
        crosshairMove();
    }
    billPositionRotating.z = player.getPosition().z - 3.0f;

    if (player.isAuto())
    {// RS: How are we not doing this stuff with GETFRAMETIME(), are we barbarians?
        camPos.z -= player.getAcceleration();
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
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_I))
    {
        keyboardY = -1.0f * m_keyboardLookSensitivity;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_K))
    {
        keyboardY = 1.0f * m_keyboardLookSensitivity;
    }
    else
    {
        keyboardY = 0.0f;
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_J))
    {
        keyboardX = -1.0f * m_keyboardLookSensitivity;
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_L))
    {
        keyboardX = 1.0f * m_keyboardLookSensitivity;
    }
    else
    {
        keyboardX = 0.0f;
    }

    billPositionRotating.x += billSpeed * (rightStickX + keyboardX);
    billPositionRotating.y += billSpeed * (- (rightStickY + keyboardY));
    billPositionRotating.x = Clamp(billPositionRotating.x, player.getPosition().x - 10.0f, player.getPosition().x + 10.0f);
    billPositionRotating.y = Clamp(billPositionRotating.y, player.getPosition().y - 5.0f, player.getPosition().y + 5.5f);
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
    AchievementManager::lockAll();
    state = GameState::GAMEPLAY;
    gameOverTick = 0;
    score = 0;
    camPos = { 0.0f, 5.0f, -2.0f };
    mapMove();
    billPositionRotating = { 0.0f, 6.0f, 5.0f };
    player.respawn();
    player.setStartingSpeed(selectedDifficulty);
    darkenColour.a = 0;
    PlayMusicStream(bgm);
    PauseMusicStream(titleScreenTrack);
}

void Game::gamepadUpdate()
{

    if (IsGamepadAvailable(gamepad) && state == GameState::GAMEPLAY)
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
            if (m_showPainVignette) painVignetteStart();
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
            score += 10 + (5 * selectedDifficulty) * player.getSpeedMultiplier();
        }

        for (int j = 0; j < MAX_SWARMERS; j++)
        {
            if (CheckCollisionBoxSphere(swarmer[j].getHitbox(), player.getBulletPositon(i), 1.0f))
            {
                swarmer[j].collision(true);
                reduceFog();
                player.despawnBullet(i);
                score += 10 + (5 * selectedDifficulty) * player.getSpeedMultiplier();
            }
        }
    }

    if (m_terrainTileCollection[m_tileCurrent].isColliding(player.getPosition() + PLAYER_COLLISION_OFFSET_LATERAL))
    {// Colliding with terrain on the right
        player.worldCollision(true);
        player.handleInput(EVENT_COLLIDE_R);
        player.hitSound(0);
        if (m_showPainVignette) painVignetteStart();
        player.rebound(player.getPosition() + PLAYER_COLLISION_OFFSET_LATERAL);
        player.stopAcceleration();
    }

    if (m_terrainTileCollection[m_tileCurrent].isColliding(player.getPosition() - PLAYER_COLLISION_OFFSET_LATERAL))
    {// Colliding with terrain on the left
        player.worldCollision(true);
        player.handleInput(EVENT_COLLIDE_L);
        player.hitSound(0);
        if (m_showPainVignette) painVignetteStart();
        player.rebound(player.getPosition() - PLAYER_COLLISION_OFFSET_LATERAL);
        player.stopAcceleration();
    }

    if (m_terrainTileCollection[m_tileCurrent].isColliding(player.getPosition() + PLAYER_COLLISION_OFFSET_FRONT))
    {// Colliding with terrain in front
        player.worldCollision(true);
        player.hitSound(0);
        if (m_showPainVignette) painVignetteStart();
        player.reboundZ(camPos);
        player.stopAcceleration();
    }

    m_collisionData = m_terrainTileCollection[m_tileCurrent].checkBoundsFurnitureItemsCollision(player.getPosition(), player.getCollisionRadius(), player.getHitbox());

    if (m_collisionData.collision)
    {
        std::cout << "Hitting a mushroom!\n\n";
        player.hitSound(0);
        if (m_showPainVignette) painVignetteStart();
        player.reboundFurniture(m_collisionData);
        player.stopAcceleration();
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
        if (m_showPainVignette) painVignetteStart();
        player.poisonPlayer(true);
    }

    m_ray = { 0 };
    m_crosshairOnScreenPos = GetWorldToScreen(billPositionRotating, camera); // Check origin of billboard
    
    m_ray = GetScreenToWorldRay(m_crosshairOnScreenPos, camera);

    m_collision.distance = FLT_MAX; // Set distance to maximum.
    m_collision.hit = false;

    for (int i = 0; i < MAX_SWARMERS; i++)
    {
        m_collision = GetRayCollisionBox(m_ray, swarmer[i].getHitbox());
    }

    //m_terrainTileCollection[m_tileCurrent].checkFeederRayCollision(m_ray, m_collision);

    if ((m_collision.hit) && (m_collision.distance < FLT_MAX))
    {
        std::cout << "We hit a SWARMER!\n";

    }
    else
    {
        m_collision = m_terrainTileCollection[m_tileCurrent].checkRay(m_ray, m_collision);
    }
    
}

void Game::mapMove()
{
    if (player.getPosition().z > -64.0f - playerZOffsetFromCamera && player.isAlive()) return;

    if (player.isAlive())

    {
        m_tileCurrent = m_tileNext;

        while (m_tileNext == m_tileCurrent)
        {
            m_tileNext = rand() % m_terrainTileCollection.size();
            std::cout << "m_tileNext is " << m_tileNext << ".\n";
        }
        camPos.z = 0.0f;
    }
    else
    {
        m_tileCurrent = 0;
        m_tileNext = 1;
    }

    for (Tile& item : m_terrainTileCollection)
    {
        item.setInPlay(false);
    }

    m_terrainTileCollection[m_tileCurrent].makeTileCurrent(true);
    m_terrainTileCollection[m_tileNext].makeTileCurrent(false);

    m_terrainTileCollection[m_tileCurrent].makeFeederSeekPlayer(true, player);
    m_terrainTileCollection[m_tileNext].makeFeederSeekPlayer(false, player);

    travelled++;

    for (int i = 0; i < MAX_SWARMERS; i++)
    {
        Vector3 pos = m_terrainTileCollection[m_tileCurrent].getSwarmerPos(i);
        swarmer[i].spawn(pos, 2, 0);
    }

    player.increaseAcceleration();
    float mapLength = 64.0f;
}

void Game::cameraMove()
{
    player.cameraMove(camPos);

    camera.target = billPositionRotating;
    camera.target.z = billPositionRotating.z - 15.0f;
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

void Game::darkenScreenUpdate()
{
    int val = darkenColour.a + 10;
    
    if (val > 255)
    {
        val = 255;
    }

    darkenColour.a = val;
    
}

void Game::healthBarUpdate()
{
    float heightPercent;
    const float MAX_HEIGHT = 427.0f;
    const int MAX_TICK = 700;

    //healthDest.height = EaseLinearInOut(healthTick, heightHealth, MAX_HEIGHT, MAX_TICK);
    heightPercent = healthDest.height / MAX_HEIGHT;

    heightPercent = 3.14;

    healthDest.height = player.getHealth() * heightPercent;
}

// RUMBLE REQUIREMENT
//void Game::Rumble(SDL_GameController* controller, Uint16 lowFreq, Uint16 highFreq, Uint32 duration){
//    if (controller && SDL_GameControllerHasRumble(controller))
//    {
//        TraceLog(LOG_INFO, "Activating Rumbleeeeeee");
//        SDL_GameControllerRumble(controller, lowFreq, highFreq, duration);
//    }
//}

void Game::Rumble(SDL_GameController* controller, Uint16 lowFreq, Uint16 highFreq, Uint32 duration) {
    if (controller && SDL_GameControllerHasRumble(controller)) {
        Uint32 currentTime = GetTime() * 1000;

        // Allow a new rumble if previous one has ended (ignores cooldown)
        if (!m_isRumbling) {
            TraceLog(LOG_INFO, "Activating Rumbleeeeeee");
            SDL_GameControllerRumble(controller, lowFreq, highFreq, duration);

            // Update timing variables
            m_rumbleStartTimeRS = currentTime;
            m_rumbleDurationRS = duration;
            m_isRumbling = true;
        }
    }
}

void Game::painVignetteStart()
{
    std::cout << "Pain starting!\n";
    m_painCounterCur = m_painCounterMax;
    m_painShow = true;
    m_painAlphaCur = m_painAlphaMax;
}

void Game::painCountDown()
{
    if (m_painCounterCur > 0)
    {
        std::cout << "Counting down pain at " << m_painCounterCur << "\n";
        m_painCounterCur -= GetFrameTime();
        m_painAlphaCur -= GetFrameTime() * 8;
    }
    else
    {
        m_painShow = false;
    }
        
}

void Game::UpdateRumble(SDL_GameController* controller) {
    if (m_isRumbling) {
        Uint32 currentTime = GetTime() * 1000;

        if (currentTime - m_rumbleStartTimeRS >= m_rumbleDurationRS) {
            SDL_GameControllerRumble(controller, 0, 0, 0); // Stop rumbling
            TraceLog(LOG_INFO, "Stopping Rumbleeeeeee");
            m_isRumbling = false;  // Allow next rumble immediately
        }
    }
}


