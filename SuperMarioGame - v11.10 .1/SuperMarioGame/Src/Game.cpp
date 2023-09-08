
#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include "RendererShapes.h"
#include "SceneManager.h"
#include <sstream>

#undef main

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Manager manager;
Collision collision;

SDL_Rect Game::camera = { 0,0,2240,0 }; // camera.w shows how far right camera can go

AssetManager* Game::assets = new AssetManager(&manager);

SceneManager* Game::scenes = new SceneManager();

bool Game::isRunning = false;
bool Game::isPaused = false;

uint32_t Game::pauseTime = 0;
bool Game::justResumed = false;

auto& player1(manager.addEntity());
auto& label(manager.addEntity());
auto& winningLabel1(manager.addEntity());
auto& winningLabel2(manager.addEntity());
auto& pauseLabel(manager.addEntity());
auto& scoreboard1(manager.addEntity());
auto& scoreboard2(manager.addEntity());

Game::Game()
{

}
Game::~Game()
{

}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;

	SDL_Color black = { 0, 0 ,0 ,255 };
	SDL_Color white = { 255, 255 ,255 ,255 };
	SDL_Color red = { 255, 0 ,0 ,255 };
	SDL_Color green = { 0, 255 ,0 ,255 };
	// create renderer
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 52, 171, 218, 255);
			std::cout << "Renderer created!" << std::endl;
		}
		isRunning = true;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}

	//add the textures to our texture library
	assets->AddTexture("terrain", "assets/village_tileset.png");
	assets->AddTexture("player", "assets/mario_luigi_animations_1.png");
	assets->AddTexture("warrior", "assets/warrior_animations.png");
	assets->AddTexture("projectile", "assets/my_projectile.png");
	assets->AddTexture("goomba", "assets/super_mario_tileset_scaled.png"); // same path since the same png has all entities
	assets->AddTexture("greenkoopatroopa", "assets/super_mario_tileset_scaled.png");

	assets->AddFont("arial", "assets/arial.ttf", 20);
	assets->AddFont("arialBig", "assets/arial.ttf", 100);

	map = new Map("terrain", 1, 32);

	map->LoadMap("assets/background_v3.csv", "assets/background.csv","assets/map_v3_Tile_Layer.csv", "assets/foreground_foreground.csv");

	player1.addComponent<TransformComponent>(200.0f, 320.0f, 32, 32, 2); // 1448 for near pipe, 200 for start
	//assets->CreatePlayerComponents(player1);
	//instead of this
	player1.addComponent<AnimatorComponent>("warrior");
	player1.addComponent<MovingAnimatorComponent>("warrior");
	player1.addComponent<RigidBodyComponent>();
	player1.addComponent<KeyboardControllerComponent>(
		(char*)"P1Idle",
		(char*)"P1Jump",
		(char*)"P1Walk",
		(char*)"P1Attack",
		SDL_SCANCODE_W,
		SDL_SCANCODE_A,
		SDL_SCANCODE_D,
		SDL_SCANCODE_K,
		SDL_SCANCODE_S,
		SDL_SCANCODE_LSHIFT
		);
	player1.addComponent<ColliderComponent>("player1");
	player1.addComponent<ScoreComponent>();

	player1.addComponent<UILabel>(10, 600, "MARIO SCORE: ", "arial", red);
	player1.addComponent<UILabel>(160, 600, "", "arial", red);
	player1.addComponent<Player_Script>();

	player1.addGroup(groupPlayers);
	
	/*label.addComponent<UILabel>(10, 42, "University of Crete", "arial", black);
	label.addComponent<UILabel>(10, 58, "Department of Computer Science", "arial", black);
	label.addComponent<UILabel>(10, 10, "Lefteris Kotsonas", "arial", black);
	label.addComponent<UILabel>(10, 26, "CS-454, Development of Intelligent Interfaces and Games. Fall Semester 2021-22", "arial", black);*/

	winningLabel1.addComponent<UILabel>(100, 300, "", "arialBig", red);
	winningLabel2.addComponent<UILabel>(100, 300, "", "arialBig", green);

	pauseLabel.addComponent<UILabel>(50, 300, "", "arialBig", white);

	//assets->CreateGoomba(Vector2D(100, 300), Vector2D(-1, 0), 200, 2, "enemy");
	assets->CreateGoomba(Vector2D(3744, 300), Vector2D(-1, 0), 200, 2, "goomba");
	assets->CreateGoomba(Vector2D(500, 300), Vector2D(1, 0), 200, 2, "goomba");
	assets->CreateGoomba(Vector2D(1000, 300), Vector2D(1, -1), 200, 2, "goomba");
	assets->CreateGoomba(Vector2D(1400, 288), Vector2D(-1, -1), 200, 2, "goomba");
	assets->CreateGoomba(Vector2D(1900, 300), Vector2D(-1, -1), 200, 2, "goomba");
	assets->CreateGoomba(Vector2D(2675, 300), Vector2D(-1, -1), 200, 2, "goomba");

	assets->CreateGreenKoopaTroopa(Vector2D(200, 400), Vector2D(-1, 0), 200, 2, "greenkoopatroopa");
	assets->CreateGreenKoopaTroopa(Vector2D(3644, 100), Vector2D(-1, -1), 200, 2, "greenkoopatroopa");

}

