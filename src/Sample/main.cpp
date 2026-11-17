#include "GameManager.h"
#include "SampleScene.h"

int main()
{
    GameManager* game = GameManager::Get();

    game->LaunchScene<SampleScene>(640, 960, "SampleScene", Color{ 45, 45, 62 });

    return 0;
}
