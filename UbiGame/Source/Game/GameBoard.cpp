#include "GameBoard.h"
#include <cmath>
#include "GameEngine\GameEngineMain.h"
#include "GameEngine\EntitySystem\Components\CollidableComponent.h"
#include "GameEngine\EntitySystem\Components\SpriteRenderComponent.h"
#include "GameEngine\Util\CameraManager.h"
#include "Game\GameEntities\PlayerEntity.h"
#include "Game\GameEntities\ObstacleEntity.h"
#include <iostream>
#include "Game\GameEntities\GhostEntity.h"
#include "Game\Components\GhostMovementComponent.h"

using namespace Game;

GameBoard::GameBoard()
	: m_lastObstacleSpawnTimer(0.f)
	, m_isGameOver(false)
	, m_player(nullptr)
	, m_backGround(nullptr)
	, m_lightCircle(nullptr)
	, m_torch(nullptr)
	, m_starHold(false)
	, numLamps(10)
	, m_won(false)
	, msg_dt(GameEngine::GameEngineMain::GetTimeDelta())
{
	m_player = new PlayerEntity();

	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_player);
	m_player->SetPos(sf::Vector2f(RandomFloatRange(0, 1920), RandomFloatRange(0, 1080)));
	m_player->SetSize(sf::Vector2f(40.f, 40.f));
	m_player->GetComponent<GameEngine::SpriteRenderComponent>()->SetZLevel(8);

	DisplayMsg(1);

	//CreateBackGround();
	CreateBounds();
	CreateLightCircle();
	//CreateTorch();
	CreateStars(numLamps);
	CreateLamps(numLamps);

	//Debug
	for (int a = 0; a < 3; ++a)
	{
		// TODO: RANDOMIZE THE POSITION WITH RAND

		SpawnNewGhost(sf::Vector2f(RandomFloatRange(0, 1920), RandomFloatRange(0, 1080)), sf::Vector2f(50.f, 50.f));
	}
}


GameBoard::~GameBoard()
{

}


void GameBoard::Update()
{
	float dt = GameEngine::GameEngineMain::GetInstance()->GetTimeDelta();

	if (msg_dt - dt  > 5)
	{
		DisplayMsg(0);
	}

	for (std::vector<GameEngine::Entity*>::iterator it = m_ghosts.begin(); it != m_ghosts.end();) {

		GameEngine::Entity* currGhost = (*it);

		int ghostCentX = currGhost->GetPos().x;// +m_torch->GetSize().x / 2;
		int ghostCentY = currGhost->GetPos().y;// +m_torch->GetSize().y / 2;

		int playerCentX = m_player->GetPos().x;// +m_player->GetSize().x / 2;
		int playerCentY = m_player->GetPos().y;// +m_player->GetSize().y / 2;

		if (sqrt(pow(ghostCentX - playerCentX, 2) + pow(ghostCentY - playerCentY, 2)) < 50) {
			m_isGameOver = true;
			break;
		}
		it++;
	}

	if (!m_isGameOver)
	{

		//UpdateObstacles(dt);
		UpdateBackGround();
		UpdateLightCircle();
		//UpdatePlayerDying();
		UpdateTorch();
		UpdateLamps();
	}
	else {
		if (m_won == true) {
			SpawnNewGhost(sf::Vector2f(RandomFloatRange(0, 1920), RandomFloatRange(0, 1080)), sf::Vector2f(50.f, 50.f));
			m_won = false;
		}
		else {
			/*for (std::vector<GameEngine::Entity*>::iterator it = m_ghosts.begin(); it != m_ghosts.end();) {

				GameEngine::Entity* ghost = (*it);
				GameEngine::GameEngineMain::GetInstance()->RemoveEntity(ghost);
				it = m_ghosts.erase(it);*/
		}
		/*SpawnNewGhost(sf::Vector2f(RandomFloatRange(0, 1920), RandomFloatRange(0, 1080)), sf::Vector2f(50.f, 50.f));
		SpawnNewGhost(sf::Vector2f(RandomFloatRange(0, 1920), RandomFloatRange(0, 1080)), sf::Vector2f(50.f, 50.f));
		SpawnNewGhost(sf::Vector2f(RandomFloatRange(0, 1920), RandomFloatRange(0, 1080)), sf::Vector2f(50.f, 50.f));*/
		for (std::vector<GameEngine::Entity*>::iterator it = m_lampList.begin(); it != m_lampList.end();) {

			GameEngine::Entity* currLamp = (*it);
			GameEngine::GameEngineMain::GetInstance()->RemoveEntity(currLamp);
			it = m_lampList.erase(it);
		}

		for (std::vector<GameEngine::Entity*>::iterator it = m_luminateList.begin(); it != m_luminateList.end();) {

			GameEngine::Entity* currLuminate = (*it);
			GameEngine::GameEngineMain::GetInstance()->RemoveEntity(currLuminate);
			it = m_luminateList.erase(it);
		}

		for (std::vector<GameEngine::Entity*>::iterator it = m_StarList.begin(); it != m_StarList.end();) {

			GameEngine::Entity* currStar = (*it);
			GameEngine::GameEngineMain::GetInstance()->RemoveEntity(currStar);
			it = m_StarList.erase(it);
		}

		m_isGameOver = false;

		m_player->SetPos(sf::Vector2f(RandomFloatRange(0, 1920), RandomFloatRange(0, 1080)));

		CreateStars(numLamps);
		CreateLamps(numLamps);

	}
}

