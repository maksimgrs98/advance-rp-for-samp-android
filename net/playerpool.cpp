#include "main.h"

extern CGame 		*pGame;
extern CNetGame		*pNetGame;

CPlayerPool::CPlayerPool()
{
	m_pLocalPlayer = new CLocalPlayer();

	// допилить
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		m_bPlayerSlotState[playerId] = false;
		m_pPlayers[playerId] = 0;
	}
}

CPlayerPool::~CPlayerPool()
{
	LOGI("CPlayerPool::~CPlayerPool destructor");

	delete m_pLocalPlayer;

	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
		Delete(playerId, 0);
}

void CPlayerPool::SetLocalPlayerName(char *szName)
{
	strcpy(m_szLocalPlayerName, szName);
}

char *CPlayerPool::GetLocalPlayerName()
{
	return m_szLocalPlayerName;
}

char *CPlayerPool::GetPlayerName(PLAYERID playerId)
{
	return m_szPlayerNames[playerId];
}

void CPlayerPool::SetPlayerName(PLAYERID playerId, char *szName)
{
	strcpy(m_szPlayerNames[playerId], szName);
}

CLocalPlayer *CPlayerPool::GetLocalPlayer()
{
	return m_pLocalPlayer;
}


CRemotePlayer *CPlayerPool::GetAt(PLAYERID playerId)
{
	if(playerId > MAX_PLAYERS)
		return 0;

	return m_pPlayers[playerId];
}

bool CPlayerPool::GetSlotState(PLAYERID playerId)
{
	if(playerId > MAX_PLAYERS)
		return false;

	return m_bPlayerSlotState[playerId];
}

void CPlayerPool::SetLocalPlayerID(PLAYERID MyPlayerID)
{
	strcpy(m_szPlayerNames[MyPlayerID], m_szLocalPlayerName);
	m_LocalPlayerID = MyPlayerID;
}

PLAYERID CPlayerPool::GetLocalPlayerID()
{
	return m_LocalPlayerID;
}

bool CPlayerPool::New(PLAYERID playerId, char* szPlayerName)
{
	m_pPlayers[playerId] = new CRemotePlayer();

	if(m_pPlayers[playerId])
	{
		strcpy(m_szPlayerNames[playerId], szPlayerName);
		m_pPlayers[playerId]->SetID(playerId);
		m_bPlayerSlotState[playerId] = true;
		return true;
	}
	else
		return false;
}

bool CPlayerPool::Delete(PLAYERID playerId, uint8_t byteReason)
{
	if(!GetSlotState(playerId) || !m_pPlayers[playerId])
		return false;

	m_bPlayerSlotState[playerId] = false;
	delete m_pPlayers[playerId];
	m_pPlayers[playerId] = 0;

	return true;
}

bool CPlayerPool::Process()
{
	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		if(m_bPlayerSlotState[playerId] == true)
		{
			m_pPlayers[playerId]->Process();
		}
	}

	m_pLocalPlayer->Process();

	return true;
}


PLAYERID CPlayerPool::FindRemotePlayerIDFromGtaPtr(PED_TYPE *pActor)
{
	CPlayerPed *pPlayerPed;

	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++)
	{
		if(m_bPlayerSlotState[playerId])
		{
			pPlayerPed = m_pPlayers[playerId]->GetPlayerPed();

			if(pPlayerPed)
			{
				PED_TYPE *pTestActor = pPlayerPed->GetGtaActor();
				if(pTestActor && (pActor == pTestActor))
					return m_pPlayers[playerId]->GetID();
			}
		}
	}

	return INVALID_PLAYER_ID;
}

void CPlayerPool::DeactivateAll()
{
	LOGI("CPlayerPool::DeactivateAll");
	m_pLocalPlayer->m_bIsActive = false;
	//m_pLocalPlayer->m_iSelectedClass = 0;

	for(PLAYERID playerId = 0; playerId < MAX_PLAYERS; playerId++) 
	{
		if(m_bPlayerSlotState[playerId])
			m_pPlayers[playerId]->Deactivate();
	}
}