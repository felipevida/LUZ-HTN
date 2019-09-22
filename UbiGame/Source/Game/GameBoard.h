#pragma once
#include "GameEngine\EntitySystem\Entity.h"

#include <SFML/System/Vector2.hpp>
#include <vector>

namespace Game
{
	//Used for storing and controling all game related entities
	class PlayerEntity;	

	class GameBoard
	{
	public:
		GameBoard();
		virtual ~GameBoard();

		//Temp - for nice architecture this should be within some sort of IUpdatable interface that GameEngine handles (so that not only entities can be updated)
		void Update();
		void DisplayMsg(int type);
		void CreateBounds();
		//void UpdateObstacles(float dt);
		//void UpdatePlayerDying();
		//void SpawnNewRandomObstacles();
		//void SpawnNewRandomTiledObstacles();
		void SpawnNewGhost(const sf::Vector2f& pos, const sf::Vector2f& size);
		//void SpawnNewObstacle(const sf::Vector2f& pos, const sf::Vector2f& size);
		bool IsGameOver() const { return m_isGameOver; }


	protected:
		//Placeholder temp - should go to helpers or smth.
		float RandomFloatRange(float a, float b)
		{
			return ((b - a) * ((float)rand() / RAND_MAX)) + a;
		}

		void CreateLightCircle();
		void CreateBackGround();
		void CreateLamps(int num);
		void CreateStars(int num);
		//void CreateTorch();
		void CreateLuminate(const sf::Vector2f& pos);

		void UpdateBackGround();
		void UpdateLightCircle();
		void UpdateTorch();
		void UpdateLamps();

		void Restart();
		
		PlayerEntity* m_player;
		GameEngine::Entity* m_backGround;
		GameEngine::Entity* m_lightCircle;
		GameEngine::Entity* m_torch;

		std::vector<GameEngine::Entity*> m_obstacles;
		std::vector<GameEngine::Entity*> m_ghosts;
		std::vector<GameEngine::Entity*> m_StarList;
		std::vector<GameEngine::Entity*> m_lampList;
		std::vector<GameEngine::Entity*> m_luminateList;

		float m_lastObstacleSpawnTimer;
		bool m_isGameOver;
		bool m_starHold;
		int numLamps;
		bool m_won;
		float msg_dt;
		
	};
}