void GameBoard::DisplayMsg(int type)//	GameEngine::Entity* left_b = new GameEngine::Entity();

{
	msg_dt = GameEngine::GameEngineMain::GetInstance()->GetTimeDelta();
	GameEngine::Entity* msg = new GameEngine::Entity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(msg);
	msg->SetPos(sf::Vector2f(
		GameEngine::GameEngineMain::getWindowWidth() / 2, GameEngine::GameEngineMain::getWindowHeight() / 2));
	msg->SetSize(sf::Vector2f(680.f, 120.f));

	GameEngine::SpriteRenderComponent* render
		= static_cast<GameEngine::SpriteRenderComponent*>(msg->AddComponent<GameEngine::SpriteRenderComponent>());
	//render->SetTexture(GameEngine::eTexture::intro);

	switch (type)
	{
	case 0:
		render->SetTexture(GameEngine::eTexture::BT);
		break;
	case 1:
		render->SetTexture(GameEngine::eTexture::intro);
		break;
	case 2: render->SetTexture(GameEngine::eTexture::cont);
		break;
	case 3: render->SetTexture(GameEngine::eTexture::restart);
		break;
	}

	render->SetZLevel(5);
}
/*
void GameBoard::UpdateObstacles(float dt)
{
	static float obstacleSpeed = 100.f;

	for (std::vector<GameEngine::Entity*>::iterator it = m_obstacles.begin(); it != m_obstacles.end();)
	{
		GameEngine::Entity* obstacle = (*it);
		sf::Vector2f currPos = obstacle->GetPos();
		currPos.x -= obstacleSpeed * dt;
		obstacle->SetPos(currPos);
		//If we are to remove ourselves
		if (currPos.x < -50.f)
		{
			GameEngine::GameEngineMain::GetInstance()->RemoveEntity(obstacle);
			it = m_obstacles.erase(it);
		}
		else
		{
			it++;
		}
	}
}


void GameBoard::UpdatePlayerDying()
{
	bool noGameOver = GameEngine::CameraManager::IsFollowCameraEnabled();

	if (noGameOver)
		return;

	static float xToPlayerDie = -1000.f;
	if (m_player->GetPos().x < xToPlayerDie)
	{
		m_isGameOver = true;
	}
}


void GameBoard::SpawnNewRandomTiledObstacles()
{
	static int minObstacleCount = 2;
	static int maxObstacleCount = 7;

	static float minNextSpawnTime = 0.3f;
	static float maxNextSpawnTime = 0.7f;

	static float minObstacleXPos = 350.f;
	static float maxObstacleXPos = 450.f;
	static float minObstacleYPos = 20.f;
	static float maxObstacleYPos = 450.f;

	sf::Vector2f pos = sf::Vector2f(RandomFloatRange(minObstacleXPos, maxObstacleXPos), RandomFloatRange(minObstacleYPos, maxObstacleYPos));
	sf::Vector2f size = sf::Vector2f(32.f, 32.f);

	int obstacleCount = (int)RandomFloatRange((float)minObstacleCount, (float)maxObstacleCount);
	for (int a = 0; a < obstacleCount; ++a)
	{
		SpawnNewObstacle(pos, size);
		pos.y += size.y;
	}

	m_lastObstacleSpawnTimer = RandomFloatRange(minNextSpawnTime, maxNextSpawnTime);
}


void GameBoard::SpawnNewObstacle(const sf::Vector2f& pos, const sf::Vector2f& size)
{
	ObstacleEntity* obstacle = new ObstacleEntity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(obstacle);
	obstacle->SetPos(pos);
	obstacle->SetSize(sf::Vector2f(size.x, size.y));

	m_obstacles.push_back(obstacle);
}
*/

