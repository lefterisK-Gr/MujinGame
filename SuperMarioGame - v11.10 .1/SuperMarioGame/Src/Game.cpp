
#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include "SceneManager.h"
#include <sstream>


#undef main

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Manager manager;
Collision collision;

SDL_Rect Game::camera = { 0,0,2240,0 }; // camera.w shows how far right camera can go
Camera2D Game::camera2D;
SpriteBatch Game::_spriteBatch;

InputManager Game::_inputManager;

AssetManager* Game::assets = new AssetManager(&manager);

SceneManager* Game::scenes = new SceneManager();

bool Game::isRunning = false;
bool Game::isPaused = false;

uint32_t Game::pauseTime = 0;
bool Game::justResumed = false;

auto& player1(manager.addEntity());
auto& sun(manager.addEntity());
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

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, float _maxFPS)
{
	int flags = 0;

	Game::camera2D.init(width, height); // Assuming a screen resolution of 800x600
	Game::camera2D.setPosition(camera2D.getPosition() /*+ glm::vec2(
		width / 2.0f,
		height / 2.0f
	)*/);;
	Game::camera2D.setScale(1.0f);
	// create renderer
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised..." << std::endl;

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		window = SDL_CreateWindow(title, xpos, ypos, width, height, SDL_WINDOW_OPENGL);
		if (window == NULL) {
			printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
			// Handle the error
		}
		if (window)
		{
			std::cout << "Window created!" << std::endl;
		}

		gOpenGLContext = SDL_GL_CreateContext(window);
		if (gOpenGLContext)
		{
			std::cout << "OpenGl context created!" << std::endl;
		}
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			std::cout << "GLEW Failed to initialize!" << std::endl;
		}

		//check opengl version
		std::printf("*** OpenGL Version %s ***\n", glGetString(GL_VERSION));

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		SDL_GL_SetSwapInterval(0);

		//enable alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//InitShaders function from Bengine
		_colorProgram.compileShaders("Src/Shaders/colorShading.vert", "Src/Shaders/colorShading.frag");
		_colorProgram.addAttribute("vertexPosition");
		_colorProgram.addAttribute("vertexColor");
		_colorProgram.addAttribute("vertexUV");
		_colorProgram.linkShaders();

		Game::_spriteBatch.init();
		_fpsLimiter.init(_maxFPS);

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
	assets->AddTexture("warrior", "assets/samurai.png");
	assets->AddTexture("projectile", "assets/my_projectile.png");
	assets->AddTexture("skeleton", "assets/skeleton.png"); // same path since the same png has all entities
	assets->AddTexture("greenkoopatroopa", "assets/mushroom.png");

	assets->Add_GLTexture("terrain", "assets/village_tileset.png");
	assets->Add_GLTexture("warrior", "assets/samurai.png");
	assets->Add_GLTexture("projectile", "assets/my_projectile.png");
	assets->Add_GLTexture("skeleton", "assets/skeleton.png"); // same path since the same png has all entities
	assets->Add_GLTexture("greenkoopatroopa", "assets/mushroom.png");

	assets->AddFont("arial", "assets/arial.ttf", 20);
	assets->AddFont("arialBig", "assets/arial.ttf", 100);

	map = new Map("terrain", 1, 32);

	map->LoadMap("assets/background_v3.csv", "assets/background.csv","assets/map_v3_Tile_Layer.csv", "assets/foreground_foreground.csv");

	assets->CreatePlayer(player1);

	assets->CreateSunShape(sun);
	/*label.addComponent<UILabel>(10, 42, "University of Crete", "arial", black);
	label.addComponent<UILabel>(10, 58, "Department of Computer Science", "arial", black);
	label.addComponent<UILabel>(10, 10, "Lefteris Kotsonas", "arial", black);
	label.addComponent<UILabel>(10, 26, "CS-454, Development of Intelligent Interfaces and Games. Fall Semester 2021-22", "arial", black);*/

	winningLabel1.addComponent<UILabel>(100, 300, "", "arialBig", assets->red);
	winningLabel2.addComponent<UILabel>(100, 300, "", "arialBig", assets->green);

	pauseLabel.addComponent<UILabel>(50, 300, "", "arialBig", assets->white);

	//assets->CreateSkeleton(Vector2D(100, 300), Vector2D(-1, 0), 200, 2, "enemy");
	assets->CreateSkeleton(Vector2D(3744, 300), Vector2D(-1, 0), 200, 2, "skeleton");
	assets->CreateSkeleton(Vector2D(500, 300), Vector2D(1, 0), 200, 2, "skeleton");
	assets->CreateSkeleton(Vector2D(1000, 300), Vector2D(1, -1), 200, 2, "skeleton");
	assets->CreateSkeleton(Vector2D(1400, 288), Vector2D(-1, -1), 200, 2, "skeleton");
	assets->CreateSkeleton(Vector2D(1900, 300), Vector2D(-1, -1), 200, 2, "skeleton");
	assets->CreateSkeleton(Vector2D(2675, 300), Vector2D(-1, -1), 200, 2, "skeleton");

	assets->CreateGreenKoopaTroopa(Vector2D(200, 400), Vector2D(-1, 0), 200, 2, "greenkoopatroopa");
	assets->CreateGreenKoopaTroopa(Vector2D(3644, 100), Vector2D(-1, -1), 200, 2, "greenkoopatroopa");

}

