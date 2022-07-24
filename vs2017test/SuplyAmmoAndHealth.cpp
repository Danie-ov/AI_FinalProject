#include "SuplyAmmoAndHealth.h"
#include "SearchAmmoAndHealth.h"
#include "SearchForCover.h"

SuplyAmmoAndHealth::SuplyAmmoAndHealth()
{
}

SuplyAmmoAndHealth::~SuplyAmmoAndHealth()
{
}

void SuplyAmmoAndHealth::Transform(NPC * ps)
{
	OnExit(ps);
	if(ps->getType() == CARRIER)
	{ 
		if (ps->getHealth() <= BASKET_HEALTH_POINTS / 3 || ps->getBulletsStock() <= BASKET_BULLETS/3
			|| ps->getGrenadeStock() <= BASKET_GRENADES/3)
			ps->setCurrentState(new SearchAmmoAndHealth());
		else
			ps->setCurrentState(new SearchForCover());
	}
	ps->getCurrentState()->OnEnter(ps);
}

void SuplyAmmoAndHealth::OnEnter(NPC * ps)
{
	ps->setIsSuplying(true);
}

void SuplyAmmoAndHealth::OnExit(NPC * ps)
{
	ps->setIsSuplying(false);
}
