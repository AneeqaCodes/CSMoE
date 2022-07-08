/*
*
*   This program is free software; you can redistribute it and/or modify it
*   under the terms of the GNU General Public License as published by the
*   Free Software Foundation; either version 2 of the License, or (at
*   your option) any later version.
*
*   This program is distributed in the hope that it will be useful, but
*   WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*   General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software Foundation,
*   Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*   In addition, as a special exception, the author gives permission to
*   link the code of this program with the Half-Life Game Engine ("HL
*   Engine") and Modified Game Libraries ("MODs") developed by Valve,
*   L.L.C ("Valve").  You must obey the GNU General Public License in all
*   respects for all of the code used other than the HL Engine and MODs
*   from Valve.  If you modify this file, you may extend this exception
*   to your version of the file, but you are not obligated to do so.  If
*   you do not wish to do so, delete this exception statement from your
*   version.
*
*/

#ifndef DOORS_H
#define DOORS_H
#ifdef _WIN32
#pragma once
#endif

#include "cbase/cbase_locksound.h"
#include "func_break.h"

namespace sv {

constexpr auto DOOR_SENTENCEWAIT = 6s;
constexpr auto DOOR_SOUNDWAIT = 3s;
constexpr auto BUTTON_SOUNDWAIT = 0.5s;

constexpr int SF_DOOR_ROTATE_Y = 0;
constexpr int SF_DOOR_START_OPEN = 1;
constexpr int SF_DOOR_ROTATE_BACKWARDS = 2;
constexpr int SF_DOOR_PASSABLE = 8;
constexpr int SF_DOOR_ONEWAY = 16;
constexpr int SF_DOOR_NO_AUTO_RETURN = 32;
constexpr int SF_DOOR_ROTATE_Z = 64;
constexpr int SF_DOOR_ROTATE_X = 128;
constexpr int SF_DOOR_USE_ONLY = 256;        // door must be opened by player's use button.
constexpr int SF_DOOR_NOMONSTERS = 512;        // Monster can't open
constexpr int SF_DOOR_SILENT = 0x80000000;

class CBaseDoor : public CBaseToggle
{
public:
	virtual void Spawn();
	virtual void Precache();
	virtual void Restart();
	virtual void KeyValue(KeyValueData *pkvd);
	virtual int Save(CSave &save);
	virtual int Restore(CRestore &restore);
	virtual int ObjectCaps()
	{
		if (pev->spawnflags & SF_ITEM_USE_ONLY)
			return (CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION) | FCAP_IMPULSE_USE;
		else
			return (CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION);
	}
	virtual void SetToggleState(int state);
	virtual void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	virtual void Blocked(CBaseEntity *pOther);

public:
	static TYPEDESCRIPTION m_SaveData[7];

	// used to selectivly override defaults
	void EXPORT DoorTouch(CBaseEntity *pOther);
	int DoorActivate();
	void EXPORT DoorGoUp();
	void EXPORT DoorGoDown();
	void EXPORT DoorHitTop();
	void EXPORT DoorHitBottom();

public:
	BYTE m_bHealthValue;        // some doors are medi-kit doors, they give players health

	BYTE m_bMoveSnd;        // sound a door makes while moving
	BYTE m_bStopSnd;        // sound a door makes when it stops

	locksound_t m_ls;        // door lock sounds

	BYTE m_bLockedSound;        // ordinals from entity selection
	BYTE m_bLockedSentence;
	BYTE m_bUnlockedSound;
	BYTE m_bUnlockedSentence;

	time_point_t m_lastBlockedTimestamp;
};

class CRotDoor : public CBaseDoor
{
public:
	virtual void Spawn();
	virtual void Precache();
	virtual void Restart();
	virtual void KeyValue(KeyValueData* pkvd);
	virtual void SetToggleState(int state);
	// breakables use an overridden takedamage
	virtual int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType);
	virtual int DamageDecal(int bitsDamageType);

public:
	void DamageSound();
	BOOL IsBreakable();
	void EXPORT Die();

	static void MaterialSoundPrecache(Materials precacheMaterial);
	static void MaterialSoundRandom(edict_t* pEdict, Materials soundMaterial, float volume);
	static const char** MaterialSoundList(Materials precacheMaterial, int& soundCount);

public:
	Materials m_Material;
	Explosions m_Explosion;
	int m_idShard;
	float m_angle;
	int m_iszGibModel;
	float m_flHealth;

	static const char* pSoundsWood[3];
	static const char* pSoundsFlesh[6];
	static const char* pSoundsGlass[3];
	static const char* pSoundsMetal[3];
	static const char* pSoundsConcrete[3];
	static const char* pSpawnObjects[32];
};

class CMomentaryDoor : public CBaseToggle
{
public:
	virtual void Spawn();
	virtual void Precache();
	virtual void KeyValue(KeyValueData *pkvd);
	virtual int Save(CSave &save);
	virtual int Restore(CRestore &restore);
	virtual int ObjectCaps() { return (CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION); }
	virtual void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);

public:
	static TYPEDESCRIPTION m_SaveData[1];

	BYTE m_bMoveSnd;    // sound a door makes while moving
};

void PlayLockSounds(entvars_t *pev, locksound_t *pls, int flocked, int fbutton);

}

#endif // DOORS_H
