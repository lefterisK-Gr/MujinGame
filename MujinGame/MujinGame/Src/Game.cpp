
#include "Game.h"
#include "TextureManager/TextureManager.h"
#include "Camera2.5D/CameraManager.h"
#include "Map/Map.h"
#include "ECS/Components.h"
#include "ECS/ScriptComponents.h"
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
float Game::backgroundColor[4] = {0.025f, 0.05f, 0.15f, 1.0f};
MujinEngine::Window* Game::_window = nullptr;

auto& player1(manager.addEntity());
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

	std::shared_ptr<PerspectiveCamera> main_camera2D = std::dynamic_pointer_cast<PerspectiveCamera>(CameraManager::getInstance().getCamera("main"));
	std::shared_ptr<OrthoCamera> hud_camera2D = std::dynamic_pointer_cast<OrthoCamera>(CameraManager::getInstance().getCamera("hud"));

	_debugRenderer.init();

	main_camera2D->init(_window->getScreenWidth(), _window->getScreenHeight()); // Assuming a screen resolution of 800x600
	main_camera2D->setPosition_X(main_camera2D->getPosition().x /*+ glm::vec2(
		width / 2.0f,
		height / 2.0f
	)*/);
	main_camera2D->setPosition_Y(main_camera2D->getPosition().y);
	main_camera2D->setScale(1.0f);

	hud_camera2D->init(_window->getScreenWidth(), _window->getScreenHeight());

	audioEngine.init();
	AnimatorManager& animManager = AnimatorManager::getInstance();
	animManager.InitializeAnimators();
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised..." << std::endl;

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		_colorProgram.compileShaders("Src/Shaders/colorShading.vert", "Src/Shaders/colorShading.frag");
		_colorProgram.addAttribute("vertexPosition");
		_colorProgram.addAttribute("vertexColor");
		_colorProgram.addAttribute("vertexUV");
		_colorProgram.linkShaders();

		_circleColorProgram.compileShaders("Src/Shaders/circleColorShading.vert", "Src/Shaders/circleColorShading.frag");
		_circleColorProgram.addAttribute("vertexPosition");
		_circleColorProgram.addAttribute("vertexColor");
		_circleColorProgram.addAttribute("vertexUV");
		_circleColorProgram.linkShaders();

		_textureLightProgram.compileShaders("Src/Shaders/textureShading.vert", "Src/Shaders/textureShading.frag");
		_textureLightProgram.addAttribute("vertexPosition");
		_textureLightProgram.addAttribute("vertexColor");
		_textureLightProgram.addAttribute("vertexUV");
		_textureLightProgram.linkShaders();

		_textureSnowProgram.compileShaders("Src/Shaders/snowTexture.vert", "Src/Shaders/snowTexture.frag");
		_textureSnowProgram.addAttribute("vertexPosition");
		_textureSnowProgram.addAttribute("vertexColor");
		_textureSnowProgram.addAttribute("vertexUV");
		_textureSnowProgram.linkShaders();

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

		_fogProgram.compileShaders("Src/Shaders/fogEffect.vert", "Src/Shaders/fogEffect.frag");
		_fogProgram.addAttribute("vertexPosition");
		_fogProgram.addAttribute("vertexColor");
		_fogProgram.addAttribute("vertexUV");
		_fogProgram.linkShaders();

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

	main_camera2D->worldDimensions= map->GetLayerDimensions("assets/Maps/map_v3_Tile_Layer.csv");
	main_camera2D->worldDimensions.x = main_camera2D->worldDimensions.x - _window->getScreenWidth();

	manager.grid = std::make_unique<Grid>(main_camera2D->worldDimensions.x + _window->getScreenWidth(), main_camera2D->worldDimensions.y, CELL_SIZE);

	map->LoadMap("assets/Maps/background.csv","assets/Maps/background_v3.csv","assets/Maps/map_v3_Tile_Layer.csv", "assets/Maps/foreground_foreground.csv");

	assets->CreatePlayer(player1);
	manager.grid->addEntity(&player1);
	assets->CreateBackground();

	assets->CreateWeather(player1);

	//assets->CreateSkeleton(Vector2D(100, 300), Vector2D(-1, 0), 200, 2, "enemy");
	assets->CreateSkeleton(glm::ivec2(3744, 300), glm::ivec2(-1, 0), "skeleton", false);
	assets->CreateSkeleton(glm::ivec2(500, 300), glm::ivec2(1, 0), "skeleton", false);
	assets->CreateSkeleton(glm::ivec2(1000, 300), glm::ivec2(1, -1), "skeleton", false);
	assets->CreateSkeleton(glm::ivec2(1400, 288), glm::ivec2(-1, -1), "skeleton", false);
	assets->CreateSkeleton(glm::ivec2(1900, 300), glm::ivec2(-1, -1), "skeleton", false);
	assets->CreateSkeleton(glm::ivec2(2675, 300), glm::ivec2(-1, -1), "skeleton", false);

	assets->CreateGreenKoopaTroopa(glm::ivec2(200, 400), glm::ivec2(-1, 0), 2, "greenkoopatroopa");
	assets->CreateGreenKoopaTroopa(glm::ivec2(3644, 100), glm::ivec2(-1, -1), 2, "greenkoopatroopa");

	assets->CreateShop();
	assets->CreateInventory();

	stagelabel.addComponent<TransformComponent>(glm::vec2(32, 608), Manager::actionLayer, glm::ivec2(32, 32), 1);
	stagelabel.addComponent<UILabel>(&manager, "stage 0", "arial", true);
	stagelabel.addGroup(Manager::groupLabels);

	Music music = audioEngine.loadMusic("Sounds/JPEGSnow.ogg");
	music.play(-1);
}

