#pragma once

#define PLAYER_STATE_NONE						0
#define PLAYER_STATE_ONFOOT						1
#define PLAYER_STATE_DRIVER						2
#define PLAYER_STATE_PASSENGER					3
#define PLAYER_STATE_EXIT_VEHICLE				4
#define PLAYER_STATE_ENTER_VEHICLE_DRIVER		5
#define PLAYER_STATE_ENTER_VEHICLE_PASSENGER	6
#define PLAYER_STATE_WASTED						7
#define PLAYER_STATE_SPAWNED					8
#define PLAYER_STATE_SPECTATING					9

#define UPDATE_TYPE_NONE		0
#define UPDATE_TYPE_ONFOOT		1
#define UPDATE_TYPE_INCAR		2
#define UPDATE_TYPE_PASSENGER	3

class CRemotePlayer
{
public:
	CRemotePlayer();
	~CRemotePlayer();

	void UpdateRotation();

	void StateChange(uint8_t byteNewState, uint8_t byteOldState);
	void SetState(uint8_t byteState);
	uint8_t GetState();

	void Process();

	void HandleVehicleEntryExit();

	void SetID(PLAYERID playerId);
	PLAYERID GetID();
	CPlayerPed *GetPlayerPed();

	bool IsActive();

	void Deactivate();

	void UpdateIncarTargetPosition();
	void UpdateOnfootTargetPosition();

	void UpdateOnFootPositionAndSpeed(VECTOR * vecPos, VECTOR *vecMoveSpeed);
	void UpdateInCarMatrixAndSpeed(MATRIX4X4 mat, VECTOR vecMove);

	void StoreOnFootFullSyncData(ONFOOT_SYNC_DATA *pofSync);
	void StoreInCarFullSyncData(INCAR_SYNC_DATA *picSync);
	void StorePassengerFullSyncData(PASSENGER_SYNC_DATA *ppsSync);

	bool Spawn(uint8_t byteTeam, int iSkin, VECTOR *vecPos, float fRotation, uint32_t dwColor, uint8_t byteFightingStyle = 4, bool bVisible = true);
	void Remove();

	void ResetAllSyncAttributes();

	float GetDistanceFromLocalPlayer();

	float GetReportedHealth() { return m_fReportedHealth; };

	void EnterVehicle(VEHICLEID VehicleID, bool bPassenger);
	void ExitVehicle();

	CPlayerPed		*m_pPlayerPed;
	CVehicle		*m_pCurrentVehicle;

	PLAYERID 		m_PlayerID;
	uint8_t			m_byteUpdateFromNetwork;
	uint8_t			m_byteState;
	VEHICLEID		m_VehicleID;
	uint8_t			m_byteSeatID;

	ONFOOT_SYNC_DATA		m_ofSync;
	INCAR_SYNC_DATA			m_icSync;
	PASSENGER_SYNC_DATA		m_psSync;

	VECTOR			m_vecOnfootTargetPos;
	VECTOR			m_vecOnfootTargetSpeed;
	VECTOR			m_vecIncarTargetPos;
	VECTOR			m_vecIncarTargetSpeed;

	float 			m_fReportedHealth;
	float 			m_fReportedArmour;
	uint32_t		m_dwWaitForEntryExitAnims;	
};