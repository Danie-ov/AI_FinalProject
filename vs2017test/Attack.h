#pragma once
#include "State.h"

class Attack :
	public State
{
public:
	Attack();
	~Attack();

	void Transform(NPC* ps);
	void OnEnter(NPC* ps);
	void OnExit(NPC* ps);
};

