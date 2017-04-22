#pragma once
#include <Framework.h>
#include <../dependancies/glm/vec3.hpp>

class SphereCollisionComponent;
class TransformComponent;
class VelocityComponent;
class MassComponent;

typedef std::tuple<SphereCollisionComponent*, TransformComponent*, VelocityComponent*, MassComponent*, NULL_COMPONENT*> SphereBodyData ;

struct CollisionPair
{
	SphereBodyData body1, body2;
	float overlap;

	CollisionPair(SphereBodyData& b1, SphereBodyData& b2, float _overlap)
		:	body1(b1)
		,	body2(b2)
		,	overlap(_overlap)
	{

	}
};

class CollisionSystem : public ISystem, public IAutoMapUser<CollisionSystem>
{
public:
	CollisionSystem();
	void process(float dt) override;
	void snapToBounds(glm::vec3& boundsMin, glm::vec3& boundsMax);
	void resolveCollidingPairs();
	void findCollidingPairs();
	void pairCollidingCheck(SphereBodyData body1, SphereBodyData body2);
	void newScene();

private:
	std::unordered_map<int, std::vector<CollisionPair>> m_collidingPairs;
	std::map<TransformComponent*, glm::vec3> m_velocityDelta;
	std::vector<SphereBodyData> m_spheres;
	SDL_semaphore* m_blocking;
};
