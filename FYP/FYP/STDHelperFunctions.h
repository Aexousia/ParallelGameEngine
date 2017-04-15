#pragma once
#include <unordered_map>

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