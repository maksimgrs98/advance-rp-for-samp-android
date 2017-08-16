#include "main.h"
#include "util.h"
#include "keystuff.h"
#include <iostream>

// OpenGL stuff
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

extern CChatWindow *pChatWindow;
extern CNetGame *pNetGame;
extern CGame *pGame;

CVehicle 		*pVehicleClass;
uintptr_t 		dwRetAddr = 0;

void DoInitStuff();

uint32_t dwCurPlayerActor = 0;
uint8_t byteCurPlayer = 0;
uint8_t byteCurDriver = 0;

bool state = true;
void tst()
{
	CCamera *pGameCamera;
	pGameCamera->Restore();
	pGameCamera->SetBehindPlayer();
	pGame->DisplayHUD(true);
	pGame->FindPlayerPed()->TeleportTo(2495.0820, -1667.7239, 13.3438 + 1.0f);
}

void (*Render2dStuff)();
void Render2dStuff_hook()
{
	DoInitStuff();

	if (pNetGame)
		pNetGame->Process();

	DrawPlayerTags();

	if(pChatWindow)
		pChatWindow->Draw();

	if(state == false)
	{
		state = true;
		tst();
	}

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

	if(!strcmp(v2, "Textures/Fonts/RussianFont.met"))
	{
		LOGI("Loading RussianFont.met");
		sprintf(path, "%sSAMP/font/samp.met", storage);
		st[1] = (uint32_t)fopen(path, "r");
		return (uintptr_t)st;
	}

	if(!strcmp(v2, "Textures/Fonts/RussianFont.png"))
	{
		LOGI("Loading RussianFont.png");
		sprintf(path, "%sSAMP/font/samp.png", storage);
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
		WriteMemory(g_libGTASA+0x274178, "\x70\x47", 2);


		// call original
		(*CPed__ProcessControl)(thiz);
		// restore
		WriteMemory(g_libGTASA+0x439B7A, "\xFA\xF7\x1D\xF8", 4);
		WriteMemory(g_libGTASA+0x274178, "\x80\xB4", 2);
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
	byteCurDriver = FindPlayerNumFromPedPtr((uint32_t)pVehicle->pDriver);

	uintptr_t vtbl = ((*(uintptr_t*)thiz) - g_libGTASA);
	uintptr_t call_addr = 0;
	switch(vtbl)
	{
		// CAutomobile
		case 0x5CC9F0:
			call_addr = 0x4E314C;
			//LOGI("vtbl CAutomobile");
			break;
		// CBoat
		case 0x5CCD48:
			call_addr = 0x4F7408;
			//LOGI("vtbl CBoat");
			break;
		// CBike
		case 0x5CCB18:
			call_addr = 0x4EE790;
			//LOGI("vtbl CBike");
			break;
		// CPlane
		case 0x5CD0B0:
			call_addr = 0x5031E8;
			//LOGI("vtbl CPlane");
			break;
		// CHeli
		case 0x5CCE60:
			call_addr = 0x4FE62C;
			//LOGI("vtbl CHeli");
			break;
		// CBmx
		case 0x5CCC30:
			call_addr = 0x4F3CE8;
			//LOGI("vtbl CBmx");
			break;
		// CMonsterTruck
		case 0x5CCF88:
			call_addr = 0x500A34;
			//LOGI("vtbl CMonsterTruck");
			break;
		// CQuadBike
		case 0x5CD1D8:
			call_addr = 0x505840;
			//LOGI("vtbl CQuadBike");
			break;
		// CTrain
		case 0x5CD428:
			call_addr = 0x50AB24;
			//LOGI("vtbl CTrain");
			break;
	}

	void (*CAEVehicleAudioEntity_Service)(uintptr_t CAEVehicleAudioEntity);
    *(void **)(&CAEVehicleAudioEntity_Service) = (void*)(g_libGTASA+0x364B64+1);

	if(pVehicle->pDriver && (pVehicle->pDriver->dwPedType == 0) && (pVehicle->pDriver != GamePool_FindPlayerPed()))
	{
		// REMOTE PLAYER

		// CWidget::setEnabled
		// ret 0
		WriteMemory(g_libGTASA+0x274178, "\x70\x47", 2);


		// radio/engine
		// допилить
		pVehicle->pDriver->dwPedType = 4;
    	(*CAEVehicleAudioEntity_Service)(thiz+0x138);
    	pVehicle->pDriver->dwPedType = 0;

    	// restore
		WriteMemory(g_libGTASA+0x274178, "\x80\xB4", 2);
	}
	else
	{
		// LOCAL PLAYER

		// radio/engine
		(*CAEVehicleAudioEntity_Service)(thiz+0x138);
	}

	uint32_t (*ProcessControl)(VEHICLE_TYPE *pVehicle);
    *(void **)(&ProcessControl) = (void*)(g_libGTASA+call_addr+1);
    uint32_t dwRet = (*ProcessControl)(pVehicle);

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

/*
viewport: 0, 0, 1280, 720

*/

static const GLfloat globVertexBufferData[] = {
	0.0f,  0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
};

const GLfloat color[] = { 0.0f, 0.6f, 1.0f, 1.0f } ;

const char* vertexShaderCode = 
	"attribute vec4 vPosition;\n"
	"void main(){\n"
	"gl_Position = vPosition;\n"
	"}";

const char* fragmentShaderCode = 
	"precision mediump float;\n"
	"uniform vec4 vColor;\n"
	"void main(){\n"
	"gl_FragColor = vColor;\n"
	"}";

int LoadShader(int type, const char* shaderCode)
{
	int shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderCode, 0);
	glCompileShader(shader);
	return shader;
}

bool ogl_bInit = false;
uint32_t (*RQ_Command_rqSwapBuffers)(uint32_t r0);
uint32_t RQ_Command_rqSwapBuffers_hook(uint32_t r0)
{
	if (!ogl_bInit)
  	{
  	}

	return (*RQ_Command_rqSwapBuffers)(r0);
}

uint32_t (*CRadar__GetRadarTraceColor)(uint32_t color, uint8_t bright, uint8_t friendly);
uint32_t CRadar__GetRadarTraceColor_hook(uint32_t color, uint8_t bright, uint8_t friendly)
{
	//LOGI("CRadar__GetRadarTraceColor");

	return TranslateColorCodeToRGBA(color);
}

bool NotifyEnterVehicle(VEHICLE_TYPE *_pVehicle)
{
	LOGI("NotifyEnterVehicle");

	if(!pNetGame) return false;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	VEHICLEID VehicleID = pVehiclePool->FindIDFromGtaPtr(_pVehicle);

	if(VehicleID == INVALID_VEHICLE_ID) return false;
	if(!pVehiclePool->GetSlotState(VehicleID)) return false;
	pVehicleClass = pVehiclePool->GetAt(VehicleID);
	//if(pVehicleClass->m_bDoorsLocked) return false;
	if(pVehicleClass->m_pVehicle->entity.nModelIndex == TRAIN_PASSENGER) return false;

	if(pVehicleClass->m_pVehicle->pDriver &&
	   	pVehicleClass->m_pVehicle->pDriver->dwPedType != 0)
		return false;

	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();

	//if(pLocalPlayer->GetPlayerPed() && pLocalPlayer->GetPlayerPed()->GetCurrentWeapon() == WEAPON_PARACHUTE)
	//	pLocalPlayer->GetPlayerPed()->SetArmedWeapon(0);

	pLocalPlayer->SendEnterVehicleNotification(VehicleID, false);

	return true;
}


void (*CTaskComplexEnterCarAsDriver)();
void __attribute__((naked)) CTaskComplexEnterCarAsDriver_hook(uint32_t thiz, VEHICLE_TYPE* pVehicle)
{

	/*__asm__ volatile("push {r0-r11, lr}\n\t"
					"mov r2, lr\n\t"
					"blx get_lib\n\t"
					"add r0, #0x3A0000\n\t"
					"add r0, #0xEE00\n\t"
					"add r0, #0xF7\n\t"
					"cmp r2, r0\n\t"
					"bne 1f\n\t" // !=
					"mov r0, r1\n\t"
					"blx NotifyEnterVehicle\n\t" // call NotifyEnterVehicle
					"1:\n\t"  // call orig
					"pop {r0-r11, lr}\n\t");
					*/

	__asm__ volatile(
		"push {r0-r11, lr}\n\t"
		"mov r2, lr\n\t"
		"mov %0, lr\n\t" : "=r" (dwRetAddr));
	dwRetAddr-= g_libGTASA;
	if(dwRetAddr == 0x3AEEF7)
		NotifyEnterVehicle(pVehicle);


	__asm__ volatile("pop {r0-r11, lr}\n\t");
	return (*CTaskComplexEnterCarAsDriver)();
}

void (*CTaskComplexLeaveCar)(uintptr_t thiz, 
	VEHICLE_TYPE *pVehicle, 
	int iTargetDoor, 
	int iDelayTime, 
	bool bSensibleLeaveCar, 
	bool bForceGetOut);

bool bIgnoreNextExit = false;
void CTaskComplexLeaveCar_hook(uintptr_t thiz, 
	VEHICLE_TYPE *pVehicle,
	int iTargetDoor,
	int iDelayTime,
	bool bSensibleLeaveCar,
	bool bForceGetOut)
{
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	dwRetAddr -= g_libGTASA;

	if (dwRetAddr == 0x3AE905 || dwRetAddr == 0x3AE9CF)
	{
		if(pNetGame)
		{
			 if (GamePool_FindPlayerPed()->pVehicle == (uint32_t)pVehicle)
			 {
			 	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
			 	VEHICLEID VehicleID = pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE *)GamePool_FindPlayerPed()->pVehicle);

			 	CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
			 	pLocalPlayer->SendExitVehicleNotification(VehicleID);
			 }
		}
	}

	(*CTaskComplexLeaveCar)(thiz, pVehicle, iTargetDoor, iDelayTime, bSensibleLeaveCar, bForceGetOut);
}

