#include "Map.h"

Map::Map(sf::Texture& texture)
	: mTexture(texture)
{
	load(sf::Vector2u(200 , 200) , 10 , 10);
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
    states.texture = &mTexture;
	target.draw(mVertices, states);
}

void Map::load(sf::Vector2u tileSize, unsigned int width, unsigned int height) 
{
	mVertices.setPrimitiveType(sf::PrimitiveType::Quads);
	mVertices.resize(width * height * 4);
	
	const int size = width * height;

	std::vector<int> tiles;

	for(unsigned int i = 0; i < width * height; ++i)
		tiles.push_back(rand()% 4);

	for (unsigned int i = 0; i < width; ++i)   
		for (unsigned int j = 0; j < height; ++j)
		{

               
			int tileNumber = tiles[i + j * width];


               
			int tu = tileNumber % (mTexture.getSize().x / tileSize.x);
               
			int tv = tileNumber / (mTexture.getSize().x / tileSize.x);

               
			sf::Vertex* quad = &mVertices[(i + j * width) * 4];
          
			quad[0].position = sf::Vector2f((float)i * tileSize.x, (float)j * tileSize.y);
			quad[1].position = sf::Vector2f((float)(i + 1) * tileSize.x, (float)j * tileSize.y);
			quad[2].position = sf::Vector2f((float)(i + 1) * tileSize.x, (float)(j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f((float)i * tileSize.x, (float)(j + 1) * tileSize.y);

			//quad[0].color = sf::Color(std::rand()%255,std::rand()%255,std::rand()%255);
			//quad[1].color = sf::Color(std::rand()%255,std::rand()%255,std::rand()%255);		
			//quad[2].color = sf::Color(std::rand()%255,std::rand()%255,std::rand()%255);
			//quad[3].color = sf::Color(std::rand()%255,std::rand()%255,std::rand()%255);
			 
               
			quad[0].texCoords = sf::Vector2f((float)tu * tileSize.x, (float)tv * tileSize.y);
			quad[1].texCoords = sf::Vector2f((float)(tu + 1) * tileSize.x, (float)tv * tileSize.y);
			quad[2].texCoords = sf::Vector2f((float)(tu + 1) * tileSize.x, (float)(tv + 1) * tileSize.y);
			quad[3].texCoords = sf::Vector2f((float)tu * tileSize.x, (float)(tv + 1) * tileSize.y);
		}
}
