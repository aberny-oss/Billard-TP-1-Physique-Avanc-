#include "GameManager.h"
#include "SampleScene.h"
#include "BillardScene.h"
#include "GravityScene.h"

int main()
{
    GameManager* game = GameManager::Get();

    //game->LaunchScene<BillardScene>(640, 960, "BillardScene", Color{ 45, 45, 62 });
    game->LaunchScene<GravityScene>(640, 960, "GravityScene", Color{ 45, 45, 62 });
    //game->LaunchScene<SampleScene>(900, 850, "SampleScene", Color{ 45, 45, 62 });


    return 0;
}