void GameBoard::CreateBounds()
{
	GameEngine::Entity* left_b = new GameEngine::Entity();
	GameEngine::Entity* right_b = new GameEngine::Entity();
	GameEngine::Entity* top_b = new GameEngine::Entity();
	GameEngine::Entity* bottom_b = new GameEngine::Entity();

	GameEngine::GameEngineMain::GetInstance()->AddEntity(left_b);
	GameEngine::GameEngineMain::GetInstance()->AddEntity(right_b);
	GameEngine::GameEngineMain::GetInstance()->AddEntity(top_b);
	GameEngine::GameEngineMain::GetInstance()->AddEntity(bottom_b);

	left_b->SetPos(sf::Vector2f(-10.f, 0.f));
	left_b->SetSize(sf::Vector2f(10.f, GameEngine::GameEngineMain::getWindowHeight()));

	right_b->SetPos(sf::Vector2f(GameEngine::GameEngineMain::getWindowWidth(), 0.f));
	right_b->SetSize(sf::Vector2f(10.f, GameEngine::GameEngineMain::getWindowHeight()));

	top_b->SetPos(sf::Vector2f(0.f, -10.f));
	top_b->SetSize(sf::Vector2f(GameEngine::GameEngineMain::getWindowWidth(), 10.f));

	bottom_b->SetPos(sf::Vector2f(0.f, GameEngine::GameEngineMain::getWindowHeight()));
	bottom_b->SetSize(sf::Vector2f(GameEngine::GameEngineMain::getWindowWidth(), 10.f));

	left_b->AddComponent<GameEngine::CollidableComponent>();
	right_b->AddComponent<GameEngine::CollidableComponent>();
	top_b->AddComponent<GameEngine::CollidableComponent>();
	bottom_b->AddComponent<GameEngine::CollidableComponent>();

}

void GameBoard::CreateBackGround()
{
	GameEngine::Entity* bgEntity = new GameEngine::Entity();
	GameEngine::SpriteRenderComponent* render = static_cast<GameEngine::SpriteRenderComponent*>(bgEntity->AddComponent<GameEngine::SpriteRenderComponent>());
	render->SetTexture(GameEngine::eTexture::BG);
	render->SetZLevel(0);
	bgEntity->SetPos(sf::Vector2f(250.f, 250.f));
	bgEntity->SetSize(sf::Vector2f(500.f, 500.f));
	GameEngine::GameEngineMain::GetInstance()->AddEntity(bgEntity);


	m_backGround = bgEntity;
}

void GameBoard::UpdateBackGround()
{
	if (!m_backGround || !m_player)
		return;

	if (!GameEngine::CameraManager::IsFollowCameraEnabled())
		return;

	m_backGround->SetPos(m_player->GetPos());
}

void GameBoard::CreateLightCircle() {

	m_lightCircle = new GameEngine::Entity();

	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_lightCircle);
	m_lightCircle->SetPos(sf::Vector2f(0.f, 0.f));
	float viewrad = 200.f;
	m_lightCircle->SetSize(sf::Vector2f(viewrad * 2, viewrad * 2));

	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(m_lightCircle->AddComponent < GameEngine::SpriteRenderComponent>());
	spriteRender->SetTexture(GameEngine::eTexture::LightCircle);


}

