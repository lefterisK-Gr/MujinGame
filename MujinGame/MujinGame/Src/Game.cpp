
#include "Game.h"
#include "TextureManager/TextureManager.h"
#include "Camera2D/CameraManager.h"
#include "Map/Map.h"
#include "ECS/Components.h"
#include "ECS/ScriptComponents.h"
#include "Vector2D/Vector2D.h"
#include "Collision/Collision.h"
#include "Map/Map.h"
#include "AssetManager/AssetManager.h"
#include "SceneManager/SceneManager.h"
#include <sstream>
#include "GameScreen/IMainGame.h"

#undef main

SDL_Event Game::event;
Manager manager;
Collision collision;

SpriteBatch Game::_spriteBatch;
SpriteBatch Game::_hudSpriteBatch;

AudioEngine Game::audioEngine;

Map* Game::map = nullptr;
AssetManager* Game::assets = nullptr;
SceneManager* Game::scenes = new SceneManager();
MujinEngine::Window* Game::_window = nullptr;

auto& player1(manager.addEntity());
auto& sun(manager.addEntity());
auto& stagelabel(manager.addEntity());

Game::Game(MujinEngine::Window* window)
{
	_window = window;
	_screenIndex = SCREEN_INDEX_GAMEPLAY;
}
Game::~Game()
{

}

int Game::getNextScreenIndex() const {
	return _nextScreenIndex;
}

int Game::getPreviousScreenIndex() const {
	return _prevScreenIndex;
}

void Game::build() {

}

void Game::destroy() {

}

