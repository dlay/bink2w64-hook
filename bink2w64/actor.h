#pragma once
#include "utils.h"
#include <iostream>
#include <map>

namespace Game
{
	struct SrvEvInst;
	struct SrvEv;
	typedef std::map<size_t, SrvEv> ServerEvents;
	typedef std::map<size_t, SrvEvInst> ServerEventInstances;
	struct SrvEvInst
	{
		const char* name;
		int length;
		int colorCode;
	};

	struct SrvEv
	{
		const char* name;
		const char* id;
		int length;
		bool display = true;
		ServerEventInstances entries;
	};

	class Actor;
	typedef std::map<QWORD, Actor>ActorMap;

	enum class actorRace
	{
		NPC,
		PLAYER,
		TBD1, TBD2, TBD3, TBD4, TBD5, TBD6, TBD7,
		TBD8, TBD9, TBD10, TBD11, TBD12, TBD13
	};

	enum class actorClass
	{
		SPEARMASTER,
		REAPER,
		BARD,
		SWORDMAGE,
		SUMMONER,
		BERSERKER
	};

	enum class mapIds
	{
		CLOUDRISE,
		HEAVENLY_RIDGE,
		ZHONGNAN_FOOTHILLS,
		DISTRICT_OF_CHANGAN,
		WEICHUAN_HIGHLANDS,
		HUAIXIU_VILLAGE,
		BAXIAN_PLATEAU,
		SHANGHUAI_GRASSLAND,
		DISTRICT_OF_JIANGDU,
		VALLEY_OF_ECHOING_BLADES
	};
	extern mapIds currentMapId;
	extern ServerEvents serverEvents;

	class Actor
	{
	public:
		static ActorMap actorMap;

		static void addActor(QWORD id, Actor newActor)
		{
			actorMap[id] = newActor;
		}
		static Actor& getActor(QWORD id)
		{
			return actorMap[id];
		}
		static void resetActorList()
		{
			actorMap.clear();
		}
		

		vec3 pos;
		int level;
		actorClass playerClass;
		actorRace race;
		int debugVal1 = 0;
		int debugVal2 = 0;
		int debugVal3 = 0;
		int debugVal4 = 0;
		int debugVal5 = 0;

		float getDist(vec3 fromPos);

		Actor();
		Actor(vec3 pos, int level, int clss, int race);
		~Actor();
	};
}
