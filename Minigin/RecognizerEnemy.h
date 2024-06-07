#pragma once
#include "BaseEnemyComponent.h"

namespace dae
{
	class RecognizerEnemy final : public BaseEnemyComponent
	{
	public:

		explicit RecognizerEnemy(GameObject* gameObject);
		~RecognizerEnemy() override;

		RecognizerEnemy(const RecognizerEnemy& other) = delete;
		RecognizerEnemy(RecognizerEnemy&& other) = delete;
		RecognizerEnemy& operator=(const RecognizerEnemy& other) = delete;
		RecognizerEnemy& operator=(RecognizerEnemy&& other) = delete;


		void Initialize(float x, float y, float w, float h, std::shared_ptr<GameObject> playerTank) override;
		void Update() override;
		bool DoDamage() override;
		void OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject) override;
		void Attack() override;

		void OnBDeath(const Event* e);


	private:

		int m_Health{ 3 };
		float m_EnemySpeed{ 80.f };
		glm::vec2 position{};
		float m_Width{};
		float m_Height{};
		bool m_BlockMoveLeft{ false };
		bool m_BlockMoveRight{ false };
		bool m_BlockMoveUp{ false };
		bool m_BlockMoveDown{ false };
		std::shared_ptr<ImageComponent> m_enemyHorizontalSprite;
		std::shared_ptr<ImageComponent> m_enemyVerticalSprite;
		std::shared_ptr<GameObject> m_PlayerTank;
	};

}

