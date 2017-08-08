#include "main.h"

uint32_t dwPlayerPedPtrs[PLAYER_PED_SLOTS];

#define NUM_RADAR_COLORS 200

#define PI 3.14159265

uint32_t dwHudColors[NUM_RADAR_COLORS] = {
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF, // (100)
0xFF8C13FF, // dark orange
0xC715FFFF, // Medium violet red
0x20B2AAFF, // sea green
0xDC143CFF, // crimson
0x6495EDFF, // cornflower blue
0xf0e68cFF, // khaki
0x778899FF, // light slate grey
0xFF1493FF, // deeppink
0xF4A460FF, // sandy
0xEE82EEFF, // violet
0xFFD720FF, // gold
0x8b4513FF, // chocolate
0x4949A0FF, // midnight blue
0x148b8bFF, // dark cyan
0x14ff7fFF, // spring green
0x556b2fFF,  // olive green
0x0FD9FAFF,
0x10DC29FF,
0x534081FF,
0x0495CDFF,
0xEF6CE8FF,
0xBD34DAFF,
0x247C1BFF,
0x0C8E5DFF,
0x635B03FF,
0xCB7ED3FF,
0x65ADEBFF,
0x5C1ACCFF,
0xF2F853FF,
0x11F891FF,
0x7B39AAFF,
0x53EB10FF,
0x54137DFF,
0x275222FF,
0xF09F5BFF,
0x3D0A4FFF,
0x22F767FF,
0xD63034FF,
0x9A6980FF,
0xDFB935FF,
0x3793FAFF,
0x90239DFF,
0xE9AB2FFF,
0xAF2FF3FF,
0x057F94FF,
0xB98519FF,
0x388EEAFF,
0x028151FF,
0xA55043FF,
0x0DE018FF,
0x93AB1CFF,
0x95BAF0FF,
0x369976FF,
0x18F71FFF,
0x4B8987FF,
0x491B9EFF,
0x829DC7FF,
0xBCE635FF,
0xCEA6DFFF,
0x20D4ADFF,
0x2D74FDFF,
0x3C1C0DFF,
0x12D6D4FF,
0x48C000FF,
0x2A51E2FF,
0xE3AC12FF,
0xFC42A8FF,
0x2FC827FF,
0x1A30BFFF,
0xB740C2FF,
0x42ACF5FF,
0x2FD9DEFF,
0xFAFB71FF,
0x05D1CDFF,
0xC471BDFF,
0x94436EFF,
0xC1F7ECFF,
0xCE79EEFF,
0xBD1EF2FF,
0x93B7E4FF,
0x3214AAFF,
0x184D3BFF,
0xAE4B99FF,
0x7E49D7FF,
0x4C436EFF,
0xFA24CCFF,
0xCE76BEFF,
0xA04E0AFF,
0x9F945CFF,
0xDCDE3DFF,
0x10C9C5FF,
0x70524DFF,
0x0BE472FF,
0x8A2CD7FF,
0x6152C2FF,
0xCF72A9FF,
0xE59338FF,
0xEEDC2DFF,
0xD8C762FF,
0x3FE65CFF
};

uint32_t dwUseHudColors[NUM_RADAR_COLORS];


void SetRadarColor(uint8_t nIndex, uint32_t dwColor)
{
	if(nIndex < sizeof(dwUseHudColors)) {
		dwUseHudColors[nIndex] = dwColor;
	}
}

uint32_t TranslateColorCodeToRGBA(int iCode)
{
	// Special internal colors
	if(iCode == 200) return 0x888888FF;
	if(iCode == 201) return 0xAA0000FF;
	if(iCode == 202) return 0xE2C063FF;

	if(iCode < sizeof(dwUseHudColors)) {
		return dwUseHudColors[iCode];
	} else {
		return 0x999999FF;
	}
}


void GameResetRadarColors()
{
	memcpy(&dwUseHudColors[0],&dwHudColors[0],sizeof(uint32_t)*NUM_RADAR_COLORS);
}

