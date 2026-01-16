#include "actor.h"
namespace Game
{
	mapIds currentMapId = mapIds::CLOUDRISE;
	ServerEvents serverEvents;
}
using namespace Game;

ActorMap Actor::actorMap;

float Actor::getDist(vec3 fromPos)
{
	return 0.0f;
}

Actor::Actor()
{
	level = 0;
	playerClass = actorClass::SPEARMASTER;
	race = actorRace::NPC;
}

Actor::Actor(vec3 pPos, int pLevel, int pClass, int pRace)
{
	pos = pPos;
	level = pLevel;
	race = actorRace::NPC;
	playerClass = actorClass::SPEARMASTER;
	if (pRace > 15 || pRace < 0)
	{
		// std::cout << "race out of bounds. " << pRace << std::endl;
	}
	else
	{
		race = static_cast<actorRace>(pRace);
	}
	if (pClass > 5 || pRace < 0)
	{
		// std::cout << "class out of bounds. " << pRace << std::endl;
	}
	else
	{
		playerClass = static_cast<actorClass>(pClass);
	}
}

Actor::~Actor()
{
}
