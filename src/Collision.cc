#include "Collision.h"
#include "SpriteNode.h"

void Collision::BeginContact(b2Contact* contact) {
    SpriteNode* a = (SpriteNode*) contact->GetFixtureA()->GetBody()->GetUserData();
    SpriteNode* b = (SpriteNode*) contact->GetFixtureB()->GetBody()->GetUserData();

    b2WorldManifold worldManifold;
    contact->GetWorldManifold( &worldManifold );
    b2Vec2 normal = worldManifold.normal;

    if (a && b) {
        a->collidedWith(b, normal);
        std::swap(normal.x, normal.y);
        normal *= -1.0f;
        b->collidedWith(a, normal);
    }
}

void Collision::EndContact(b2Contact* contact) {
    SpriteNode* a = (SpriteNode*) contact->GetFixtureA()->GetBody()->GetUserData();
    SpriteNode* b = (SpriteNode*) contact->GetFixtureB()->GetBody()->GetUserData();

    b2WorldManifold worldManifold;
    contact->GetWorldManifold( &worldManifold );
    b2Vec2 normal = worldManifold.normal;

    if (a && b) {
        a->endContactWith(b, normal);
        std::swap(normal.x, normal.y);
        normal *= -1.0f;
        b->endContactWith(a, normal);
    }
}

