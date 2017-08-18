#include "main.h"

uintptr_t pcaInternalAim = 0x8B0808;
CAMERA_AIM caLocalPlayerAim;
CAMERA_AIM caRemotePlayerAim[PLAYER_PED_SLOTS];
uintptr_t pbyteCameraMode = 0x8B0910;

uintptr_t pfAspectRatio = 0x98525C;
uintptr_t pfCameraExtZoom = 0x8B088C;
float fCameraExtZoom[PLAYER_PED_SLOTS];		// stored as a normalized multiplier float
float fLocalCameraExtZoom;

uint8_t byteCameraMode[PLAYER_PED_SLOTS];

void GameStoreLocalPlayerCameraExtZoom()
{
	fLocalCameraExtZoom = *(float*)(g_libGTASA+pfCameraExtZoom);
}

void GameSetLocalPlayerCameraExtZoom()
{
	*(float*)(g_libGTASA+pfCameraExtZoom) = fLocalCameraExtZoom;
}

void GameSetPlayerCameraExtZoom(uint8_t bytePlayerID, float fZoom)
{
	fCameraExtZoom[bytePlayerID] = fZoom;
}

float GameGetPlayerCameraExtZoom(uint8_t bytePlayerID)
{
	return fCameraExtZoom[bytePlayerID];
}

float GameGetLocalPlayerCameraExtZoom()
{
	float value = ((*(float*)(g_libGTASA+pfCameraExtZoom)) - 35.0f) / 35.0f; // normalize for 35.0 to 70.0
	return value;
}

void GameSetRemotePlayerCameraExtZoom(uint8_t bytePlayerID)
{
	float fZoom = fCameraExtZoom[bytePlayerID];
	float fValue = fZoom * 35.0f + 35.0f; // unnormalize for 35.0 to 70.0
	*(float*)(g_libGTASA+pfCameraExtZoom) = fValue;
}

void GameSetPlayerCameraMode(uint8_t byteMode, uint8_t bytePlayerID)
{
	byteCameraMode[bytePlayerID] = byteMode;
}

uint8_t GameGetPlayerCameraMode(uint8_t bytePlayerID)
{
	return byteCameraMode[bytePlayerID];
}

uint8_t GameGetLocalPlayerCameraMode()
{
	return *(uint8_t*)(g_libGTASA+pbyteCameraMode);
}

void GameAimSyncInit()
{
	memset(&caLocalPlayerAim,0,sizeof(CAMERA_AIM));
	memset(caRemotePlayerAim,0,sizeof(CAMERA_AIM) * PLAYER_PED_SLOTS);
	memset(byteCameraMode,4,PLAYER_PED_SLOTS);
	for(int i=0; i<PLAYER_PED_SLOTS; i++)
		fCameraExtZoom[i] = 1.0f;
}

void GameStoreLocalPlayerAim()
{
	memcpy(&caLocalPlayerAim,(void*)pcaInternalAim,sizeof(CAMERA_AIM));
}

void GameSetLocalPlayerAim()
{
	memcpy((void*)pcaInternalAim,&caLocalPlayerAim,sizeof(CAMERA_AIM));
	//memcpy(pInternalCamera,&SavedCam,sizeof(MATRIX4X4));
}

CAMERA_AIM * GameGetInternalAim()
{
	return (CAMERA_AIM*)pcaInternalAim;
}

void GameStoreRemotePlayerAim(int iPlayer, CAMERA_AIM * caAim)
{
	memcpy(&caRemotePlayerAim[iPlayer],caAim,sizeof(CAMERA_AIM));
}

void GameSetRemotePlayerAim(int iPlayer)
{
	memcpy((void*)pcaInternalAim,&caRemotePlayerAim[iPlayer],sizeof(CAMERA_AIM));
}

CAMERA_AIM * GameGetRemotePlayerAim(int iPlayer)
{
    return &caRemotePlayerAim[iPlayer];
}
