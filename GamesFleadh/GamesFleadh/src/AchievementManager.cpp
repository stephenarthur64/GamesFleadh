#include "../include/AchievementManager.h"

std::vector<Achievement> AchievementManager::achievements;
Sound AchievementManager::achievementComplete;

void AchievementManager::init()
{
    // Texture
    box = LoadTexture("ASSETS/2D/UI/ScoreBox.png");
    // Sound
    achievementComplete = LoadSound("ASSETS/Audio/SFX/Achievement/achievementSound.mp3");
    SetSoundVolume(achievementComplete, 0.2);


    // Type::NONE
    achievements.push_back(Achievement(box, "Hello World!", AchievementType::NONE));

    // Type::ENEMY
    achievements.push_back(Achievement(box, "Test", AchievementType::ENEMY));

}

void AchievementManager::checkForChanges()
{
    for (Achievement& a : achievements)
    {
        if (a.checkIfCompleted())
        {
            SetSoundPitch(achievementComplete, 0.8 + static_cast<double>(std::rand()) / RAND_MAX * (1.2 - 0.8));
            PlaySound(achievementComplete);
        }
    }
}

void AchievementManager::draw()
{
    achievementAmount = -1;
    for (Achievement& a : achievements)
    {
        if (a.isOnShow())
        {
            achievementAmount++;
            a.draw(achievementAmount);
        }

    }
}

void AchievementManager::addGoalToAchievement(std::string t_achievementTitle, int *t_valueToKeepTrackOf, int t_goal)
{
    bool found = false;
    for (Achievement& a : achievements)
    {
        if (a.getTitle() == t_achievementTitle)
        {
            found = true;
            a.addGoal(t_valueToKeepTrackOf, t_goal);
        }
    }

    if (!found)
    {
        printf("\n\nCouldnt find: %s\n\n", t_achievementTitle.c_str());
    }

    if (t_valueToKeepTrackOf == nullptr)
    {
        printf("\n\nVALUE IS NULLPTR\n\n");
    }
}