#pragma once
#include "State.h"
class SearchForEnemy :
	public State
{
public:
	SearchForEnemy();
	~SearchForEnemy();

	void Transform(NPC* ps);
	void OnEnter(NPC* ps);
	void OnExit(NPC* ps);
};

