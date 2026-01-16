#include "utils.h"
#include "actor.h"

namespace hotkeys {
	int render = VK_UP;
	int openMenu = VK_DOWN;
	int showEventTimer = VK_NUMPAD3;
	int hideAll = VK_NUMPAD4;
	int scaleUiSmall = VK_NUMPAD5;
	int scaleUiBig = VK_NUMPAD6;
	int renderDebug = VK_NUMPAD7;
	int shutdown = VK_NUMPAD8;
	int uninject = VK_NUMPAD9;
}

namespace state {
	boolean showMenu = false;
	boolean showEventTimer = false;
	boolean render = false;
	boolean renderDebug = false;
	boolean recordingMarker = false;
	boolean hideAll = false;
	boolean shutdown = false;
}

FILE* hf_out = nullptr;
tinyxml2::XMLDocument eventTimerDoc;

void initIO()
{
	AllocConsole();
	// int hCrt = _open_osfhandle(INT_PTR(GetStdHandle(STD_OUTPUT_HANDLE)), _O_TEXT);
	// hf_out = _fdopen(hCrt, "w");
	hf_out = new FILE();
	// setvbuf(hf_out, NULL, _IONBF, 1);
	freopen_s(&hf_out, "CONOUT$", "w", stdout);
	// *stdout = *_fdopen(hCrt, "w");
	std::cout << "console allocated" << std::endl;
}

void teardownIO()
{
	if (hf_out != nullptr)
	{
		fclose(hf_out);
		FreeConsole();
	}
}

void log(const char* line)
{
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	fprintf(hf_out, "%02d:%02d:%02d.%03d  %s\n", localTime.wHour, localTime.wMinute, localTime.wSecond, localTime.wMilliseconds, line);
	// std::cout << line << std::endl;
}

void fillServerEvents()
{
	eventTimerDoc.LoadFile("Interface/Addons/soloUI/eventtimer.xml");
	if (eventTimerDoc.ErrorID() != 0) return;
	tinyxml2::XMLElement* eventTimerElement = eventTimerDoc.FirstChildElement("EventTimer");
	tinyxml2::XMLElement* eventElement = eventTimerElement->FirstChildElement("Event");
	size_t i = 0;
	do {
		Game::SrvEv srvEv;
		srvEv.name = eventElement->FindAttribute("Name")->Value();
		srvEv.id = eventElement->FindAttribute("id")->Value();
		srvEv.length = eventElement->FindAttribute("Length")->IntValue();
		tinyxml2::XMLElement* entryElement = eventElement->FirstChildElement("Entry");
		Game::ServerEventInstances entries;
		size_t j = 0;
		do {
			Game::SrvEvInst evInst;
			evInst.name = entryElement->FindAttribute("Name")->Value();
			evInst.length = entryElement->FindAttribute("Length")->IntValue();
			evInst.colorCode = entryElement->FindAttribute("Color")->IntValue();
			entries[j] = evInst;
			j++;
		} while (entryElement = entryElement->NextSiblingElement());
		srvEv.entries = entries;
		srvEv.display = true;
		Game::serverEvents[i] = srvEv;
		i++;
	} while (eventElement = eventElement->NextSiblingElement());
}

void startRecordingMarker()
{
	log("start recording...");
	state::recordingMarker = !state::recordingMarker;
}

void endRecordingMarker()
{
	log("stop recording...");
	tinyxml2::XMLDocument doc;
	std::string fileLoc = "Interface/Addons/soloUI/pois";
	fileLoc += std::to_string((int)Game::currentMapId);
	fileLoc += ".xml";
	doc.LoadFile(fileLoc.c_str());
	if (doc.ErrorID() == tinyxml2::XML_ERROR_FILE_NOT_FOUND)
	{
		tinyxml2::XMLNode* baseElement = doc.InsertEndChild(doc.NewElement("OverlayData"));
		tinyxml2::XMLElement* poisElement = doc.NewElement("POIs");
		baseElement->InsertFirstChild(poisElement);
	}
	tinyxml2::XMLElement* poisElement = doc.FirstChildElement()->FirstChildElement("POIs");

	for (const std::pair<QWORD, Game::Actor>& pair : Game::Actor::actorMap) {
		tinyxml2::XMLElement* poiElement = doc.NewElement("POI");
		poiElement->SetAttribute("MapID", (int)Game::currentMapId);
		poiElement->SetAttribute("uID", getUID(pair.second.pos));
		poiElement->SetAttribute("xPos", pair.second.pos.x);
		poiElement->SetAttribute("yPos", pair.second.pos.y);
		poiElement->SetAttribute("zPos", pair.second.pos.z);
		poiElement->SetAttribute("type", getTypeString((int)pair.second.race));
		poisElement->InsertEndChild(poiElement);
	}
	// doc.Print();
	// fprintf(hf_out, "error code: %i\n", doc.ErrorID());
	doc.SaveFile(fileLoc.c_str());

	state::recordingMarker = !state::recordingMarker;
	std::string out = "recording stopped, file '" + fileLoc + "' saved.";
	log(out.c_str());
}

const char* getUID(vec3 pos)
{
	const unsigned char* px = reinterpret_cast<const unsigned char*>(&pos.x);
	const unsigned char* py = reinterpret_cast<const unsigned char*>(&pos.y);
	const unsigned char* pz = reinterpret_cast<const unsigned char*>(&pos.z);
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for (int i = 0; i < 4; ++i)
	{
		ss << std::setw(2) << static_cast<unsigned>(px[i]);
		ss << std::setw(2) << static_cast<unsigned>(py[i]);
		ss << std::setw(2) << static_cast<unsigned>(pz[i]);
	}
	std::string str = ss.str();
	return str.c_str();
}

const char* getTypeString(int type)
{
	return "to.be.done";
}

void* DetourFunction64(void* pSource, void* pDestination, int dwLen)
{
	if (dwLen < 14) return NULL;

	BYTE stub[] = {
		0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, // jmp qword ptr [$+6]
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // ptr
	};

	void* pTrampoline = VirtualAlloc(0, dwLen + sizeof(stub), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	DWORD dwOld = 0;
	VirtualProtect(pSource, dwLen, PAGE_EXECUTE_READWRITE, &dwOld);

	DWORD64 retto = (DWORD64)pSource + dwLen;

	// trampoline
	memcpy(stub + 6, &retto, 8);
	memcpy((void*)((DWORD_PTR)pTrampoline), pSource, dwLen);
	memcpy((void*)((DWORD_PTR)pTrampoline + dwLen), stub, sizeof(stub));

	// orig
	memcpy(stub + 6, &pDestination, 8);
	memcpy(pSource, stub, sizeof(stub));

	for (int i = 14; i < dwLen; i++)
	{
		*(BYTE*)((DWORD_PTR)pSource + i) = 0x90;
	}

	VirtualProtect(pSource, dwLen, dwOld, &dwOld);
	return (void*)((DWORD_PTR)pTrampoline);
}

uintptr_t followPointerPath(uintptr_t baseAddress, std::vector<unsigned int> offsets) {
	uintptr_t currentAddr = baseAddress;
	for (int i = 0; i < offsets.size(); i++) {
		currentAddr = *(uintptr_t*)currentAddr;
		currentAddr += offsets[i];
	}
	return currentAddr;
}