#include "main.h"
#include "util.h"
#include "keystuff.h"

extern CNetGame *pNetGame;
extern CGame *pGame;

void DoInitStuff();

uint32_t dwCurPlayerActor = 0;
uint8_t byteCurPlayer = 0;
uint8_t byteCurDriver = 0;

void (*Render2dStuff)();
void Render2dStuff_hook()
{
	DoInitStuff();

	if (pNetGame)
		pNetGame->Process();

	//if(pChatWindow)
	//	pChatWindow->Draw();

	return (*Render2dStuff)();
}

uintptr_t (*NvFOpen)(const char*, const char*, uint32_t, uint32_t);
uintptr_t NvFOpen_hook(const char* v1, const char* v2, uint32_t v3, uint32_t v4)
{
	char path[0xFF] = {0};
	uint32_t *st = (uint32_t*)malloc(8);
	st[0] = 1;

	char* storage = (char*)(g_libGTASA+0x63C4B8);

	if(!strcmp(v2+12, "mainV1.scm"))
	{
			LOGI("Loading main.scm");
			sprintf(path, "%sSAMP/main.scm", storage);
			st[1] = (uint32_t)fopen(path, "rb");
			return (uintptr_t)st;
	}

	if(!strcmp(v2+12, "SCRIPTV1.IMG"))
	{
			LOGI("Loading SCRIPT.IMG");
			sprintf(path, "%sSAMP/SCRIPT.IMG", storage);
			st[1] = (uint32_t)fopen(path, "rb");
			return (uintptr_t)st;
	}

	if(!strcmp(v2+5, "DEFAULT.DAT"))
	{
		LOGI("Loading DEFAULT.DAT");
		sprintf(path, "%sSAMP/DEFAULT.DAT", storage);
		st[1] = (uint32_t)fopen(path, "rb");
		return (uintptr_t)st;
	}

	if(!strcmp(v2, "DATA/PEDS.IDE"))
	{
		LOGI("Loading PEDS.IDE");
		sprintf(path, "%sSAMP/PEDS.IDE", storage);
		st[1] = (uint32_t)fopen(path, "r");
		return (uintptr_t)st;
	}
	
	if(!strcmp(v2+5, "VEHICLES.IDE"))
	{
		LOGI("Loading VEHICLES.IDE");
		sprintf(path, "%sSAMP/VEHICLES.IDE", storage);
		st[1] = (uint32_t)fopen(path, "rb");
		return (uintptr_t)st;
	}

	if(!strcmp(v2+11, "tracks2.dat"))
	{
		LOGI("Loading tracks2.dat");
		sprintf(path, "%sSAMP/tracks2.dat", storage);
		st[1] = (uint32_t)fopen(path, "rb");
		return (uintptr_t)st;
	}

	if(!strcmp(v2+11, "tracks4.dat"))
	{
		LOGI("Loading tracks4.dat");
		sprintf(path, "%sSAMP/tracks4.dat", storage);
		st[1] = (uint32_t)fopen(path, "rb");
		return (uintptr_t)st;
	}

	if(!strcmp(v2+5, "CUSTOM.IMG"))
	{
		LOGI("Loading CUSTOM.IMG");
		sprintf(path, "%sSAMP/CUSTOM.IMG", storage);
		st[1] = (uint32_t)fopen(path, "rb");
		return (uintptr_t)st;
	}

		if(!strcmp(v2+5, "SAMP.IMG"))
	{
		LOGI("Loading SAMP.IMG");
		sprintf(path, "%sSAMP/SAMP.IMG", storage);
		st[1] = (uint32_t)fopen(path, "rb");
		return (uintptr_t)st;
	}

	if(!strcmp(v2+5, "GTA.DAT"))
	{
		LOGI("Loading GTA.DAT");
		sprintf(path, "%sSAMP/GTA.DAT", storage);
		st[1] = (uint32_t)fopen(path, "rb");
		return (uintptr_t)st;
	}

	if(!strcmp(v2+5, "samp.IDE"))
	{
		LOGI("Loading samp.IDE");
		sprintf(path, "%sSAMP/samp.IDE", storage);
		st[1] = (uint32_t)fopen(path, "rb");
		return (uintptr_t)st;
	}

	if(!strcmp(v2+5, "custom.IDE"))
	{
		LOGI("Loading custom.IDE");
		sprintf(path, "%sSAMP/custom.IDE", storage);
		st[1] = (uint32_t)fopen(path, "rb");
		return (uintptr_t)st;
	}

	if(!strcmp(v2, "texdb/samp")) // texdb/%s/%s.txt
	{
		LOGI("Loading %s", v2);
		sprintf(path, "%sSAMP/%s", storage, v2);
		st[1] = (uint32_t)fopen(path, "rb");
		return (uintptr_t)st;
	}

	return NvFOpen(v1, v2, v3, v4);
}