void Game::onEntry()
{
	assets = new AssetManager(&manager, _game->_inputManager, _game->_window);

	std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));
	std::shared_ptr<Camera2D> hud_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("hud"));


	main_camera2D->init(_window->getScreenWidth(), _window->getScreenHeight()); // Assuming a screen resolution of 800x600
	main_camera2D->setPosition(main_camera2D->getPosition() /*+ glm::vec2(
		width / 2.0f,
		height / 2.0f
	)*/);;
	main_camera2D->setScale(1.0f);

	hud_camera2D->init(_window->getScreenWidth(), _window->getScreenHeight());

	audioEngine.init();
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised..." << std::endl;

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		_colorProgram.compileShaders("Src/Shaders/colorShading.vert", "Src/Shaders/colorShading.frag");
		_colorProgram.addAttribute("vertexPosition");
		_colorProgram.addAttribute("vertexColor");
		_colorProgram.addAttribute("vertexUV");
		_colorProgram.linkShaders();

		_textureLightProgram.compileShaders("Src/Shaders/textureShading.vert", "Src/Shaders/textureShading.frag");
		_textureLightProgram.addAttribute("vertexPosition");
		_textureLightProgram.addAttribute("vertexColor");
		_textureLightProgram.addAttribute("vertexUV");
		_textureLightProgram.linkShaders();

		_textureProgram.compileShaders("Src/Shaders/textureBright.vert", "Src/Shaders/textureBright.frag");
		_textureProgram.addAttribute("vertexPosition");
		_textureProgram.addAttribute("vertexColor");
		_textureProgram.addAttribute("vertexUV");
		_textureProgram.linkShaders();

		_lightProgram.compileShaders("Src/Shaders/lightShading.vert", "Src/Shaders/lightShading.frag");
		_lightProgram.addAttribute("vertexPosition");
		_lightProgram.addAttribute("vertexColor");
		_lightProgram.addAttribute("vertexUV");
		_lightProgram.linkShaders();

		_waveProgram.compileShaders("Src/Shaders/waveEffect.vert", "Src/Shaders/waveEffect.frag");
		_waveProgram.addAttribute("vertexPosition");
		_waveProgram.addAttribute("vertexColor");
		_waveProgram.addAttribute("vertexUV");
		_waveProgram.linkShaders();

		Game::_spriteBatch.init();
		Game::_hudSpriteBatch.init();

		//_spriteFont = new SpriteFont("assets/arial.ttf", 32);

	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}

	//add the textures to our texture library
	TextureManager::getInstance().Add_GLTexture("backgroundMountains","assets/Sprites/villageBackground.png");
	TextureManager::getInstance().Add_GLTexture("terrain", "assets/Sprites/village_tileset.png");
	TextureManager::getInstance().Add_GLTexture("warrior", "assets/Sprites/samurai.png");
	TextureManager::getInstance().Add_GLTexture("projectile", "assets/Sprites/my_projectile.png");
	TextureManager::getInstance().Add_GLTexture("warriorProjectile", "assets/Sprites/warriorSlash.png");
	TextureManager::getInstance().Add_GLTexture("skeleton", "assets/Sprites/skeleton.png"); // same path since the same png has all entities
	TextureManager::getInstance().Add_GLTexture("greenkoopatroopa", "assets/Sprites/mushroom.png");
	TextureManager::getInstance().Add_GLTexture("sword", "assets/Sprites/sword.png");
	TextureManager::getInstance().Add_GLTexture("shield", "assets/Sprites/shield.png");
	TextureManager::getInstance().Add_GLTexture("healthPotion", "assets/Sprites/healthPotion.png");

	TextureManager::getInstance().Add_GLTexture("arial", "assets/Fonts/arial_cropped_white.png");

	Game::map = new Map("terrain", 1, 32);

	main_camera2D->worldLocation = map->GetLayerDimensions("assets/Maps/map_v3_Tile_Layer.csv");
	main_camera2D->worldLocation.w = main_camera2D->worldLocation.w - _window->getScreenWidth();

	manager.grid = std::make_unique<Grid>(main_camera2D->worldLocation.w + _window->getScreenWidth(), main_camera2D->worldLocation.h, CELL_SIZE);

	map->LoadMap("assets/Maps/background.csv","assets/Maps/background_v3.csv","assets/Maps/map_v3_Tile_Layer.csv", "assets/Maps/foreground_foreground.csv");

	assets->CreatePlayer(player1);
	manager.grid->addEntity(&player1);
	assets->CreateBackground();

	assets->CreateSunShape(sun);
	assets->CreateRandomParticlesGenerator();
	assets->CreateRain(player1);

	//assets->CreateSkeleton(Vector2D(100, 300), Vector2D(-1, 0), 200, 2, "enemy");
	assets->CreateSkeleton(Vector2D(3744, 300), Vector2D(-1, 0), "skeleton", false);
	assets->CreateSkeleton(Vector2D(500, 300), Vector2D(1, 0), "skeleton", false);
	assets->CreateSkeleton(Vector2D(1000, 300), Vector2D(1, -1), "skeleton", false);
	assets->CreateSkeleton(Vector2D(1400, 288), Vector2D(-1, -1), "skeleton", false);
	assets->CreateSkeleton(Vector2D(1900, 300), Vector2D(-1, -1), "skeleton", false);
	assets->CreateSkeleton(Vector2D(2675, 300), Vector2D(-1, -1), "skeleton", false);

	assets->CreateGreenKoopaTroopa(Vector2D(200, 400), Vector2D(-1, 0), 2, "greenkoopatroopa");
	assets->CreateGreenKoopaTroopa(Vector2D(3644, 100), Vector2D(-1, -1), 2, "greenkoopatroopa");

	assets->CreateShop();
	assets->CreateInventory();

	stagelabel.addComponent<TransformComponent>(32, 608, 32, 32, 1);
	stagelabel.addComponent<UILabel>(&manager, "stage 0", "arial", true);
	stagelabel.addGroup(Manager::groupLabels);

	Music music = audioEngine.loadMusic("Sounds/JPEGSnow.ogg");
	music.play(-1);
}

void Game::onExit() {

}

