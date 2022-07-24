#pragma once
#include "State.h"
class SearchAmmoAndHealth :
	public State
{
public:
	SearchAmmoAndHealth();
	~SearchAmmoAndHealth();

	void Transform(NPC* ps);
	void OnEnter(NPC* ps);
	void OnExit(NPC* ps);
};

