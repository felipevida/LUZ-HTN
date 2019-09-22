#pragma once
#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

namespace GameEngine
{
	//TODO - if needed, move out of Engine part to some sort of loader on the game side of things
	namespace eTexture
	{
		enum type
		{
			None = -1,
			Player = 0,
			BT,
			Ghost,
			Tileset,
			BG,
			Particles,
			LightCircle,
			Star,
			Transparent,
			Lamppost,
			LampLit,
			intro,
			cont,
			restart,
			Count,
		};
	}

	inline const char* GetPath(eTexture::type texture)
	{
		switch (texture)
		{
		case eTexture::Player:    return "player.png";
		case eTexture::Ghost:    return "ghost.png";
		case eTexture::Tileset:   return "tileset.png";
		case eTexture::BG:		  return "bg.png";
		case eTexture::BT:		  return "	bg.png";
		case eTexture::Particles: return "particles.png";
		case eTexture::LightCircle: return "LightCircle.png";
		case eTexture::Star: return "star.png";
		case eTexture::Transparent: return "Transparent.png";
		case eTexture::Lamppost: return "lamp_off.png";
		case eTexture::LampLit: return "lamp_on.png";
		case eTexture::intro: return "text_intro.png";
		case eTexture::cont: return "text_continue.png";
		case eTexture::restart: return "text_restart.png";
		default:       return "UnknownTexType";
		}
	}

	class TextureManager
	{
	public:
		static TextureManager* GetInstance() { if (!sm_instance) sm_instance = new TextureManager(); return sm_instance; }
		~TextureManager();

		void LoadTextures();
		void UnLoadTextures();

		sf::Texture* GetTexture(eTexture::type texture) const { return m_textures[(int)texture]; }

	private:
		TextureManager();
		static TextureManager* sm_instance;

		sf::Texture* m_textures[eTexture::Count];
	};
}

namespace TextureHelper
{
	static sf::Vector2f GetTextureTileSize(GameEngine::eTexture::type texture)
	{
		switch (texture)
		{
		case  GameEngine::eTexture::Player:  return sf::Vector2f(32.f, 32.f);
		case  GameEngine::eTexture::Ghost:  return sf::Vector2f(32.f, 32.f);
		case  GameEngine::eTexture::Tileset: return sf::Vector2f(32.f, 32.f);
		case  GameEngine::eTexture::BG:	     return sf::Vector2f(680.f, 120.f);
		case  GameEngine::eTexture::intro:	     return sf::Vector2f(680.f, 120.f);
		case  GameEngine::eTexture::cont:	     return sf::Vector2f(680.f, 120.f);
		case  GameEngine::eTexture::restart:	     return sf::Vector2f(500.f, 500.f);
		case  GameEngine::eTexture::Particles: return sf::Vector2f(31.f, 32.f);
		default:							 return sf::Vector2f(-1.f, -1.f);
		}
	}
}

