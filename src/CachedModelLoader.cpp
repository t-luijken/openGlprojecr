#include "CachedModelLoader.hpp"
#include <map>
#include <algorithm>
static std::map< const std::string, ObjModel*> cachedMap;

/**
* Turns a string to lowercase
*/
static inline std::string toLower(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}

ObjModel* cachedModelLoaderGetModel(std::string& filename)
{
	filename = toLower(filename);
	if (cachedMap.find(filename) == cachedMap.end())
	{
		cachedModelLoaderCacheModel(filename);
	}
	return cachedMap[filename];
}

void cachedModelLoaderCacheModel(const std::string& filename)
{
	cachedMap[filename] = new ObjModel(filename);
}
