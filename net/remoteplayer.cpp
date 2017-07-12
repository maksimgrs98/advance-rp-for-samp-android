#include "main.h"
#include "game/util.h"

extern CGame 		*pGame;
extern CNetGame		*pNetGame;

CRemotePlayer::CRemotePlayer()
{
	m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
	m_byteState = PLAYER_STATE_NONE;
	m_PlayerID = INVALID_PLAYER_ID;
	m_pPlayerPed = 0;
	m_VehicleID = 0;
	m_dwWaitForEntryExitAnims = GetTickCount();
}

CRemotePlayer::~CRemotePlayer()
{
	if(m_pPlayerPed)
	{
		m_pPlayerPed->Destroy();
		delete m_pPlayerPed;
		m_pPlayerPed = 0;
	}
}

void CRemotePlayer::Process()
{
	CPlayerPool *pPool = pNetGame->GetPlayerPool();
	CLocalPlayer *pLocalPlayer = pPool->GetLocalPlayer();
	MATRIX4X4 matVehicle;
	VECTOR vecMoveSpeed;

	if(IsActive())
	{
		if(m_pPlayerPed)
		{
			if(GetTickCount() > m_dwWaitForEntryExitAnims)
				HandleVehicleEntryExit();

			// ---- ONFOOT NETWORK PROCESSING ----
			if(GetState() == PLAYER_STATE_ONFOOT && m_byteUpdateFromNetwork == UPDATE_TYPE_ONFOOT)
			{
				if(GetTickCount() > m_dwWaitForEntryExitAnims)
				HandleVehicleEntryExit();

				//if (!IsSurfingOrTurretMode())
				//{
					UpdateOnFootPositionAndSpeed(&m_ofSync.vecPos,&m_ofSync.vecMoveSpeed);
					UpdateOnfootTargetPosition();
				//}

				m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
			}

			// ---- DRIVER NETWORK PROCESSING ----
			else if(GetState() == PLAYER_STATE_DRIVER && m_byteUpdateFromNetwork == UPDATE_TYPE_INCAR && m_pPlayerPed->IsInVehicle())
			{
				if(!m_pCurrentVehicle) return;

				// MATRIX
				ConvertQuaternionToMatrix(&matVehicle, &m_icSync.Quat);
				matVehicle.pos.X = m_icSync.vecPos.X;
				matVehicle.pos.Y = m_icSync.vecPos.Y;
				matVehicle.pos.Z = m_icSync.vecPos.Z;

				// MOVE SPEED
				vecMoveSpeed.X = m_icSync.vecMoveSpeed.X;
				vecMoveSpeed.Y = m_icSync.vecMoveSpeed.Y;
				vecMoveSpeed.Z = m_icSync.vecMoveSpeed.Z;

				if( m_pCurrentVehicle->GetModelIndex() == TRAIN_PASSENGER_LOCO || 
					m_pCurrentVehicle->GetModelIndex() == TRAIN_FREIGHT_LOCO ||
					m_pCurrentVehicle->GetModelIndex() == TRAIN_TRAM) 
				{
					// TRAIN MATRIX UPDATE
				}
				else
				{
					// GENERIC VEHICLE MATRIX UPDATE
					UpdateInCarMatrixAndSpeed(matVehicle, vecMoveSpeed);
					UpdateIncarTargetPosition();
				}

				m_pCurrentVehicle->SetHealth(m_icSync.fCarHealth);

				m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
			}

			// ---- PASSENGER NETWORK PROCESSING ----
			else if(GetState() == PLAYER_STATE_PASSENGER && m_byteUpdateFromNetwork == UPDATE_TYPE_PASSENGER)
			{
				if(!m_pCurrentVehicle) return;
				// UPDATE CURRENT WEAPON

				// FOR INITIALISING PASSENGER DRIVEBY

				m_byteUpdateFromNetwork = UPDATE_TYPE_NONE;
			}

			// ------ PROCESSED FOR ALL FRAMES ----- 
			if(GetState() == PLAYER_STATE_ONFOOT)
			{
				UpdateRotation(); // SLERP
				// UpdateHead();
				// ProcessSpecialAction();
				m_pPlayerPed->SetOFKeys(m_ofSync.wKeys, m_ofSync.lrAnalog, m_ofSync.udAnalog);
				// test
				m_pPlayerPed->SetMoveSpeedVector(m_ofSync.vecMoveSpeed);
			}
			else if(GetState() == PLAYER_STATE_DRIVER)
			{
				m_pPlayerPed->SetICKeys(m_icSync.wKeys, m_icSync.lrAnalog, m_icSync.udAnalog);
			}
			else if(GetState() == PLAYER_STATE_PASSENGER)
			{
				m_pPlayerPed->SetICKeys(0, 0, 0);
				m_pPlayerPed->SetOFKeys(0, 0, 0);
			}

			m_pPlayerPed->SetHealth(100.0f);
		}
	}
	else
	{
		if(m_pPlayerPed)
		{
			ResetAllSyncAttributes();
			m_pPlayerPed->Destroy();
			delete m_pPlayerPed;
			m_pPlayerPed = 0;
		}
	}
}