auto& tiles(manager.getGroup(Game::groupActionLayer));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& goombas(manager.getGroup(Game::groupGoombas));
auto& greenkoopatroopas(manager.getGroup(Game::groupGreenKoopaTroopas));
auto& mysteryboxtiles(manager.getGroup(Game::groupMysteryBoxes));
auto& winningtiles(manager.getGroup(Game::groupWinningTiles));
auto& pipeforegroundsprites(manager.getGroup(Game::groupPipeRingForeground));
auto& foregroundtiles(manager.getGroup(Game::groupForegroundLayer));
auto& backgroundtiles(manager.getGroup(Game::groupBackgroundLayer));
auto& sewerbackgroundtiles(manager.getGroup(Game::groupSewerBackgroundLayer));

void Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_p:
				if (Game::isPaused)
				{
					pauseLabel.getComponent<UILabel>().SetLabelText("", "arialBig");
					Game::justResumed = true;
					Game::isPaused = false;
				}
				else
				{
					pauseLabel.getComponent<UILabel>().SetLabelText("GAME PAUSED", "arialBig");
					Game::isPaused = true;
					Game::pauseTime = SDL_GetTicks();
				}
					
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
			default:
				break;
			}
			break;
		default:
			break;
	}
}

void Game::update() //game objects updating
{
	std::stringstream ss1;
	std::stringstream ss2;
	std::stringstream winningss;

	//ss << "Player Position: " << player1Pos << " and Score: " << player1.getComponent<ScoreComponent>().score;

	Vector2D plMaxDistance;

	manager.refresh(); 
	manager.update();

		
	for (auto& p : players)
	{
		p->getComponent<RigidBodyComponent>().onGround = false;
	}

	for (auto& enemy : goombas)
	{
		enemy->getComponent<RigidBodyComponent>().onGround = false;
	}

	for (auto& enemy : greenkoopatroopas)
	{
		enemy->getComponent<RigidBodyComponent>().onGround = false;
	}

	for (auto& p : players) //players with mysteryboxes
	{
		for (auto& c : mysteryboxtiles)
		{
			for (auto& ccomp : c->components)
			{
				ColliderComponent* colliderComponentPtr = dynamic_cast<ColliderComponent*>(ccomp.get());

				if (!colliderComponentPtr) {
					continue;
				}

				SDL_Rect cCol = ccomp->getRect();
				SDL_Rect pCol = p->getComponent<ColliderComponent>().collider;

				bool hasCollision = collision.checkCollisionIsSideways(pCol, cCol);
				if (hasCollision) {
					collision.moveFromCollision(*p);
				}

				if (collision.movingRectColSide == Collision::ColSide::TOP) {
					if (c->hasComponent<MysteryBox_Script>()
						&& !c->getComponent<MysteryBox_Script>().getCoinAnimation()
						)// hitted mystery box
					{
						p->getComponent<ScoreComponent>().addToScore(100);
						c->getComponent<MysteryBox_Script>().doCoinAnimation = true;
						c->getComponent<AnimatorComponent>().Play("CoinFlip");
						//PlaySound(TEXT("coin_collect.wav"), NULL, SND_ASYNC);
						ss1 << p->getComponent<ScoreComponent>().getScore();
						p->getComponent<UILabel>().SetLabelText(ss1.str(), "arial");
					}
				}

				collision.isCollision = false;
				collision.isSidewaysCollision = false;
				collision.movingRectColSide = Collision::ColSide::NONE;
			}
		}
	}

	for (auto& p : players) //player with colliders
	{
		for (auto& c : colliders)
		{
			if (c->hasGroup(Game::groupWinningTiles)) {
				continue;
			}
			//SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
			for (auto& ccomp : c->components) { // get all the ColliderComponents

				ColliderComponent* colliderComponentPtr = dynamic_cast<ColliderComponent*>(ccomp.get());

				if (!colliderComponentPtr) {
					continue;
				}

				SDL_Rect cCol = ccomp->getRect();
				SDL_Rect pCol = p->getComponent<ColliderComponent>().collider;

				bool hasCollision = collision.checkCollisionIsSideways(pCol, cCol);
				if (hasCollision) {
					if (collision.isSidewaysCollision) {

						collision.moveFromCollision(*p);

						p->getComponent<Player_Script>().leftofPipe = assets->LeftOfPipeTrigger(pCol);
					}
					if (!collision.isSidewaysCollision) {

						collision.moveFromCollision(*p);

						p->getComponent<Player_Script>().onPipe = assets->OnPipeTrigger(cCol);
					}
				}

				if (collision.movingRectColSide == Collision::ColSide::TOP) {
					if (c->hasComponent<PlatformBlock_Script>())
					{
						c->getComponent<PlatformBlock_Script>().didBlockAnimation = true;
					}
				}

				collision.isCollision = false;
				collision.isSidewaysCollision = false;
				collision.movingRectColSide = Collision::ColSide::NONE; 
			}
		}
	}


	for (auto& enemyGroup : { goombas, greenkoopatroopas }) // enemies with colliders
	{
		for (auto& enemy : enemyGroup) {
			assets->ActivateEnemy(*enemy);
			for (auto& c : colliders)
			{
				//SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
				for (auto& ccomp : c->components) {

					ColliderComponent* colliderComponentPtr = dynamic_cast<ColliderComponent*>(ccomp.get());

					if (!colliderComponentPtr) {
						continue;
					}

					SDL_Rect cCol = ccomp->getRect();
					SDL_Rect eCol = enemy->getComponent<ColliderComponent>().collider;

					bool hasCollision = collision.checkCollisionIsSideways(eCol, cCol);

					if (hasCollision) {
						if (collision.isSidewaysCollision) {
							
							collision.moveFromCollision(*enemy);

							if ((enemy->getComponent<TransformComponent>().velocity.x < 0 && collision.movingRectColSide == Collision::ColSide::LEFT) 
								|| (enemy->getComponent<TransformComponent>().velocity.x > 0 && collision.movingRectColSide == Collision::ColSide::RIGHT)) {
								enemy->getComponent<TransformComponent>().velocity.x *= -1;

								if (enemyGroup == greenkoopatroopas) {
									if (enemy->getComponent<SpriteComponent>().spriteFlip == SDL_FLIP_HORIZONTAL)
									{
										enemy->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_NONE;
									}
									else
										enemy->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
								}
							}
						}
						if (!collision.isSidewaysCollision) {
							collision.moveFromCollision(*enemy);

							if (c != NULL && c->hasComponent<PlatformBlock_Script>() && c->getComponent<PlatformBlock_Script>().didBlockAnimation)
							{
								enemy->destroy();
							}
						}
					}

					collision.isCollision = false;
					collision.isSidewaysCollision = false;
					collision.movingRectColSide = Collision::ColSide::NONE;
				}
			}
			if (enemy->getComponent<TransformComponent>().position.y > (camera.y + 640))
				enemy->destroy();
		}
	}

	for (auto& player : players) //enemies with players
	{
		for (auto& enemy : { goombas , greenkoopatroopas })
		{
			for (auto& e : enemy)
			{
				SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
				SDL_Rect pCol = player->getComponent<ColliderComponent>().collider;

				bool hasCollision = collision.checkCollisionIsSideways(pCol, eCol);

				if (hasCollision) {
					collision.moveFromCollision(*player);

					if (collision.movingRectColSide == Collision::ColSide::DOWN) {
						player->getComponent<RigidBodyComponent>().onGround = true;
						player->getComponent<TransformComponent>().velocity.y = -20;
						if (enemy == greenkoopatroopas) //green koopa troopa case
						{
							e->getComponent<TransformComponent>().velocity.x = 0;
							e->getComponent<AnimatorComponent>().Play("GreenShell");
							e->getComponent<GreenKoopaTroopa_Script>().shelltoturtle = true;
						}
						else //goomba case
						{
							player->getComponent<ScoreComponent>().addToScore(100);
							ss1 << player->getComponent<ScoreComponent>().getScore();
							player->getComponent<UILabel>().SetLabelText(ss1.str(), "arial");
							e->destroy();
						}

					}
					else if (collision.isSidewaysCollision
						&& enemy == greenkoopatroopas
						&& e->getComponent<AnimatorComponent>().animimationName == "GreenShell"
						&& !e->getComponent<TransformComponent>().velocity.x)
					{
						e->getComponent<TransformComponent>().velocity.x = 5;
						if (player->getComponent<TransformComponent>().velocity.x < 0)
						{
							e->getComponent<TransformComponent>().velocity.x *= -1;
						}
						e->getComponent<SpriteComponent>().initTime = 0;
					}
					else //player death
					{
						winningss << "YOU DIED";
						winningLabel2.getComponent<UILabel>().SetLabelText(winningss.str(), "arialBig");


						for (auto& pl : players)
						{
							pl->destroy();
						}
						assets->ProjectileExplosion(camera.x);
					}
				}

				collision.isCollision = false;
				collision.isSidewaysCollision = false;
				collision.movingRectColSide = Collision::ColSide::NONE;
			}
		}
	}

	for (auto& greenkoopatroopa : greenkoopatroopas)//shells with enemies
	{
		if (greenkoopatroopa->getComponent<AnimatorComponent>().getPlayName() == "GreenShell")
		{
			for (auto& goomba : goombas)
			{
				SDL_Rect gktCol = greenkoopatroopa->getComponent<ColliderComponent>().collider;
				SDL_Rect gCol = goomba->getComponent<ColliderComponent>().collider;

				bool hasCollision = collision.checkCollisionIsSideways(gktCol, gCol);

				if (hasCollision && collision.isSidewaysCollision) {
					goomba->destroy();
				}

				collision.isCollision = false;
				collision.isSidewaysCollision = false;
				collision.movingRectColSide = Collision::ColSide::NONE;
			}
		}
	}

	for (auto& p : projectiles) //projectiles with players
	{
		for (auto& pl : players)
		{
			if (Collision::checkCollision(pl->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
			{
				std::cout << "Hit player!" << std::endl;
				p->destroy();
			}
			collision.isCollision = false;
		}
	}

	for (auto& p : winningtiles) //winning tiles with players
	{
		for (auto& wcomp : p->components)
		{
			SDL_Rect wCol = wcomp->getRect();

			for (auto& pl : players)
			{
				if (Collision::checkCollision(wCol, pl->getComponent<ColliderComponent>().collider))
				{
					pl->getComponent<ScoreComponent>().addToScore(100);

					winningss << "MARIO WINS     ";
					winningLabel1.getComponent<UILabel>().SetLabelText(winningss.str(), "arialBig");

					for (auto& player : players)
					{
						player->destroy();
					}
					assets->ProjectileExplosion(camera.x);
					collision.isCollision = true;
					break;
				}
			}
			if (collision.isCollision)
				break;
		}
		if (collision.isCollision)
			break;
	}
	
	for (auto& pl : players) //player rules
	{
		if (!plMaxDistance.x)
		{
			plMaxDistance.x = pl->getComponent<TransformComponent>().position.x;
		}
		else if (pl->getComponent<TransformComponent>().position.x > plMaxDistance.x)
		{
			plMaxDistance.x = pl->getComponent<TransformComponent>().position.x;
		}
		camera.x = plMaxDistance.x - 400;
		if (pl->getComponent<TransformComponent>().position.y >(camera.y + 640)) //player death
		{
			winningss << "YOU DIED";
			winningLabel2.getComponent<UILabel>().SetLabelText(winningss.str(), "arialBig");

			for (auto& player : players)
			{
				player->destroy();
			}
			assets->ProjectileExplosion(camera.x);
		}
	}
	
	for (auto& p : players) //scene transition
	{
		if (p->getComponent<Player_Script>().finishedVertAnimation)
		{
			scenes->sceneSelected = 1;
			for (auto& pl : players)
			{
				pl->getComponent<TransformComponent>().position = scenes->GetSceneStartupPosition(1);
			}
			std::cout << "position teleported: " << p->getComponent<TransformComponent>().position << std::endl;
			camera = scenes->GetSceneCamera(1);
		}
		if (p->getComponent<Player_Script>().finishedHorAnimation)
		{
			scenes->sceneSelected = 0;
			for (auto& pl : players)
			{
				pl->getComponent<TransformComponent>().position = scenes->GetSceneStartupPosition(0);
			}
			std::cout << "position teleported: " << p->getComponent<TransformComponent>().position << std::endl;
			camera = scenes->GetSceneCamera(0);
		}
		p->getComponent<Player_Script>().finishedHorAnimation = false;
		p->getComponent<Player_Script>().finishedVertAnimation = false;
	}

	if (camera.x < scenes->GetSceneCamera(scenes->sceneSelected).x)
		camera.x = scenes->GetSceneCamera(scenes->sceneSelected).x;
	if (camera.y < 0)
		camera.y = 0;
	if(camera.x > (scenes->GetSceneCamera(scenes->sceneSelected).x + camera.w))
		camera.x = (scenes->GetSceneCamera(scenes->sceneSelected).x + camera.w);
	if(camera.y > camera.h)
		camera.y = camera.h;
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 52, 171, 218, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	DrawCircle(renderer, 200, 150, 20);
	//the order of rendered objects are the order of layers
	//in SDL_RenderCopy 3rd arg is for the part of the texture to be displayed and 4th argument is for part of the display Window to be covered
	//SDL_RenderCopy(renderer, playerTex, NULL, &destR);
	// instead of the above, we use the GameObjectRenderer
	
	for (auto& p : backgroundtiles)
	{
		p->draw();
	}

	for (auto& p : sewerbackgroundtiles)
	{
		p->draw();
	}

	for (auto& t : tiles)
	{
		t->draw();
	}

	for (auto& c : colliders) //remove this if you dont want to see the yellow lines in colliders
	{
		c->draw();
	}

	for (auto& p : players) //todo manager.draw()
	{
		p->draw();
	}

	for (auto& p : projectiles)
	{
		p->draw();
	}

	for (auto& p : goombas)
	{
		p->draw();
	}

	for (auto& p : greenkoopatroopas)
	{
		p->draw();
	}

	for (auto& p : winningtiles)
	{
		p->draw();
	}

	for (auto& p : pipeforegroundsprites)
	{
		p->draw();
	}

	for (auto& p : foregroundtiles)
	{
		p->draw();
	}

	label.draw();
	winningLabel1.draw();
	winningLabel2.draw();
	pauseLabel.draw();
	scoreboard1.draw();
	scoreboard2.draw();
	//add stuff to render
	//SDL_RenderClear(renderer);
	
	SDL_RenderPresent(renderer);

}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}