auto& tiles(manager.getGroup(Game::groupActionLayer));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& skeletons(manager.getGroup(Game::groupSkeletons));
auto& greenkoopatroopas(manager.getGroup(Game::groupGreenKoopaTroopas));
auto& mysteryboxtiles(manager.getGroup(Game::groupMysteryBoxes));
auto& winningtiles(manager.getGroup(Game::groupWinningTiles));
auto& pipeforegroundsprites(manager.getGroup(Game::groupPipeRingForeground));
auto& foregroundtiles(manager.getGroup(Game::groupForegroundLayer));
auto& backgroundtiles(manager.getGroup(Game::groupBackgroundLayer));
auto& sewerbackgroundtiles(manager.getGroup(Game::groupSewerBackgroundLayer));
auto& screenshapes(manager.getGroup(Game::screenShapes));

void Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type)
	{
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			Game::_inputManager.pressKey(event.key.keysym.sym);
			break;
		case SDL_KEYUP:
			Game::_inputManager.releaseKey(event.key.keysym.sym);
		case SDL_MOUSEMOTION:
			//std::cout << event.motion.x << " " << event.motion.y << std::endl;
			_inputManager.setMouseCoords(event.motion.x, event.motion.y);
			break;
		case SDL_MOUSEWHEEL:
			if (Game::event.wheel.y > 0)
			{
				// Scrolling up
				Game::camera2D.setScale(Game::camera2D.getScale() + SCALE_SPEED);
			}
			else if (Game::event.wheel.y < 0)
			{
				// Scrolling down
				Game::camera2D.setScale(Game::camera2D.getScale() - SCALE_SPEED);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			Game::_inputManager.pressKey(event.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			Game::_inputManager.releaseKey(event.button.button);
			break;
		default:
			break;
	}

	if (Game::_inputManager.isKeyPressed(SDLK_ESCAPE)) {
		Game::isRunning = false;
	}

	if (Game::_inputManager.isKeyPressed(SDLK_p)) {
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
	}

	if (Game::_inputManager.isKeyPressed(SDL_BUTTON_LEFT)) {
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = Game::camera2D.convertScreenToWorld(mouseCoords);
		std::cout << mouseCoords.x << " " << mouseCoords.y << std::endl;
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

	Game::camera2D.update();

	_time += 0.01f;
		
	auto& slices(manager.getGroup(Game::groupSlices));
	
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
							ss1 << player->getComponent<ScoreComponent>().getScore();
							player->getComponent<UILabel>().SetLabelText(ss1.str(), "arial");
							e->destroy();
						}

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
	if (camera.x > (scenes->GetSceneCamera(scenes->sceneSelected).x + camera.w))
		camera.x = (scenes->GetSceneCamera(scenes->sceneSelected).x + camera.w);
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
		entity->draw();
	}
	_spriteBatch.end();
	_spriteBatch.renderBatch();
}

void Game::render()
{
	////////////OPENGL USE
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	_colorProgram.unuse();

	SDL_GL_SwapWindow(window);

	////////////SDL USE

	label.draw();
	winningLabel1.draw();
	winningLabel2.draw();
	pauseLabel.draw();
	scoreboard1.draw();
	//scoreboard2.draw();
	////add stuff to render
	//SDL_RenderPresent(renderer);
	
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned" << std::endl;
}