void GameBoard::UpdateLightCircle() {
	m_lightCircle->SetPos(m_player->GetPos());
}

/*
void GameBoard::CreateTorch() {
	m_torch = new GameEngine::Entity();

	GameEngine::GameEngineMain::GetInstance()->AddEntity(m_torch);
	m_torch->SetPos(sf::Vector2f(500.f, 500.f));
	m_torch->SetSize(sf::Vector2f(45.f, 10.f));

	GameEngine::SpriteRenderComponent* spriteRender = static_cast<GameEngine::SpriteRenderComponent*>(m_torch->AddComponent < GameEngine::SpriteRenderComponent>());
	spriteRender->SetTexture(GameEngine::eTexture::Star);
	//m_torch->AddComponent<GameEngine::SpriteRenderComponent>();
}*/

void GameBoard::UpdateTorch() {

	for (std::vector<GameEngine::Entity*>::iterator it = m_StarList.begin(); it != m_StarList.end();) {

		GameEngine::Entity* currStar = (*it);

		int torchCentX = currStar->GetPos().x;// +m_torch->GetSize().x / 2;
		int torchCentY = currStar->GetPos().y;// +m_torch->GetSize().y / 2;

		int playerCentX = m_player->GetPos().x;// +m_player->GetSize().x / 2;
		int playerCentY = m_player->GetPos().y;// +m_player->GetSize().y / 2;

		if (!m_starHold && sqrt(pow(torchCentX - playerCentX, 2) + pow(torchCentY - playerCentY, 2)) < 10) {

			GameEngine::GameEngineMain::GetInstance()->RemoveEntity(currStar);
			it = m_StarList.erase(it);
			m_starHold = true;

		}
		else if (sqrt(pow(torchCentX - playerCentX, 2) + pow(torchCentY - playerCentY, 2)) < 100) {
			currStar->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(GameEngine::eTexture::Star);
			currStar->GetComponent<GameEngine::SpriteRenderComponent>()->SetZLevel(4);
			it++;

		}
		else {
			currStar->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(GameEngine::eTexture::Transparent);
			it++;
		}

		for (std::vector<GameEngine::Entity*>::iterator sit = m_luminateList.begin(); sit != m_luminateList.end();) {

			GameEngine::Entity* currLamp = (*sit);

			int lampx = currLamp->GetPos().x;
			int lampy = currLamp->GetPos().y;

			if (sqrt(pow(torchCentX - lampx, 2) + pow(torchCentY - lampy, 2)) < 175) {
				currStar->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(GameEngine::eTexture::Star);
				currStar->GetComponent<GameEngine::SpriteRenderComponent>()->SetZLevel(4);
				break;
			}
			sit++;
		}
		currStar->GetComponent<GameEngine::SpriteRenderComponent>()->UpdateSpriteParams();

	}

}

void GameBoard::CreateStars(int num) {

	for (int i = 0; i < num; i++) {

		GameEngine::Entity* star = new GameEngine::Entity();
		GameEngine::SpriteRenderComponent* render = static_cast<GameEngine::SpriteRenderComponent*>(star->AddComponent<GameEngine::SpriteRenderComponent>());

		render->SetTexture(GameEngine::eTexture::Star); //UPDATE LATER W CORRECT TEXTURE
		render->SetZLevel(4);

		star->SetPos(sf::Vector2f(RandomFloatRange(0, 1920), RandomFloatRange(0, 1080)));
		star->SetSize(sf::Vector2f(50.f, 50.f));

		GameEngine::GameEngineMain::GetInstance()->AddEntity(star);

		m_StarList.push_back(star);
	}
}

void GameBoard::CreateLamps(int num) {

	for (int i = 0; i < num; i++) {

		GameEngine::Entity* lamp = new GameEngine::Entity();
		GameEngine::SpriteRenderComponent* render = static_cast<GameEngine::SpriteRenderComponent*>(lamp->AddComponent<GameEngine::SpriteRenderComponent>());

		render->SetTexture(GameEngine::eTexture::Lamppost); //UPDATE LATER W CORRECT TEXTURE
		render->SetZLevel(9);

		lamp->SetPos(sf::Vector2f(RandomFloatRange(0, 1920), RandomFloatRange(0, 1080)));
		lamp->SetSize(sf::Vector2f(150.f, 150.f));

		GameEngine::GameEngineMain::GetInstance()->AddEntity(lamp);

		m_lampList.push_back(lamp);
	}
}