auto& tiles(manager.getGroup(Manager::groupActionLayer));
auto& players(manager.getGroup(Manager::groupPlayers));
auto& backgrounds(manager.getGroup(Manager::groupBackgrounds));
auto& colliders(manager.getGroup(Manager::groupColliders));
auto& projectiles(manager.getGroup(Manager::groupProjectiles));
auto& warriorprojectiles(manager.getGroup(Manager::groupWarriorProjectiles));
auto& skeletons(manager.getGroup(Manager::groupSkeletons));
auto& labels(manager.getGroup(Manager::groupLabels));
auto& stageupbtns(manager.getGroup(Manager::groupStageUpButtons));
auto& stageupattackbtns(manager.getGroup(Manager::groupStageUpAttackButtons));
auto& stageupdefencebtns(manager.getGroup(Manager::groupStageUpDefenceButtons));
auto& stageuphpbtns(manager.getGroup(Manager::groupStageUpHpButtons));
auto& stageupbtnsback(manager.getGroup(Manager::groupStageUpButtonsBack));
auto& itemslots(manager.getGroup(Manager::groupSlots));
auto& shop(manager.getGroup(Manager::groupShops));
auto& inventory(manager.getGroup(Manager::groupInventories));
auto& greenkoopatroopas(manager.getGroup(Manager::groupGreenKoopaTroopas));
auto& mysteryboxtiles(manager.getGroup(Manager::groupMysteryBoxes));
auto& winningtiles(manager.getGroup(Manager::groupWinningTiles));
auto& slices(manager.getGroup(Manager::groupSlices));
auto& enemyslices(manager.getGroup(Manager::groupEnemySlices));
auto& lights(manager.getGroup(Manager::groupLights));
auto& texturelights(manager.getGroup(Manager::groupTextureLights));
auto& raindrops(manager.getGroup(Manager::groupRainDrop));
auto& pipeforegroundsprites(manager.getGroup(Manager::groupPipeRingForeground));
auto& foregroundtiles(manager.getGroup(Manager::groupForegroundLayer));
auto& backgroundtiles(manager.getGroup(Manager::groupBackgroundLayer));
auto& sewerbackgroundtiles(manager.getGroup(Manager::groupSewerBackgroundLayer));
auto& markettiles(manager.getGroup(Manager::groupMarket));
auto& screenshapes(manager.getGroup(Manager::screenShapes));
auto& hpbars(manager.getGroup(Manager::groupHPBars));

