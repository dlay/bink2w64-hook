#pragma once
#include "imgui/imgui.h"
#include "actor.h"

extern bool showNPCMarker;
extern bool showPlayerMarker;
extern bool showTBD1Marker;
extern bool showTBD2Marker;
extern bool showTBD3Marker;
extern bool showTBD4Marker;
extern bool showTBD5Marker;
extern bool showTBD6Marker;
extern bool showTBD7Marker;
extern bool showTBD8Marker;
extern bool showTBD9Marker;
extern bool showTBD10Marker;
extern bool showTBD11Marker;
extern bool showTBD12Marker;
extern bool showTBD13Marker;

void drawMarkerMenu();
void drawRecordingMenu();
void drawEventTimer();
bool canShowMarker(Game::Actor actor);
vec4 getDistinctColor(int id, bool standardRep = true);