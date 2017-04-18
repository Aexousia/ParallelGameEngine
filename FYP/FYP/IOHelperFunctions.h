#pragma once
#include <string>
#include <fstream>
#include <vector>

inline std::string readFile(const char* filePath)
{
	std::ifstream shaderFile(filePath);

	// find the file size
	shaderFile.seekg(0, std::ios::end);
	std::streampos          length = shaderFile.tellg();
	shaderFile.seekg(0, std::ios::beg);

	// read whole file into a vector:
	std::vector<char>       buffer(length);
	shaderFile.read(&buffer[0], length);

	// return the shader string
	return std::string(buffer.begin(), buffer.end());
}

//helper functions
struct LoadException : public std::exception
{
	std::string s;
	LoadException(std::string ss) : s(ss) { printf(s.c_str()); }
	~LoadException() throw () {}
	const char* what() const throw() { return s.c_str(); }
};

//checks if a file exists in a given path
inline bool fileExists(const std::string& name) {
	std::ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	}
	else {
		f.close();
		return false;
	}
}