void CRemotePlayer::HandleVehicleEntryExit()
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
	MATRIX4X4 mat;

	if(!m_pPlayerPed) return;

	if(GetState() == PLAYER_STATE_ONFOOT) 
	{
		if(m_pPlayerPed->IsInVehicle()) 
		{
			m_pPlayerPed->GetMatrix(&mat);
			m_pPlayerPed->RemoveFromVehicleAndPutAt(mat.pos.X,mat.pos.Y,mat.pos.Z);
		}
		return;
	}

	if( GetState() == PLAYER_STATE_DRIVER || GetState() == PLAYER_STATE_PASSENGER )
	{
		if(!m_pPlayerPed->IsInVehicle())
		{
			// if they're onfoot, put them in the vehicle.
			CVehicle *pVehicle = pVehiclePool->GetAt(m_VehicleID);
			if(pVehicle) 
			{
				int iCarID = pVehiclePool->FindGtaIDFromID(m_VehicleID);
				m_pPlayerPed->PutDirectlyInVehicle(iCarID,m_byteSeatID);
			}
		} 
		else 
		{
			// validate the vehicle they're driving is the one they're
			// reporting to be in.
			int iCurrentVehicle = m_pPlayerPed->GetCurrentVehicleID();
			int iReportedVehicle = pVehiclePool->FindGtaIDFromID(m_VehicleID);
			if(iCurrentVehicle != iReportedVehicle)
				m_pPlayerPed->PutDirectlyInVehicle(iReportedVehicle,m_byteSeatID);
		}
	}
}

void CRemotePlayer::UpdateRotation()
{
	MATRIX4X4 matEnt;

	if(!m_pPlayerPed) return;

	m_pPlayerPed->GetMatrix(&matEnt);
	ConvertQuaternionToMatrix(&matEnt, &m_ofSync.Quat);
	m_pPlayerPed->SetMatrix(matEnt);
	// atan2(r21, r11)
	float fZ = atan2(-matEnt.up.X, matEnt.right.X);

	m_pPlayerPed->m_pPed->fRotation1 = fZ;
	m_pPlayerPed->m_pPed->fRotation2 = fZ;
}

void CRemotePlayer::UpdateOnfootTargetPosition()
{
	MATRIX4X4 matEnt;
	VECTOR vec;

	if(!m_pPlayerPed) return;
	m_pPlayerPed->GetMatrix(&matEnt);

	float fx = FloatOffset(m_vecOnfootTargetPos.X, matEnt.pos.X);
	float fy = FloatOffset(m_vecOnfootTargetPos.Y, matEnt.pos.Y);
	float fz = FloatOffset(m_vecOnfootTargetPos.Z, matEnt.pos.Z);

	if((fx > 0.00001f) || (fy > 0.00001f) || (fz > 0.00001f))
	{
		if( fx > 2.0 || fy > 2.0 || fz > 1.0 )
		{
			matEnt.pos.X = m_vecOnfootTargetPos.X;
			matEnt.pos.Y = m_vecOnfootTargetPos.Y;
			matEnt.pos.Z = m_vecOnfootTargetPos.Z;

			m_pPlayerPed->SetMatrix(matEnt);
			return;
		}

		m_pPlayerPed->GetMoveSpeedVector(&vec);

		float fMultiplyAmount = 0.1f;

		if( fx > 0.00001f )
		vec.X += (m_vecOnfootTargetPos.X - matEnt.pos.X) * fMultiplyAmount;
		if( fy > 0.00001f )
		vec.Y += (m_vecOnfootTargetPos.Y - matEnt.pos.Y) * fMultiplyAmount;
		if( fz > 0.00001f )
		vec.Z += (m_vecOnfootTargetPos.Z - matEnt.pos.Z) * fMultiplyAmount;

		m_pPlayerPed->SetMoveSpeedVector(vec);
	}
}

