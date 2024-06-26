#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "Scene/SceneManager.h"
#include "Resources/ResourceManager.h"
#include "Scene/Scene.h"

// FPS counter
#include "GameObject.h"
#include "Componennts/ImageComponent.h"
#include "Componennts/TextComponent.h"
#include "Componennts/TransformComponent.h"
#include "Componennts/ImageRenderComponent.h"
#include "Componennts/TextRenderComponent.h"
#include "Componennts/FPSComponent.h"
#include "Componennts/RotatorComponent.h"


// Input 
#include "Input/InputManager.h"
#include "Input/Controller.h"
#include "Commands/MoveCommand.h"

// Event queue
#include "Events/EventManager.h"
#include "Componennts/HealthComponent.h"
#include "Componennts/LivesComponent.h"
#include "Componennts/AddPointsComponnent.h"
#include "Commands/AddPointsCommand.h"
#include "Componennts/ScoreComponent.h"
#include "Commands/DamageCommand.h"

#include "Componennts/PlayManualComponent.h"

#include "Commands/SchootCommand.h"
#include "Componennts/SchootComponent.h"
#include "Componennts/CollisionComponent.h"
#include "Sound/SoundManager.h"
#include "Sound/SDLSoundSystem.h"

#include "RecognizerEnemy.h"
#include "EnemyController.h"
#include "BaseEnemyComponent.h"

#include <iostream>


//temp, look into changing this
constexpr unsigned int g_WindowWidth{ 640 }, WindowHeight{ 480 };
dae::Minigin m_Engine("../Data/", g_WindowWidth, WindowHeight);



void LoadLevel(dae::Scene& scene)
{
	const auto font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25) };

	// Background
	{
		const auto backGroundImage = std::make_shared<dae::GameObject>();
		backGroundImage->AddComponent<dae::ImageComponent>();
		backGroundImage->AddComponent<dae::ImageRenderComponent>();

		backGroundImage->GetComponent<dae::ImageComponent>()->SetTexture("background.tga");


		scene.Add(backGroundImage);
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
}



void LoadPlayerStats(dae::Scene& scene, const unsigned int playerIndex, const glm::vec2 position)
{
	const SDL_Color color{ 0, 255, 0 };
	const auto font{ dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 25) };


	// Player lives
	{
		const auto livesDisplay = std::make_shared<dae::GameObject>();
		livesDisplay->AddComponent<dae::TextComponent>();
		livesDisplay->AddComponent<dae::TextRenderComponent>();
		livesDisplay->GetComponent<dae::TextComponent>()->SetFont(font);
		livesDisplay->GetComponent<dae::TextComponent>()->SetColor(color);
		livesDisplay->AddComponent<dae::LivesComponent>();


		livesDisplay->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y);
		livesDisplay->GetComponent<dae::LivesComponent>()->SetPlayerIndex(playerIndex);

		scene.Add(livesDisplay);
	}

	//PLayer score
	{
		const auto scoreDisplay = std::make_shared<dae::GameObject>();
		scoreDisplay->AddComponent<dae::TextComponent>();
		scoreDisplay->AddComponent<dae::TextRenderComponent>();
		scoreDisplay->GetComponent<dae::TextComponent>()->SetFont(font);
		scoreDisplay->GetComponent<dae::TextComponent>()->SetColor(color);
		scoreDisplay->AddComponent<dae::ScoreComponent>();


		scoreDisplay->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x, position.y + 40.0f);
		scoreDisplay->GetComponent<dae::ScoreComponent>()->SetPlayerIndex(playerIndex);

		scene.Add(scoreDisplay);
	}

}

