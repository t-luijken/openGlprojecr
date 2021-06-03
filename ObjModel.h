#pragma once

#include <glm/glm.hpp>
#include "tigl.h"

#include <string>
#include <vector>
#include <list>

class Texture;

class ObjModel
{
private:
	class Vertex
	{
	public:
		int position;
		int normal;
		int texcoord;
	};

	class MaterialInfo
	{
	public:
		MaterialInfo();
		~MaterialInfo();
		std::string name;
		Texture* texture;
	};

	class ObjGroup
	{
	public:
		~ObjGroup();
		std::string name;
		int materialIndex;
		tigl::VBO* vbo;
	};

	std::vector<glm::vec3>	vertices;
	std::vector<glm::vec3>	normals;
	std::vector<glm::vec2>	texcoords;
	std::vector<ObjGroup*> groups;
	std::vector<MaterialInfo*> materials;

	//bottom left hind
	glm::fvec3 blh;
	//top right front
	glm::fvec3 trf;

	void loadMaterialFile(const std::string& fileName, const std::string& dirName);
public:
	ObjModel(const std::string& filename);
	~ObjModel(void);

	void draw();

	const glm::vec3 getBLH();
	const glm::vec3 getTRF();
};
