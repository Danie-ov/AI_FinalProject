#pragma once
#include "State.h"
class SuplyAmmoAndHealth :
	public State
{
public:
	SuplyAmmoAndHealth();
	~SuplyAmmoAndHealth();

	void Transform(NPC* ps);
	void OnEnter(NPC* ps);
	void OnExit(NPC* ps);
};

