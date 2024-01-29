#include "AssetManager.h"
#include "../ECS/Components.h"
#include "../ECS/ScriptComponents.h"
#include "../Collision/Collision.h"

#include <random>


AssetManager::AssetManager(Manager* man, InputManager& inputManager) 
	: manager(man), _inputManager(inputManager) // todo make triggers manager
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
	//assets->CreatePlayerComponents(player1);
	//instead of this
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
		SDLK_s,
		SDLK_LSHIFT
		);
	player.addComponent<ColliderComponent>("player1");
	player.addComponent<ScoreComponent>();

	player.addComponent<Sword>();
	player.addComponent<Player_Script>();

	player.addGroup(Game::groupPlayers);
}

void AssetManager::CreateBackground()
{
	auto& background(manager->addEntity());

	background.addComponent<TransformComponent>(0, 0, 640, 1142, 1);
	background.addComponent<SpriteComponent>("backgroundMountains", 10.0f);


	background.addGroup(Game::groupBackgrounds);

}

void AssetManager::CreateSunShape(Entity& sun)
{
	SDL_Color black = { 0, 0 ,0 ,255 };
	sun.addComponent<TransformComponent>(200.0f, 100.0f);
	sun.addComponent<Sun_Script>(black);

	sun.addGroup(Game::screenShapes);
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D dest,int range, int speed, std::string id)
{ //this is almost how we create the player
	auto& projectile(manager->addEntity());
	const GLTexture* gl_texture = Game::assets->Get_GLTexture(id);
	projectile.addComponent<TransformComponent>(pos.x-gl_texture->width/2, pos.y- gl_texture->height/2, gl_texture->width, gl_texture->height, 1, speed);
	projectile.addComponent<AnimatorComponent>(id);
	Vector2D direction = Vector2D::Distance(dest, pos).Normalize();
	projectile.addComponent<ProjectileComponent>(range, speed, direction);
	if (direction.x < 0) {
		projectile.getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
	}
	//projectile.getComponent<SpriteComponent>().animIndex = 0; //this actually shouldnt be here, this may result in errors, solution: decouple the character and his/her animations from SpriteComponent so we dont have to zero animIndex
	projectile.addComponent<ColliderComponent>(id);
	if(id == "projectile")
		projectile.addGroup(Game::groupProjectiles);
	else
		projectile.addGroup(Game::groupWarriorProjectiles);
}

void AssetManager::CreateSkeleton(Vector2D pos, Vector2D vel, int speed, std::string id)
{ //this is almost how we create the player
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 64, 64, 1);
	enemy.getComponent<TransformComponent>().velocity = vel;
	enemy.addComponent<AnimatorComponent>(id);
	enemy.addComponent<ColliderComponent>(id);
	enemy.addComponent<RigidBodyComponent>();
	enemy.getComponent<AnimatorComponent>().Play("SkeletonWalk");
	enemy.addComponent<Sword>(true);
	enemy.addComponent<Skeleton_Script>();
	//enemy.addComponent<ProjectileComponent>(range, speed, vel);

	//enemy.getComponent<TransformComponent>().velocity.x = 1;
	
	enemy.addGroup(Game::groupSkeletons);
}

void AssetManager::CreateGreenKoopaTroopa(Vector2D pos, Vector2D vel, int speed, std::string id)
{ //this is almost how we create the player
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 64, 64, 1);
	enemy.getComponent<TransformComponent>().velocity = vel;
	enemy.addComponent<AnimatorComponent>(id);
	enemy.addComponent<GreenKoopaTroopa_Script>();
	//enemy.addComponent<ProjectileComponent>(range, speed, vel);
	enemy.addComponent<ColliderComponent>(id);
	enemy.addComponent<RigidBodyComponent>();
	enemy.getComponent<AnimatorComponent>().Play("GreenKoopaTroopaWalk");
	//enemy.getComponent<TransformComponent>().velocity.x = 1;

	enemy.addGroup(Game::groupGreenKoopaTroopas);
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
		int speed = 1;

		CreateSkeleton(pos, vel, speed, "skeleton");
	}

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


void AssetManager::Add_GLTexture(std::string id, const char* path)
{
	const GLTexture *testPNG = TextureManager::loadPNG(path);
	const int testHeight = testPNG->height;
	const int testWidth = testPNG->width;
	const unsigned int testId = testPNG->id;
	const GLTexture* test1PNG = new GLTexture{ testId, testWidth, testHeight };

	std::cout << id << "     " << test1PNG->width<< std::endl;
	std::cout << id << "     " << testPNG->width << std::endl;
	std::cout << testHeight << std::endl;
	gl_textures.emplace(id, test1PNG);
	std::cout << id << "     " << (*gl_textures[id]).height << std::endl;
}

const GLTexture* AssetManager::Get_GLTexture(std::string id)
{
	return gl_textures[id];
}