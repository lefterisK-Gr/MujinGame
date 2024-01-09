#include "AssetManager.h"
#include "../ECS/Components.h"
#include "../Collision/Collision.h"


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
		SDLK_w,
		SDLK_a,
		SDLK_d,
		SDLK_k,
		SDLK_s,
		SDLK_LSHIFT
		);
	player.addComponent<ColliderComponent>("player1");
	player.addComponent<ScoreComponent>();

	player.addComponent<Sword>();
	player.addComponent<Player_Script>();

	player.addGroup(Game::groupPlayers);
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
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<AnimatorComponent>(id);
	Vector2D direction = Vector2D::Distance(dest, pos).Normalize();
	projectile.addComponent<ProjectileComponent>(range, speed, direction);
	//projectile.getComponent<SpriteComponent>().animIndex = 0; //this actually shouldnt be here, this may result in errors, solution: decouple the character and his/her animations from SpriteComponent so we dont have to zero animIndex
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::CreateSkeleton(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{ //this is almost how we create the player
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 64, 64, 1);
	enemy.getComponent<TransformComponent>().velocity = vel;
	enemy.addComponent<AnimatorComponent>("skeleton");
	enemy.addComponent<ColliderComponent>("skeleton");
	enemy.addComponent<RigidBodyComponent>();
	enemy.getComponent<AnimatorComponent>().Play("SkeletonWalk");
	enemy.addComponent<Sword>(true);
	enemy.addComponent<Skeleton_Script>();
	//enemy.addComponent<ProjectileComponent>(range, speed, vel);

	//enemy.getComponent<TransformComponent>().velocity.x = 1;
	
	enemy.addGroup(Game::groupSkeletons);
}

void AssetManager::CreateGreenKoopaTroopa(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{ //this is almost how we create the player
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 64, 64, 1);
	enemy.getComponent<TransformComponent>().velocity = vel;
	enemy.addComponent<AnimatorComponent>("greenkoopatroopa");
	enemy.addComponent<GreenKoopaTroopa_Script>();
	//enemy.addComponent<ProjectileComponent>(range, speed, vel);
	enemy.addComponent<ColliderComponent>("greenkoopatroopa");
	enemy.addComponent<RigidBodyComponent>();
	enemy.getComponent<AnimatorComponent>().Play("GreenKoopaTroopaWalk");
	//enemy.getComponent<TransformComponent>().velocity.x = 1;

	enemy.addGroup(Game::groupGreenKoopaTroopas);
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