PED_TYPE * GamePool_Ped_GetAt(int iID)
{
	PED_TYPE* (*GetPoolPed)(int iID);
	*(void **) (&GetPoolPed) = (void*)g_libGTASA+ADDR_GETPOOLPED+1;
	return (*GetPoolPed)(iID);
}

int GamePool_Ped_GetIndex(PED_TYPE *pActor)
{
	int iRetVal = 0;

    int (*GetPoolPedRef)(PED_TYPE *pActor);
    *(void **) (&GetPoolPedRef) = (void*)(g_libGTASA+0x41DD60+1);
    iRetVal = (*GetPoolPedRef)(pActor);

    return iRetVal;
}

VEHICLE_TYPE *GamePool_Vehicle_GetAt(int iID)
{
	VEHICLE_TYPE* (*GetPoolVehicle)(int iID);
	*(void **) (&GetPoolVehicle) = (void*)g_libGTASA+ADDR_GETPOOLVEHICLE+1;
	return (GetPoolVehicle)(iID);
}


uint32_t GamePool_Vehicle_GetIndex(VEHICLE_TYPE *pVehicle)
{
	uint32_t dwID = 0;

	uint32_t (*GetPoolVehicleRef)(VEHICLE_TYPE *pVehicle);
    *(void **)(&GetPoolVehicleRef) = (void*)g_libGTASA+ADDR_GETPOOLVEHICLEREF+1;
    dwID = (*GetPoolVehicleRef)(pVehicle);

	return dwID;
}

extern char *PLAYERS_REALLOC;
PED_TYPE *GamePool_FindPlayerPed()
{
	return *(PED_TYPE**)PLAYERS_REALLOC;
}

bool IsValidModel(int iModelID)
{
    if(iModelID < 0 || iModelID > 20000) return false;
    // _DWORD *CModelInfo::ms_modelInfoPtrs[20000]
    uintptr_t *dwModelArray = (uintptr_t*)(g_libGTASA+0x87BF48);

    if(dwModelArray[iModelID] == 0)
      return false;

    return true;
}

uint16_t GetModelReferenceCount(int nModelIndex)
{
	uintptr_t *dwModelarray = (uintptr_t*)(g_libGTASA+0x87BF48);
	uint8_t *pModelInfoStart = (uint8_t*)dwModelarray[nModelIndex];
	
	return *(uint16_t*)(pModelInfoStart+0x1E);
}

void SetPlayerPedPtrRecord(uint8_t bytePlayer, uint32_t dwPedPtr) 
{
	dwPlayerPedPtrs[bytePlayer] = dwPedPtr;
}

uint8_t FindPlayerNumFromPedPtr(uint32_t dwPedPtr)
{
	uint8_t x = 0;
	while(x != PLAYER_PED_SLOTS)
	{
		if(dwPlayerPedPtrs[x] == dwPedPtr) return x;
		x++;
	}
	return 0;
}

float DegToRad(float fDegrees)
{
	if (fDegrees > 360.0f || fDegrees < 0.0f) return 0.0f;
	if (fDegrees > 180.0f) {
		return (float)(-(PI - (((fDegrees - 180.0f) * PI) / 180.0f)));
	} else {
		return (float)((fDegrees * PI) / 180.0f);
	}
}

float FloatOffset(float f1, float f2)
{	
	if(f1 >= f2) return f1 - f2;
	else return (f2 - f1);
}

void RtQuatConvertFromMatrix(PMATRIX4X4 m, PQUATERNION q)
{
	float rtQuat[4];

	void (*RtQuatConvertFromMatrix)(float *q, PMATRIX4X4 m);
	*(void **)(&RtQuatConvertFromMatrix) = (void*)(g_libGTASA+0x1DE0DC+1);
    (*RtQuatConvertFromMatrix)(rtQuat, m);

    q->W = rtQuat[3];
    q->X = rtQuat[0];
    q->Y = rtQuat[1];
    q->Z = rtQuat[2];
}
