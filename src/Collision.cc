#include "Collision.h"
#include "SpriteNode.h"

void Collision::BeginContact(b2Contact* contact) {
  SpriteNode* a = (SpriteNode*) contact->GetFixtureA()->GetBody()->GetUserData();
  SpriteNode* b = (SpriteNode*) contact->GetFixtureB()->GetBody()->GetUserData();

  if (a and b) {
    a->collidedWith(b);
    b->collidedWith(a);
  }
}

void Collision::EndContact(b2Contact* contact) {
}

