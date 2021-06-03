#include"GameObject.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "../tigl.h"


GameObject::GameObject(glm::vec3 pos, glm::vec3 rotationAngles, glm::vec3 scale) : components(), drawComponents()
{
	this->position = pos;
	this->rotationAngles = rotationAngles;

	this->scale = scale;
}

GameObject::~GameObject()
{
	for (Component* component : components)
	{
		delete component;
	}
}

void GameObject::addComponent(Component* newComponent)
{
	newComponent->setGameObject(this);
	components.push_back(newComponent);

	DrawComponent* drawComponent = dynamic_cast<DrawComponent*>(newComponent);
	if (drawComponent)
		drawComponents.push_back(drawComponent);
	BoundingBoxComponent* boundingBoxComponent = dynamic_cast<BoundingBoxComponent*>(newComponent);
	if (boundingBoxComponent)
		boundingBoxComponents.push_back(boundingBoxComponent);
}

const std::vector<Component*> GameObject::getComponents() const
{
	return this->components;
}

void GameObject::update(double timeMillis)
{
	for (Component* component : components)
	{
		component->update(timeMillis);
	}
}

void GameObject::draw() const
{
	glm::mat4 originalModelMatrix = tigl::shader->getModelMatrix();

	tigl::shader->setModelMatrix(originalModelMatrix * getModelMatrix());
	for (DrawComponent* drawComponent : drawComponents)
	{
		drawComponent->draw();
	}

	tigl::shader->setModelMatrix(originalModelMatrix);
}

const std::vector<BoundingBoxComponent*> GameObject::getBoundingBoxes() const
{
	return boundingBoxComponents;
}

const glm::mat4 GameObject::getModelMatrix() const
{
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, this->position);
	modelMatrix = glm::rotate(modelMatrix, this->rotationAngles[0], glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, this->rotationAngles[1], glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, this->rotationAngles[2], glm::vec3(0, 0, 1));
	modelMatrix = glm::scale(modelMatrix, this->scale);

	return modelMatrix;
}
