#include "Attack.h"
#include "SearchAmmoAndHealth.h"
#include "SearchForCover.h"


Attack::Attack()
{
}

Attack::~Attack()
{
}

void Attack::Transform(NPC * ps)
{
	OnExit(ps);
	if (ps->getType() == SOLDIER)
	{
		if (ps->getHealth() <= START_HEALTH_POINTS / 3)
			ps->setCurrentState(new SearchAmmoAndHealth());
		else
			ps->setCurrentState(new SearchForCover());
	}
	ps->getCurrentState()->OnEnter(ps);
}

void Attack::OnEnter(NPC * ps)
{
	ps->setIsAttacking(true);
}

void Attack::OnExit(NPC * ps)
{
	ps->setIsAttacking(false);
}
