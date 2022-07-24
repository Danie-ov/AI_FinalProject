#include "SearchAmmoAndHealth.h"
#include "Attack.h"
#include "SearchForCover.h"

SearchAmmoAndHealth::SearchAmmoAndHealth()
{
}

SearchAmmoAndHealth::~SearchAmmoAndHealth()
{
}

void SearchAmmoAndHealth::Transform(NPC * ps)
{
	OnExit(ps);
	if (ps->getType() == SOLDIER)
	{
		if(ps->getHealth() > START_HEALTH_POINTS/3)
			ps->setCurrentState(new Attack());
	}
	else
	{
		ps->setCurrentState(new SearchForCover());
	}

	ps->getCurrentState()->OnEnter(ps);
}

void SearchAmmoAndHealth::OnEnter(NPC * ps)
{
	ps->setIsSearchingAmmoAndHealth(true);
}

void SearchAmmoAndHealth::OnExit(NPC * ps)
{
	ps->setIsSearchingAmmoAndHealth(false);
}