void LoadPlayer(dae::Scene& scene, dae::InputManager& input, const unsigned int playerIndex, const bool IsControllerUsed, const std::string texture, const glm::vec2 position)
{
	// Setting up some needed variables
	dae::MoveCommand* command{ nullptr };
	constexpr float originalSpeed{ 100.0f };
	constexpr glm::vec2 playerMoveDirectionx{ 1,0 };
	constexpr glm::vec2 playerMoveDirectiony{ 0,1 };




	const auto playerObject = std::make_shared<dae::GameObject>();
	playerObject->AddComponent<dae::ImageComponent>();
	playerObject->AddComponent<dae::ImageRenderComponent>();
	playerObject->AddComponent<dae::HealthComponent>();
	playerObject->AddComponent<dae::AddPointsComponnent>();
	playerObject->AddComponent<dae::RotatorComponent>();


	playerObject->GetComponent<dae::TransformComponent>()->SetLocalPosition(position.x + 30.0f, position.y);
	playerObject->GetComponent<dae::ImageComponent>()->SetTexture(texture);
	playerObject->GetComponent<dae::AddPointsComponnent>()->SetPlayerIndex(playerIndex);
	playerObject->GetComponent<dae::HealthComponent>()->SetPlayerIndex(playerIndex);

	auto playerAttack = playerObject->AddComponent<dae::SchootComponent>();
	playerAttack->SetScene(&scene);



	scene.Add(playerObject);



	// Controls
	if (IsControllerUsed)
	{
		unsigned controller = input.AddController();


		// Movement
		command = input.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(playerObject.get(), playerMoveDirectiony, -originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_UP, dae::ButtonState::Pressed }
		);

		command = input.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(playerObject.get(), playerMoveDirectiony, originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_DOWN,dae::ButtonState::Pressed }
		);

		command = input.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(playerObject.get(), playerMoveDirectionx, -originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_LEFT, dae::ButtonState::Pressed }
		);

		command = input.AddControllerCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(playerObject.get(), playerMoveDirectionx, originalSpeed),
			dae::ControllerInput{ controller, dae::ControlerButton::DPAD_RIGHT, dae::ButtonState::Pressed }
		);




		// points added
		input.AddControllerCommand<dae::AddPointsCommand>(
			std::make_unique<dae::AddPointsCommand>(playerObject.get()),
			dae::ControllerInput{ playerIndex, dae::ControlerButton::B, dae::ButtonState::Up });

		// Lifes taken
		input.AddControllerCommand<dae::DamageCommand>(
			std::make_unique<dae::DamageCommand>(playerObject.get(), 1.0f),
			dae::ControllerInput{ playerIndex, dae::ControlerButton::A, dae::ButtonState::Up });
	
		// Schooting
		input.AddControllerCommand<dae::SchootCommand>(
			std::make_unique<dae::SchootCommand>(playerObject.get()),
			dae::ControllerInput{ playerIndex, dae::ControlerButton::X, dae::ButtonState::Up });
	}
	else
	{
		// Movement
		command = input.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(playerObject.get(), playerMoveDirectiony, -originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_W, dae::ButtonState::Pressed }
		);

		command = input.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(playerObject.get(), playerMoveDirectiony, originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_S, dae::ButtonState::Pressed }
		);


		command = input.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(playerObject.get(), playerMoveDirectionx, -originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_A, dae::ButtonState::Pressed }
		);

		command = input.AddKeyboardCommand<dae::MoveCommand>(
			std::make_unique<dae::MoveCommand>(playerObject.get(), playerMoveDirectionx, originalSpeed),
			dae::KeyboardInput{ SDL_SCANCODE_D, dae::ButtonState::Pressed }
		);




		// points added
		input.AddKeyboardCommand<dae::AddPointsCommand>(
			std::make_unique<dae::AddPointsCommand>(playerObject.get()),
			dae::KeyboardInput{ SDL_SCANCODE_Q , dae::ButtonState::Up });

		// Lifes taken
		input.AddKeyboardCommand<dae::DamageCommand>(
			std::make_unique<dae::DamageCommand>(playerObject.get(), 1.0f),
			dae::KeyboardInput{ SDL_SCANCODE_E, dae::ButtonState::Up });

		// Schooting
		input.AddKeyboardCommand<dae::SchootCommand>(
			std::make_unique<dae::SchootCommand>(playerObject.get()),
			dae::KeyboardInput{ SDL_SCANCODE_X, dae::ButtonState::Up });

	}




	// ---- Printing player stats -----
	LoadPlayerStats(scene, playerIndex, { position.x - 190.0f, position.y });




	if (!IsControllerUsed)
	{
		//Enemy
		enum class EnemyTypes
		{
			Recognizer,
			Tank,
		};
		EnemyTypes enemyType = EnemyTypes::Recognizer;



		auto enemy = std::make_shared<dae::GameObject>();
		enemy->GetComponent<dae::TransformComponent>()->SetLocalPosition(300.0f, 50.0f);
		enemy->AddComponent<dae::ImageComponent>();
		enemy->AddComponent<dae::ImageRenderComponent>();
		enemy->AddComponent<dae::HealthComponent>();


		switch (enemyType)
		{
		case EnemyTypes::Recognizer:
		{
			enemy->GetComponent<dae::ImageComponent>()->SetTexture("EnemyTest.png");
			enemy->AddComponent<dae::RecognizerEnemy>();
			enemy->GetComponent<dae::RecognizerEnemy>()->Initialize(enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().x, enemy->GetComponent<dae::TransformComponent>()->GetWorldPosition().y, 10, 10, playerObject);
		}
		break;
		case EnemyTypes::Tank:

			enemy->GetComponent<dae::ImageComponent>()->SetTexture("EnemyTest.png");
			break;

		}


		auto attacking = enemy->GetComponent<dae::BaseEnemyComponent>();
		enemy->AddComponent<dae::EnemyController>();


		auto playerImage = enemy->GetComponent<dae::ImageComponent>();

		const auto imageDimensions = playerImage->GetTextureDimensions();

		auto collision = enemy->AddComponent<dae::CollisionComponent>();
		scene.AddCollision(collision);
		collision->SetCollisionData({ "Enemy", enemy.get() });

		float collisionWidth{ imageDimensions.x }, collisionHeight{ imageDimensions.y };
		collision->SetBounds(collisionWidth, collisionHeight);

		auto boundHitCallback = std::bind(&dae::BaseEnemyComponent::OnHitCallback, attacking, std::placeholders::_1, std::placeholders::_2);
		collision->SetCallback(boundHitCallback);

		collision->SetScene(&scene);

		scene.Add(enemy);
	}

}

