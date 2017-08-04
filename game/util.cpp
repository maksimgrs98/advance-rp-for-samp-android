#include "main.h"

uint32_t dwPlayerPedPtrs[PLAYER_PED_SLOTS];

#define PI 3.14159265

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