void InstallSpecialHooks()
{
	// NvFOpen redirect
	SetUpHook(g_libGTASA+ADDR_NVFOPEN, (uintptr_t)NvFOpen_hook, (uintptr_t*)&NvFOpen);
	//
	//SetUpHook(g_libGTASA+0x1A2B5C, RQ_Command_rqSwapBuffers_hook, (uintptr_t*)&RQ_Command_rqSwapBuffers);
}

void InstallGameAndGraphicsLoopHooks()
{
	SetUpHook(g_libGTASA+ADDR_RENDER2DSTUFF, (uintptr_t)Render2dStuff_hook, (uintptr_t*)&Render2dStuff);

	SetUpHook(g_libGTASA+0x39D08C, (uintptr_t)CPad__GetPedWalkLeftRight_hook, (uintptr_t*)&CPad__GetPedWalkLeftRight);
	SetUpHook(g_libGTASA+0x39D110, (uintptr_t)CPad__GetPedWalkUpDown_hook, (uintptr_t*)&CPad__GetPedWalkUpDown);
	SetUpHook(g_libGTASA+0x39E7B0, (uintptr_t)CPad__DuckJustDown_hook, (uintptr_t*)&CPad__DuckJustDown);
	SetUpHook(g_libGTASA+0x39E9B8, (uintptr_t)CPad__JumpJustDown_hook, (uintptr_t*)&CPad__JumpJustDown);
	SetUpHook(g_libGTASA+0x39EAA4, (uintptr_t)CPad__GetSprint_hook, (uintptr_t*)&CPad__GetSprint);
	SetUpHook(g_libGTASA+0x39DD9C, (uintptr_t)CPad__MeleeAttackJustDown_hook, (uintptr_t*)&CPad__MeleeAttackJustDown);
	SetUpHook(g_libGTASA+0x39C9E4, (uintptr_t)CPad__GetSteeringLeftRight_hook, (uintptr_t*)&CPad__GetSteeringLeftRight);
	SetUpHook(g_libGTASA+0x39DB7C, (uintptr_t)CPad__GetAccelerate_hook, (uintptr_t*)&CPad__GetAccelerate);
	SetUpHook(g_libGTASA+0x39D938, (uintptr_t)CPad__GetBrake_hook, (uintptr_t*)&CPad__GetBrake);

	SetUpHook(g_libGTASA+0x482E60, (uintptr_t)CTaskComplexEnterCarAsDriver_hook, (uintptr_t*)&CTaskComplexEnterCarAsDriver);
	SetUpHook(g_libGTASA+0x4833CC, (uintptr_t)CTaskComplexLeaveCar_hook, (uintptr_t*)&CTaskComplexLeaveCar);
}

