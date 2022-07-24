#include "SearchForCover.h"
#include "SearchAmmoAndHealth.h"
#include "Attack.h"

SearchForCover::SearchForCover()
{
}

SearchForCover::~SearchForCover()
{
}

void SearchForCover::Transform(NPC * ps)
{
	OnExit(ps);
	if (ps->getType() == SOLDIER)
	{
		if (ps->getHealth() <= START_HEALTH_POINTS / 3)
			ps->setCurrentState(new SearchAmmoAndHealth());
		else
			ps->setCurrentState(new Attack());
	}
	else
	{
		ps->setCurrentState(new SearchAmmoAndHealth());
	}
	ps->getCurrentState()->OnEnter(ps);
}

void SearchForCover::OnEnter(NPC * ps)
{
	ps->setIsSearchingCover(true);
}

void SearchForCover::OnExit(NPC * ps)
{
	ps->setIsSearchingCover(false);
}
