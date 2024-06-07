#include "SchootCommand.h"
#include "GameObject.h"
#include "Componennts/SchootComponent.h"
#include <iostream>


dae::SchootCommand::SchootCommand(GameObject* actor)
	:BaseCommand()
{
	m_pSchootComponent = actor->GetComponent<dae::SchootComponent>();
}

dae::SchootCommand::~SchootCommand()
{
	std::cout << "SchootCommand\n";
}

void dae::SchootCommand::Execute()
{
	m_pSchootComponent->Attack();
}