void CRemotePlayer::UpdateOnFootPositionAndSpeed(VECTOR * vecPos, VECTOR * vecMove)
{
    m_vecOnfootTargetPos.X = vecPos->X;
	m_vecOnfootTargetPos.Y = vecPos->Y;
	m_vecOnfootTargetPos.Z = vecPos->Z;
    m_vecOnfootTargetSpeed.X = vecMove->X;
	m_vecOnfootTargetSpeed.Y = vecMove->Y;
	m_vecOnfootTargetSpeed.Z = vecMove->Z;
}

void CRemotePlayer::UpdateIncarTargetPosition()
{
	// допилить

	MATRIX4X4 matEnt;
	VECTOR vec = { 0.0f, 0.0f, 0.0f };

	if(!m_pCurrentVehicle) return;

	m_pCurrentVehicle->GetMatrix(&matEnt);

	// Directly translate way-out position
	if(	(FloatOffset(m_vecIncarTargetPos.X, matEnt.pos.X) > 8.0f) ||
		(FloatOffset(m_vecIncarTargetPos.Y, matEnt.pos.Y) > 8.0f) ||
		(FloatOffset(m_vecIncarTargetPos.Z, matEnt.pos.Z) > 8.0f) ) 
	{
		matEnt.pos.X = m_vecIncarTargetPos.X;
		matEnt.pos.Y = m_vecIncarTargetPos.Y;
		matEnt.pos.Z = m_vecIncarTargetPos.Z;

		m_pCurrentVehicle->SetMatrix(matEnt);
        m_pCurrentVehicle->SetMoveSpeedVector(m_vecIncarTargetSpeed);
		return;
	}

	m_pCurrentVehicle->GetMoveSpeedVector(&vec);

	if( FloatOffset(m_vecIncarTargetPos.X,matEnt.pos.X) > 0.05f )
		vec.X += (m_vecIncarTargetPos.X - matEnt.pos.X) * 0.05f;

	if( FloatOffset(m_vecIncarTargetPos.Y,matEnt.pos.Y) > 0.05f )
		vec.Y += (m_vecIncarTargetPos.Y - matEnt.pos.Y) * 0.05f;

	if( FloatOffset(m_vecIncarTargetPos.Z,matEnt.pos.Z) > 0.05f )
		vec.Z += (m_vecIncarTargetPos.Z - matEnt.pos.Z) * 0.05f;
	
	m_pCurrentVehicle->SetMoveSpeedVector(vec);
}

void CRemotePlayer::UpdateInCarMatrixAndSpeed(MATRIX4X4 mat, VECTOR vecMove)
{
	// допилить

	MATRIX4X4 matEnt;
	m_pCurrentVehicle->GetMatrix(&matEnt);

	CPlayerPed *pPlayer = pGame->FindPlayerPed();

	matEnt.right.X = mat.right.X;
	matEnt.right.Y = mat.right.Y;
	matEnt.right.Z = mat.right.Z;

	matEnt.up.X = mat.up.X;
	matEnt.up.Y = mat.up.Y;
	matEnt.up.Z = mat.up.Z;

	m_vecIncarTargetPos.X = mat.pos.X;
	m_vecIncarTargetPos.Y = mat.pos.Y;
	m_vecIncarTargetPos.Z = mat.pos.Z;

	m_vecIncarTargetSpeed.X = vecMove.X;
	m_vecIncarTargetSpeed.Y = vecMove.Y;
	m_vecIncarTargetSpeed.Z = vecMove.Z;

	m_pCurrentVehicle->SetMoveSpeedVector(vecMove);
	m_pCurrentVehicle->SetMatrix(matEnt);
}