void Game::onExit() {
	_debugRenderer.dispose();
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
auto& gemtiles(manager.getGroup(Manager::groupGems));
auto& winningtiles(manager.getGroup(Manager::groupWinningTiles));
auto& slices(manager.getGroup(Manager::groupSlices));
auto& enemyslices(manager.getGroup(Manager::groupEnemySlices));
auto& lights(manager.getGroup(Manager::groupLights));
auto& texturelights(manager.getGroup(Manager::groupTextureLights));
auto& raindrops(manager.getGroup(Manager::groupRainDrop));
auto& snowdrops(manager.getGroup(Manager::groupSnow));
auto& pipeforegroundsprites(manager.getGroup(Manager::groupPipeRingForeground));
auto& foregroundtiles(manager.getGroup(Manager::groupForegroundLayer));
auto& backgroundtiles(manager.getGroup(Manager::groupBackgroundLayer));
auto& sewerbackgroundtiles(manager.getGroup(Manager::groupSewerBackgroundLayer));
auto& markettiles(manager.getGroup(Manager::groupMarket));
auto& screenshapes(manager.getGroup(Manager::screenShapes));
auto& hpbars(manager.getGroup(Manager::groupHPBars));
auto& fog(manager.getGroup(Manager::groupFog));
auto& generators(manager.getGroup(Manager::groupEnvironmentGenerators));

void Game::update(float deltaTime) //game objects updating
{
	std::shared_ptr<PerspectiveCamera> main_camera2D = std::dynamic_pointer_cast<PerspectiveCamera>(CameraManager::getInstance().getCamera("main"));
	std::shared_ptr<OrthoCamera> hud_camera2D = std::dynamic_pointer_cast<OrthoCamera>(CameraManager::getInstance().getCamera("hud"));


	if (map->getMapCompleted()) {
		_firstLoop = true;
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
		for (auto& raindrop: raindrops)
		{
			raindrop->destroy();
		}
		raindrops.clear();
		for (auto& snowdrop : snowdrops)
		{
			snowdrop->destroy();
		}
		snowdrops.clear();
		for (auto& texturelight : texturelights)
		{
			texturelight->destroy();
		}
		texturelights.clear();
		for (auto& f : fog)
		{
			f->destroy();
		}
		fog.clear();
		for (auto& g : generators)
		{
			g->destroy();
		}
		generators.clear();
		projectiles.clear();
		map->resetMap();
		assets->CreateEnemies();
		assets->CreateStageUpButtons();
		assets->RefreshShop();
		stagelabel.GetComponent<UILabel>().setLetters("stage" + std::to_string(map->getStage()));
		assets->CreateWeather(player1);
		manager.refresh();
		manager.updateFully(deltaTime);

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
								enemy->GetComponent<TransformComponent>().setPosition_Y(enemy->GetComponent<TransformComponent>().getPosition().y - 32);
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

	main_camera2D->update();
	hud_camera2D->update();

	manager.refresh();
	if (_firstLoop) {
		manager.updateFully(deltaTime);
		_firstLoop = false;
	}
	else {
		manager.update(deltaTime);
	}

	
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
					if (c->hasComponent<MysteryBox_Script>())// hitted mystery box
					{ 
						auto& gem = assets->CreateGem(c->GetComponent<TransformComponent>().getPosition());

						gem.GetComponent<Gem_Script>().doCoinAnimation();
						// when collision done then destroy mysterybox and create gem in its place where its going to have the same position and then exec its animation
						p->GetComponent<ScoreComponent>().addToScore(100);
						c->destroy();
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

							if ((enemy->GetComponent<TransformComponent>().getVelocity().x < 0 && collision.movingRectColSide == Collision::ColSide::LEFT)
								|| (enemy->GetComponent<TransformComponent>().getVelocity().x > 0 && collision.movingRectColSide == Collision::ColSide::RIGHT)) {
								enemy->GetComponent<TransformComponent>().setVelocity_X(enemy->GetComponent<TransformComponent>().getVelocity().x * -1);
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
				enemy->GetComponent<TransformComponent>().setVelocity_X(enemy->GetComponent<TransformComponent>().getVelocity().x * -1);
			}
			if (enemy->GetComponent<TransformComponent>().getPosition().y > (main_camera2D->getPosition().y + main_camera2D->worldDimensions.y))
				enemy->destroy();
			if (enemy->GetComponent<LivingCharacter>().hp_bar->GetComponent<HPBar>()._healthPoints <= 0) {
				auto& gem = assets->CreateGem(enemy->GetComponent<TransformComponent>().getCenterTransform());

				gem.addGroup(Manager::groupGems);

				TransformComponent* gem_tr = &gem.GetComponent<TransformComponent>();
				gem.addComponent<ColliderComponent>("gem", gem_tr->getPosition().x, gem_tr->getPosition().y, gem_tr->width);

				enemy->destroy();
			}
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
					if (e->GetComponent<TransformComponent>().getPosition().x < player->GetComponent<TransformComponent>().getPosition().x + 300
						&& e->GetComponent<TransformComponent>().getPosition().x > player->GetComponent<TransformComponent>().getPosition().x) {
						e->GetComponent<TransformComponent>().setVelocity_X(-1) ;
					}
					else if (e->GetComponent<TransformComponent>().getPosition().x > player->GetComponent<TransformComponent>().getPosition().x - 300
						&& e->GetComponent<TransformComponent>().getPosition().x < player->GetComponent<TransformComponent>().getPosition().x) {
						e->GetComponent<TransformComponent>().setVelocity_X(1);
					}
				}
				if (enemy == skeletons && e->GetComponent<Skeleton_Script>().attackAnimation ||
					enemy == greenkoopatroopas && e->GetComponent<GreenKoopaTroopa_Script>().attackAnimation) {
					e->GetComponent<TransformComponent>().setVelocity_X (0);
				}
				

				if (hasCollision) {

					if (collision.movingRectColSide == Collision::ColSide::DOWN) {
						player->GetComponent<RigidBodyComponent>().onGround = true;
						player->GetComponent<TransformComponent>().setVelocity_Y(-20) ;
						if (enemy == greenkoopatroopas) //green koopa troopa case
						{
							e->GetComponent<TransformComponent>().setVelocity_X(0) ;
							e->GetComponent<AnimatorComponent>().Play("GreenShell");
							e->GetComponent<GreenKoopaTroopa_Script>().shelltoturtle = true;
						}
						else //skeleton case
						{
							e->GetComponent<LivingCharacter>().applyDamage(10);
								
							if (!player->GetComponent<Player_Script>().tookDamage) {
								player->GetComponent<LivingCharacter>().applyDamage(5);
							}
						}

					}
					else //player death
					{
						for (auto& pl : players)
						{
							if (!pl->GetComponent<Player_Script>().tookDamage) {
								pl->GetComponent<LivingCharacter>().applyDamage(5);
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

					if ((enemyTransform.getPosition().x < playerTransform.getPosition().x + 200 && enemyTransform.getPosition().x > playerTransform.getPosition().x) ||
						(enemyTransform.getPosition().x > playerTransform.getPosition().x - 200 && enemyTransform.getPosition().x < playerTransform.getPosition().x))
					{
						e->GetComponent<GreenKoopaTroopa_Script>().activateShoot();
					}
				}
			}
		}
	}

	for (auto& p : players) {
		for (auto& gem : gemtiles) {
			SDL_Rect gemCol = gem->GetComponent<ColliderComponent>().collider;
			SDL_Rect pCol = p->GetComponent<ColliderComponent>().collider;

			bool hasCollision = collision.checkCollision(pCol, gemCol);

			if (hasCollision)
			{
				p->GetComponent<ScoreComponent>().addToScore(100);
			}

			collision.isCollision = false;
			gem->destroy();
		}
	}


	for (auto& p : projectiles) //projectiles with players
	{
		for (auto& pl : players)
		{
			if (Collision::checkCollision(pl->GetComponent<ColliderComponent>().collider, p->GetComponent<ColliderComponent>().collider))
			{
				if (!pl->GetComponent<Player_Script>().tookDamage) {
					pl->GetComponent<LivingCharacter>().applyDamage(5);
				}
				p->destroy();
			}
			collision.isCollision = false;
		}
	}

	for (auto& sl : slices)
	{
		sl->destroy();
	}

	for (auto& esl : enemyslices)
	{
		for (auto& pl : players)
		{
			if (Collision::checkCollision(esl->GetComponent<ColliderComponent>().collider, pl->GetComponent<ColliderComponent>().collider))
			{
				if (!pl->GetComponent<Player_Script>().tookDamage) {
					pl->GetComponent<LivingCharacter>().applyDamage(esl->GetComponent<Slice>().sliceDamage);
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
		main_camera2D->setPosition_X(pl->GetComponent<TransformComponent>().getPosition().x - main_camera2D->getCameraDimensions().x / 2);

		if (pl->GetComponent<TransformComponent>().getPosition().y >(main_camera2D->getPosition().y + main_camera2D->worldDimensions.y)) //player death
		{
			for (auto& player : players)
			{
				player->destroy();
			}
		}
		if (pl->GetComponent<LivingCharacter>().hp_bar->GetComponent<HPBar>()._healthPoints <= 0) {
			pl->destroy();
		}
	}
	
	for (auto& p : players) //scene transition
	{
		if (p->GetComponent<Player_Script>().finishedHorAnimation)
		{
			for (auto& pl : players)
			{
				pl->GetComponent<TransformComponent>().setPosition_X(scenes->GetSceneStartupPosition(0).x);
				pl->GetComponent<TransformComponent>().setPosition_Y(scenes->GetSceneStartupPosition(0).y);
			}
			main_camera2D->worldDimensions = map->GetLayerDimensions("assets/Maps/RandomMap.csv");
			main_camera2D->worldDimensions.x = main_camera2D->worldDimensions.x - _window->getScreenWidth();
		}
		p->GetComponent<Player_Script>().finishedHorAnimation = false;
		p->GetComponent<Player_Script>().finishedVertAnimation = false;
	}

	for (auto& clouds : backgroundtiles) {
		if (clouds->GetComponent<TransformComponent>().getPosition().x + clouds->GetComponent<TransformComponent>().width < 0) {
			clouds->GetComponent<TransformComponent>().setPosition_X(main_camera2D->worldDimensions.x * 1.0f / 3.0f + _window->getScreenWidth())  ; //due to cloud z-index
		}
	}

	for (auto& sb : stageupbtns)
	{
		SpriteComponent entitySprite = sb->GetComponent<SpriteComponent>();

		if (_game->_inputManager.checkMouseCollision(entitySprite.destRect)) { //culling
			std::cout << "clicked button" << std::endl;
			sb->GetComponent<ButtonComponent>().setState(ButtonComponent::ButtonState::PRESSED);
			break;
		}
	}

	if (main_camera2D->getPosition().x < 0)
		main_camera2D->setPosition_X(0.0f);
	if (main_camera2D->getPosition().x > main_camera2D->worldDimensions.x)
		main_camera2D->setPosition_X(main_camera2D->worldDimensions.x);
}

glm::vec2 convertScreenToWorld(glm::vec2 screenCoords) {
	std::shared_ptr<PerspectiveCamera> main_camera2D = std::dynamic_pointer_cast<PerspectiveCamera>(CameraManager::getInstance().getCamera("main"));

	screenCoords = main_camera2D->convertScreenToWorld(screenCoords);  
	return screenCoords;
}

void Game::selectEntityAtPosition(glm::vec2 worldCoords) {
	std::shared_ptr<PerspectiveCamera> main_camera2D = std::dynamic_pointer_cast<PerspectiveCamera>(CameraManager::getInstance().getCamera("main"));
	for (auto& groups : { tiles,skeletons , greenkoopatroopas })
	{
		for (auto& entity : groups) {
			TransformComponent* tr = &entity->GetComponent<TransformComponent>();
			glm::vec2 pos = glm::vec2(tr->getPosition().x, tr->getPosition().y);
			// Check if the mouse click is within the entity's bounding box
			if (worldCoords.x > pos.x && worldCoords.x < pos.x + tr->width &&
				worldCoords.y > pos.y && worldCoords.y < pos.y + tr->height) {
				_selectedEntity = entity;  // Store a pointer or reference to the selected entity
				break;
			}
		}
	}
	
}

void Game::checkInput() {
	std::shared_ptr<PerspectiveCamera> main_camera2D = std::dynamic_pointer_cast<PerspectiveCamera>(CameraManager::getInstance().getCamera("main"));
	std::shared_ptr<OrthoCamera> hud_camera2D = std::dynamic_pointer_cast<OrthoCamera>(CameraManager::getInstance().getCamera("hud"));

	_game->_inputManager.update();

	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
		ImGui_ImplSDL2_ProcessEvent(&evnt);
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
		case SDL_MOUSEMOTION:
			glm::vec2 mouseCoordsVec = _game->_inputManager.getMouseCoords();
			_game->_inputManager.setMouseCoords(mouseCoordsVec.x * main_camera2D->getCameraDimensions().x / _window->getScreenWidth(), mouseCoordsVec.y * main_camera2D->getCameraDimensions().y / _window->getScreenHeight());
			mouseCoordsVec = _game->_inputManager.getMouseCoords();
			if (_selectedEntity) {
				_selectedEntity->GetComponent<TransformComponent>().setPosition_X(convertScreenToWorld(mouseCoordsVec ).x);
				_selectedEntity->GetComponent<TransformComponent>().setPosition_Y (convertScreenToWorld(mouseCoordsVec ).y);
				if(_selectedEntity->hasComponent<GridComponent>())
					_selectedEntity->GetComponent<GridComponent>().updateCollidersGrid();
			}
		}
		if (_game->_inputManager.isKeyPressed(SDLK_p)) {
			onPauseGame();
		}

		if (_game->_inputManager.isKeyPressed(SDLK_n)) {
			Game::map->setMapCompleted(true);
		}

		if (_game->_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
			glm::vec2 mouseCoordsVec = _game->_inputManager.getMouseCoords();
			if ( _selectedEntity == nullptr) {
				selectEntityAtPosition(convertScreenToWorld(mouseCoordsVec));
			}	
		}
		if (!_game->_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
			_selectedEntity = nullptr;
		}

	}
}

void Game::updateUI() {

	// Default ImGui window
	ImGui::Begin("Default UI");
	ImGui::Text("This is a permanent UI element.");
	ImGui::End();

	ImGui::Begin("Background UI");
	ImGui::Text("This is a Background UI element.");
	ImGui::ColorEdit4("Background Color", backgroundColor);
	// Change color based on the debug mode state
	if (_renderDebug) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));  // Green for ON
	}
	else {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));  // Red for OFF
	}

	// Button toggles the debug mode
	if (ImGui::Button("Enable Debug Mode")) {
		_renderDebug = !_renderDebug;  // Toggle the state
	}

	ImGui::PopStyleColor(1);
	ImGui::Text("Camera Position");

	std::shared_ptr<PerspectiveCamera> main_camera2D = std::dynamic_pointer_cast<PerspectiveCamera>(CameraManager::getInstance().getCamera("main"));

	ImGui::Text("Rect: {x: %d, y: %d, w: %d, h: %d}", main_camera2D->getCameraRect().x, main_camera2D->getCameraRect().y, main_camera2D->getCameraRect().w, main_camera2D->getCameraRect().h);
	
	if (ImGui::SliderFloat3("Eye Position", &main_camera2D->eyePos[0], -1000.0f, 1000.0f)) {
		main_camera2D->setCameraMatrix(glm::lookAt(main_camera2D->eyePos, main_camera2D->aimPos, main_camera2D->upDir));
	}
	if (ImGui::SliderFloat3("Aim Position", &main_camera2D->aimPos[0], -1000.0f, 1000.0f)) {
		main_camera2D->setCameraMatrix(glm::lookAt(main_camera2D->eyePos, main_camera2D->aimPos, main_camera2D->upDir));
	}
	if (ImGui::SliderFloat3("Up Direction", &main_camera2D->upDir[0], -1000.0f, 1000.0f)) {
		main_camera2D->setCameraMatrix(glm::lookAt(main_camera2D->eyePos, main_camera2D->aimPos, main_camera2D->upDir));
	}

	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[0][0]);
	//ImGui::SliderFloat("_orthoMatrix[0][0]", &main_camera2D->_orthoMatrix[0][0], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[0][1]);
	//ImGui::SliderFloat("_orthoMatrix[0][1]", &main_camera2D->_orthoMatrix[0][1], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[0][2]);
	//ImGui::SliderFloat("_orthoMatrix[0][2]", &main_camera2D->_orthoMatrix[0][2], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[0][3]);
	//ImGui::SliderFloat("_orthoMatrix[0][3]", &main_camera2D->_orthoMatrix[0][3], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[1][0]);
	//ImGui::SliderFloat("_orthoMatrix[1][0]", &main_camera2D->_orthoMatrix[1][0], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[1][1]);
	//ImGui::SliderFloat("_orthoMatrix[1][1]", &main_camera2D->_orthoMatrix[1][1], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[1][2]);
	//ImGui::SliderFloat("_orthoMatrix[1][2]", &main_camera2D->_orthoMatrix[1][2], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[1][3]);
	//ImGui::SliderFloat("_orthoMatrix[1][3]", &main_camera2D->_orthoMatrix[1][3], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[2][0]);
	//ImGui::SliderFloat("_orthoMatrix[2][0]", &main_camera2D->_orthoMatrix[2][0], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[2][1]);
	//ImGui::SliderFloat("_orthoMatrix[2][1]", &main_camera2D->_orthoMatrix[2][1], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[2][2]);
	//ImGui::SliderFloat("_orthoMatrix[2][2]", &main_camera2D->_orthoMatrix[2][2], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[2][3]);
	//ImGui::SliderFloat("_orthoMatrix[2][3]", &main_camera2D->_orthoMatrix[2][3], -1.0f, 1.0f);
	//ImGui::Text("Camera Value: %f}", main_camera2D->_orthoMatrix[3][3]);
	//ImGui::SliderFloat("_orthoMatrix[3][3]", &main_camera2D->_orthoMatrix[3][3], -1.0f, 1.0f);


	ImGui::Text("Mouse Coords: {x: %f, y: %f}", _game->_inputManager.getMouseCoords().x, _game->_inputManager.getMouseCoords().y);

	if (_selectedEntity) {
		ImGui::Text("Selected Entity Details");

		// Example: Display components of the selected entity
		if (_selectedEntity->hasComponent<TransformComponent>()) {
			TransformComponent* tr = &_selectedEntity->GetComponent<TransformComponent>();
			ImGui::Text("Position: (%d, %d)", tr->getPosition().x, tr->getPosition().y);
			ImGui::Text("Size: (%d, %d)", tr->width, tr->height);
		}
	}
	ImGui::End();

}

