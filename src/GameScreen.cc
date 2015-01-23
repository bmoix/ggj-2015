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

	// Add the background sprite to the scene
	std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(backTexture));
	//centrar la pantalla i escalar la imatge
	backgroundSprite->setPosition(sf::Vector2f(0.0f, 0.0f));
	mSceneLayers[Background]->attachChild(std::move(backgroundSprite));
	
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
    return true;
}

bool GameScreen::handleEvent(const sf::Event& event) {
    return true;
}

void GameScreen::handleRealtimeInput(){
}