void Game::update(float deltaTime) //game objects updating
{
	std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));
	std::shared_ptr<Camera2D> hud_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("hud"));


	if (map->getMapCompleted()) {

		//manager.clearAllEntities();
		for (auto& tile : tiles)
		{
			tile->destroy();
		}
		tiles.clear();
		for (auto& collider : colliders)
		{
			collider->destroy();
		}
		colliders.clear();
		for (auto& sewerbackgroundtile : sewerbackgroundtiles)
		{
			sewerbackgroundtile->destroy();
		}
		sewerbackgroundtiles.clear();
		backgroundtiles.clear();
		foregroundtiles.clear();
		mysteryboxtiles.clear();
		for (auto& enemy : skeletons)
		{
			enemy->destroy();
		}
		skeletons.clear();
		for (auto& enemy : greenkoopatroopas)
		{
			enemy->destroy();
		}
		greenkoopatroopas.clear();
		projectiles.clear();
		map->resetMap();
		assets->CreateEnemies();
		assets->CreateStageUpButtons();
		assets->RefreshShop();
		stagelabel.GetComponent<UILabel>().setLetters("stage" + std::to_string(map->getStage()));

		manager.refresh();
		manager.update(deltaTime);

		for (auto& enemyGroup : { skeletons, greenkoopatroopas }) // enemies with colliders
		{
			for (auto& enemy : enemyGroup) {
				bool collisionDetected = false;
				do {
					collisionDetected = false;
					for (auto& c : colliders)
					{
						for (auto& ccomp : c->components) {

							ColliderComponent* colliderComponentPtr = dynamic_cast<ColliderComponent*>(ccomp.get());

							if (!colliderComponentPtr) {
								continue;
							}

							SDL_Rect cCol = ccomp->getRect();
							SDL_Rect eCol = enemy->GetComponent<ColliderComponent>().collider;

							bool hasCollision = collision.checkCollision(eCol, cCol);

							if (hasCollision) {
								collisionDetected = true;
								enemy->GetComponent<TransformComponent>().position.y -= 32;
								enemy->GetComponent<ColliderComponent>().update(1.0f);
								break;
							}
						}
						if (collisionDetected) {
							break; // Break second innermost loop if collision is detected
						}
					}
				} while (collisionDetected);
				collision.isCollision = false;
				collision.isSidewaysCollision = false;
				collision.movingRectColSide = Collision::ColSide::NONE;
			}
		}
	}

	checkInput(); //handleEvents

	manager.refresh(); 
	manager.update(deltaTime );

	main_camera2D->update();
	hud_camera2D->update();

	
	for (auto& p : players)
	{
		p->GetComponent<RigidBodyComponent>().onGround = false;
	}

	for (auto& enemy : skeletons)
	{
		enemy->GetComponent<RigidBodyComponent>().onGround = false;
	}

	for (auto& enemy : greenkoopatroopas)
	{
		enemy->GetComponent<RigidBodyComponent>().onGround = false;
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
				SDL_Rect pCol = p->GetComponent<ColliderComponent>().collider;

				bool hasCollision = collision.checkCollisionIsSideways(pCol, cCol);
				if (hasCollision) {
					collision.moveFromCollision(*p);
					p->GetComponent<ColliderComponent>().update(1.0f);
				}

				if (collision.movingRectColSide == Collision::ColSide::TOP) {
					if (c->hasComponent<MysteryBox_Script>()
						&& !c->GetComponent<MysteryBox_Script>().getCoinAnimation()
						)// hitted mystery box
					{
						p->GetComponent<ScoreComponent>().addToScore(100);
						c->GetComponent<MysteryBox_Script>().doCoinAnimation = true;
						c->GetComponent<AnimatorComponent>().Play("CoinFlip");
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
		for (auto& c : manager.adjacentEntities(p, Manager::groupColliders))
		{
			if (c->hasGroup(Manager::groupWinningTiles)) {
				continue;
			}
			//SDL_Rect cCol = c->GetComponent<ColliderComponent>().collider;
			for (auto& ccomp : c->components) { // get all the ColliderComponents

				ColliderComponent* colliderComponentPtr = dynamic_cast<ColliderComponent*>(ccomp.get());

				if (!colliderComponentPtr) {
					continue;
				}

				SDL_Rect cCol = ccomp->getRect();
				SDL_Rect pCol = p->GetComponent<ColliderComponent>().collider;

				bool hasCollision = collision.checkCollisionIsSideways(pCol, cCol);
				if (hasCollision) {
					if (collision.isSidewaysCollision) {

						collision.moveFromCollision(*p);
						p->GetComponent<ColliderComponent>().update(1.0f);

						p->GetComponent<Player_Script>().leftofPipe = assets->LeftOfPipeTrigger(pCol);
					}
					if (!collision.isSidewaysCollision) {

						collision.moveFromCollision(*p);
						p->GetComponent<ColliderComponent>().update(1.0f);

						p->GetComponent<Player_Script>().leftofPipe = assets->LeftOfPipeTrigger(pCol);
					}
				}

				if (collision.movingRectColSide == Collision::ColSide::TOP) {
					if (c->hasComponent<PlatformBlock_Script>())
					{
						c->GetComponent<PlatformBlock_Script>().didBlockAnimation = true;
					}
				}

				collision.isCollision = false;
				collision.isSidewaysCollision = false;
				collision.movingRectColSide = Collision::ColSide::NONE; 
			}
		}

		collision.moveFromOuterBounds(*p, *_window);
	}


	for (auto& enemyGroup : { skeletons, greenkoopatroopas }) // enemies with colliders
	{
		for (auto& enemy : enemyGroup) {
			for (auto& c : manager.adjacentEntities(enemy, Manager::groupColliders))
			{
				//SDL_Rect cCol = c->GetComponent<ColliderComponent>().collider;
				for (auto& ccomp : c->components) {

					ColliderComponent* colliderComponentPtr = dynamic_cast<ColliderComponent*>(ccomp.get());

					if (!colliderComponentPtr) {
						continue;
					}

					SDL_Rect cCol = ccomp->getRect();
					SDL_Rect eCol = enemy->GetComponent<ColliderComponent>().collider;

					bool hasCollision = collision.checkCollisionIsSideways(eCol, cCol);

					if (hasCollision) {
						if (collision.isSidewaysCollision) {
							
							collision.moveFromCollision(*enemy);
							enemy->GetComponent<ColliderComponent>().update(1.0f);

							if ((enemy->GetComponent<TransformComponent>().velocity.x < 0 && collision.movingRectColSide == Collision::ColSide::LEFT) 
								|| (enemy->GetComponent<TransformComponent>().velocity.x > 0 && collision.movingRectColSide == Collision::ColSide::RIGHT)) {
								enemy->GetComponent<TransformComponent>().velocity.x *= -1;
							}
						}
						if (!collision.isSidewaysCollision) {
							collision.moveFromCollision(*enemy);
							enemy->GetComponent<ColliderComponent>().update(1.0f);
						}
					}

					collision.isCollision = false;
					collision.isSidewaysCollision = false;
					collision.movingRectColSide = Collision::ColSide::NONE;
				}
			}
			if (collision.moveFromOuterBounds(*enemy, *_window)) {
				enemy->GetComponent<TransformComponent>().velocity.x *= -1;
			}
			if (enemy->GetComponent<TransformComponent>().position.y > (main_camera2D->worldLocation.y + main_camera2D->worldLocation.h))
				enemy->destroy();
		}
	}

	for (auto& player : players) //enemies with players
	{
		for (auto& enemy : { skeletons , greenkoopatroopas })
		{
			for (auto& e : enemy)
			{
				SDL_Rect eCol = e->GetComponent<ColliderComponent>().collider;
				SDL_Rect pCol = player->GetComponent<ColliderComponent>().collider;

				bool hasCollision = collision.checkCollisionIsSideways(pCol, eCol);
				
				if (enemy != greenkoopatroopas || !e->GetComponent<GreenKoopaTroopa_Script>().shelltoturtle)
				{
					if (e->GetComponent<TransformComponent>().position.x < player->GetComponent<TransformComponent>().position.x + 300
						&& e->GetComponent<TransformComponent>().position.x > player->GetComponent<TransformComponent>().position.x) {
						e->GetComponent<TransformComponent>().velocity.x = -1;
					}
					else if (e->GetComponent<TransformComponent>().position.x > player->GetComponent<TransformComponent>().position.x - 300
						&& e->GetComponent<TransformComponent>().position.x < player->GetComponent<TransformComponent>().position.x) {
						e->GetComponent<TransformComponent>().velocity.x = 1;
					}
				}
				if (enemy == skeletons && e->GetComponent<Skeleton_Script>().attackAnimation ||
					enemy == greenkoopatroopas && e->GetComponent<GreenKoopaTroopa_Script>().attackAnimation) {
					e->GetComponent<TransformComponent>().velocity.x = 0;
				}
				

				if (hasCollision) {

					if (collision.movingRectColSide == Collision::ColSide::DOWN) {
						player->GetComponent<RigidBodyComponent>().onGround = true;
						player->GetComponent<TransformComponent>().velocity.y = -20;
						if (enemy == greenkoopatroopas) //green koopa troopa case
						{
							e->GetComponent<TransformComponent>().velocity.x = 0;
							e->GetComponent<AnimatorComponent>().Play("GreenShell");
							e->GetComponent<GreenKoopaTroopa_Script>().shelltoturtle = true;
						}
						else //skeleton case
						{
							if (e->GetComponent<LivingCharacter>().applyDamage(10)) {
								player->GetComponent<ScoreComponent>().addToScore(100);
								e->destroy();
							}
							if (!player->GetComponent<Player_Script>().tookDamage) {
								if (player->GetComponent<LivingCharacter>().applyDamage(5)) {
									player->destroy();
								}
							}
						}

					}
					else //player death
					{
						for (auto& pl : players)
						{
							if (!pl->GetComponent<Player_Script>().tookDamage) {
								if (pl->GetComponent<LivingCharacter>().applyDamage(5)) {
									pl->destroy();
								}
							}
						}
					}
				}

				collision.isCollision = false;
				collision.isSidewaysCollision = false;
				collision.movingRectColSide = Collision::ColSide::NONE;
			}

			for (auto& e : enemy) //enemies attack
			{
				if (enemy == skeletons) {
					SDL_Rect eCol = e->GetComponent<Sword>().hitBoxCollider;
					SDL_Rect pCol = player->GetComponent<ColliderComponent>().collider;

					bool hasCollision = collision.checkCollisionIsSideways(pCol, eCol);

					if (hasCollision) {
						e->GetComponent<Skeleton_Script>().activateAttack();
					}

					collision.isCollision = false;
					collision.isSidewaysCollision = false;
					collision.movingRectColSide = Collision::ColSide::NONE;
				}
				if (enemy == greenkoopatroopas && !e->GetComponent<GreenKoopaTroopa_Script>().shelltoturtle)
				{
					auto enemyTransform = e->GetComponent<TransformComponent>();
					auto playerTransform = player->GetComponent<TransformComponent>();

					if ((enemyTransform.position.x < playerTransform.position.x + 200 && enemyTransform.position.x > playerTransform.position.x) ||
						(enemyTransform.position.x > playerTransform.position.x - 200 && enemyTransform.position.x < playerTransform.position.x))
					{
						e->GetComponent<GreenKoopaTroopa_Script>().activateShoot();
					}
				}
			}
		}
	}

	for (auto& p : projectiles) //projectiles with players
	{
		for (auto& pl : players)
		{
			if (Collision::checkCollision(pl->GetComponent<ColliderComponent>().collider, p->GetComponent<ColliderComponent>().collider))
			{
				if (!pl->GetComponent<Player_Script>().tookDamage) {
					if (pl->GetComponent<LivingCharacter>().applyDamage(5)) {
						pl->destroy();
					}
				}
				p->destroy();
			}
			collision.isCollision = false;
		}
	}

	for (auto& sl : slices)
	{
		for (auto& enemy : { skeletons , greenkoopatroopas })
		{
			for (auto& e : enemy)
			{
				if (Collision::checkCollision(sl->GetComponent<ColliderComponent>().collider, e->GetComponent<ColliderComponent>().collider))
				{
					if (e->GetComponent<LivingCharacter>().applyDamage(sl->GetComponent<Slice>().sliceDamage)) {
						player1.GetComponent<ScoreComponent>().addToScore(100);
						e->destroy();
					}
				}
			}
		}
		sl->destroy();

	}

	for (auto& wpr : warriorprojectiles) {
		for (auto& enemy : { skeletons , greenkoopatroopas })
		{
			for (auto& e : enemy)
			{
				if (Collision::checkCollision(e->GetComponent<ColliderComponent>().collider, wpr->GetComponent<ColliderComponent>().collider))
				{
					if (e->GetComponent<LivingCharacter>().applyDamage(1)) {
						player1.GetComponent<ScoreComponent>().addToScore(100);
						e->destroy();
					}
				}
				collision.isCollision = false;
			}
		}
	}

	for (auto& esl : enemyslices)
	{
		for (auto& pl : players)
		{
			if (Collision::checkCollision(esl->GetComponent<ColliderComponent>().collider, pl->GetComponent<ColliderComponent>().collider))
			{
				if (!pl->GetComponent<Player_Script>().tookDamage) {
					if (pl->GetComponent<LivingCharacter>().applyDamage(esl->GetComponent<Slice>().sliceDamage)) {
						pl->destroy();
					}
				}
			}
		}
		esl->destroy();
	}

	for (auto& p : winningtiles) //winning tiles with players
	{
		for (auto& wcomp : p->components)
		{
			SDL_Rect wCol = wcomp->getRect();

			for (auto& pl : players)
			{
				if (Collision::checkCollision(wCol, pl->GetComponent<ColliderComponent>().collider))
				{
					pl->GetComponent<ScoreComponent>().addToScore(100);

					for (auto& player : players)
					{
						player->destroy();
					}
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
		main_camera2D->worldLocation.x = pl->GetComponent<TransformComponent>().position.x - main_camera2D->getCameraDimensions().x/2;
		if (pl->GetComponent<TransformComponent>().position.y >(main_camera2D->worldLocation.y + main_camera2D->worldLocation.h)) //player death
		{
			for (auto& player : players)
			{
				player->destroy();
			}
		}
	}
	
	for (auto& p : players) //scene transition
	{
		if (p->GetComponent<Player_Script>().finishedHorAnimation)
		{
			for (auto& pl : players)
			{
				pl->GetComponent<TransformComponent>().position = scenes->GetSceneStartupPosition(0);
			}
			main_camera2D->worldLocation = map->GetLayerDimensions("assets/Maps/RandomMap.csv");
			main_camera2D->worldLocation.w = main_camera2D->worldLocation.w - _window->getScreenWidth();
		}
		p->GetComponent<Player_Script>().finishedHorAnimation = false;
		p->GetComponent<Player_Script>().finishedVertAnimation = false;
	}

	for (auto& clouds : backgroundtiles) {
		if (clouds->GetComponent<TransformComponent>().position.x + clouds->GetComponent<TransformComponent>().width < 0) {
			clouds->GetComponent<TransformComponent>().position.x = main_camera2D->worldLocation.w * 1.0f/3.0f +_window->getScreenWidth(); //due to cloud z-index
		}
	}

	for (auto& sb : stageupbtns)
	{
		SpriteComponent entitySprite = sb->GetComponent<SpriteComponent>();
		if (collision.checkCollision(entitySprite.destRect, _mouseCoords)) { //culling
			std::cout << "clicked button" << std::endl;
			sb->GetComponent<ButtonComponent>().setState(ButtonComponent::ButtonState::PRESSED);
			break;
		}
	}

	_mouseCoords.x = -100.0f;
	_mouseCoords.y = -100.0f;

	if (main_camera2D->worldLocation.x < 0)
		main_camera2D->worldLocation.x = 0;
	if (main_camera2D->worldLocation.x > main_camera2D->worldLocation.w)
		main_camera2D->worldLocation.x = main_camera2D->worldLocation.w;
}

void Game::checkInput() {
	std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));
	std::shared_ptr<Camera2D> hud_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("hud"));

	_game->_inputManager.update();

	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
	
		_game->onSDLEvent(evnt);

		switch (evnt.type)
		{
		case SDL_MOUSEWHEEL:
			if (evnt.wheel.y > 0)
			{
				// Scrolling up
				main_camera2D->setScale(main_camera2D->getScale() + SCALE_SPEED);
			}
			else if (evnt.wheel.y < 0)
			{
				// Scrolling down
				main_camera2D->setScale(main_camera2D->getScale() - SCALE_SPEED);
			}
			break;
		}
		if (_game->_inputManager.isKeyPressed(SDLK_p)) {
			onPauseGame();
		}

		if (_game->_inputManager.isKeyPressed(SDLK_n)) {
			Game::map->setMapCompleted(true);
		}

		if (_game->_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
			glm::vec2 mouseCoordsVec = _game->_inputManager.getMouseCoords();
			_mouseCoords.x = mouseCoordsVec.x;
			_mouseCoords.y = mouseCoordsVec.y;
			std::cout << _mouseCoords.x << " " << _mouseCoords.y << std::endl;
		}

	}
}


