#include "FPSComponent.h"
#include "TextComponent.h"
#include "Time.h"
#include "GameObject.h"
#include <stdexcept>
#include <iostream>


namespace dae
{
	FPSComponent::FPSComponent(GameObject* Owner)
		:BaseComponent(Owner)
		,m_fps{}
	{
		if (GetOwner()->HasComponent<TextComponent>())
		{
			m_pText = GetOwner()->GetComponent<TextComponent>();
		}
		else
		{
			m_pText = GetOwner()->GetComponent<TextComponent>();
		}
	}

	FPSComponent::~FPSComponent()
	{
		std::cout << "FPSComponent\n";
	}


	void FPSComponent::Update()
	{
		if (!m_pText)
		{
			throw std::invalid_argument("FPSComponent needs a TextComponent");
		}

		m_fps =  1.0f / Time::GetInstance().GetDeltaTime();

		std::string fpsText = std::to_string((int)m_fps) + "FPS"; // todo: Look into what caused the issue that this can't be in the line below.
		m_pText->SetText(fpsText); 
		
	}
}