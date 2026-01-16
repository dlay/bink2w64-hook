#pragma once
#include "pch.h"
#include <SimpleMath.h>
#include <io.h>
#include <fcntl.h>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include "tinyxml2.h"

namespace hotkeys {
	extern int uninject;
	extern int openMenu;
	extern int renderDebug;
	extern int render;
	extern int shutdown;
	extern int hideAll;
	extern int showEventTimer;
	extern int scaleUiBig;
	extern int scaleUiSmall;
}

namespace state {
	extern boolean showMenu;
	extern boolean showEventTimer;
	extern boolean render;
	extern boolean renderDebug;
	extern boolean recordingMarker;
	extern boolean hideAll;
	extern boolean shutdown;
}



#define COLOR_BLACK { return ImVec4( 0, 0, 0, 255 ) }; // black
#define COLOR_WHITE ImVec4 { 255, 255, 255, 255 }; // white
#define COLOR_RED { 230, 25, 75, 255 }; // red
#define COLOR_GREEN { 60, 180, 75, 255 }; // green
#define COLOR_YELLOW { 255, 225, 25, 255 }; // yellow
#define COLOR_BLUE { 0, 130, 200, 255 }; // blue
#define COLOR_ORANGE { 245, 130, 48, 255 }; // orange
#define COLOR_CYAN { 70, 240, 240, 255 }; // cyan
#define COLOR_MAGENTA { 240, 50, 230, 255 }; // magenta
#define COLOR_PINK { 250, 190, 212, 255 }; // pink
#define COLOR_TEAL { 0, 128, 128, 255 }; // teal
#define COLOR_LAVENDER { 220, 190, 255, 255 }; // lavender
#define COLOR_BROWN { 170, 110, 40, 255 }; // brown
#define COLOR_BEIGE { 255, 250, 200, 255 }; // beige
#define COLOR_MAROON { 128, 0, 0, 255 }; // maroon
#define COLOR_MINT { 170, 255, 195, 255 }; // mint
#define COLOR_NAVY { 0, 0, 128, 255 }; // navy
#define COLOR_PURPLE { 145, 30, 180, 255 }; // purple
#define COLOR_LIME { 210, 245, 60, 255 }; // lime
#define COLOR_OLIVE { 128, 128, 0, 255 }; // olive
#define COLOR_GREY { 128, 128, 128, 255 }; // grey

typedef unsigned __int64 QWORD;
typedef __m128 OWORD;
struct QWORDPTR { unsigned __int64* content; }; // 8 byte
struct OWORDPTR { __m128* content; }; // 16 byte
struct OWORD2FLOAT {
	float x;
	float y;
	float z;
	float a;
};

class vec3
{
public:

	union
	{
		struct { float x, y, z; };
		float v[3];
	};

	vec3() { x = y = z = 0; }
	vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	vec3 operator + (const vec3& rhs) const { return vec3(x + rhs.x, y + rhs.y, z + rhs.z); }
	vec3 operator - (const vec3& rhs) const { return vec3(x - rhs.x, y - rhs.y, z - rhs.z); }
	vec3 operator * (const float& rhs) const { return vec3(x * rhs, y * rhs, z * rhs); }
	vec3 operator / (const float& rhs) const { return vec3(x / rhs, y / rhs, z / rhs); }
	vec3& operator += (const vec3& rhs) { return *this = *this + rhs; }
	vec3& operator -= (const vec3& rhs) { return *this = *this - rhs; }
	vec3& operator *= (const float& rhs) { return *this = *this * rhs; }
	vec3& operator /= (const float& rhs) { return *this = *this / rhs; }
	float dot() const { return x * x + y * y + z * z; }
	float Length() const { return sqrtf(dot()); }
	vec3 Normalize() const { return *this * (1 / Length()); }
	float Distance(const vec3& rhs) const { return (*this - rhs).Length(); }
};

using Vector3 = vec3;

struct vec2 { float x, y; };
struct vec4 { float x, y, z, w; };

extern FILE* hf_out;
extern tinyxml2::XMLDocument eventTimerDoc;

void initIO();
void teardownIO();
void log(const char* line);

void fillServerEvents();

void startRecordingMarker();
void endRecordingMarker();
const char* getUID(vec3 pos);
const char* getTypeString(int type);

void* DetourFunction64(void* pSource, void* pDestination, int dwLen);
uintptr_t followPointerPath(uintptr_t baseAddress, std::vector<unsigned int> offsets);
