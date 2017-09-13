#include "main.h"

extern CGame *pGame;
extern CNetGame *pNetGame;

CPickupPool::CPickupPool()
{
	memset(&m_Pickups[0],0,sizeof(PICKUP) * MAX_PICKUPS);
	m_iPickupCount = 0;
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		m_dwHnd[i] = NULL;
		m_iTimer[i] = NULL;
	}
}

CPickupPool::~CPickupPool()
{
	for(int i = 0; i < MAX_PICKUPS; i++)
	{
		if (m_dwHnd[i] != NULL)
		{
			ScriptCommand(&destroy_pickup, m_dwHnd[i]);
		}
	}
}

void CPickupPool::New(PICKUP *pPickup, int iPickup)
{
	if(m_iPickupCount >= MAX_PICKUPS || iPickup < 0 || iPickup >= MAX_PICKUPS) return;
	if(m_dwHnd[iPickup] != 0) ScriptCommand(&destroy_pickup, m_dwHnd[iPickup]);
	memcpy(&m_Pickups[iPickup], pPickup, sizeof (PICKUP));
	m_dwHnd[iPickup] = pGame->CreatePickup(pPickup->iModel, pPickup->iType, pPickup->fX, pPickup->fY, pPickup->fZ);
	LOGI("m_dwHnd[iPickup] = %d", m_dwHnd[iPickup]);
	m_iPickupCount++;
}

void CPickupPool::Destroy(int iPickup)
{
	if (m_iPickupCount <= 0 || iPickup < 0 || iPickup >= MAX_PICKUPS) return;
	if (m_dwHnd[iPickup] != 0)
	{
		ScriptCommand(&destroy_pickup, m_dwHnd[iPickup]);
		m_dwHnd[iPickup] = 0;
		m_iTimer[iPickup] = 0;
		m_iPickupCount--;
	}
}

/*
int CPickupPool::GetID_037(int iPickup)
{
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		if(m_dw???[i] == iPickup)
			return i;
	}

	return -1;
}
*/

void CPickupPool::PickedUp(int iPickup)
{
	int iPickup037 =0;// = GetID_037(iPickup);

	if (iPickup037 < 0 || iPickup037 >= MAX_PICKUPS) return;
	if (m_dwHnd[ iPickup037 ] != 0 && m_iTimer[ iPickup037 ] == 0) 
	{
		// Allright, we've got a normal pickup;
		RakNet::BitStream bsPickup;
		bsPickup.Write( iPickup037 );
		pNetGame->GetRakClient()->RPC(&RPC_PickedUpPickup, &bsPickup, HIGH_PRIORITY, RELIABLE_SEQUENCED,0,false,UNASSIGNED_NETWORK_ID,0);
		m_iTimer[iPickup037] = 15; // Ignore for about 5-10 seconds
	}
}

void CPickupPool::Process()
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		if (m_dwHnd[i] != 0)
		{
			if (m_Pickups[i].iType == 14)
			{
				if(ScriptCommand(&is_pickup_picked_up, m_dwHnd[i]))
				{
					LOGI("picked up %d pickup", i);
					RakNet::BitStream bsPickup;
					bsPickup.Write( i );
					pNetGame->GetRakClient()->RPC(&RPC_PickedUpPickup, &bsPickup, HIGH_PRIORITY, RELIABLE_SEQUENCED,0,false,UNASSIGNED_NETWORK_ID,0);
				}
			}
			else if(m_iTimer[i] > 0)
				m_iTimer[i]--;
		}
	}
}