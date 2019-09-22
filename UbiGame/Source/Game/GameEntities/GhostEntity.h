#pragma once
#include "GameEngine\EntitySystem\Entity.h"
#include "GameEngine\EntitySystem\Components\SpriteRenderComponent.h"
#include "Game\Components\GhostMovementComponent.h"


namespace Game
{

	class GhostEntity : public GameEngine::Entity
	{
	public:
		GhostEntity();
		~GhostEntity();

		virtual void OnAddToWorld() override;
		virtual void OnRemoveFromWorld() override;

	protected:
		GameEngine::SpriteRenderComponent* m_renderComponent;
		GhostMovementComponent* m_playerMovementComponent;
		GameEngine::AnimationComponent* m_animComponent;
	};
}