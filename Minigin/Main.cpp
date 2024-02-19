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


dae::Minigin m_Engine("../Data/");


void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	const auto font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25) };


	// Background
	{
		const auto backGroundImage = std::make_shared<dae::GameObject>();
		backGroundImage->AddComponent<dae::ImageRenderComponent>();

		backGroundImage->GetComponent<dae::ImageComponent>()->SetTexture("background.tga");


		scene.Add(backGroundImage);
	}


	// Title
	{
		const SDL_Color tittleColor{ 255, 255, 255 };
		std::string text = "Programming 4 Assignment - W01";


		const auto title = std::make_shared<dae::GameObject>();
		title->AddComponent<dae::TextRenderComponent>();

		title->GetComponent<dae::TextComponent>()->SetFont(font);
		title->GetComponent<dae::TextComponent>()->SetColor(tittleColor);
		title->GetComponent<dae::TextComponent>()->SetText(text);
		title->GetComponent<dae::TransformComponent>()->SetPosition(150.0f, 30.0f, 0.0f);


		scene.Add(title);
	}


	// DAE Image
	{
		const auto daeImage = std::make_shared<dae::GameObject>();
		daeImage->AddComponent<dae::ImageRenderComponent>();

		daeImage->GetComponent<dae::ImageComponent>()->SetTexture("logo.tga");
		daeImage->GetComponent<dae::TransformComponent>()->SetPosition(216.0f, 180.0f, 0.0f);


		scene.Add(daeImage);
	}


	// FPS
	{
		const SDL_Color fpsColor{ 255, 255, 0 };
		std::string text = "W01";


		const auto fpsComponent = std::make_shared<dae::GameObject>();
		fpsComponent->AddComponent<dae::TextRenderComponent>();
		fpsComponent->AddComponent<dae::FPSComponent>();


		fpsComponent->GetComponent<dae::TextComponent>()->SetFont(font);
		fpsComponent->GetComponent<dae::TextComponent>()->SetColor(fpsColor);
		fpsComponent->GetComponent<dae::TransformComponent>()->SetPosition(0.0f, 10.0f, 0.0f);


		scene.Add(fpsComponent);

	}
}

int main(int, char*[]) 
{
	m_Engine.Run(load);
    return 0;
}