void CRemotePlayer::StoreOnFootFullSyncData(ONFOOT_SYNC_DATA *pofSync)
{
	if(GetTickCount() < m_dwWaitForEntryExitAnims) return;

	//m_pCurrentVehicle = 0;
	memcpy(&m_ofSync,pofSync,sizeof(ONFOOT_SYNC_DATA));
	m_fReportedHealth = (float)pofSync->byteHealth;
	m_fReportedArmour = (float)pofSync->byteArmour;
	m_byteUpdateFromNetwork = UPDATE_TYPE_ONFOOT;

	SetState(PLAYER_STATE_ONFOOT);
}

void CRemotePlayer::StoreInCarFullSyncData(INCAR_SYNC_DATA *picSync)
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(GetTickCount() < m_dwWaitForEntryExitAnims) return;

	//if(a3 || a3 - *(dword*)(this+457) < 0) return; ???
	// *(dword*)(this+457) = a3; ???

	memcpy(&m_icSync, picSync, sizeof(INCAR_SYNC_DATA));
	m_VehicleID = picSync->VehicleID;
	if(pVehiclePool) m_pCurrentVehicle = pVehiclePool->GetAt(m_VehicleID);

	m_byteSeatID = 0;
	m_fReportedHealth = (float)picSync->bytePlayerHealth;
	m_fReportedArmour = (float)picSync->bytePlayerArmour;
	m_byteUpdateFromNetwork = UPDATE_TYPE_INCAR;

	SetState(PLAYER_STATE_DRIVER);
}

void CRemotePlayer::StorePassengerFullSyncData(PASSENGER_SYNC_DATA *ppsSync)
{
	if(GetTickCount() < m_dwWaitForEntryExitAnims) return;

	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	memcpy(&m_psSync,ppsSync,sizeof(PASSENGER_SYNC_DATA));
	m_VehicleID = ppsSync->VehicleID;
	m_byteSeatID = ppsSync->byteSeatFlags & 127;
	if (pVehiclePool) m_pCurrentVehicle = pVehiclePool->GetAt(m_VehicleID);
	m_fReportedHealth = (float)ppsSync->bytePlayerHealth;
	m_fReportedArmour = (float)ppsSync->bytePlayerArmour;
	m_byteUpdateFromNetwork = UPDATE_TYPE_PASSENGER;

	SetState(PLAYER_STATE_PASSENGER);
}

bool CRemotePlayer::Spawn(uint8_t byteTeam, int iSkin, VECTOR *vecPos, float fRotation, uint32_t dwColor, uint8_t byteFightingStyle, bool bVisible)
{
	//if(!pGame->IsGameLoaded()) return false;
	if(m_pPlayerPed != 0)
	{
		m_pPlayerPed->Destroy();
		delete m_pPlayerPed;
	}

	LOGI("pGame->NewPlayer");
	CPlayerPed *pPlayer = pGame->NewPlayer(iSkin, vecPos->X, vecPos->Y, vecPos->Z, fRotation);

	if(pPlayer)
	{
		/*if(pNetGame->m_bShowPlayerMarkers)*/
		 pPlayer->ShowMarker(m_PlayerID);

		m_pPlayerPed = pPlayer;
		pPlayer->SetOFKeys(0, 0, 0);
		//pPlayer->SetFightingStyle(byteFightingStyle);
		pPlayer->SetVisible(bVisible);

		SetState(PLAYER_STATE_SPAWNED);

		return true;
	}

	SetState(PLAYER_STATE_NONE);
	return false;
}

void CRemotePlayer::Remove()
{
	if(m_pPlayerPed != 0) 
	{
		m_pPlayerPed->Destroy();
		delete m_pPlayerPed;
		m_pPlayerPed = 0;
	}

	Deactivate();
}

void CRemotePlayer::ResetAllSyncAttributes()
{	
	m_VehicleID = 0;
	memset(&m_ofSync,0,sizeof(ONFOOT_SYNC_DATA));
	memset(&m_icSync,0,sizeof(INCAR_SYNC_DATA));
	m_fReportedHealth = 0.0f;
	m_fReportedArmour = 0.0f;
	m_pCurrentVehicle = 0;
	m_byteSeatID = 0;
	//m_bPassengerDriveByMode = false;
}