uint32_t (*CPed__ProcessControl)(uintptr_t thiz);
uint32_t CPed__ProcessControl_hook(uintptr_t thiz)
{
	dwCurPlayerActor = thiz;
	byteCurPlayer = FindPlayerNumFromPedPtr(dwCurPlayerActor);

	if(dwCurPlayerActor && (byteCurPlayer != 0))
	{
		// REMOTE PLAYER

		// CPed::UpdatePosition nulled from CPed::ProcessControl
		UnFuck(g_libGTASA+0x439B7A);
		NOP(g_libGTASA+0x439B7A, 2);

		// CWidget::setEnabled
		WriteMemory(g_libGTASA+0x274178, "\x4F\xF0\x00\x00\xF7\x46", 6);

		// call original
		(*CPed__ProcessControl)(thiz);
		// restore
		WriteMemory(g_libGTASA+0x439B7A, "\xFA\xF7\x1D\xF8", 4);
		WriteMemory(g_libGTASA+0x274178, "\x80\xB4\x00\xAF\x80\xF8", 6);
	}
	else
	{
		// LOCAL PLAYER

		// Apply the original code to set ped rot from Cam
		WriteMemory(g_libGTASA+0x4BED92, "\x10\x60", 2);

		(*CPed__ProcessControl)(thiz);

		// Reapply the no ped rots from Cam patch
		WriteMemory(g_libGTASA+0x4BED92, "\x00\x46", 2);
	}

	return 0;
}

/*
	W = 0xFF80
	S = 0x80
	A = 0xFF80
	D = 0x80
*/

uint16_t (*CPad__GetPedWalkLeftRight)(uintptr_t thiz);
uint16_t CPad__GetPedWalkLeftRight_hook(uintptr_t thiz)
{
	// REMOTE PLAYER
	if(dwCurPlayerActor && (byteCurPlayer != 0))
	{
		return GcsRemotePlayerKeys[byteCurPlayer].wWalkLR;
	}

	// LOCAL PLAYER
	GcsLocalPlayerKeys.wWalkLR = CPad__GetPedWalkLeftRight(thiz);
	return GcsLocalPlayerKeys.wWalkLR;
}

uint16_t (*CPad__GetPedWalkUpDown)(uintptr_t thiz);
uint16_t CPad__GetPedWalkUpDown_hook(uintptr_t thiz)
{
	// REMOTE PLAYER
	if(dwCurPlayerActor && (byteCurPlayer != 0))
		return GcsRemotePlayerKeys[byteCurPlayer].wWalkUD;

	// LOCAL PLAYER
	GcsLocalPlayerKeys.wWalkUD = CPad__GetPedWalkUpDown(thiz);
	return GcsLocalPlayerKeys.wWalkUD;
}

