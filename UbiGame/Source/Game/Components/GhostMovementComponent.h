
#pragma once
#include <iostream>
#include "GameEngine\EntitySystem\Component.h"
#include "Game\Components\GhostMovementComponent.h"

namespace Game
{
	class PlayerEntity;

	class GhostMovementComponent : public GameEngine::Component
	{
	public:
		GhostMovementComponent();
		~GhostMovementComponent();

		void SetPlayerEntity(PlayerEntity* player) { m_player = player; }

		virtual void Update() override;
		virtual void OnAddToWorld() override;
	private:
		PlayerEntity* m_player;
	};
	
}