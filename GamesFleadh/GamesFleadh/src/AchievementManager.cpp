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
    achievements.push_back(Achievement(box, "First Kill", AchievementType::ENEMY));
    achievements.push_back(Achievement(box, "Defender", AchievementType::ENEMY));
    achievements.push_back(Achievement(box, "Soldier Bee", AchievementType::ENEMY));
    achievements.push_back(Achievement(box, "Tiny Titan", AchievementType::ENEMY));
    achievements.push_back(Achievement(box, "Hive Hero", AchievementType::ENEMY));
    achievements.push_back(Achievement(box, "Bee-hemoth", AchievementType::ENEMY));
    achievements.push_back(Achievement(box, "Vs Harvesters", AchievementType::ENEMY));
    achievements.push_back(Achievement(box, "Jazz Fan", AchievementType::ENEMY));

    achievements.push_back(Achievement(box, "Tiny Traveller", AchievementType::MAP));
    achievements.push_back(Achievement(box, "Long Way", AchievementType::MAP));
    achievements.push_back(Achievement(box, "500 Miles", AchievementType::MAP));
    achievements.push_back(Achievement(box, "Trench Run", AchievementType::MAP));
    achievements.push_back(Achievement(box, "Spitfire", AchievementType::MAP));
    achievements.push_back(Achievement(box, "12 Parsecs", AchievementType::MAP));
    achievements.push_back(Achievement(box, "Tired Yet?", AchievementType::MAP));
    achievements.push_back(Achievement(box, "True Dedication", AchievementType::MAP));

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