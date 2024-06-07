#include <SDL.h>
#include <backends/imgui_impl_sdl2.h>
#include <iostream>
#include "InputManager.h"


dae::InputManager::~InputManager()
{
	std::cout << "InputManager" << '\n';


	for (int i{}; i < (int)m_Controllers.size(); ++i)
	{
		delete m_Controllers[i];
		m_Controllers[i] = nullptr;
	}
}


bool dae::InputManager::ProcessInput()
{
	if (!ReadKeyBoardInput()) return false;

	HandleConrollerInputs();

	HandleKeyboardInputs();


	return true;
}

bool dae::InputManager::ReadKeyBoardInput()
{
	m_PreviousState = m_CurrentState;

	SDL_Event e;
	while (SDL_PollEvent(&e))
	{

		if (e.type == SDL_QUIT)
		{
			return false;
		}
		if (e.type == SDL_KEYDOWN)
		{
			const auto pressedButton = SDL_GetScancodeFromKey(e.key.keysym.sym);

			m_CurrentState[pressedButton] = true;

		}
		if (e.type == SDL_KEYUP)
		{
			const auto releasedButton = SDL_GetScancodeFromKey(e.key.keysym.sym);

			m_CurrentState[releasedButton] = false;
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

void dae::InputManager::HandleConrollerInputs()
{
	for (auto& controller : m_Controllers)
	{
		controller->Update();
	}


	for (const auto& [input, command] : m_ControllerCommands)
	{
		const unsigned int index = input.controllerIndex;
		const ButtonState buttonState = input.buttonsState;
		const dae::ControlerButton button = input.controllerKey;

		switch (buttonState)
		{
		case ButtonState::Up:

			if (m_Controllers[index]->IsButtonUp( static_cast<unsigned int>(button) ))
				command->Execute();
			
			break;
		case ButtonState::Down:
			
			if (m_Controllers[index]->IsButtonDown( static_cast<unsigned int>(button) ))
				command->Execute();
			
			break;
		case ButtonState::Pressed:
			
			if (m_Controllers[index]->IsButtonPressed(static_cast<unsigned int>(button) ))
				command->Execute();
			
			break;
		}

	}
}

void dae::InputManager::HandleKeyboardInputs()
{
	for (const auto& [input, command] : m_KeyboardCommands)
	{
		const unsigned scancode = input.keyboardScancode;
		const ButtonState buttonState = input.buttonsState;

		switch (buttonState)
		{
		case ButtonState::Up:

			if (m_PreviousState[scancode] && !m_CurrentState[scancode])
				command->Execute();
			
			break;
		case ButtonState::Down:
			
			if (!m_PreviousState[scancode] && m_CurrentState[scancode])
				command->Execute();
			
			break;
		case ButtonState::Pressed:
			
			if (m_PreviousState[scancode] && m_CurrentState[scancode])
				command->Execute();
			
			break;
		}


	}
}


unsigned int dae::InputManager::AddController()
{
	unsigned int index = static_cast<unsigned int>(m_Controllers.size());
	m_Controllers.emplace_back( new Controller(index) );
	return index;
}