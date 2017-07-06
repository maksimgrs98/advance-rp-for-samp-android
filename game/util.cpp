#include "main.h"
#include <algorithm>

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

void ConvertMatrixToQuaternion(PQUATERNION q, PMATRIX4X4 m)
{
											//m00		//m11		//m22
	q->W = sqrt( std::max( (float)0, 1.0f + m->right.X + m->up.Y + m->at.Z ) ) * 0.5f;
    q->X = sqrt( std::max( (float)0, 1.0f + m->right.X - m->up.Y - m->at.Z ) ) * 0.5f;
    q->Y = sqrt( std::max( (float)0, 1.0f - m->right.X + m->up.Y - m->at.Z ) ) * 0.5f;
    q->Z = sqrt( std::max( (float)0, 1.0f - m->right.X - m->up.Y + m->at.Z ) ) * 0.5f;

    q->X = static_cast < float > ( copysign( q->X, m->at.Y - m->up.Z ) );
    q->Y = static_cast < float > ( copysign( q->Y, m->right.Z - m->at.X ) );
    q->Z = static_cast < float > ( copysign( q->Z, m->up.X - m->right.Y ) );
}

void ConvertQuaternionToMatrix(PMATRIX4X4 m, PQUATERNION q)
{     
	/*
	a1 = q.w   a5 = right.x   a8 = up.x    a11 = at.x
	a2 = q.x   a6 = right.y   a9 = up.y    a12 = at.y
	a3 = q.y   a7 = right.z   a10 = up.z   a13 = at.z
	a4 = q.z
	*/

	float sqw = q->W*q->W; // v13 = a1 * a1;
    float sqx = q->X*q->X; // v14 = a2 * a2;
    float sqy = q->Y*q->Y; // v15 = a3 * a3;
    float sqz = q->Z*q->Z; // v16 = a4 * a4;

    m->right.X = ( sqx - sqy - sqz + sqw); 	// a5 = v14 - v15 - v16 + v13;
    m->up.Y = (-sqx + sqy - sqz + sqw);		// a9 = v15 - v14 - v16 + v13;
    m->at.Z = (-sqx - sqy + sqz + sqw);		// a13 = v16 - (v15 + v14) + v13;
    
    float tmp1 = q->X*q->Y;					// v17 = a2 * a3;
    float tmp2 = q->Z*q->W;					// v18 = a1 * a4;
    m->up.X = 2.0 * (tmp1 + tmp2);			// a8 = v18 + v17 + v18 + v17;
    m->right.Y = 2.0 * (tmp1 - tmp2);		// a6 = v17 - v18 + v17 - v18;
    
    tmp1 = q->X*q->Z;						// v20 = a2 * a4;
    tmp2 = q->Y*q->W;						// v21 = a1 * a3;
    m->at.X = 2.0 * (tmp1 - tmp2);			// a11 = v20 - v21 + v20 - v21;
    m->right.Z 	= 2.0 * (tmp1 + tmp2);		// a7 = v21 + v20 + v21 + v20;
    tmp1 = q->Y*q->Z;						// v22 = a3 * a4;
    tmp2 = q->X*q->W;						// v23 = a1 * a2;
    m->at.Y = 2.0 * (tmp1 + tmp2);			// a12 = v23 + v22 + v23 + v22;
    m->up.Z = 2.0 * (tmp1 - tmp2);			// a10 = v22 - v23 + v22 - v23;
}