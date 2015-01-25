#ifndef COLLISION_H
#define COLLISION_H

#include <Box2D/Box2D.h>

enum class CollisionType {
    None,
    Ground,
    Wall,
    Spikes,
    SpikesBall,
};

class Collision : public b2ContactListener {
private:
  void BeginContact(b2Contact* contact) override;
  void EndContact(b2Contact* contact) override;
};

#endif // COLLISION_H
