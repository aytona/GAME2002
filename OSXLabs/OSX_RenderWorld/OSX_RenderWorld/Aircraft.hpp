#ifndef AIRCRAFT_HPP
#define AIRCRAFT_HPP

#include <Entity.hpp>
#include <ResourceIdentifiers.hpp>

#include <SFML/Graphics/Sprite.hpp>


class Aircraft : public Entity
{
	public:
		enum Type
		{
			Eagle,
			Raptor,
		};


	public:
							Aircraft(Type type, const TextureHolder& textures);


	private:
		virtual void		drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


	private:
		Type				mType;
		sf::Sprite			mSprite;
};

#endif // AIRCRAFT_HPP