void CRemotePlayer::SetID(PLAYERID playerId)
{
	m_PlayerID = playerId; 
}

PLAYERID CRemotePlayer::GetID()
{
	return m_PlayerID;
}

CPlayerPed *CRemotePlayer::GetPlayerPed()
{
	return m_pPlayerPed;
}

void CRemotePlayer::SetState(uint8_t byteState)
{
	if(byteState != m_byteState)
	{
		StateChange(byteState, m_byteState);
		m_byteState = byteState;
	}
}

bool CRemotePlayer::IsActive()
{
	if(m_pPlayerPed && m_byteState != PLAYER_STATE_NONE && m_byteState != PLAYER_STATE_SPECTATING)
		return true;

	return false;
}

void CRemotePlayer::Deactivate() 
{
	m_byteState = PLAYER_STATE_NONE;
};

uint8_t CRemotePlayer::GetState()
{
	return m_byteState;
}

float CRemotePlayer::GetDistanceFromLocalPlayer()
{
	if(!m_pPlayerPed) return 10000.0f; // very far away

	if(GetState() == PLAYER_STATE_DRIVER && m_pCurrentVehicle) {
		return m_pCurrentVehicle->GetDistanceFromLocalPlayerPed();
	} else {
		return m_pPlayerPed->GetDistanceFromLocalPlayerPed();
	}
}

void CRemotePlayer::EnterVehicle(VEHICLEID VehicleID, bool bPassenger)
{
	CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();

	if(m_pPlayerPed && !m_pPlayerPed->IsInVehicle())
	{
		if(bPassenger)
			SetState(PLAYER_STATE_ENTER_VEHICLE_PASSENGER);
		else
			SetState(PLAYER_STATE_ENTER_VEHICLE_DRIVER);

		m_pPlayerPed->SetOFKeys(0, 0, 0);
		m_pPlayerPed->SetICKeys(0, 0, 0);
		if(m_pPlayerPed->GetDistanceFromLocalPlayerPed() < 120.0f) 
		{
			int iGtaVehicleID = pVehiclePool->FindGtaIDFromID(VehicleID);
			if(iGtaVehicleID && iGtaVehicleID != INVALID_VEHICLE_ID)
				m_pPlayerPed->EnterVehicle(iGtaVehicleID,bPassenger);
		}
	}

	m_dwWaitForEntryExitAnims = GetTickCount() + 500;
}

void CRemotePlayer::ExitVehicle()
{
	if(m_pPlayerPed && m_pPlayerPed->IsInVehicle()) 
	{
		SetState(PLAYER_STATE_EXIT_VEHICLE);
		m_pPlayerPed->SetOFKeys(0, 0, 0);
		m_pPlayerPed->SetICKeys(0, 0, 0);
		if(m_pPlayerPed->GetDistanceFromLocalPlayerPed() < 120.0f)
			m_pPlayerPed->ExitCurrentVehicle();
	}

	m_dwWaitForEntryExitAnims = GetTickCount() + 500;
}

// CALLBACK
void CRemotePlayer::StateChange(uint8_t byteNewState, uint8_t byteOldState)
{
	if(byteNewState == PLAYER_STATE_DRIVER && byteOldState == PLAYER_STATE_ONFOOT)
	{
		CPlayerPed *pLocalPlayerPed = pGame->FindPlayerPed();
		CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
		VEHICLEID LocalVehicle = INVALID_VEHICLE_ID;
		MATRIX4X4 mat;

		if(pLocalPlayerPed && pLocalPlayerPed->IsInVehicle() && !pLocalPlayerPed->IsAPassenger() && pVehiclePool)
		{
			LocalVehicle = pVehiclePool->FindIDFromGtaPtr(pLocalPlayerPed->GetGtaVehicle());
			if(LocalVehicle == m_VehicleID)
			{
				pLocalPlayerPed->GetMatrix(&mat);
				pLocalPlayerPed->RemoveFromVehicleAndPutAt(mat.pos.X,mat.pos.Y,mat.pos.Z + 1.0f);
				//pGame->DisplayGameText("~r~Car Jacked~w~!",1000,5);
			}
		}
	}
}