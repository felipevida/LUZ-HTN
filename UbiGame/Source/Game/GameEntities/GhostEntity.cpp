#pragma once
#include "GameEngine\EntitySystem\Entity.h"
#include "GameEngine\EntitySystem\Components\AnimationComponent.h"
#include "GameEngine\EntitySystem\Components\SpriteRenderComponent.h"
#include "Game\Components\GhostMovementComponent.h"
#include <Game\GameEntities\GhostEntity.h>
#include <GameEngine\EntitySystem\Components\CollidablePhysicsComponent.h>
#include <GameEngine\EntitySystem\Components\ParticleEmitterComponent.h>

using namespace Game;

GhostEntity::GhostEntity()
{
	//Render 
	m_renderComponent = static_cast<GameEngine::SpriteRenderComponent*>(AddComponent<GameEngine::SpriteRenderComponent>());
	m_renderComponent->SetTexture(GameEngine::eTexture::Ghost);
	m_renderComponent->SetZLevel(2);

	//Animation
	m_animComponent = static_cast<GameEngine::AnimationComponent*>(AddComponent<GameEngine::AnimationComponent>());
	
	//Collisions
	AddComponent<GameEngine::CollidablePhysicsComponent>();

	//Particles
	GameEngine::ParticleEmitterComponent* emitterComponent = static_cast<GameEngine::ParticleEmitterComponent*>(AddComponent<GameEngine::ParticleEmitterComponent>());
	GameEngine::SParticleDefinition particleDef = GameEngine::SParticleDefinition(GameEngine::eTexture::Particles, 1, sf::Vector2f(32.f, 32.f), GameEngine::EAnimationId::Smoke, 1.f);
	emitterComponent->SetParticleDefinition(particleDef);
}


GhostEntity::~GhostEntity()
{

}


void GhostEntity::OnAddToWorld()
{
	__super::OnAddToWorld();

	if (m_animComponent)
	{
		m_animComponent->PlayAnim(GameEngine::EAnimationId::BirdFly);
	}
}


void GhostEntity::OnRemoveFromWorld()
{
	__super::OnRemoveFromWorld();
}