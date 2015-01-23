/*********************************GameScreen.cc***********************************/
#include "GameScreen.h"

GameScreen::GameScreen(StatesStack& stack, Context context)
: State(stack, context) {
	// CREACIÓ ESCENA
	// Inicialitza les dues capes
	for (std::size_t i = 0; i < LayerCount; ++i) {
		SceneNode::Ptr layer(new SceneNode());
		mSceneLayers[i] = layer.get();
		mSceneGraph.attachChild(std::move(layer));
	}
	// Prepara el fons de pantalla i la font
	sf::Font& font = getContext().mFonts->get(Fonts::Sansation);
	sf::Texture& backTexture = getContext().mTextures->get(Textures::GameBackground);
	sf::Texture& playerTexture = getContext().mTextures->get(Textures::Player1);

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(backTexture));
	//centrar la pantalla i escalar la imatge
	backgroundSprite->setPosition(sf::Vector2f(0.0f, 0.0f));
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));

	// Add the player to the scene
	std::unique_ptr<Player> player(new Player(playerTexture));
	mPlayer = player.get();
	mPlayer->setPosition(0.0f,1080-256);
	mSceneLayers[Players]->attachChild(std::move(player));

	// Prepara el text
	std::unique_ptr<TextNode> textNode(new TextNode(font, "WOLOLO"));
	mText = textNode.get(); // Guarda una referència al TextNode
	mText->setPosition(100 , 0);
	mText->setScale(sf::Vector2f(2,2));
	mSceneLayers[Text]->attachChild(std::move(textNode));

}

void GameScreen::draw() {
	getContext().mRTexture->draw(mSceneGraph);
}
bool GameScreen::update(sf::Time dt) {
	handleRealtimeInput();
	mSceneGraph.update(dt);
    return true;
}

bool GameScreen::handleEvent(const sf::Event& event) {
    return true;
}

void GameScreen::handleRealtimeInput(){
	/*bool moving = false;
	mPlayer->setVel(0,0);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up ) ) {
	mPlayer->setVel(mPlayer->getVel().x,mPlayer->getVel().y-movVel);
	mPlayer->setDir(dir_up); moving = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down ) ) {
	mPlayer->setVel(mPlayer->getVel().x,mPlayer->getVel().y+movVel);
	mPlayer->setDir(dir_down); moving = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left ) ) {
	mPlayer->setVel(mPlayer->getVel().x-movVel,mPlayer->getVel().y);
	mPlayer->setDir(dir_left); moving = true;
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ) {
	mPlayer->setVel(mPlayer->getVel().x+movVel,mPlayer->getVel().y);
	mPlayer->setDir(dir_right); moving = true;
	}
	if(! moving) {
		mPlayer->setVel(0,0);
		mPlayer->setDir(dir_none);
	}*/
}