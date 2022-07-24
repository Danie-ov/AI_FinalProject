#include "SearchForEnemy.h"
#include "Attack.h"

SearchForEnemy::SearchForEnemy()
{
}

SearchForEnemy::~SearchForEnemy()
{
}

void SearchForEnemy::Transform(NPC * ps)
{
	OnExit(ps);
	if(ps->getType() == SOLDIER)
		ps->setCurrentState(new Attack());

	ps->getCurrentState()->OnEnter(ps);
}

void SearchForEnemy::OnEnter(NPC * ps)
{
	ps->setIsSearchingEnemy(true);
}

void SearchForEnemy::OnExit(NPC * ps)
{
	ps->setIsSearchingEnemy(false);
}
