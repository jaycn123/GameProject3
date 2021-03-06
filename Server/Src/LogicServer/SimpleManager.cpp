﻿#include <stdafx.h>
#include "SimpleManager.h"
#include "GameService.h"
#include "CommonFunc.h"
#include "Log.h"

CSimpleManager::CSimpleManager()
{

}

CSimpleManager::~CSimpleManager()
{

}

CSimpleManager* CSimpleManager::GetInstancePtr()
{
	static CSimpleManager _StaticMgr;

	return &_StaticMgr;
}

BOOL CSimpleManager::LoadSimpleData(CppMySQL3DB& tDBConnection)
{
	CppMySQLQuery QueryResult = tDBConnection.querySQL("SELECT * FROM player");
	while(!QueryResult.eof())
	{
		CSimpleInfo* pInfo = CreateSimpleInfo(QueryResult.getInt64Field("id"),
		                                      QueryResult.getInt64Field("account_id"),
		                                      QueryResult.getStringField("name"),
		                                      QueryResult.getIntField("carrerid"));

		QueryResult.nextRow();
	}

	return TRUE;
}

UINT64 CSimpleManager::GetRoleIDByName( std::string Name )
{
	std::map<std::string, UINT64>::iterator itor = m_mapName2ID.find(Name);
	if(itor != m_mapName2ID.end())
	{
		return itor->second;
	}

	return 0;
}

CSimpleInfo* CSimpleManager::GetSimpleInfoByID( UINT64 u64ID )
{
	std::map<UINT64, CSimpleInfo*>::iterator itor =  m_mapID2Simple.find(u64ID);
	if (itor != m_mapID2Simple.end())
	{
		return itor->second;
	}

	return NULL;
}

UINT32 CSimpleManager::GetPlayerLogoffTime( UINT64 u64ID )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_NULL(pInfo != NULL);

	return pInfo->dwLogoffTime;
}

UINT32 CSimpleManager::Get_FightValue( UINT64 u64ID )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_NULL(pInfo != NULL);

	return pInfo->dwFightValue;
}

BOOL CSimpleManager::Set_FightValue( UINT64 u64ID, UINT32 dwFight, UINT32 dwLevel )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->dwFightValue = dwFight;
	pInfo->dwLevel = dwLevel;

	return TRUE;
}

BOOL CSimpleManager::Set_PlayerName( UINT64 u64ID, std::string strName )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->Name = strName;
	return TRUE;
}

BOOL CSimpleManager::Set_LogoffTime( UINT64 u64ID, UINT32 dwTime )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->dwLogoffTime = dwTime;

	return TRUE;
}

BOOL CSimpleManager::Set_VipLevel( UINT64 u64ID, UINT32 dwVipLvl )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->dwVipLevel = dwVipLvl;

	return TRUE;
}

BOOL CSimpleManager::Set_LoginDay( UINT64 u64ID, UINT32 dwDay )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->dwLoginDay = dwDay;

	return TRUE;
}

BOOL CSimpleManager::Set_GuildID( UINT64 u64ID, UINT32 guildid )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);

	pInfo->dwGuildID = guildid;

	return TRUE;
}

BOOL CSimpleManager::CheckNameExist(std::string strName)
{
	std::map<std::string, UINT64>::iterator itor = m_mapName2ID.find(strName);
	if(itor != m_mapName2ID.end())
	{
		return TRUE;
	}

	return FALSE;
}

UINT32 CSimpleManager::Get_GuildID( UINT64 u64ID )
{
	CSimpleInfo* pInfo = GetSimpleInfoByID(u64ID);
	ERROR_RETURN_FALSE(pInfo != NULL);
	return  pInfo->dwGuildID;
}

CSimpleInfo* CSimpleManager::CreateSimpleInfo( UINT64 u64ID, UINT64 u64AccID, std::string strName, UINT32 dwCarrerID)
{
	CSimpleInfo* pInfo = new CSimpleInfo();
	pInfo->u64RoleID = u64ID;
	pInfo->u64AccountID = u64AccID;
	pInfo->Name = strName;
	pInfo->dwCarrerID = dwCarrerID;
	pInfo->IsOnline = TRUE;

	m_mapID2Simple.insert(std::make_pair(u64ID, pInfo));
	m_mapName2ID.insert(std::make_pair(strName, u64ID));

	return pInfo;
}