void Game::setupShaderAndLightTexture(const std::string& textureName, Camera2D& camera) { // todo add camera2D.worldLocation argument
	_textureLightProgram.use();
	glActiveTexture(GL_TEXTURE0);
	const GLTexture* texture = TextureManager::getInstance().Get_GLTexture(textureName);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	GLint textureLocation = _textureLightProgram.getUniformLocation("texture_sampler");
	glUniform1i(textureLocation, 0);
	GLint pLocation = _textureLightProgram.getUniformLocation("projection");
	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	struct Light {
		glm::vec2 position;
		float radius;
	};

	std::vector<Light> lightsPos;

	for (auto& light : lights) {
		TransformComponent* lightTransform = &light->GetComponent<TransformComponent>();

		if (lightTransform) {
			Light tempLight;
			tempLight.position = glm::vec2(lightTransform->position.x, lightTransform->position.y);
			tempLight.radius = 100.0f;

			lightsPos.push_back(tempLight);
			//Light tempLight2;
			//tempLight2.position = glm::vec2(lightTransform->position.x + 100.0f, lightTransform->position.y);
			//tempLight2.radius = 0.0f;

			//lightsPos.push_back(tempLight2);
		}
	}

	GLint lightPosLocation = _textureLightProgram.getUniformLocation("lightPos[0].position");
	glUniform2fv(lightPosLocation, 1, &(lightsPos[0].position[0]));  // Pass the address of the first element of lightPos
	GLint lightRadiusLocation = _textureLightProgram.getUniformLocation("lightPos[0].radius");
	glUniform1f(lightRadiusLocation, lightsPos[0].radius);

	//GLint lightPosLocation2 = _textureLightProgram.getUniformLocation("lightPos[1].position");
	//glUniform2fv(lightPosLocation2, 1, &(lightsPos[1].position[0]));  // Pass the address of the first element of lightPos
}

