#include "GameManager.h"
#include "SampleScene.h"

int main()
{
    GameManager* game = GameManager::Get();

    game->LaunchScene<SampleScene>(640, 960, "SampleScene", Color{ 25, 25, 35 });

    return 0;
}
