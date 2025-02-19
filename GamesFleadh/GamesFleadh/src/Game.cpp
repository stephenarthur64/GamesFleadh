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
}

Game::~Game()
{
    UnloadTexture(heightmapTexture);     // Unload texture
    UnloadModel(heightmapModel);         // Unload model
    UnloadImage(heightmapImage);         // Unload heightmap image from RAM, already uploaded to VRAM
    
    UnloadShader(skybox.materials[0].shader);// Skybox memory management
    UnloadTexture(skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(skybox);
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
    camPos = { 0.0f, 4.0f, -2.0f };             // Determines player's starting position!
    camera.position = camPos;                   // Camera position
    camera.target = { 0.0f, 0.0f, -2300.0f };   // Camera looking at point
    camera.up = { 0.0f, 1.0f, 0.0f };           // Camera up vector (rotation towards target)
    camera.fovy = 60.0f;                        // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;     // Camera projection type

    loadAssets();
    
    setupSkybox();
    
    gamepadInit();

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
}

void Game::placeObjectsFromImage(Image placementMap)
{
    // 626 111
    // Color testCol = GetImageColor(placementMap, 626, 111);

    int mushroomCount = 0;

    for (int u = 0; u < placementMap.width; u++)
    {
        for (int v = 0; v < placementMap.height; v++)
        {
            Color col = GetImageColor(placementMap, u, v);

            if (col.r == 255 && col.b == 0 && col.g == 0) // Possibly early out here
            {
                std::cout << "Found red pixel.";

                Color heightFromCol = GetImageColor(heightmapImage, u, v);

                float placementTexUcoord = static_cast<float>(u);
                float placementColYcoord = static_cast<float>(heightFromCol.r);
                float placementTexVcoord = static_cast<float>(v);

                float placeWorldNormX = placementTexUcoord / heightmapImage.width;
                float placeWorldNormY = placementColYcoord / 255.0f;
                float placeWorldNormZ = placementTexVcoord / heightmapImage.height;

                float placeWorldCoordX = (placeWorldNormX * mapSize.x) - abs(mapPosition.x);
                float placeWorldCoordY = placeWorldNormY * mapSize.y;
                float placeWorldCoordZ = ((placeWorldNormZ * mapSize.y) - abs(mapPosition.z) - SeemingMagicalOffset); // Not sure I need the last offset for objects

                //objectPlacementTest = { placeWorldCoordX, placeWorldCoordY, placeWorldCoordZ };
                Vector3 mushroomOrigin = { placeWorldCoordX, placeWorldCoordY, placeWorldCoordZ };

                if (mushroomCount < MAX_MUSHROOMS)
                {
                    mushroom[mushroomCount].init();
                    //if (mushroomCount != mushroomOnMap)
                    //{
                        mushroom[mushroomCount].spawn(mushroomOrigin);
                    //}
                    mushroom[mushroomCount].spawnEnemy();
                    mushroomCount++;
                }
                mushroom[0].playerDetected(true);

                /*for (int i = 0; i < MAX_MUSHROOMS; i++)
                {
                    mushroom[i].init();
                    if (i != mushroomOnMap)
                    {
                        mushroom[i].spawn({ -1.0f, 2.0f, -79.0f });
                    }
                    mushroom[i].spawnEnemy();
                }*/
                
            }
        }
    }
}

void Game::loadAssets()
{
    imgPlacementTest = LoadImage("ASSETS/2D/Heightmaps/test1_EnemyPlacement01001RS.png");

    heightmapImage = LoadImage("ASSETS/2D/Heightmaps/test1_3xWider_halfDark4_Rot_halfDark3.png");
    heightmapTexture = LoadTextureFromImage(heightmapImage);        // Convert image to texture (VRAM)

    bill = LoadTexture("ASSETS/2D/Crosshair/crosshair.png");
    source = { 0.0f, 0.0f, (float)bill.width, (float)bill.height };
    billUp = { 0.0f, 1.0f, 0.0f };
    size = { source.width / source.height, 1.0f };
    origin = Vector2Scale(size, 0.5f);

    heightmapMesh = GenMeshHeightmap(heightmapImage, mapSize); // Generate heightmap mesh (RAM and VRAM)
    heightmapModel = LoadModelFromMesh(heightmapMesh);         // Load model from generated mesh

    heightMapBounds = GetModelBoundingBox(heightmapModel); // Getting data for collision

    heightmapModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = heightmapTexture; // Set map diffuse texture

    //mapPosition = { -32.0f, -0.0f, -64.0f };          // Define model position
    //mapPosition2 = { -32.0f, -0.0f, -128.0f };
    
    player.init();
    billPositionStatic = { 2.0f,2.0f,0.0f };

    
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

    ClearBackground({ 22, 22, 31, 255 });

    BeginMode3D(camera);

    // SKYBOX RENDER 
    rlDisableBackfaceCulling(); // We are inside the cube, we need to disable backface culling!
    rlDisableDepthMask();
    DrawModel(skybox, Vector3{ 0, 0, 0 }, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
    // SKYBOX RENDER ENDS

    DrawModel(heightmapModel, mapPosition, 1.0f, { 147, 204, 147, 255 });
    DrawModel(heightmapModel, mapPosition2, 1.0f, { 147, 204, 147, 255 });
    
    player.render();
    
    for (int i = 0; i < MAX_MUSHROOMS; i++)
    {
        mushroom[i].render();
    }

    DrawGrid(20, 1.0f);
    
    DrawBillboardPro(camera, bill, source, billPositionRotating, billUp, size, origin, rotation, WHITE);

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

    EndMode3D();

    DrawText(TextFormat("PLAYER Z POSITION: %f", player.getPosition().z), 10, 430, 10, RED);
    DrawText(TextFormat("PLAYER Y POSITION: %f", player.getPosition().y), 10, 440, 10, RED);
    DrawText(TextFormat("PLAYER X POSITION: %f", player.getPosition().x), 10, 450, 10, RED);
    DrawText(TextFormat("SCORE: %i", score), 10, 70, 25, RED);
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
    gamepadUpdate();
    inputControl();
    player.updateZPos(camPos.z - playerZOffsetFromCamera);
    player.update();
    cameraMove();
    player.faceCrosshair(billPositionRotating);

    distanceStatic = Vector3Distance(camera.position, billPositionStatic);
    distanceRotating = Vector3Distance(camera.position, billPositionRotating);
    for (int i = 0; i < MAX_MUSHROOMS; i++)
    {
        mushroom[i].update();
    }
    mapMove(); // Repositions terrain meshes based on camera X (distance/z) pos

    // RoB'S HEIGHT MAP COLLISION STUFF STARTS HERE (Probably move into collision function)
    // Get Normalised Coord
    worldNormalX = (player.getPosition().x + abs(mapPosition.x)) / mapSize.x;
    worldNormalZ = ((player.getPosition().z + SeemingMagicalOffset) + abs(mapPosition.z)) / mapSize.z;
    texUcoord = worldNormalX * heightmapImage.width;
    texVcoord = worldNormalZ * heightmapImage.height;

    texUcoord = Clamp(texUcoord, 0, heightmapImage.height - 0.001f); // Avoids OOBounds error
    texVcoord = Clamp(texVcoord, 0, heightmapImage.width - 0.001f);

    colorFromPosition = GetImageColor(heightmapImage, texUcoord, texVcoord);
    worldYNormalFromCol = colorFromPosition.r / 255.0f;
    worldYPos = worldYNormalFromCol * mapSize.y;

    if (player.getPosition().y <= worldYPos)
    {
        player.collision(true);
        //std::cout << "\nColliding!\n";
    }
    else
    {
        player.collision(false);
        //std::cout << "\nNot Colliding!\n";
    }// RoB's HEIGHT MAP COLLISION STUFF ENDS HERE

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
        billPositionRotating.x = player.getPosition().x;
        if (billSpeed < 2.0f)
        {
            billSpeed += 0.3f;
        }
        billPositionRotating.y = player.getPosition().y + billSpeed;
        player.move({0, -1, 0});
    }
    if (IsKeyDown(KEY_DOWN))
    {
        billPositionRotating.x = player.getPosition().x;
        if (billSpeed > -2.0f)
        {
            billSpeed -= 0.3f;
        }
        billPositionRotating.y = player.getPosition().y + billSpeed;
        player.move({0,1,0});
    }
    if (IsKeyDown(KEY_LEFT))
    {
        billPositionRotating.y = player.getPosition().y;
        if (billSpeed > -2.0f)
        {
            billSpeed -= 0.3f;
        }
        billPositionRotating.x = player.getPosition().x + billSpeed;
        player.move({-1,0,0});
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        billPositionRotating.y = player.getPosition().y;
        if (billSpeed < 2.0f)
        {
            billSpeed += 0.3f;
        }
        billPositionRotating.x = player.getPosition().x + billSpeed;
        player.move({1,0,0});
    }

    if (IsKeyReleased(KEY_SPACE))
    {// RS: Toggle! Is nice, you like.
        autoScroll = !autoScroll;
    }

    if (IsKeyPressed(KEY_Z))
    {
        player.collision(true);
    }
    else
    {
        player.collision(false);
    }

    if (IsKeyReleased(KEY_X))
    {
        std::cout << "\nPlacing objects.\n";
        placeObjectsFromImage(imgPlacementTest);
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        player.shootBullet();
    }

    Vector3 normVelocity = Vector3Normalize({ rightStickX, rightStickY, 0 });

    player.move(normVelocity);
    camPos += normVelocity * Vector3{ 0.1, -0.1, 0.1 };

    billPositionRotating.z = player.getPosition().z - 1.0f;

    if (autoScroll)
    {
        camPos.z += -0.1f;
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
    float newMapX = mapPosition.x;
    float newMapX2 = mapPosition2.x;
    const Vector3 mainMap = { -32.0f, -0.0f, -64.0f };
    const Vector3 nextMap = { -32.0f, -0.0f, -128.0f };
    float mapLength = 64.0f;
    
    if (player.getPosition().z > -64.0f) return; // -playerZOffsetFromCamera) return;

    if(activeMap == 1)
    {
        mapPosition2 = mainMap; // These should possibly be constants
        mapPosition = nextMap;
        activeMap = 2;
    }
    else if (activeMap == 2)
    {
        mapPosition = mainMap;
        mapPosition2 = nextMap;
        activeMap = 1;
    } 

    mushroomOnMap = 1;
    mushroom[1].spawn({ -1.0f, 2.0f, -15.0f });
    mushroom[1].spawnEnemy();
    mushroom[1].playerDetected(true);

    mushroom[0].spawn({ -1.0f, 2.0f, -mapLength - 15.0f});
    mushroom[0].spawnEnemy();
    mushroom[0].playerDetected(false);

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
}