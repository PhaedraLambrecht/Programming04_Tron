#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"

#include "GameObject.h"
#include "ImageComponent.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "ImageRenderComponent.h"
#include "TextRenderComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include <iostream>

dae::Minigin m_Engine("../Data/");


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	const auto font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25) };


	// Background
	{
		const auto backGroundImage = std::make_shared<dae::GameObject>();
		backGroundImage->AddComponent<dae::ImageComponent>();
		backGroundImage->AddComponent<dae::ImageRenderComponent>();

		// todo: Tests the remove (all others are tested in the code)
		backGroundImage->AddComponent<dae::TextComponent>();
		backGroundImage->RemoveComponent<dae::TextComponent>();


		backGroundImage->GetComponent<dae::ImageComponent>()->SetTexture("background.tga");

		scene.Add(backGroundImage);
	}


	// Title
	{
		const SDL_Color tittleColor{ 255, 255, 255 };
		std::string text = "Programming 4 Assignment - W01";


		const auto title = std::make_shared<dae::GameObject>();
		title->AddComponent<dae::TextComponent>();
		title->AddComponent<dae::TextRenderComponent>();

		title->GetComponent<dae::TextComponent>()->SetFont(font);
		title->GetComponent<dae::TextComponent>()->SetColor(tittleColor);
		title->GetComponent<dae::TextComponent>()->SetText(text);
		title->GetComponent<dae::TransformComponent>()->SetLocalPosition(120.0f, 30.0f);


		scene.Add(title);
	}


	// DAE Image
	{
		const auto daeImage = std::make_shared<dae::GameObject>();
		daeImage->AddComponent<dae::ImageComponent>();
		daeImage->AddComponent<dae::ImageRenderComponent>();
	

		daeImage->GetComponent<dae::ImageComponent>()->SetTexture("logo.tga");
		daeImage->GetComponent<dae::TransformComponent>()->SetLocalPosition(216.0f, 180.0f);

		scene.Add(daeImage);
	}


	// FPS
	{
		const SDL_Color fpsColor{ 255, 255, 0 };


		const auto fpsComponent = std::make_shared<dae::GameObject>();
		fpsComponent->AddComponent<dae::TextComponent>();
		fpsComponent->AddComponent<dae::TextRenderComponent>();
		fpsComponent->AddComponent<dae::FPSComponent>();


		fpsComponent->GetComponent<dae::TextComponent>()->SetFont(font);
		fpsComponent->GetComponent<dae::TextComponent>()->SetColor(fpsColor);
		fpsComponent->GetComponent<dae::TransformComponent>()->SetLocalPosition(0.0f, 10.0f);

		scene.Add(fpsComponent);
	}


	// TODO: The parent is a parent but the child says f you when it comes to being taken care of
	// Parent & child
	{
		// todo: This is the parent of the coming blue tank
		const auto deleteParent = std::make_shared<dae::GameObject>();


		// Parent
		const auto blueTank = std::make_shared<dae::GameObject>();
		blueTank->AddComponent<dae::RotatorComponent>();
		blueTank->AddComponent<dae::ImageComponent>();
		blueTank->AddComponent<dae::ImageRenderComponent>();


		blueTank->GetComponent<dae::TransformComponent>()->SetLocalPosition(350.0f, 180.0f);
		blueTank->GetComponent<dae::RotatorComponent>()->SetRotationpeed(10.0f);
		blueTank->GetComponent<dae::RotatorComponent>()->setRadius(5.0f);
		blueTank->GetComponent<dae::ImageComponent>()->SetTexture("BlueTank.png");

		// todo: This removes a set parent from the blue tank
		blueTank->SetParent(deleteParent, false);
		blueTank->SetParent(nullptr, false);

		scene.Add(blueTank);


		// Child
		auto redTank = std::make_shared<dae::GameObject>();
		redTank->AddComponent<dae::RotatorComponent>();
		redTank->AddComponent<dae::ImageComponent>();
		redTank->AddComponent<dae::ImageRenderComponent>();
		

		redTank->GetComponent<dae::RotatorComponent>()->SetRotationpeed(-15.0f);
		redTank->GetComponent<dae::RotatorComponent>()->setRadius(50.0f);
		redTank->GetComponent<dae::ImageComponent>()->SetTexture("RedTank.png");

		// todo: Testing the setparent
		redTank->SetParent(blueTank, false);

	
		scene.Add(redTank);
	}

}

int main(int, char*[]) 
{
	m_Engine.Run(load);
    return 0;
}
