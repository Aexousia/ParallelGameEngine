#pragma once
#include <unordered_map>
#include <../dependancies/glm/vec3.hpp>

template <typename T>
std::vector<T> &operator+=(std::vector<T> &A, const std::vector<T> &B)
{
	A.reserve(A.size() + B.size());                // preallocate memory without erase original data
	A.insert(A.end(), B.begin(), B.end());         // add B;
	return A;                                        // here A could be named AB
}

//helper function
template <typename Key, typename Value>
Value& get_or(std::unordered_map<Key, Value>& m, const Key& key, Value& default_value)
{
	auto it = m.find(key);
	if (it == m.end()) {
		return default_value;
	}
	return it->second;
}

#define PI 3.14159265359

inline static float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

inline static glm::vec3 randomUnitVector() {
	float phi = RandomFloat(0, 2 * PI);
	float costheta = RandomFloat(-1, 1);

	float theta = std::acos(costheta);
	float x = std::sin(theta) * std::cos(phi);
	float y = std::sin(theta) * std::sin(phi);
	float z = std::cos(theta);
	return glm::vec3(x, y, z);
}