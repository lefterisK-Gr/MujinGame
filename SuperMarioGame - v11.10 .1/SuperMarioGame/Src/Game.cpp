
#include "Game.h"
#include "TextureManager.h"
#include "Map/Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision/Collision.h"
#include "AssetManager/AssetManager.h"
#include "SceneManager.h"
#include <sstream>
#include "GameScreen/IMainGame.h"


#undef main

Map* map;

SDL_Event Game::event;
Manager manager;
Collision collision;

SDL_Rect Game::camera = { 0,0,2240,0 }; // camera.w shows how far right camera can go
Camera2D Game::camera2D;
Camera2D Game::hudCamera2D;
SpriteBatch Game::_spriteBatch;
SpriteBatch Game::_hudSpriteBatch;

AudioEngine Game::audioEngine;

AssetManager* Game::assets = nullptr;
SceneManager* Game::scenes = new SceneManager();

auto& player1(manager.addEntity());
auto& sun(manager.addEntity());
auto& label(manager.addEntity());
auto& pauseLabel(manager.addEntity());
auto& scoreboard1(manager.addEntity());
auto& scoreboard2(manager.addEntity());

Game::Game(MujinEngine::Window* window)
	: _window(window)
{
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
	assets = new AssetManager(&manager, _game->_inputManager);

	Game::camera2D.init(_window->getScreenWidth(), _window->getScreenHeight()); // Assuming a screen resolution of 800x600
	Game::camera2D.setPosition(camera2D.getPosition() /*+ glm::vec2(
		width / 2.0f,
		height / 2.0f
	)*/);;
	Game::camera2D.setScale(1.0f);

	Game::hudCamera2D.init(_window->getScreenWidth(), _window->getScreenHeight());

	audioEngine.init();
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised..." << std::endl;

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		//InitShaders function from Bengine
		_colorProgram.compileShaders("Src/Shaders/colorShading.vert", "Src/Shaders/colorShading.frag");
		_colorProgram.addAttribute("vertexPosition");
		_colorProgram.addAttribute("vertexColor");
		_colorProgram.addAttribute("vertexUV");
		_colorProgram.linkShaders();

		_lightProgram.compileShaders("Src/Shaders/lightShading.vert", "Src/Shaders/lightShading.frag");
		_lightProgram.addAttribute("vertexPosition");
		_lightProgram.addAttribute("vertexColor");
		_lightProgram.addAttribute("vertexUV");
		_lightProgram.linkShaders();

		Game::_spriteBatch.init();
		Game::_hudSpriteBatch.init();

		_spriteFont = new SpriteFont("assets/arial.ttf", 32);

	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}

	//add the textures to our texture library
	assets->Add_GLTexture("terrain", "assets/village_tileset.png");
	assets->Add_GLTexture("warrior", "assets/samurai.png");
	assets->Add_GLTexture("projectile", "assets/my_projectile.png");
	assets->Add_GLTexture("skeleton", "assets/skeleton.png"); // same path since the same png has all entities
	assets->Add_GLTexture("greenkoopatroopa", "assets/mushroom.png");

	map = new Map("terrain", 1, 32);

	map->LoadMap("assets/background_v3.csv", "assets/background.csv","assets/map_v3_Tile_Layer.csv", "assets/foreground_foreground.csv");

	assets->CreatePlayer(player1);

	assets->CreateSunShape(sun);

	//assets->CreateSkeleton(Vector2D(100, 300), Vector2D(-1, 0), 200, 2, "enemy");
	assets->CreateSkeleton(Vector2D(3744, 300), Vector2D(-1, 0), 200, 2, "skeleton");
	assets->CreateSkeleton(Vector2D(500, 300), Vector2D(1, 0), 200, 2, "skeleton");
	assets->CreateSkeleton(Vector2D(1000, 300), Vector2D(1, -1), 200, 2, "skeleton");
	assets->CreateSkeleton(Vector2D(1400, 288), Vector2D(-1, -1), 200, 2, "skeleton");
	assets->CreateSkeleton(Vector2D(1900, 300), Vector2D(-1, -1), 200, 2, "skeleton");
	assets->CreateSkeleton(Vector2D(2675, 300), Vector2D(-1, -1), 200, 2, "skeleton");

	assets->CreateGreenKoopaTroopa(Vector2D(200, 400), Vector2D(-1, 0), 200, 2, "greenkoopatroopa");
	assets->CreateGreenKoopaTroopa(Vector2D(3644, 100), Vector2D(-1, -1), 200, 2, "greenkoopatroopa");

	Music music = audioEngine.loadMusic("Sounds/JPEGSnow.ogg");
	//music.play(-1);
}

void Game::onExit() {

}

auto& tiles(manager.getGroup(Game::groupActionLayer));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& skeletons(manager.getGroup(Game::groupSkeletons));
auto& greenkoopatroopas(manager.getGroup(Game::groupGreenKoopaTroopas));
auto& mysteryboxtiles(manager.getGroup(Game::groupMysteryBoxes));
auto& winningtiles(manager.getGroup(Game::groupWinningTiles));
auto& slices(manager.getGroup(Game::groupSlices));
auto& lights(manager.getGroup(Game::groupLights));
auto& pipeforegroundsprites(manager.getGroup(Game::groupPipeRingForeground));
auto& foregroundtiles(manager.getGroup(Game::groupForegroundLayer));
auto& backgroundtiles(manager.getGroup(Game::groupBackgroundLayer));
auto& sewerbackgroundtiles(manager.getGroup(Game::groupSewerBackgroundLayer));
auto& screenshapes(manager.getGroup(Game::screenShapes));