void loadSound(dae::SoundSystem* sounds)
{
	sounds->NotifySound(dae::SoundData{ 0, 0,  dae::SoundData::SoundType::Music, "Sound/Background.mp3", true });
	sounds->NotifySound(dae::SoundData{ 1, 0,  dae::SoundData::SoundType::SoundEffect, "Sound/Shoot.mp3", true });
}

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	auto& inputManager = dae::InputManager::GetInstance();
	auto sounds = dae::SoundManager::GetInstance().GetSoundSystem();

	LoadLevel(scene);
	loadSound(sounds);

	// Load the players (and their stats)
	LoadPlayer(scene, inputManager, 0, true, "BlueTank.png", { 200.0f, 200.0f }); // PLayer 0 - Blue
	LoadPlayer(scene, inputManager, 1, false, "RedTank.png", { 200.0f, 300.0f }); // PLayer 1 - Red


	// Sound notifications
	sounds->NotifySound(dae::SoundData{ 0, 0.1f, dae::SoundData::SoundType::Music });

	for (size_t i = 0; i < 5; i++)
	{
		sounds->NotifySound(dae::SoundData{ 1, 0.5 });
	}
	// ---- Play manual -----
	const auto playManual = std::make_shared<dae::GameObject>();
	playManual->AddComponent<dae::PlayManualComponent>();


	// Keyboard
	{
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("WASD", "Move");

		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("E", "Lose a life");
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("Q", "Gain 100 points");

		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("X", "schoot a bullet - Do not do this with both keyboard and controller\n");

	}
	// Controller
	{
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("DPAD", "Move");

		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("A_BUTTON", "Lose a life");
		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("B_BUTTON", "Gain 100 points");

		playManual->GetComponent<dae::PlayManualComponent>()->AddExplanation("X", "schoot a bullet - Do not do this with both keyboard and controller\n");
	}

	scene.Add(playManual);

}

int main(int, char*[]) 
{
	m_Engine.Run(load);
    return 0;
}
