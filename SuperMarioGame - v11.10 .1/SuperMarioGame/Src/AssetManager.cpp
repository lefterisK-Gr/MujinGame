#include "AssetManager.h"
#include "ECS/Components.h"


AssetManager::AssetManager(Manager* man) : manager(man)
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
	std::cout << "entered Projectile Explosion" << std::endl;
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(2, 0), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(-2, 0), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(0, 2), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(0, -2), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(2, 2), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(2, -2), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(-2, 2), 200, 2, "projectile");
	CreateProjectile(Vector2D(camerapos + 400, 320), Vector2D(-2, -2), 200, 2, "projectile");
}

void CreatePlayerComponents(Entity& player) // todo: this isn't used
{
	player.addComponent<AnimatorComponent>("player");
	player.addComponent<Player_Script>();
	player.addComponent<RigidBodyComponent>();
	//player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("player1");
	player.addComponent<ScoreComponent>();
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel,int range, int speed, std::string id)
{ //this is almost how we create the player
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	projectile.addComponent<AnimatorComponent>(id);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	//projectile.getComponent<SpriteComponent>().animIndex = 0; //this actually shouldnt be here, this may result in errors, solution: decouple the character and his/her animations from SpriteComponent so we dont have to zero animIndex
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::CreateGoomba(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{ //this is almost how we create the player
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	enemy.getComponent<TransformComponent>().storedVelocity = vel;
	enemy.addComponent<AnimatorComponent>("goomba");
	enemy.addComponent<Goomba_Script>();
	//enemy.addComponent<ProjectileComponent>(range, speed, vel);
	enemy.addComponent<ColliderComponent>("goomba");
	enemy.addComponent<RigidBodyComponent>();
	enemy.getComponent<AnimatorComponent>().Play("GoombaWalk");
	//enemy.getComponent<TransformComponent>().velocity.x = 1;
	
	enemy.addGroup(Game::groupGoombas);
}

void AssetManager::CreateGreenKoopaTroopa(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{ //this is almost how we create the player
	auto& enemy(manager->addEntity());
	enemy.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
	enemy.getComponent<TransformComponent>().storedVelocity = vel;
	enemy.addComponent<AnimatorComponent>("greenkoopatroopa");
	enemy.addComponent<GreenKoopaTroopa_Script>();
	//enemy.addComponent<ProjectileComponent>(range, speed, vel);
	enemy.addComponent<ColliderComponent>("greenkoopatroopa");
	enemy.addComponent<RigidBodyComponent>();
	enemy.getComponent<AnimatorComponent>().Play("GreenKoopaTroopaWalk");
	//enemy.getComponent<TransformComponent>().velocity.x = 1;

	enemy.addGroup(Game::groupGreenKoopaTroopas);
}

void AssetManager::ActivateEnemy(Entity& enemy)
{ //this is almost how we create the player
	Vector2D transform = enemy.getComponent<TransformComponent>().position;
	if (transform.x < Game::camera.x + 800	&&
		transform.x > Game::camera.x		&&
		transform.y < Game::camera.y + 640	&&
		transform.y > Game::camera.y		&&
		(enemy.getComponent<TransformComponent>().storedVelocity.x ||
		enemy.getComponent<TransformComponent>().storedVelocity.y))
	{
		enemy.getComponent<TransformComponent>().velocity.x = enemy.getComponent<TransformComponent>().storedVelocity.x;
		enemy.getComponent<TransformComponent>().storedVelocity.Zero();
	}
}

bool AssetManager::OnPipeTrigger(SDL_Rect collider)
{
	for (int i = 0; i < 2; i++)
	{
		if (onpipeTriggers[i].x <= collider.x						&&
			onpipeTriggers[i].x + onpipeTriggers[i].w > collider.x	&&
			onpipeTriggers[i].y <= collider.y						&&
			onpipeTriggers[i].y + onpipeTriggers[i].h > collider.y
			)
		{
			return true;
		}
	}
	return false;
}

bool AssetManager::LeftOfPipeTrigger(SDL_Rect collider)
{
	for (int i = 0; i < 1; i++)
	{
		if (leftofpipeTriggers[i].x <= collider.x							&&
			leftofpipeTriggers[i].x + leftofpipeTriggers[i].w > collider.x	&&
			leftofpipeTriggers[i].y <= collider.y							&&
			leftofpipeTriggers[i].y + leftofpipeTriggers[i].h > collider.y
			)
		{
			return true;
		}
	}
	return false;
}

void AssetManager::AddPipeRingForegroundTile(int srcX, int srcY, Vector2D pos)
{
	auto& pipesprite(manager->addEntity());
	pipesprite.addComponent<SpriteComponent>("terrain", pos, true); //insert piperingforegroundtile (has same texture as simple terrain)
	pipesprite.getComponent<SpriteComponent>().SetFrames(srcX, srcY, 1, 100);//speed isnt needed
	pipesprite.addGroup(Game::groupPipeRingForeground);
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::GetFont(std::string id)
{
	return fonts[id];
}