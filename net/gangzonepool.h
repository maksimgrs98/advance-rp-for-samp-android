#pragma once

typedef struct _GANG_ZONE
{
	float 		fPos[4];
	uint32_t 	dwColor;
	uint32_t 	dwAltColor;
} GANG_ZONE;

class CGangZonePool
{
private:
	GANG_ZONE 		*m_pGangZone[MAX_GANG_ZONES];
	bool			m_bSlotState[MAX_GANG_ZONES];

public:
	CGangZonePool();
	~CGangZonePool();
	void New(uint16_t wZone, float fMinX, float fMinY, float fMaxX, float fMaxY, uint32_t dwColor);
	void Flash(uint16_t wZone, uint32_t dwColor);
	void StopFlash(uint16_t wZone);
	void Delete(uint16_t wZone);
	void Draw();
};