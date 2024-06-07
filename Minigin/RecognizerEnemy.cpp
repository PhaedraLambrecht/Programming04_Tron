#include "RecognizerEnemy.h"
#include "GameObject.h"
#include "GameTime.h"
#include "Events/EventManager.h"
#include "Commands/AddPointsCommand.h"
#include <iostream>


dae::RecognizerEnemy::RecognizerEnemy(GameObject* gameObject)
	: BaseEnemyComponent(gameObject)
{
}

dae::RecognizerEnemy::~RecognizerEnemy()
{
	std::cout << "RecognizerEnemy \n";
}


void dae::RecognizerEnemy::Initialize(float x, float y, float w, float h, std::shared_ptr<GameObject> playerTank)
{
	position = glm::vec2(x, y);
	m_Width = w;
	m_Height = h;
	m_PlayerTank = playerTank;
}

void dae::RecognizerEnemy::Update()
{
	float playerPosX = m_PlayerTank->GetComponent<TransformComponent>()->GetWorldPosition().x;
	float playerPosY = m_PlayerTank->GetComponent<TransformComponent>()->GetWorldPosition().y;

	float valueDiffX{ 0.f };
	float valueDiffY{ 0.f };
	float diffX = playerPosX - GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x;
	if (diffX < 0) { valueDiffX = diffX * -1; }
	else { valueDiffX = diffX; }
	float diffY = playerPosY - GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y;
	if (diffY < 0) { valueDiffY = diffY * -1; }
	else { valueDiffY = diffY; }
	float deltaTime = GameTime::GetInstance().GetDeltaTime();


	if (valueDiffX > valueDiffY && diffX < 0 && (!m_BlockMoveLeft))
	{
		GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
	}
	else if (valueDiffX > valueDiffY && diffX > 0 && (!m_BlockMoveRight))
	{
		GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
	}
	else if (valueDiffX < valueDiffY && diffY > 0 && (!m_BlockMoveUp))
	{
		GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
	}
	else if (valueDiffX < valueDiffY && diffY < 0 && (!m_BlockMoveDown))
	{
		GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
	}
	else
	{
		if (m_BlockMoveLeft && (!m_BlockMoveUp))
		{
			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}
		else if (m_BlockMoveLeft && m_BlockMoveUp && (!m_BlockMoveDown))
		{
			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}
		else if (m_BlockMoveLeft && (!m_BlockMoveRight))
		{
			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}
		else if (m_BlockMoveRight && (!m_BlockMoveUp))
		{
			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}
		else if (m_BlockMoveRight && m_BlockMoveUp && (!m_BlockMoveDown))
		{
			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}
		else if (m_BlockMoveRight && (!m_BlockMoveLeft))
		{
			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}

		else if (m_BlockMoveUp && (!m_BlockMoveLeft))
		{
			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}
		else if (m_BlockMoveUp && m_BlockMoveLeft && (!m_BlockMoveRight))
		{
			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}
		else if (m_BlockMoveUp && (!m_BlockMoveDown))
		{

			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}

		else if (m_BlockMoveDown && (!m_BlockMoveLeft))
		{
			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}
		else if (m_BlockMoveDown && m_BlockMoveLeft && (!m_BlockMoveRight))
		{
			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}
		else if (m_BlockMoveDown && (!m_BlockMoveUp))
		{
			GetOwner()->GetComponent<TransformComponent>()->SetLocalPosition(GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x - (deltaTime * m_EnemySpeed), GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y);
		}


	}


	m_BlockMoveRight = false;
	m_BlockMoveLeft = false;
	m_BlockMoveUp = false;
	m_BlockMoveDown = false;
}

bool dae::RecognizerEnemy::DoDamage()
{
	--m_Health;
	if (m_Health <= 0)
	{
		dae::AddPointsCommand addPointsCommand(GetOwner());
		addPointsCommand.Execute();
	
		Event event;
		event.eventType = "EnemyDeath";

		GetOwner()->MarkForDestruction();

		return true;

	}
	return false;
}



void dae::RecognizerEnemy::OnHitCallback(const CollisionData& collisionOwner, const CollisionData& hitObject)
{
	if (hitObject.ownerType != "PlayerAttack")
		return;

	DoDamage();


	if (!((GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x + collisionOwner.owningObject->GetComponent<CollisionComponent>()->GetBounds().x - 2) > GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x))
	{
		m_BlockMoveLeft = true;
	}
	if (!((GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x + collisionOwner.owningObject->GetComponent<CollisionComponent>()->GetBounds().x) > (GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().x + 2)))
	{
		m_BlockMoveRight = true;
	}


	if (!((GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y + 2) < (collisionOwner.owningObject->GetComponent<TransformComponent>()->GetWorldPosition().y + collisionOwner.owningObject->GetComponent<CollisionComponent>()->GetBounds().y)))
	{
		m_BlockMoveDown = true;
	}
	if (!((GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition().y) < (collisionOwner.owningObject->GetComponent<TransformComponent>()->GetWorldPosition().y + collisionOwner.owningObject->GetComponent<CollisionComponent>()->GetBounds().y - 2)))
	{
		m_BlockMoveUp = true;
	}
}

void dae::RecognizerEnemy::Attack()
{
	std::cout << "attack\n";
}

void dae::RecognizerEnemy::OnBDeath(const Event* e)
{
	if (strcmp(e->eventType, "EnemyDeath") != 0)
		return;
}