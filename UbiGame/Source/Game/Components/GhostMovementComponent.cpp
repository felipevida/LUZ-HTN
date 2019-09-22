#include "GhostMovementComponent.h"
#include "GameEngine\GameEngineMain.h"
#include <vector>
#include "GameEngine\EntitySystem\Components\SpriteRenderComponent.h"
#include "GameEngine\EntitySystem\Components\ParticleEmitterComponent.h"
#include "Game\GameEntities\PlayerEntity.h"
#include "Game\GameBoard.h"
#include <SFML/Window/Keyboard.hpp>

using namespace Game;
GhostMovementComponent::GhostMovementComponent()
{}

GhostMovementComponent::~GhostMovementComponent() {}

void GhostMovementComponent::OnAddToWorld() {

	__super::OnAddToWorld();
}

float turnTime = 0;
int luck = 5;

void GhostMovementComponent::Update()
{
	__super::Update();

	//Grabs how much time has passed since the last Update() call
	float dt = GameEngine::GameEngineMain::GetTimeDelta();
	float angle = 1;
	__super::Update();

	float ghostVelocity = 00.0f;

	turnTime = turnTime + dt;

	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
	sf::Vector2f ghostVelo = sf::Vector2f(0.0f, 0.0f);

	int playerPos[] = { 0,0 };
	if (m_player)
	{
		playerPos[0] = m_player->GetPos().x;
		playerPos[1] = m_player->GetPos().y;
	}
	int difference[] = { 1,1 };
	int ghostPos[] = { 100,1000 };
	ghostPos[0] = GetEntity()->GetPos().x;
	ghostPos[1] = GetEntity()->GetPos().y;

	difference[0] = playerPos[0] - ghostPos[0];
	difference[1] = playerPos[1] - ghostPos[1];
	if (difference[0] != 0) {
		angle = atan(difference[1] / difference[0]);
	}
	
	


	int goal[] = { 0,0 };
	goal[0] = abs(40 * cos(angle));
	goal[1] = abs(40 * sin(angle));
	if (difference[1] < 0) {
		goal[1] = goal[1] * -1;
	}
	if (difference[0] < 0) {
		goal[0] = goal[0] * -1;
	};

	float x = goal[0] * dt;
	float y = goal[1] * dt;

	GetEntity()->SetPos((sf::Vector2f(x, y) + GetEntity()->GetPos()));

	GetEntity()->SetPos((sf::Vector2f(x, y) + GetEntity()->GetPos()));
	GetEntity()->SetPos((sf::Vector2f(x, y) + GetEntity()->GetPos()));

	if (sqrt(pow((GetEntity()->GetPos().x - m_player->GetPos().x), 2) +
		pow((GetEntity()->GetPos().y - m_player->GetPos().y), 2)) > 150) {

		GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(GameEngine::eTexture::BT); //Original: BT
		GameEngine::SParticleDefinition particleDef = GameEngine::SParticleDefinition(GameEngine::eTexture::BT, 1, sf::Vector2f(32.f, 32.f), GameEngine::EAnimationId::Smoke, 1.f);
		GetEntity()->GetComponent<GameEngine::ParticleEmitterComponent>()->SetParticleDefinition(particleDef);

		GetEntity()->GetComponent<GameEngine::ParticleEmitterComponent>()->Update();
		GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>()->UpdateSpriteParams();
	}
	else {
		GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>()->SetTexture(GameEngine::eTexture::Ghost);// not transparent?
	
		GameEngine::SParticleDefinition particleDef = GameEngine::SParticleDefinition(GameEngine::eTexture::Particles, 1, sf::Vector2f(32.f, 32.f), GameEngine::EAnimationId::Smoke, 1.f);
		GetEntity()->GetComponent<GameEngine::ParticleEmitterComponent>()->SetParticleDefinition(particleDef);

		GetEntity()->GetComponent<GameEngine::ParticleEmitterComponent>()->Update();
		GetEntity()->GetComponent<GameEngine::SpriteRenderComponent>()->UpdateSpriteParams();

	}


}

