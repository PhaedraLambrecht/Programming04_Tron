#pragma once
#include "BaseComponent.h"
#include "Scene/Scene.h"
#include "glm/glm.hpp"
#include "CollisionComponent.h"
#include <vector>


namespace dae
{
	class SoundSystem;
	class Texture2D;
	class Scene;
	class TransformComponent;

	class SchootComponent final : public BaseComponent
	{
	public:

		SchootComponent(GameObject* Owner);
		~SchootComponent() override;

		// Copy and move
		SchootComponent(const SchootComponent& other) = delete;
		SchootComponent(SchootComponent&& other) = delete;
		SchootComponent& operator=(const SchootComponent& other) = delete;
		SchootComponent& operator=(SchootComponent&& other) = delete;


		void Attack();
		void Update() override;

		void SetScene(Scene* scene);


	private:

		SoundSystem* m_pSoundsystem;

		std::vector<std::shared_ptr<GameObject>> m_FiredBullets{};
		std::shared_ptr<Texture2D> m_AttackTexture{};

		std::string m_SceneName{};

		glm::vec2 m_BulletStartOffset{};
		TransformComponent* m_pPlayerTransform{};

		Scene* m_pScene{};
		bool m_HasDestroyedBullet{ false };


		void BulletHitCallback(const dae::CollisionData& collisionOwner, const dae::CollisionData& hitObject);
	};
}

