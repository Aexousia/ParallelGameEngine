#pragma once
#include <Framework.h>

class PhysicsSystem : public ISystem, public IAutoMapUser<PhysicsSystem>
{
public:
	void process(float dt) override;
private:

};