void Game::update(float deltaTime) //game objects updating
{
	checkInput(); //handleEvents
	Vector2D plMaxDistance;

	manager.refresh(); 
	manager.update(deltaTime);

	Game::camera2D.update();
	Game::hudCamera2D.update();

	
	for (auto& p : players)
	{
		p->getComponent<RigidBodyComponent>().onGround = false;
	}

	for (auto& enemy : skeletons)
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


	for (auto& enemyGroup : { skeletons, greenkoopatroopas }) // enemies with colliders
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
		for (auto& enemy : { skeletons , greenkoopatroopas })
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
						else //skeleton case
						{
							player->getComponent<ScoreComponent>().addToScore(100);
							e->destroy();
						}

					}
					else //player death
					{
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

	for (auto& sl : slices)
	{
		for (auto& enemy : { skeletons , greenkoopatroopas })
		{
			for (auto& e : enemy)
			{
				if (Collision::checkCollision(sl->getComponent<ColliderComponent>().collider, e->getComponent<ColliderComponent>().collider))
				{
					std::cout << "sword touched enemy!" << std::endl;
					if (e->getComponent<LivingCharacter>().applyDamage(sl->getComponent<Slice>().sliceDamage)) {
						e->destroy();
					}
				}
			}
		}
		sl->destroy();

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
	if (camera.x > (scenes->GetSceneCamera(scenes->sceneSelected).x + camera.w))
		camera.x = (scenes->GetSceneCamera(scenes->sceneSelected).x + camera.w);
}

void Game::checkInput() {
	SDL_Event evnt;
	while (SDL_PollEvent(&evnt)) {
	
		_game->onSDLEvent(evnt);

		switch (evnt.type)
		{
		case SDL_MOUSEWHEEL:
			if (evnt.wheel.y > 0)
			{
				// Scrolling up
				camera2D.setScale(camera2D.getScale() + SCALE_SPEED);
			}
			else if (evnt.wheel.y < 0)
			{
				// Scrolling down
				camera2D.setScale(camera2D.getScale() - SCALE_SPEED);
			}
			break;
		}
		if (_game->_inputManager.isKeyPressed(SDLK_p)) {
			onPauseGame();
		}

		if (_game->_inputManager.isKeyDown(SDL_BUTTON_LEFT)) {
			glm::vec2 mouseCoords = _game->_inputManager.getMouseCoords();
			mouseCoords = camera2D.convertScreenToWorld(mouseCoords);
			std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;
		}

		_game->_inputManager.update();
	}
}


void Game::setupShaderAndTexture(const std::string& textureName) {
	_colorProgram.use();
	glActiveTexture(GL_TEXTURE0);
	const GLTexture* texture = assets->Get_GLTexture(textureName);
	glBindTexture(GL_TEXTURE_2D, texture->id);
	GLint textureLocation = _colorProgram.getUniformLocation("texture_sampler");
	glUniform1i(textureLocation, 0);
	GLint pLocation = _colorProgram.getUniformLocation("projection");
	glm::mat4 cameraMatrix = camera2D.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));
}

void Game::renderBatch(const std::vector<Entity*>& entities) {
	_spriteBatch.begin();
	for (const auto& entity : entities) {
		SpriteComponent entitySprite = entity->getComponent<SpriteComponent>();
		if (collision.checkCollision(entitySprite.destRect, camera2D.getCameraRect())) { //culling
			entity->draw();
		}
	}
	_spriteBatch.end();
	_spriteBatch.renderBatch();
}

void Game::draw()
{
	////////////OPENGL USE
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);


	/////////////////////////////////////////////////////
	setupShaderAndTexture("terrain");
	//renderBatch(screenshapes);
	renderBatch(backgroundtiles);
	renderBatch(sewerbackgroundtiles);
	renderBatch(tiles);
	//renderBatch(colliders);
	setupShaderAndTexture("warrior");
	renderBatch(players);
	setupShaderAndTexture("projectile");
	renderBatch(projectiles);
	setupShaderAndTexture("skeleton");
	renderBatch(skeletons);
	renderBatch(greenkoopatroopas);
	setupShaderAndTexture("terrain");
	renderBatch(winningtiles);
	renderBatch(foregroundtiles);

	
	glBindTexture(GL_TEXTURE_2D, 0);
	//drawHUD();
	_colorProgram.unuse();

	_lightProgram.use();

	GLint pLocation = _colorProgram.getUniformLocation("projection");
	glm::mat4 cameraMatrix = camera2D.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	//renderBatch(lights);
	_spriteBatch.begin();

	for(const auto& l : lights)
	{
		l->draw();
	}

	_spriteBatch.end();
	_spriteBatch.renderBatch();

	_lightProgram.unuse();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	////////////SDL USE

	label.draw();
	pauseLabel.draw();
	scoreboard1.draw();
	////add stuff to render
	
}


// TODO: DOESNT WORK, ONLY WORKS FOR SOME CHARACTERS
void Game::drawHUD() { 
	char buffer[256];

	GLint pLocation = _colorProgram.getUniformLocation("projection");
	glm::mat4 cameraMatrix = hudCamera2D.getCameraMatrix();
	glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

	_hudSpriteBatch.begin();

	snprintf(buffer, sizeof(buffer), "start game");

	_spriteFont->draw(_hudSpriteBatch, buffer, glm::vec2(32, -96),
		glm::vec2(1), 0.0f, Color(255,255,255,255),
		Justification::LEFT);
		
	_hudSpriteBatch.end();
	_hudSpriteBatch.renderBatch();
}

bool Game::onPauseGame() {
	_prevScreenIndex = SCREEN_INDEX_MAIN_MENU;
	_currentState = ScreenState::CHANGE_PREVIOUS;
	return true;
}