#include "AssetManager.h"
#include "ECS/Components.h"
#include "../ECS/ScriptComponents.h"
#include "../Collision/Collision.h"

#include <random>


AssetManager::AssetManager(Manager* man, InputManager& inputManager, MujinEngine::Window& window)
	: manager(man), _inputManager(inputManager), _window(window) // todo make triggers manager
{
	onpipeTriggers[0].x = 1840;
	onpipeTriggers[0].y = 512;
	onpipeTriggers[0].w = 24;
	onpipeTriggers[0].h = 8;

	onpipeTriggers[1].x = 2608;
	onpipeTriggers[1].y = 448;
	onpipeTriggers[1].w = 32;
	onpipeTriggers[1].h = 8;

	leftofpipeTriggers[0].x = 3752;
	leftofpipeTriggers[0].y = 608; 
	leftofpipeTriggers[0].w = 16; 
	leftofpipeTriggers[0].h = 8; 
}

AssetManager::~AssetManager()
{}

void AssetManager::ProjectileExplosion(int camerapos)
{
	std::cout << "Projectile Explosion!" << std::endl;
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(2, 0), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(-2, 0), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(0, 2), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(0, -2), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(2, 2), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(2, -2), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(-2, 2), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(-2, -2), 200, 2, "projectile");
}

void AssetManager::CreatePlayer(Entity& player)
{
	player.addComponent<TransformComponent>(200.0f, 320.0f, 64, 64, 1); // 1448 for near pipe, 200 for start
	player.addComponent<AnimatorComponent>("warrior");
	player.addComponent<MovingAnimatorComponent>("warrior");
	player.addComponent<FlashAnimatorComponent>("warrior");
	player.addComponent<RigidBodyComponent>();
	player.addComponent<KeyboardControllerComponent>(
		_inputManager,
		(char*)"P1Idle",
		(char*)"P1Jump",
		(char*)"P1Walk",
		(char*)"P1Attack",
		(char*)"P1Ability1",
		SDLK_w,
		SDLK_a,
		SDLK_d,
		SDLK_k,
		SDLK_l,
		SDLK_e,
		SDLK_i,
		SDLK_s,
		SDLK_LSHIFT
		);
	player.addComponent<ColliderComponent>("player1");
	player.addComponent<ScoreComponent>();

	player.addComponent<Sword>();
	player.addComponent<Player_Script>();

	player.addGroup(Manager::groupPlayers);
}

void AssetManager::CreateBackground()
{
	auto& background(manager->addEntity());

	background.addComponent<TransformComponent>(0, 0, 640, 1142, 1);
	background.addComponent<SpriteComponent>("backgroundMountains", 10.0f);


	background.addGroup(Manager::groupBackgrounds);

}

void AssetManager::CreateRandomParticlesGenerator() {
	auto& particlesGenerator(manager->addEntity());

	particlesGenerator.addComponent<CreateRandomParticles>(*Game::_window);
	particlesGenerator.addGroup(Manager::groupEnvironmentGenerators);
}

void AssetManager::CreateRain(Entity& entity) {
	auto& rainGenerator(manager->addEntity());

	rainGenerator.addComponent<CreateRainDrops>(100.0f);
	rainGenerator.addGroup(Manager::groupEnvironmentGenerators);
}

void AssetManager::CreateSnow(Entity& entity) {
	auto& snowGenerator(manager->addEntity());

	snowGenerator.addComponent<CreateSnowDrops>(100.0f);
	snowGenerator.addGroup(Manager::groupEnvironmentGenerators);
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D dest,int range, int speed, std::string id)
{ //this is almost how we create the player
	auto& projectile(manager->addEntity());
	const GLTexture* gl_texture = TextureManager::getInstance().Get_GLTexture(id);
	projectile.addComponent<TransformComponent>(pos.x-gl_texture->width/2, pos.y- gl_texture->height/2, gl_texture->width, gl_texture->height, 1, speed);
	projectile.addComponent<AnimatorComponent>(id);
	Vector2D direction = Vector2D::Distance(dest, pos).Normalize();
	projectile.addComponent<ProjectileComponent>(range, speed, direction);
	if (direction.x < 0) {
		projectile.GetComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
	}
	//projectile.GetComponent<SpriteComponent>().animIndex = 0; //this actually shouldnt be here, this may result in errors, solution: decouple the character and his/her animations from SpriteComponent so we dont have to zero animIndex
	projectile.addComponent<ColliderComponent>(id);
	if(id == "projectile")
		projectile.addGroup(Manager::groupProjectiles);
	else
		projectile.addGroup(Manager::groupWarriorProjectiles);
	manager->grid->addEntity(&projectile);
}

void AssetManager::CreateSkeleton(Vector2D pos, Vector2D vel, std::string id, bool isGiant)
{ //this is almost how we create the player
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 64, 64, isGiant ? 2 : 1);
	enemy.GetComponent<TransformComponent>().velocity = vel;
	enemy.addComponent<AnimatorComponent>(id);
	enemy.addComponent<ColliderComponent>(id);
	enemy.addComponent<RigidBodyComponent>();
	enemy.GetComponent<AnimatorComponent>().Play("SkeletonWalk");
	enemy.addComponent<Sword>(true);
	enemy.addComponent<Skeleton_Script>(isGiant);
	
	enemy.addGroup(Manager::groupSkeletons);
	manager->grid->addEntity(&enemy);
}