void Game::setupShaderAndLightTexture(const std::string& textureName, PerspectiveCamera& camera) { // todo add camera2D.worldLocation argument
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
		glm::vec3 position;
		float radius;
	};

	std::vector<Light> lightsPos;

	for (auto& light : lights) {
		TransformComponent* lightTransform = &light->GetComponent<TransformComponent>();

		if (lightTransform) {
			Light tempLight;
			tempLight.position = glm::vec3(lightTransform->getPosition().x, lightTransform->getPosition().y, lightTransform->getZIndex());
			tempLight.radius = 100.0f;

			lightsPos.push_back(tempLight);
			//Light tempLight2;
			//tempLight2.position = glm::vec2(lightTransform->position.x + 100.0f, lightTransform->position.y);
			//tempLight2.radius = 0.0f;

			//lightsPos.push_back(tempLight2);
		}
	}

	GLint lightPosLocation = _textureLightProgram.getUniformLocation("lightPos[0].position");
	glUniform3fv(lightPosLocation, 1, &(lightsPos[0].position[0]));  // Pass the address of the first element of lightPos
	GLint lightRadiusLocation = _textureLightProgram.getUniformLocation("lightPos[0].radius");
	glUniform1f(lightRadiusLocation, lightsPos[0].radius);

	//GLint lightPosLocation2 = _textureLightProgram.getUniformLocation("lightPos[1].position");
	//glUniform2fv(lightPosLocation2, 1, &(lightsPos[1].position[0]));  // Pass the address of the first element of lightPos
}

