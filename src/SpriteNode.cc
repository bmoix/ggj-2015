#include "SpriteNode.h"

SpriteNode::SpriteNode(const sf::Texture& texture) : 
    mBody(NULL),
    mSprite(texture),
    mSize(texture.getSize()) {
	Utils::centerOrigin(mSprite);
}   

SpriteNode::SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect) 
: mSprite(texture, textureRect) {
	Utils::centerOrigin(mSprite);
}

void SpriteNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mSprite, states);
}

void SpriteNode::setSize(sf::Vector2u desiredSize){
    float scalex, scaley;
    scalex = scaley = 0.0;
    scalex = float(desiredSize.x)/mSprite.getTexture()->getSize().x;
    scaley = float(desiredSize.y)/mSprite.getTexture()->getSize().y;
    setScale(scalex, scaley);
    mSize = sf::Vector2f(desiredSize.x, desiredSize.y);
}

void SpriteNode::setColor(sf::Color color){
    mSprite.setColor(color);
}

sf::Color SpriteNode::getColor(){
    return mSprite.getColor();
}

void SpriteNode::updateCurrent(sf::Time dt) {
    if (mBody != NULL) {
        b2Vec2 pos = mBody->GetPosition();
        this->setPosition(pos.x*metersToPixels, pos.y*metersToPixels);
    }
}

void SpriteNode::setupBody(b2World* world, bool dynamic, float bbscale) {
    sf::Vector2f vpos = this->getWorldPosition();

    b2Vec2 pos(vpos.x/metersToPixels, vpos.y/metersToPixels);
    b2Vec2 size(mSize.x/metersToPixels, mSize.y/metersToPixels);

    if (dynamic) {
        b2BodyDef body_def;
        body_def.type = b2_dynamicBody;
        body_def.position.Set(pos.x, pos.y);
        mBody = world->CreateBody(&body_def);

        b2PolygonShape dynamic_box;
        dynamic_box.SetAsBox(size.x*bbscale/2.0, size.y*bbscale/2.0);

        b2FixtureDef fixture_def;
        fixture_def.shape = &dynamic_box;
        fixture_def.density = 1.0f;
        fixture_def.friction = 20.0f;
        mBody->CreateFixture(&fixture_def);
    }
    else {
        b2BodyDef body_def;
        body_def.position.Set(pos.x, pos.y);
        mBody = world->CreateBody(&body_def);

        b2PolygonShape shape;
        shape.SetAsBox(size.x*bbscale/2.0, size.y*bbscale/2.0);

        // Add fixture to the body
        mBody->CreateFixture(&shape, 0.0f);
    }
}

void SpriteNode::createBody(b2World* world, bool dynamic, float bbscale) {
    setupBody(world, dynamic, bbscale);
    mBody->SetUserData(this);
}

void SpriteNode::collidedWith(SpriteNode* other) {
  (void) other;
}

sf::IntRect SpriteNode::getBounds() {
    sf::Vector2f pos = getWorldPosition();
    sf::Vector2i newPos = sf::Vector2i(pos.x-mSize.x/2, pos.y-mSize.y/2);
    sf::Vector2i size = sf::Vector2i(mSize.x, mSize.y);
    return sf::IntRect(newPos, size);
}
