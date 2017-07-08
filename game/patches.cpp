#include "main.h"

char *PLAYERS_REALLOC = 0;

void ApplyPreGamePatches()
{
	PLAYERS_REALLOC = new char[404*MAX_PLAYERS];
	UnFuck(g_libGTASA+0x5D021C);
	*(uint32_t*)(g_libGTASA+0x5D021C) = PLAYERS_REALLOC;
	LOGI("CWorld::Players new address 0x%X", PLAYERS_REALLOC);
}

void ApplyInGamePatches()
{
	// CPlayerPed__CPlayerPed task fix
	WriteMemory(g_libGTASA+0x458ED1, "\xD0", 1);

	// CPhysical destructor crash fix (CRealTimeShadowManager::ReturnRealTimeShadow)
	UnFuck(g_libGTASA+0x3A0196);
	NOP(g_libGTASA+0x3A0196, 5);

	// Remove the FindPlayerInfoForThisPlayerPed
	UnFuck(g_libGTASA+0x434D94);
	NOP(g_libGTASA+0x434D94, 6);

	// no  vehicle name rendering
	UnFuck(g_libGTASA+0x3D6FDC);
	NOP(g_libGTASA+0x3D6FDC, 2);

	// CTaskSimplePlayerOnFoot::PlayIdleAnimations 
	WriteMemory(g_libGTASA+0x4BDB18, "\x4F\xF0\x00\x00\xF7\x46", 6);

	// interior peds
	UnFuck(g_libGTASA+0x2C2C22);
	NOP(g_libGTASA+0x2C2C22, 4);

	// CarCtl::GenerateRandomCars nulled from CGame::Process
	UnFuck(g_libGTASA+0x398A3A);
	NOP(g_libGTASA+0x398A3A, 2);

	// CTheCarGenerators::Process nulled from CGame::Process
	UnFuck(g_libGTASA+0x398A34);
	NOP(g_libGTASA+0x398A34, 2);

	// ToggleDebugFPS
	void (*ToggleDebugFps)();
    *(void **) (&ToggleDebugFps) = (void*)(g_libGTASA+0x39A02C+1);
    (*ToggleDebugFps)();

    // VehicleStruct increase (0x32C*0x50 = 0xFDC0)
    WriteMemory(g_libGTASA+0x405338, "\x4F\xF6\xC0\x50", 4);	// MOV  R0, #0xFDC0
    WriteMemory(g_libGTASA+0x405342, "\x50\x20", 2);			// MOVS R0, #0x50
    WriteMemory(g_libGTASA+0x405348, "\x50\x22", 2);			// MOVS R2, #0x50
    WriteMemory(g_libGTASA+0x405374, "\x50\x2B", 2);			// CMP  R3, #0x50
}