void Game::setupShaderAndTexture(GLSLProgram& shaderProgram, const std::string& textureName, ICamera& camera) { // todo add camera2D.worldLocation argument
	shaderProgram.use();
	glActiveTexture(GL_TEXTURE0);
	const GLTexture* texture = TextureManager::getInstance().Get_GLTexture(textureName);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	GLint textureLocation = shaderProgram.getUniformLocation("texture_sampler");
	glUniform1i(textureLocation, 0);
	GLint pLocation = shaderProgram.getUniformLocation("projection");
	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
}

void Game::setupShaderAndWaveTexture(const std::string& textureName, PerspectiveCamera& camera) { // todo add camera2D.worldLocation argument
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

void Game::setupShaderAndFogTexture(PerspectiveCamera& camera) { // todo add camera2D.worldLocation argument
	_fogProgram.use();
	float currentTime = SDL_GetTicks() / 1000.0f;
	float elapsedTime = currentTime - Game::startTime;
	GLuint timeLocation = _fogProgram.getUniformLocation("time");
	glUniform1f(timeLocation, elapsedTime);
	GLint pLocation = _fogProgram.getUniformLocation("projection");
	glm::mat4 cameraMatrix = camera.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
}

void Game::renderBatch(const std::vector<Entity*>& entities, SpriteBatch& batch) { // todo add batch argument
	std::shared_ptr<PerspectiveCamera> main_camera2D = std::dynamic_pointer_cast<PerspectiveCamera>(CameraManager::getInstance().getCamera("main"));

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
	std::shared_ptr<PerspectiveCamera> main_camera2D = std::dynamic_pointer_cast<PerspectiveCamera>(CameraManager::getInstance().getCamera("main"));
	std::shared_ptr<OrthoCamera> hud_camera2D = std::dynamic_pointer_cast<OrthoCamera>(CameraManager::getInstance().getCamera("hud"));

	////////////OPENGL USE
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);


	/////////////////////////////////////////////////////
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//based on weather change Shader of background
	if (assets->getWeather() == AssetManager::Weather::RAIN) {
		setupShaderAndWaveTexture("backgroundMountains", *main_camera2D);
		renderBatch(backgrounds, _spriteBatch);
		_waveProgram.unuse();
	}
	else {
		setupShaderAndTexture(_textureProgram, "backgroundMountains", *main_camera2D);
		renderBatch(backgrounds, _spriteBatch);
		_textureProgram.unuse();
	}
	if (assets->getWeather() == AssetManager::Weather::RAIN) {
		setupShaderAndLightTexture("terrain", *main_camera2D);
		renderBatch(backgroundtiles, _spriteBatch);
		renderBatch(sewerbackgroundtiles, _spriteBatch);
		renderBatch(tiles, _spriteBatch);
		//renderBatch(colliders);
		//based on weather change Shader of textures
		setupShaderAndLightTexture("projectile", *main_camera2D);
		renderBatch(projectiles, _spriteBatch);
		setupShaderAndLightTexture("warriorProjectile", *main_camera2D);
		renderBatch(warriorprojectiles, _spriteBatch);
		setupShaderAndLightTexture("skeleton", *main_camera2D);
		renderBatch(skeletons, _spriteBatch);
		renderBatch(greenkoopatroopas, _spriteBatch);

		setupShaderAndLightTexture("warrior", *main_camera2D);
		renderBatch(players, _spriteBatch);
		setupShaderAndLightTexture("terrain", *main_camera2D);
		renderBatch(winningtiles, _spriteBatch);
		renderBatch(foregroundtiles, _spriteBatch);
		_textureLightProgram.unuse();
	}
	else {
		setupShaderAndTexture(_textureProgram,"terrain", *main_camera2D);
		renderBatch(backgroundtiles, _spriteBatch);
		renderBatch(sewerbackgroundtiles, _spriteBatch);
		if (assets->getWeather() == AssetManager::Weather::SNOW) {
			setupShaderAndTexture(_textureSnowProgram, "terrain", *main_camera2D);
		}
		renderBatch(tiles, _spriteBatch);
		_textureSnowProgram.unuse();
		//renderBatch(colliders);
		//based on weather change Shader of textures
		setupShaderAndTexture(_textureProgram, "projectile", *main_camera2D);
		renderBatch(projectiles, _spriteBatch);
		setupShaderAndTexture(_textureProgram, "warriorProjectile", *main_camera2D);
		renderBatch(warriorprojectiles, _spriteBatch);
		setupShaderAndTexture(_textureProgram, "skeleton", *main_camera2D);
		renderBatch(skeletons, _spriteBatch);
		renderBatch(greenkoopatroopas, _spriteBatch);

		setupShaderAndTexture(_textureProgram, "warrior", *main_camera2D);
		renderBatch(players, _spriteBatch);
		setupShaderAndTexture(_textureProgram, "terrain", *main_camera2D);
		renderBatch(winningtiles, _spriteBatch);
		renderBatch(foregroundtiles, _spriteBatch);
		_textureLightProgram.unuse();
	}
	


	_lightProgram.use();
	GLint pLocation = _lightProgram.getUniformLocation("projection");
	glm::mat4 cameraMatrix = main_camera2D->getCameraMatrix();
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
	setupShaderAndFogTexture(*main_camera2D);

	renderBatch(fog, _spriteBatch);
	_fogProgram.unuse();
	///////////////////////////////////////////////////////
	_colorProgram.use();

	pLocation = _colorProgram.getUniformLocation("projection");
	cameraMatrix = main_camera2D->getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	//renderBatch(screenshapes);
	renderBatch(hpbars, _spriteBatch);
	renderBatch(raindrops, _spriteBatch);
	_colorProgram.unuse();
	_circleColorProgram.use();

	pLocation = _colorProgram.getUniformLocation("projection");
	cameraMatrix = main_camera2D->getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	renderBatch(snowdrops, _spriteBatch);

	_circleColorProgram.unuse();

	
	// Debug Rendering
	if (_renderDebug) {
		int cellIndex = 0;
		for (const auto& cell : manager.grid->getCells()) {
			// Calculate the position of the cell in world coordinates based on its index
			int x = (cellIndex % manager.grid->getNumXCells()) * manager.grid->getCellSize();
			int y = (cellIndex / manager.grid->getNumXCells()) * manager.grid->getCellSize();

			glm::vec4 destRect(x, y, manager.grid->getCellSize(), manager.grid->getCellSize());
			_debugRenderer.drawBox(destRect, Color(0, 255, 0, 25), 0.0f);  // Drawing each cell in red for visibility

			cellIndex++;
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
					glm::vec4 destRect(cCol.x, cCol.y, cCol.w, cCol.h);
					_debugRenderer.drawBox(destRect, Color(255, 0, 0, 255), 0.0f);
					_debugRenderer.end();
					_debugRenderer.render(cameraMatrix, 4.0f);
				}
			}
		}
		for (std::size_t group = Manager::groupBackgroundLayer; group != Manager::groupCircles; group++) {

			if (group == Manager::groupHPBars) {
				continue; 
			}

			std::vector<Entity*>& groupVec = manager.getGroup(group);
			for (auto& entity : groupVec) {
				
				if (entity->hasComponent<TransformComponent>())
				{
					TransformComponent* tr = &entity->GetComponent<TransformComponent>();

					glm::vec4 destRect;
					destRect.x = tr->getPosition().x;
					destRect.y = tr->getPosition().y;
					destRect.z = tr->width;
					destRect.w = tr->height;
					_debugRenderer.drawBox(destRect, Color(255, 255, 255, 255), 0.0f); //todo add angle for drawbox
					//_debugRenderer.drawCircle(glm::vec2(tr->position.x, tr->position.y), Color(255, 255, 255, 255), tr->getCenterTransform().x);
					//break;
				}
				
			}
		}
		if (_selectedEntity) {
			TransformComponent* tr = &_selectedEntity->GetComponent<TransformComponent>();

			glm::vec4 destRect;
			destRect.x = tr->getPosition().x;
			destRect.y = tr->getPosition().y;
			destRect.z = tr->width;
			destRect.w = tr->height;
			_debugRenderer.drawBox(destRect, Color(255, 255, 0, 255), 0.0f); //todo add angle for drawbox
		}
		_debugRenderer.end();
		_debugRenderer.render(cameraMatrix, 2.0f);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void Game::drawHUD(const std::vector<Entity*>& entities, const std::string& textureName) {
	std::shared_ptr<OrthoCamera> hud_camera2D = std::dynamic_pointer_cast<OrthoCamera>(CameraManager::getInstance().getCamera("hud"));

	setupShaderAndTexture(_textureProgram, textureName, *hud_camera2D);
	renderBatch(entities, _hudSpriteBatch);
}

bool Game::onPauseGame() {
	_prevScreenIndex = SCREEN_INDEX_MAIN_MENU;
	_currentState = ScreenState::CHANGE_PREVIOUS;
	return true;
}