// допилить
uint8_t (*CPad__DuckJustDown)(uintptr_t thiz);
uint8_t CPad__DuckJustDown_hook(uintptr_t thiz)
{
	// REMOTE PLAYER
	if(dwCurPlayerActor && (byteCurPlayer != 0))
		return GcsRemotePlayerKeys[byteCurPlayer].bDuckJustDown;

	// LOCAL PLAYER
	GcsLocalPlayerKeys.bDuckJustDown = CPad__DuckJustDown(thiz);
	return GcsLocalPlayerKeys.bDuckJustDown;
}

uint8_t (*CPad__JumpJustDown)(uintptr_t thiz);
uint8_t CPad__JumpJustDown_hook(uintptr_t thiz)
{

	// REMOTE PLAYER
	if(dwCurPlayerActor && (byteCurPlayer != 0))
		return GcsRemotePlayerKeys[byteCurPlayer].bJumpJustDown;
	
	// LOCAL PLAYER
	GcsLocalPlayerKeys.bJumpJustDown = CPad__JumpJustDown(thiz);
	return GcsLocalPlayerKeys.bJumpJustDown;
}

uint8_t (*CPad__GetSprint)(uintptr_t thiz, uint32_t unk);
uint8_t CPad__GetSprint_hook(uintptr_t thiz, uint32_t unk)
{
	// REMOTE PLAYER
	if(dwCurPlayerActor && (byteCurPlayer != 0))
		return GcsRemotePlayerKeys[byteCurPlayer].bSprintJustDown;
		
	// LOCAL PLAYER
	GcsLocalPlayerKeys.bSprintJustDown = CPad__GetSprint(thiz, unk);
	return GcsLocalPlayerKeys.bSprintJustDown;
}

uint8_t (*CPad__MeleeAttackJustDown)(uintptr_t thiz);
uint8_t CPad__MeleeAttackJustDown_hook(uintptr_t thiz)
{
	// REMOTE PLAYER
	if(dwCurPlayerActor && (byteCurPlayer != 0))
	return GcsRemotePlayerKeys[byteCurPlayer].bMeleeAttackJustDown;

	// LOCAL PLAYER
	GcsLocalPlayerKeys.bMeleeAttackJustDown = CPad__MeleeAttackJustDown(thiz);
	return GcsLocalPlayerKeys.bMeleeAttackJustDown;
}

// -------------- VEHICLE ------------------------

int16_t (*CPad__GetSteeringLeftRight)(uintptr_t thiz);
int16_t CPad__GetSteeringLeftRight_hook(uintptr_t thiz)
{	
	// REMOTE PLAYER
	if(byteCurDriver != 0)
		return (int16_t)GcsRemotePlayerKeys[byteCurDriver].wSteeringLR;

	// LOCAL PLAYER
	GcsLocalPlayerKeys.wSteeringLR = CPad__GetSteeringLeftRight(thiz);
	return (int16_t)GcsLocalPlayerKeys.wSteeringLR;
}

uint16_t (*CPad__GetBrake)(uintptr_t thiz);
uint16_t CPad__GetBrake_hook(uintptr_t thiz)
{
	// REMOTE PLAYER
	if(byteCurDriver != 0)
		return GcsRemotePlayerKeys[byteCurDriver].wBrake;

	// LOCAL PLAYER
	GcsLocalPlayerKeys.wBrake = CPad__GetBrake(thiz);
	return GcsLocalPlayerKeys.wBrake;
}

uint16_t (*CPad__GetAccelerate)(uintptr_t thiz);
uint16_t CPad__GetAccelerate_hook(uintptr_t thiz)
{
	// REMOTE PLAYER
	if(byteCurDriver != 0)
		return GcsRemotePlayerKeys[byteCurDriver].wAccelerate;

	// LOCAL PLAYER
	GcsLocalPlayerKeys.wAccelerate = CPad__GetAccelerate(thiz);
	return GcsLocalPlayerKeys.wAccelerate;
}

