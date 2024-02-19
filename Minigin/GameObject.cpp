#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"


dae::GameObject::GameObject()
{
	m_pTransform = AddComponent<TransformComponent>();
}

dae::GameObject::~GameObject()
{
}


void dae::GameObject::Update()
{
	for (const auto& component : m_pComponents)
	{
		component->Update();
	}
}

void dae::GameObject::Render() const
{
	for (const auto& component : m_pComponents)
	{
		component->Render();
	}
}