void GameBoard::UpdateLamps() {

	int numLampsGotten = 0;

	for (std::vector<GameEngine::Entity*>::iterator it = m_lampList.begin(); it != m_lampList.end();) {

		GameEngine::Entity* currLamp = (*it);

		int lampCentX = currLamp->GetPos().x;// +m_torch->GetSize().x / 2;
		int lampCentY = currLamp->GetPos().y;// +m_torch->GetSize().y / 2;

		int playerCentX = m_player->GetPos().x;// +m_player->GetSize().x / 2;
		int playerCentY = m_player->GetPos().y;// +m_player->GetSize().y / 2;

		bool isLightable = true;
		bool isAlreadyNearLight = false;

		for (std::vector<GameEngine::Entity*>::iterator sit = m_luminateList.begin(); sit != m_luminateList.end();) {

			GameEngine::Entity* currLuminate = (*sit);

			int luminatex = currLuminate->GetPos().x;
			int luminatey = currLuminate->GetPos().y;

			if (luminatex == lampCentX && luminatey == lampCentY) {
				currLamp->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(GameEngine::eTexture::LampLit);
				isLightable = false;
				break;
			}
			else if (sqrt(pow(lampCentX - luminatex, 2) + pow(lampCentY - luminatey, 2)) < 200) {
				currLamp->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(GameEngine::eTexture::Lamppost);
				isAlreadyNearLight = true;
			}
			sit++;
		}

		if (!isLightable) {

		}
		else if (m_starHold && sqrt(pow(lampCentX - playerCentX, 2) + pow(lampCentY - playerCentY, 2)) < 25) {

			currLamp->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(GameEngine::eTexture::LampLit);
			numLampsGotten++;
			CreateLuminate(currLamp->GetPos());
			m_starHold = false;

			if (numLampsGotten >= numLamps) {
				m_isGameOver = true;
				m_won = true;
			}
		}
		else if (sqrt(pow(lampCentX - playerCentX, 2) + pow(lampCentY - playerCentY, 2)) < 100) {
			currLamp->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(GameEngine::eTexture::Lamppost);
		}
		else if (isAlreadyNearLight) {

		}
		else {
			currLamp->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(GameEngine::eTexture::Transparent);
		}

		currLamp->GetComponent<GameEngine::SpriteRenderComponent>()->UpdateSpriteParams();
		currLamp->GetComponent<GameEngine::SpriteRenderComponent>()->SetZLevel(9);
		it++;
	}
}

void GameBoard::CreateLuminate(const sf::Vector2f& pos) {

	GameEngine::Entity* luminate = new GameEngine::Entity();
	GameEngine::SpriteRenderComponent* render = static_cast<GameEngine::SpriteRenderComponent*>(luminate->AddComponent<GameEngine::SpriteRenderComponent>());

	render->SetTexture(GameEngine::eTexture::LightCircle);
	render->SetZLevel(3);

	luminate->SetPos(pos);
	luminate->SetSize(sf::Vector2f(500.f, 500.f));

	GameEngine::GameEngineMain::GetInstance()->AddEntity(luminate);

	m_luminateList.push_back(luminate);
}


void GameBoard::SpawnNewGhost(const sf::Vector2f& pos, const sf::Vector2f& size)
{
	GhostEntity* ghost = new GhostEntity();
	GameEngine::GameEngineMain::GetInstance()->AddEntity(ghost);
	ghost->SetPos(pos);
	ghost->SetSize(sf::Vector2f(size.x, size.y));
	ghost->AddComponent<GhostMovementComponent>();
	ghost->GetComponent<GhostMovementComponent>()->SetPlayerEntity(m_player);
	ghost->GetComponent<GameEngine::SpriteRenderComponent>()->SetZLevel(7);

	m_ghosts.push_back(ghost);
}
