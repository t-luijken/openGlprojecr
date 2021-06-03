#pragma once
#include <string>

#include "../ObjModel.h"

class ObjModel;
ObjModel* cachedModelLoaderGetModel(std::string& filename);

void cachedModelLoaderCacheModel(const std::string& filename);