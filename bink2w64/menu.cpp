#include "menu.h"

bool showNPCMarker = false;
bool showPlayerMarker = false;
bool showTBD1Marker = false;
bool showTBD2Marker = false;
bool showTBD3Marker = false;
bool showTBD4Marker = false;
bool showTBD5Marker = false;
bool showTBD6Marker = false;
bool showTBD7Marker = true;
bool showTBD8Marker = false;
bool showTBD9Marker = false;
bool showTBD10Marker = false;
bool showTBD11Marker = false;
bool showTBD12Marker = false;
bool showTBD13Marker = false;
bool showGatherMarker = true;

void drawMarkerMenu()
{
	ImGuiColorEditFlags flags = ImGuiColorEditFlags_NoAlpha;
	flags |= ImGuiColorEditFlags_NoTooltip;
	float offset = 0.f;
	float spacing = 20.f;

	ImGui::Begin("Marker Menu");

	vec4 color = getDistinctColor(0, false);
	ImGui::Checkbox("Player", &showNPCMarker);
	ImGui::SameLine();
	ImGui::ColorButton("Player", ImVec4(color.x, color.y, color.z, color.w), flags);
	ImGui::SameLine(offset, spacing);

	color = getDistinctColor(1, false);
	ImGui::Checkbox("NPC", &showPlayerMarker);
	ImGui::SameLine();
	ImGui::ColorButton("NPC", ImVec4(color.x, color.y, color.z, color.w), flags);
	ImGui::SameLine(offset, spacing);

	color = getDistinctColor(2, false);
	ImGui::Checkbox("TBD1", &showTBD1Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD1", ImVec4(color.x, color.y, color.z, color.w), flags);

	color = getDistinctColor(3, false);
	ImGui::Checkbox("TBD2", &showTBD2Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD2", ImVec4(color.x, color.y, color.z, color.w), flags);
	ImGui::SameLine(offset, spacing);

	color = getDistinctColor(4, false);
	ImGui::Checkbox("TBD3", &showTBD3Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD3", ImVec4(color.x, color.y, color.z, color.w), flags);
	ImGui::SameLine(offset, spacing);

	color = getDistinctColor(5, false);
	ImGui::Checkbox("TBD4", &showTBD4Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD4", ImVec4(color.x, color.y, color.z, color.w), flags);

	color = getDistinctColor(6, false);
	ImGui::Checkbox("TBD5", &showTBD5Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD5", ImVec4(color.x, color.y, color.z, color.w), flags);
	ImGui::SameLine(offset, spacing);

	color = getDistinctColor(7, false);
	ImGui::Checkbox("TBD6", &showTBD6Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD6", ImVec4(color.x, color.y, color.z, color.w), flags);
	ImGui::SameLine(offset, spacing);

	color = getDistinctColor(8, false);
	ImGui::Checkbox("Fish", &showTBD7Marker);
	ImGui::SameLine();
	ImGui::ColorButton("Fish", ImVec4(color.x, color.y, color.z, color.w), flags);

	color = getDistinctColor(9, false);
	ImGui::Checkbox("TBD8", &showTBD8Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD8", ImVec4(color.x, color.y, color.z, color.w), flags);
	ImGui::SameLine(offset, spacing);

	color = getDistinctColor(10, false);
	ImGui::Checkbox("TBD9", &showTBD9Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD9", ImVec4(color.x, color.y, color.z, color.w), flags);
	ImGui::SameLine(offset, spacing);

	color = getDistinctColor(11, false);
	ImGui::Checkbox("TBD10", &showTBD10Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD10", ImVec4(color.x, color.y, color.z, color.w), flags);

	color = getDistinctColor(12, false);
	ImGui::Checkbox("TBD11", &showTBD11Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD11", ImVec4(color.x, color.y, color.z, color.w), flags);
	ImGui::SameLine(offset, spacing);

	color = getDistinctColor(13, false);
	ImGui::Checkbox("TBD12", &showTBD12Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD12", ImVec4(color.x, color.y, color.z, color.w), flags);
	ImGui::SameLine(offset, spacing);

	color = getDistinctColor(14, false);
	ImGui::Checkbox("TBD13", &showTBD13Marker);
	ImGui::SameLine();
	ImGui::ColorButton("TBD13", ImVec4(color.x, color.y, color.z, color.w), flags);

	color = getDistinctColor(15, false);
	ImGui::Checkbox("Gather", &showGatherMarker);
	ImGui::SameLine();
	ImGui::ColorButton("Gather", ImVec4(color.x, color.y, color.z, color.w), flags);

	ImGui::End();
}

void drawRecordingMenu()
{
	ImGui::Begin("Recording Map Marker");
	const char* items[] = { "Cloudrise", "Heavenly Ridge", "Zhongnan Foothills", "District of Changan", "Weichuan Highlands", "Huaixiu Village", "Baxian Plateau", "Shanghuai Grassland", "District of Jiangdu", "Valley of Echoing Blades" };
	static const char* current_item = NULL;
	ImGuiComboFlags flags = ImGuiComboFlags_NoPreview;
	if (ImGui::BeginCombo("Current Map", current_item))
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(items[n], is_selected))
			{
				current_item = items[n];
				Game::currentMapId = static_cast<Game::mapIds>(n);
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
		}
		ImGui::EndCombo();
	}

	if (!state::recordingMarker)
		if (ImGui::Button("Start Recording"))
			startRecordingMarker();
	if (state::recordingMarker)
		if (ImGui::Button("Stop Recording"))
			endRecordingMarker();
	ImGui::End();
}

void drawEventTimer()
{
	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoScrollbar;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoNav;
	windowFlags |= ImGuiWindowFlags_NoBackground;
	windowFlags |= ImGuiWindowFlags_NoMouseInputs;
	// windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	bool* closable = NULL;

	if (state::showMenu)
		ImGui::Begin("Event Timer");
	else
		ImGui::Begin("Event Timer", closable, windowFlags);

	std::time_t nowUnix = std::time(0);
	nowUnix -= (4 * 3600); // TODO: ingame daily reset on UTC-4 midnight, maybe wrong?
	std::tm nowUTC;
	gmtime_s(&nowUTC, &nowUnix);
	int minOfWeek = (nowUTC.tm_wday * 24 + nowUTC.tm_hour) * 60 + nowUTC.tm_min;

	for (size_t i = 0; i < Game::serverEvents.size(); i++)
	{
		int min = Game::serverEvents[i].length;
		size_t j = Game::serverEvents[i].entries.size() - 1;
		while (min > minOfWeek)
		{
			min -= Game::serverEvents[i].entries[j].length;
			j--;
		}
		minOfWeek -= min;
		int restMin = Game::serverEvents[i].entries[j].length - minOfWeek - 1;
		min = 0;
		if (Game::serverEvents[i].entries[j].length <= 0) return;
		float eventProgress = (float)minOfWeek / Game::serverEvents[i].entries[j].length;
		std::tm timeTm;
		timeTm.tm_hour = restMin / 60;
		timeTm.tm_min = (restMin % 60);
		timeTm.tm_sec = 60 - nowUTC.tm_sec;
		char timeString[9];
		strftime(timeString, sizeof(timeString), "%H:%M:%S", &timeTm);
		ImGui::ProgressBar(eventProgress, ImVec2(0.0f, 0.0f), timeString);
		ImGui::SameLine();
		ImGui::Text(Game::serverEvents[i].name);
	}
	ImGui::End();
}

bool canShowMarker(Game::Actor actor)
{
	if (state::renderDebug)
	{
		// fprintf(hf_out, "actor race: %i, level: %i, class: %i, dbg1: %i, dbg2: %i, dbg3: %i, dbg4: %i, dbg5: %i\n", actor.race, actor.level, actor.playerClass, actor.debugVal1, actor.debugVal2, actor.debugVal3, actor.debugVal4, actor.debugVal5);
	}
	switch (actor.race)
	{
	case(Game::actorRace::NPC):
		if ((int)actor.playerClass == -1 && (int)actor.level == 1) {
			return showGatherMarker;
		}
		return showNPCMarker;
	case(Game::actorRace::PLAYER):
		return showPlayerMarker;
	case(Game::actorRace::TBD1):
		return showTBD1Marker;
	case(Game::actorRace::TBD2):
		return showTBD2Marker;
	case(Game::actorRace::TBD3):
		return showTBD3Marker;
	case(Game::actorRace::TBD4):
		if (state::renderDebug)
		{
			fprintf(hf_out, "actor race: %i, level: %i, class: %i, dbg1: %i, dbg2: %i, dbg3: %i, dbg4: %i, dbg5: %i\n", actor.race, actor.level, actor.playerClass, actor.debugVal1, actor.debugVal2, actor.debugVal3, actor.debugVal4, actor.debugVal5);
		}
		return showTBD4Marker;
	case(Game::actorRace::TBD5):
		return showTBD5Marker;
	case(Game::actorRace::TBD6):
		return showTBD6Marker;
	case(Game::actorRace::TBD7):
		return showTBD7Marker;
	case(Game::actorRace::TBD8):
		return showTBD8Marker;
	case(Game::actorRace::TBD9):
		return showTBD9Marker;
	case(Game::actorRace::TBD10):
		return showTBD10Marker;
	case(Game::actorRace::TBD11):
		return showTBD11Marker;
	case(Game::actorRace::TBD12):
		return showTBD12Marker;
	case(Game::actorRace::TBD13):
		return showTBD13Marker;
	default:
		break;
	}
	return false;
}

vec4 getDistinctColor(int id, bool standardRep)
{
	switch (id)
	{
	case(0): // teal
		if (standardRep)
			return { 0, 128, 128, 255 };
		else
			return { 0 / 255.f, 128 / 255.f, 128 / 255.f, 1.f };
	case(1): // white
		if (standardRep)
			return { 255, 255, 255, 255 };
		else
			return { 1.f, 1.f, 1.f, 1.f };
	case(2): // red
		if (standardRep)
			return { 230, 25, 75, 255 };
		else
			return { 230/255.f, 25 / 255.f, 75 / 255.f, 1.f };
	case(3): // green
		if (standardRep)
			return { 60, 180, 75, 255 };
		else
			return { 60 / 255.f, 180 / 255.f, 75 / 255.f, 1.f };
	case(4): // yellow
		if (standardRep)
			return { 255, 225, 25, 255 };
		else
			return { 255 / 255.f, 225 / 255.f, 25 / 255.f, 1.f };
	case(5): // blue
		if (standardRep)
			return { 0, 130, 200, 255 };
		else
			return { 0 / 255.f, 130 / 255.f, 200 / 255.f, 1.f };
	case(6): // orange
		if (standardRep)
			return { 245, 130, 48, 255 };
		else
			return { 245 / 255.f, 130 / 255.f, 48 / 255.f, 1.f };
	case(7): // cyan
		if (standardRep)
			return { 70, 240, 240, 255 };
		else
			return { 70 / 255.f, 240 / 255.f, 240 / 255.f, 1.f };
	case(8): // magenta
		if (standardRep)
			return { 240, 50, 230, 255 };
		else
			return { 240 / 255.f, 50 / 255.f, 230 / 255.f, 1.f };
	case(9): // pink
		if (standardRep)
			return { 250, 190, 212, 255 };
		else
			return { 250 / 255.f, 190 / 255.f, 212 / 255.f, 1.f };
	case(10): // black
		if (standardRep)
			return { 0, 0, 0, 255 };
		else
			return { 0.f, 0.f, 0.f, 1.f };
	case(11): // lavender
		if (standardRep)
			return { 220, 190, 255, 255 };
		else
			return { 220 / 255.f, 190 / 255.f, 255 / 255.f, 1.f };
	case(12): // brown
		if (standardRep)
			return { 170, 110, 40, 255 };
		else
			return { 170 / 255.f, 110 / 255.f, 40 / 255.f, 1.f };
	case(13): // beige
		if (standardRep)
			return { 255, 250, 200, 255 };
		else
			return { 255 / 255.f, 250 / 255.f, 200 / 255.f, 1.f };
	case(14): // maroon
		if (standardRep)
			return { 128, 0, 0, 255 };
		else
			return { 128 / 255.f, 0 / 255.f, 0 / 255.f, 1.f };
	case(15): // mint
		if (standardRep)
			return { 170, 255, 195, 255 };
		else
			return { 170 / 255.f, 255 / 255.f, 195 / 255.f, 1.f };
	case(16): // navy
		if (standardRep)
			return { 0, 0, 128, 255 };
		else
			return { 0 / 255.f, 0 / 255.f, 128 / 255.f, 1.f };
	case(17): // purple
		if (standardRep)
			return { 145, 30, 180, 255 };
		else
			return { 145 / 255.f, 30 / 255.f, 180 / 255.f, 1.f };
	case(18): // lime
		if (standardRep)
			return { 210, 245, 60, 255 };
		else
			return { 210 / 255.f, 245 / 255.f, 60 / 255.f, 1.f };
	case(19): // olive
		if (standardRep)
			return { 128, 128, 0, 255 };
		else
			return { 128 / 255.f, 128 / 255.f, 0 / 255.f, 1.f };
	default: // grey
		if (standardRep)
			return { 128, 128, 128, 255 };
		else
			return { 128 / 255.f, 128 / 255.f, 128 / 255.f, 1.f };
	}
}

/* GameEvent::GameEvent(const char* pName)
{
	name = pName;
	timeframes;
}*/
