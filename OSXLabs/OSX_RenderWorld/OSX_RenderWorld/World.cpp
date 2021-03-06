#include <World.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include "ResourcePath.hpp"

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures() 
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-200.f)
, mPlayerAircraft(nullptr)
{
	loadTextures();
	buildScene();

	// Prepare the view
	mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
	// Scroll the world
	mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());	

	// Move the player sidewards (plane scouts follow the main aircraft)
	sf::Vector2f position = mPlayerAircraft->getPosition();
	sf::Vector2f velocity = mPlayerAircraft->getVelocity();

	// If player touches borders, flip its X velocity
//	if (position.x <= mWorldBounds.left + 150.f
//	 || position.x >= mWorldBounds.left + mWorldBounds.width - 150.f)
//	{
//		velocity.x = -velocity.x;
//		mPlayerAircraft->setVelocity(velocity);
//	}


	// Apply movements
	mSceneGraph.update(dt);
}

void World::draw()
{
	mWindow.setView(mWorldView);
	mWindow.draw(mSceneGraph);
}

void World::loadTextures()
{
	mTextures.load(Textures::Eagle, resourcePath() + "Eagle.png");
	mTextures.load(Textures::Raptor,resourcePath() + "Raptor.png");
	mTextures.load(Textures::Desert, resourcePath() + "Desert.png");
    mTextures.load(Textures::Water, resourcePath() + "water.png");
    mTextures.load(Textures::Grass, resourcePath() + "grass.png");
}

void World::buildScene()
{
	// Initialize the different layers
	for (std::size_t i = 0; i < LayerCount; ++i)
	{
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();

		mSceneGraph.attachChild(std::move(layer));
	}

	// Prepare the tiled background
	sf::Texture& dTexture = mTextures.get(Textures::Desert);
    sf::Texture& wTexture = mTextures.get(Textures::Water);
    sf::Texture& gTexture = mTextures.get(Textures::Grass);
	sf::IntRect textureRect(mWorldBounds);
	dTexture.setRepeated(true);
    wTexture.setRepeated(true);
    gTexture.setRepeated(true);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> dSprite(new SpriteNode(dTexture, textureRect));
    std::unique_ptr<SpriteNode> wSprite(new SpriteNode(wTexture, textureRect));
    std::unique_ptr<SpriteNode> gSprite(new SpriteNode(gTexture, textureRect));
    
	dSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
    wSprite->setPosition(mWorldBounds.left, mWorldBounds.top - 500.f);
    gSprite->setPosition(mWorldBounds.left, mWorldBounds.top - 1000.f);
    
    if (dSprite->getPosition().y > mWorldView.getCenter().y - 500.f)
    {
        dSprite->move(mWorldBounds.left, mWorldView.getCenter().y - 1500.f);
    }
    
    if (wSprite->getPosition().y > mWorldView.getCenter().y - 500.f)
    {
        wSprite->move(mWorldBounds.left, mWorldView.getCenter().y - 1500.f);
    }
    
    if (gSprite->getPosition().y > mWorldView.getCenter().y - 500.f)
    {
        gSprite->move(mWorldBounds.left, mWorldView.getCenter().y - 1500.f);
    }
    
    mSceneLayers[Background]->attachChild(std::move(dSprite));
    mSceneLayers[Background]->attachChild(std::move(wSprite));
    mSceneLayers[Background]->attachChild(std::move(gSprite));

	// Add player's aircraft
	std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, mTextures));
	mPlayerAircraft = leader.get();
	mPlayerAircraft->setPosition(mSpawnPosition);
	mPlayerAircraft->setVelocity(0.f, mScrollSpeed);
	mSceneLayers[Air]->attachChild(std::move(leader));

	// Add two escorting aircrafts, placed relatively to the main plane
	std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Raptor, mTextures));
	leftEscort->setPosition(-80.f, 50.f);
	mPlayerAircraft->attachChild(std::move(leftEscort));

	std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Raptor, mTextures));
	rightEscort->setPosition(80.f, 50.f); 
	mPlayerAircraft->attachChild(std::move(rightEscort));
}