// допилить
uint32_t  AllVehicles_ProcessControl_Hook(uint32_t thiz)
{
	VEHICLE_TYPE *pVehicle = (VEHICLE_TYPE*)thiz;

	byteCurDriver = FindPlayerNumFromPedPtr(pVehicle->pDriver);

	if(pVehicle->pDriver && pVehicle->pDriver != GamePool_FindPlayerPed())
	{
		// REMOTE PLAYER
		// CWidget::setEnabled
		// ret 0
		WriteMemory(g_libGTASA+0x274178, "\x4F\xF0\x00\x00\xF7\x46", 6);
	}
	else
	{
		// LOCAL PLAYER
	}

	uint32_t (*CAutomobile_ProcessControl)(VEHICLE_TYPE *pVehicle);
    *(void **)(&CAutomobile_ProcessControl) = (void*)(g_libGTASA+0x4E314C+1);
    uint32_t dwRet = (*CAutomobile_ProcessControl)(pVehicle);

    	// restore
	WriteMemory(g_libGTASA+0x274178, "\x80\xB4\x00\xAF\x80\xF8", 6);

    return dwRet;
}

uint8_t bGZ = 0;
uint32_t (*CRadar__DrawRadarGangOverlay)(uint8_t v1);
uint32_t CRadar__DrawRadarGangOverlay_hook(uint8_t v1)
{
	bGZ = v1;
	if (pNetGame && pNetGame->GetGangZonePool()) pNetGame->GetGangZonePool()->Draw();

	return 0;
}

void InstallSpecialHooks()
{
	// NvFOpen redirect
	SetUpHook(g_libGTASA+ADDR_NVFOPEN, NvFOpen_hook, (uintptr_t*)&NvFOpen);
}

void InstallGameAndGraphicsLoopHooks()
{
	SetUpHook(g_libGTASA+ADDR_RENDER2DSTUFF, Render2dStuff_hook, (uintptr_t*)&Render2dStuff);

	SetUpHook(g_libGTASA+0x39D08C, CPad__GetPedWalkLeftRight_hook, (uintptr_t*)&CPad__GetPedWalkLeftRight);
	SetUpHook(g_libGTASA+0x39D110, CPad__GetPedWalkUpDown_hook, (uintptr_t*)&CPad__GetPedWalkUpDown);
	SetUpHook(g_libGTASA+0x39E7B0, CPad__DuckJustDown_hook, (uintptr_t*)&CPad__DuckJustDown);
	SetUpHook(g_libGTASA+0x39E9B8, CPad__JumpJustDown_hook, (uintptr_t*)&CPad__JumpJustDown);
	SetUpHook(g_libGTASA+0x39EAA4, CPad__GetSprint_hook, (uintptr_t*)&CPad__GetSprint);
	SetUpHook(g_libGTASA+0x39DD9C, CPad__MeleeAttackJustDown_hook, (uintptr_t*)&CPad__MeleeAttackJustDown);
	SetUpHook(g_libGTASA+0x39C9E4, CPad__GetSteeringLeftRight_hook, (uintptr_t*)&CPad__GetSteeringLeftRight);
	SetUpHook(g_libGTASA+0x39DB7C, CPad__GetAccelerate_hook, (uintptr_t*)&CPad__GetAccelerate);
	SetUpHook(g_libGTASA+0x39D938, CPad__GetBrake_hook, (uintptr_t*)&CPad__GetBrake);
}

void GameInstallHooks()
{
	InstallGameAndGraphicsLoopHooks();

	SetUpHook(g_libGTASA+0x45A280, CPed__ProcessControl_hook, (uintptr_t*)&CPed__ProcessControl);
	SetUpHook(g_libGTASA+0x3DE9A8, CRadar__DrawRadarGangOverlay_hook, (uintptr_t*)&CRadar__DrawRadarGangOverlay);

	InstallMethodHook(g_libGTASA+0x5CCA1C, (uintptr_t)AllVehicles_ProcessControl_Hook); // CAutomobile::ProcessControl
}