void GameInstallHooks()
{
	InstallGameAndGraphicsLoopHooks();

	SetUpHook(g_libGTASA+0x45A280, (uintptr_t)CPed__ProcessControl_hook, (uintptr_t*)&CPed__ProcessControl);
	SetUpHook(g_libGTASA+0x3DE9A8, (uintptr_t)CRadar__DrawRadarGangOverlay_hook, (uintptr_t*)&CRadar__DrawRadarGangOverlay);
	SetUpHook(g_libGTASA+0x3DBA88, (uintptr_t)CRadar__GetRadarTraceColor_hook, (uintptr_t*)&CRadar__GetRadarTraceColor);

	InstallMethodHook(g_libGTASA+0x5CCA1C, (uintptr_t)AllVehicles_ProcessControl_Hook); // CAutomobile::ProcessControl
	InstallMethodHook(g_libGTASA+0x5CCD74, (uintptr_t)AllVehicles_ProcessControl_Hook); // CBoat::ProcessControl
	InstallMethodHook(g_libGTASA+0x5CCB44, (uintptr_t)AllVehicles_ProcessControl_Hook); // CBike::ProcessControl
	InstallMethodHook(g_libGTASA+0x5CD0DC, (uintptr_t)AllVehicles_ProcessControl_Hook); // CPlane::ProcessControl
	InstallMethodHook(g_libGTASA+0x5CCE8C, (uintptr_t)AllVehicles_ProcessControl_Hook); // CHeli::ProcessControl
	InstallMethodHook(g_libGTASA+0x5CCC5C, (uintptr_t)AllVehicles_ProcessControl_Hook); // CBmx::ProcessControl
	InstallMethodHook(g_libGTASA+0x5CCFB4, (uintptr_t)AllVehicles_ProcessControl_Hook); // CMonsterTruck::ProcessControl
	InstallMethodHook(g_libGTASA+0x5CD204, (uintptr_t)AllVehicles_ProcessControl_Hook); // CQuadBike::ProcessControl
	InstallMethodHook(g_libGTASA+0x5CD454, (uintptr_t)AllVehicles_ProcessControl_Hook); // CTrain::ProcessControl
}
