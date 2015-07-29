#ifndef MAP_H
#define MAP_H

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\VertexArray.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\Texture.hpp>

#include <string>


class Map : public sf::Drawable, public sf::Transformable
{
public:

	Map(sf::Texture& texture);

private:

	void load(sf::Vector2u tileSize, unsigned int width, unsigned int height);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture& mTexture;
	sf::VertexArray mVertices;
};

#endif 