void Game::setupShaderAndTexture(const std::string& textureName, Camera2D& camera) { // todo add camera2D.worldLocation argument
	_textureProgram.use();
	glActiveTexture(GL_TEXTURE0);
	const GLTexture* texture = TextureManager::getInstance().Get_GLTexture(textureName);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	GLint textureLocation = _textureProgram.getUniformLocation("texture_sampler");
	glUniform1i(textureLocation, 0);
	GLint pLocation = _textureProgram.getUniformLocation("projection");
	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
}

void Game::setupShaderAndWaveTexture(const std::string& textureName, Camera2D& camera) { // todo add camera2D.worldLocation argument
	_waveProgram.use();
	glActiveTexture(GL_TEXTURE0);
	const GLTexture* texture = TextureManager::getInstance().Get_GLTexture(textureName);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	GLint textureLocation = _waveProgram.getUniformLocation("texture_sampler");
	glUniform1i(textureLocation, 0);
	float currentTime = SDL_GetTicks() / 1000.0f;
	float elapsedTime = currentTime - Game::startTime;
	GLuint timeLocation = _waveProgram.getUniformLocation("time");
	glUniform1f(timeLocation, elapsedTime);
	GLint pLocation = _waveProgram.getUniformLocation("projection");
	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
}

void Game::renderBatch(const std::vector<Entity*>& entities, SpriteBatch& batch) { // todo add batch argument
	std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));

	batch.begin();
	for (const auto& entity : entities) {
		if (entity->hasComponent<SpriteComponent>()) {
			SpriteComponent entitySprite = entity->GetComponent<SpriteComponent>();
			if (collision.checkCollision(entitySprite.destRect, main_camera2D->getCameraRect())) { //culling
				entity->draw(batch, *Game::_window);
			}
		}
		else {
			entity->draw(batch, *Game::_window);
		}
	}
	batch.end();
	batch.renderBatch();
}

