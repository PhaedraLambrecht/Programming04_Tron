#pragma once
#include <utility>
#include <map>
#include <vector>
#include <memory>
#include <stdexcept>

#include "Singleton.h"
#include "Controller.h"
#include "Commands/Command.h"
#include <functional>


namespace dae
{
	enum class ButtonState
	{
		Up,
		Down,
		Pressed
	};

	struct ControllerInput
	{
		unsigned int controllerIndex{0};
		ControlerButton controllerKey{};
		ButtonState buttonsState{};

	};
	// Define equality comparison for ControllerInput
	inline bool operator==(const ControllerInput& lhs, const ControllerInput& rhs)
	{
		// Compare all members of the structs for equality using std::tie
		return std::tie(lhs.controllerIndex, lhs.controllerKey, lhs.buttonsState) ==
			   std::tie(rhs.controllerIndex, rhs.controllerKey, rhs.buttonsState);
	}

	// Define comparison operator for ControllerInput
	inline bool operator<(const ControllerInput& lhs, const ControllerInput& rhs) {
		return std::tie(lhs.controllerIndex, lhs.controllerKey, lhs.buttonsState) <
			   std::tie(rhs.controllerIndex, rhs.controllerKey, rhs.buttonsState);
	}


		
	struct KeyboardInput
	{
		SDL_Scancode keyboardScancode{};
		ButtonState buttonsState{};
	};
	// Define equality comparison for KeyboardInput
	inline bool operator==(const KeyboardInput& lhs, const KeyboardInput& rhs)
	{
		// Compare all members of the structs for equality using std::tie
		return std::tie(lhs.keyboardScancode, lhs.buttonsState) ==
			   std::tie(rhs.keyboardScancode, rhs.buttonsState);
	}

	// Define comparison operator for KeyboardInput
	inline bool operator<(const KeyboardInput& lhs, const KeyboardInput& rhs) 
	{
		return std::tie(lhs.keyboardScancode, lhs.buttonsState) <
			   std::tie(rhs.keyboardScancode, rhs.buttonsState);
	}
}

namespace std
{
	// Specialization of hash function for dae::ControllerInput struct
	template<>
	struct hash<dae::ControllerInput>
	{
		std::size_t operator()(const dae::ControllerInput& input) const
		{
			std::size_t hashValue = 0;

			// Combine hash values of controllerIndex, controllerKey, and buttonsState
			hash_combine(hashValue, input.controllerIndex);
			hash_combine(hashValue, static_cast<int>(input.controllerKey));
			hash_combine(hashValue, static_cast<int>(input.buttonsState));
			
			
			return hashValue;
		}


	private:

		// Helper function to combine hash values using XOR and shift operations
		template<typename T>
		void hash_combine(std::size_t& hashValue, const T& value) const
		{
			hashValue ^= std::hash<T>{}(value)+0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
		}
	};


	// Specialization of hash function for dae::KeyboardInput struct
	template<>
	struct hash<dae::KeyboardInput>
	{
		std::size_t operator()(const dae::KeyboardInput& input) const
		{
				std::size_t hashValue = 0;
				// Combine hash values of keyboardScancode and buttonsState
				hash_combine(hashValue, input.keyboardScancode);
				hash_combine(hashValue, static_cast<int>(input.buttonsState));
				
				
				return hashValue;
		}


	private:
		// Helper function to combine hash values using XOR and shift operations
		template<typename T>
		void hash_combine(std::size_t& hashValue, const T& value) const 
		{
			hashValue ^= std::hash<T>{}(value)+0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
		}
	};
}

namespace dae
{

	class InputManager final : public Singleton<InputManager>
	{
	public:

		InputManager() = default;
		virtual ~InputManager() override;

		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;





		bool ProcessInput();
		unsigned int AddController();

		template<typename T> T* AddControllerCommand(std::unique_ptr<T> command, ControllerInput input);
		template<typename T> T* AddKeyboardCommand(std::unique_ptr<T> command, KeyboardInput input);


	private:
		

		//keep track of all buttons with commands 
		using ControllerCommandsMap = std::map<ControllerInput, std::unique_ptr<BaseCommand>>;
		using KeyboardCommandsMap = std::map<KeyboardInput, std::unique_ptr<BaseCommand>>;


		
		void HandleConrollerInputs();
		void HandleKeyboardInputs();

		bool ReadKeyBoardInput();

		//make collections of commands
		ControllerCommandsMap m_ControllerCommands{ };
		KeyboardCommandsMap m_KeyboardCommands{ };


		std::vector<Controller*> m_Controllers{};

		std::vector<bool> m_CurrentState{ std::vector<bool>(322, false) };
		std::vector<bool> m_PreviousState{ std::vector<bool>(322, false) };
	};


	template<typename T>
	inline T* InputManager::AddControllerCommand(std::unique_ptr<T> command, ControllerInput input)
	{
		static_assert(std::is_base_of<BaseCommand, T>::value && "T must inherit from BaseCommand");


		while (input.controllerIndex >= m_Controllers.size())
		{
			AddController();
		}

		// Get the raw pointer from command
		T* commandPtr = command.get(); 
		m_ControllerCommands.emplace( std::move(input), std::move(command) ); // Insert input and move command into the map
		
		
		return commandPtr;
	}

	template<typename T>
	inline T* InputManager::AddKeyboardCommand(std::unique_ptr<T> command, KeyboardInput input)
	{
		static_assert(std::is_base_of<BaseCommand, T>::value && "T must inherit from BaseCommand");

		// Get the raw pointer from command
		T* toReturn = command.get();
		m_KeyboardCommands.emplace( std::move(input) , std::move(command) );// Insert input and move command into the map

		
		return toReturn;
	}

}