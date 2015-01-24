#ifndef COLLISION_H
#define COLLISION_H

#include <Box2D/Box2D.h>

class Collision : public b2ContactListener {
private:
  void BeginContact(b2Contact* contact) override;
  void EndContact(b2Contact* contact) override;
};

#endif // COLLISION_H