void Game::draw()
{
	std::shared_ptr<Camera2D> main_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("main"));
	std::shared_ptr<Camera2D> hud_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("hud"));

	////////////OPENGL USE
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.025f, 0.05f, 0.15f, 1.0f);


	/////////////////////////////////////////////////////
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	setupShaderAndWaveTexture("backgroundMountains", *main_camera2D);
	renderBatch(backgrounds, _spriteBatch);
	_waveProgram.unuse();
	setupShaderAndLightTexture("terrain", *main_camera2D);
	renderBatch(backgroundtiles, _spriteBatch);
	renderBatch(sewerbackgroundtiles, _spriteBatch);
	renderBatch(tiles, _spriteBatch);
	//renderBatch(colliders);
	
	setupShaderAndLightTexture("projectile", *main_camera2D);
	renderBatch(projectiles, _spriteBatch);
	setupShaderAndLightTexture("warriorProjectile", *main_camera2D);
	renderBatch(warriorprojectiles, _spriteBatch);
	setupShaderAndLightTexture("skeleton", *main_camera2D);
	renderBatch(skeletons, _spriteBatch);
	renderBatch(greenkoopatroopas, _spriteBatch);
	setupShaderAndLightTexture("arial", *main_camera2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	_textureLightProgram.unuse();
	///////////////////////////////////////////////////////

	GLint pLocation = _lightProgram.getUniformLocation("projection");
	glm::mat4 cameraMatrix = main_camera2D->getCameraMatrix();

	///////////////////////////////////////////////////////
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setupShaderAndLightTexture("warrior", *main_camera2D);
	renderBatch(players, _spriteBatch);
	setupShaderAndLightTexture("terrain", *main_camera2D);
	renderBatch(winningtiles, _spriteBatch);
	renderBatch(foregroundtiles, _spriteBatch);
	_lightProgram.use();

	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//renderBatch(lights);
	renderBatch(texturelights, _spriteBatch);

	_lightProgram.unuse();
	drawHUD(labels, "arial");
	_colorProgram.use();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	pLocation = _textureLightProgram.getUniformLocation("projection");
	cameraMatrix = hud_camera2D->getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
	renderBatch(shop, _hudSpriteBatch);
	renderBatch(stageupbtnsback, _hudSpriteBatch);
	renderBatch(inventory, _hudSpriteBatch);
	renderBatch(itemslots, _hudSpriteBatch);

	_colorProgram.unuse();

	drawHUD(stageupattackbtns, "sword");
	drawHUD(stageupdefencebtns, "shield");
	drawHUD(stageuphpbtns, "healthPotion");

	glBindTexture(GL_TEXTURE_2D, 0);
	_textureLightProgram.unuse();
	///////////////////////////////////////////////////////
	_colorProgram.use();

	pLocation = _textureLightProgram.getUniformLocation("projection");
	cameraMatrix = main_camera2D->getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//renderBatch(screenshapes);
	renderBatch(hpbars, _spriteBatch);
	renderBatch(raindrops, _spriteBatch);

	_colorProgram.unuse();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
}


void Game::drawHUD(const std::vector<Entity*>& entities, const std::string& textureName) {
	std::shared_ptr<Camera2D> hud_camera2D = std::dynamic_pointer_cast<Camera2D>(CameraManager::getInstance().getCamera("hud"));

	setupShaderAndTexture(textureName, *hud_camera2D);
	renderBatch(entities, _hudSpriteBatch);
}

bool Game::onPauseGame() {
	_prevScreenIndex = SCREEN_INDEX_MAIN_MENU;
	_currentState = ScreenState::CHANGE_PREVIOUS;
	return true;
}