void AssetManager::CreateStageUpButtons() {
	auto& stageupbuttons(manager->addEntity());

	stageupbuttons.addComponent<TransformComponent>(284, 600, 0, 0, 1);
	stageupbuttons.addComponent<StageUpButtons>(true);
}

void AssetManager::CreateShop() {
	auto& shop(manager->addEntity());

	shop.addComponent<Shop>();

	shop.addGroup(Manager::groupShops);
}

void AssetManager::RefreshShop() {
	auto& shop(manager->getGroup(Manager::groupShops));

	for (auto& s : shop)
	{
		s->GetComponent<Shop>().generateRandomItems();
	}
}

void AssetManager::CreateInventory() {
	auto& inventory(manager->addEntity());

	inventory.addComponent<Inventory>();

	inventory.addGroup(Manager::groupInventories);
}

void AssetManager::CreateFog()
{
	std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));
	auto& fog(manager->addEntity());

	fog.addComponent<TransformComponent>(0,0, main_camera2D->worldLocation.h, main_camera2D->worldLocation.w, 1);
	fog.addComponent<Rectangle_w_Color>();

	fog.addGroup(Manager::groupFog);
}

void AssetManager::CreateWeather(Entity& entity) {
	_weather = static_cast<Weather>((static_cast<int>(_weather) + 1) % (static_cast<int>(Weather::SNOW) + 1));

	switch (_weather) {
	case Weather::CLEAR:
		SetBackGroundColor(141,228,255,50);
		break;
	case Weather::RAIN:
		SetBackGroundColor(20, 5, 40, 256);
		CreateRandomParticlesGenerator();
		CreateRain(entity);
		CreateFog();
		break;
	case Weather::SNOW:
		SetBackGroundColor(130, 130, 130, 130);
		CreateSnow(entity);
		CreateFog();
		break;
	}
}

void AssetManager::SetBackGroundColor(float r, float g, float b, float a)
{
	Game::backgroundColor[0] = r / 256;
	Game::backgroundColor[1] = g / 256;
	Game::backgroundColor[2] = b / 256;
	Game::backgroundColor[3] = a / 256;

}

void AssetManager::CreateGreenKoopaTroopa(Vector2D pos, Vector2D vel, int speed, std::string id)
{ //this is almost how we create the player
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 64, 64, 1);
	enemy.GetComponent<TransformComponent>().velocity = vel;
	enemy.addComponent<AnimatorComponent>(id);
	enemy.addComponent<GreenKoopaTroopa_Script>();
	//enemy.addComponent<ProjectileComponent>(range, speed, vel);
	enemy.addComponent<ColliderComponent>(id);
	enemy.addComponent<RigidBodyComponent>();
	enemy.GetComponent<AnimatorComponent>().Play("GreenKoopaTroopaWalk");
	//enemy.GetComponent<TransformComponent>().velocity.x = 1;

	enemy.addGroup(Manager::groupGreenKoopaTroopas);
	manager->grid->addEntity(&enemy);
}

void AssetManager::CreateEnemies() {
	int mapStage = Game::map->getStage();

	int numSkeletons = (0.15 * mapStage) + 4; // For example, 4 skeletons per stage
	int numGreenKoopaTroopas = (0.15 * mapStage) + 2; // For example, 2 green koopa troopas per stage

	// Initialize a random number generator
	std::random_device rd;  // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

	// Define ranges for x and y positions
	std::uniform_int_distribution<> disX(320, 3200);

	//// Create skeletons
	for (int i = 0; i < numSkeletons; ++i) {
		Vector2D pos(disX(gen), 512);
		Vector2D vel(-1, 0);

		CreateSkeleton(pos, vel, "skeleton", false);
	}
	Vector2D pos(disX(gen), 512);
	Vector2D vel(-1, 0);

	CreateSkeleton(pos, vel, "skeleton", true);
	// Create green koopa troopas
	for (int i = 0; i < numGreenKoopaTroopas; ++i) {
		Vector2D pos(disX(gen), 512);
		Vector2D vel(-1, 0);
		int speed = 1;

		CreateGreenKoopaTroopa(pos, vel, speed, "greenkoopatroopa");
	}
}

bool AssetManager::OnPipeTrigger(SDL_Rect collider)
{
	for (int i = 0; i < 2; i++)
	{
		if (Collision::checkCollision(onpipeTriggers[i], collider)) {
			return true;
		}
	}
	return false;
}

bool AssetManager::LeftOfPipeTrigger(SDL_Rect collider)
{
	for (int i = 0; i < 1; i++)
	{
		if (Collision::checkCollision(leftofpipeTriggers[i], collider)) {
			return true;
		}
	}
	return false;
}