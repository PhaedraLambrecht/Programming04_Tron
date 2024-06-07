#pragma once
#include "Command.h"


namespace dae
{
	class SchootComponent;
	class GameObject;

	class SchootCommand final : public BaseCommand
	{
	public:

		SchootCommand(GameObject* actor);
		~SchootCommand() override;

		SchootCommand(const SchootCommand& other) = delete;
		SchootCommand(SchootCommand&& other) = delete;
		SchootCommand& operator=(const SchootCommand& other) = delete;
		SchootCommand& operator=(SchootCommand&& other) = delete;


		void Execute() override;

	private:

		SchootComponent* m_pSchootComponent{ nullptr };
	};
}

