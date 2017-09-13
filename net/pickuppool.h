#pragma once

#define MAX_PICKUPS 400

#pragma pack(1)
typedef struct _PICKUP
{
	int iModel;
	int iType;
	float fX;
	float fY;
	float fZ;
} PICKUP;

class CPickupPool
{
private:
	PICKUP 		m_Pickups[MAX_PICKUPS];
	int			m_iPickupCount;
	uint32_t 	m_dwHnd[MAX_PICKUPS];
	uint32_t	m_iTimer[MAX_PICKUPS];

public:
	CPickupPool();
	~CPickupPool();

	void New(PICKUP *pPickup, int iPickup);
	void Destroy(int iPickup);
	void PickedUp(int iPickup);
	void Process();
	int  GetID_037(int iPickup);
};