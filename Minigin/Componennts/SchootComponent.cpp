#include "SchootComponent.h"
#include "GameObject.h"
#include "Base/Texture2D.h"
#include "TransformComponent.h"
#include "Resources/ResourceManager.h"
#include "ImageComponent.h"
#include "BulletMovementComponent.h"
#include "ImageRenderComponent.h"
#include "Sound/SoundManager.h"
#include "Sound/SoundSystem.h"
#include "CollisionComponent.h"
#include <iostream>


dae::SchootComponent::SchootComponent(GameObject* Owner)
	:BaseComponent(Owner)
{
	m_AttackTexture = ResourceManager::GetInstance().LoadTexture("Bullet.png");
	auto pOwner = GetOwner();

	m_pPlayerTransform = pOwner->GetComponent<TransformComponent>();
	glm::vec2 playerSpriteDimension = pOwner->GetComponent<dae::ImageComponent>()->GetTextureDimensions();
	glm::vec2 bulletSpriteDimensions = static_cast<glm::vec2>(m_AttackTexture->GetSize());

	m_BulletStartOffset = { playerSpriteDimension.x / 2.0f + bulletSpriteDimensions.x / 2.0f, playerSpriteDimension.y };

	m_pSoundsystem = SoundManager::GetInstance().GetSoundSystem();
}

dae::SchootComponent::~SchootComponent()
{
	std::cout << "SchootComponent\n";
}

void dae::SchootComponent::Attack()
{
	if (!m_pScene)
	{
		std::cerr << "Error: No scene set for ShootComponent. Use ShootComponent::SetScene to set the scene.\n";
		return;
	}

	if (m_FiredBullets.size() >= 1)
		return;

	const glm::vec2 ownPos = m_pPlayerTransform->GetWorldPosition();
	auto bullet = std::make_shared<GameObject>();


	bullet->GetComponent<TransformComponent>()->SetLocalPosition(ownPos.x, ownPos.y);
	bullet->AddComponent<ImageComponent>()->SetTexture("Bullet.png");
	bullet->AddComponent<BulletMovementComponent>();
	bullet->AddComponent<dae::ImageRenderComponent>();



	auto collision = bullet->AddComponent<dae::CollisionComponent>();
	collision->SetCollisionData({ "PlayerAttack", bullet.get() });

	float collisionWidht{ 11 }, collisionHeight{ 10 };
	collision->SetBounds(collisionWidht, collisionHeight);


	auto boundHitCallback = std::bind(&SchootComponent::BulletHitCallback, this, std::placeholders::_1, std::placeholders::_2);
	collision->SetCallback(boundHitCallback);

	collision->SetScene(m_pScene);
	m_pScene->Add(bullet);
	m_pScene->AddCollision(collision);
	m_FiredBullets.push_back(bullet);


	// Play shooting sound
	m_pSoundsystem->NotifySound(SoundData{ 1, 0.1f, SoundData::SoundType::SoundEffect });
}

void dae::SchootComponent::Update()
{
	if (m_HasDestroyedBullet)
	{
		m_FiredBullets.erase(
			std::remove_if(
				std::begin(m_FiredBullets),
				std::end(m_FiredBullets),
				[&](const auto& bullet)
				{ return bullet->IsReadyForDestruction(); }),
			std::end(m_FiredBullets));

		m_HasDestroyedBullet = false;
	}


	for (auto& bullet : m_FiredBullets)
	{
		if (bullet->IsReadyForDestruction())
		{
			m_HasDestroyedBullet = true;
		}
	}
}

void dae::SchootComponent::SetScene(Scene* scene)
{
	m_pScene = scene;
}

void dae::SchootComponent::BulletHitCallback(const dae::CollisionData& collisionOwner, const dae::CollisionData& hitObject)
{
	if (!(strcmp(hitObject.ownerType.c_str(), "Enemy") == 0))
		return;

	collisionOwner.owningObject->MarkForDestruction();
}
