#pragma once
#include <Framework.h>

class CollisionSystem : public ISystem, public IAutoMapUser<CollisionSystem>
{
public:
	void process(float dt) override;
	
private:

};
