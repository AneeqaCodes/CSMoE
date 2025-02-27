
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "trains.h"
#include "saverestore.h"
#include "plats.h"

namespace sv {

/*
* Globals initialization
*/
TYPEDESCRIPTION CBasePlatTrain::m_SaveData[] =
		{
				DEFINE_FIELD(CBasePlatTrain, m_bMoveSnd, FIELD_CHARACTER),
				DEFINE_FIELD(CBasePlatTrain, m_bStopSnd, FIELD_CHARACTER),
				DEFINE_FIELD(CBasePlatTrain, m_volume, FIELD_FLOAT),
		};

TYPEDESCRIPTION CFuncPlatRot::m_SaveData[] =
		{
				DEFINE_FIELD(CFuncPlatRot, m_end, FIELD_VECTOR),
				DEFINE_FIELD(CFuncPlatRot, m_start, FIELD_VECTOR),
		};

TYPEDESCRIPTION CFuncTrain::m_SaveData[] =
		{
				DEFINE_FIELD(CFuncTrain, m_sounds, FIELD_INTEGER),
				DEFINE_FIELD(CFuncTrain, m_pevCurrentTarget, FIELD_EVARS),
				DEFINE_FIELD(CFuncTrain, m_activated, FIELD_BOOLEAN),
		};

TYPEDESCRIPTION CFuncTrackTrain::m_SaveData[] =
		{
				DEFINE_FIELD(CFuncTrackTrain, m_ppath, FIELD_CLASSPTR),
				DEFINE_FIELD(CFuncTrackTrain, m_length, FIELD_FLOAT),
				DEFINE_FIELD(CFuncTrackTrain, m_height, FIELD_FLOAT),
				DEFINE_FIELD(CFuncTrackTrain, m_speed, FIELD_FLOAT),
				DEFINE_FIELD(CFuncTrackTrain, m_dir, FIELD_FLOAT),
				DEFINE_FIELD(CFuncTrackTrain, m_startSpeed, FIELD_FLOAT),
				DEFINE_FIELD(CFuncTrackTrain, m_controlMins, FIELD_VECTOR),
				DEFINE_FIELD(CFuncTrackTrain, m_controlMaxs, FIELD_VECTOR),
				DEFINE_FIELD(CFuncTrackTrain, m_sounds, FIELD_INTEGER),
				DEFINE_FIELD(CFuncTrackTrain, m_flVolume, FIELD_FLOAT),
				DEFINE_FIELD(CFuncTrackTrain, m_flBank, FIELD_FLOAT),
				DEFINE_FIELD(CFuncTrackTrain, m_oldSpeed, FIELD_FLOAT),
		};

TYPEDESCRIPTION CGunTarget::m_SaveData[] =
		{
				DEFINE_FIELD(CGunTarget, m_on, FIELD_BOOLEAN),
		};

IMPLEMENT_SAVERESTORE(CBasePlatTrain, CBaseToggle);

void CBasePlatTrain::KeyValue(KeyValueData *pkvd)
{
	if (FStrEq(pkvd->szKeyName, "lip")) {
		m_flLip = Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "wait")) {
		m_flWait = Q_atof(pkvd->szValue) * 1s;
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "height")) {
		m_flHeight = Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "rotation")) {
		m_vecFinalAngle.x = Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "movesnd")) {
		m_bMoveSnd = (BYTE) Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "stopsnd")) {
		m_bStopSnd = (BYTE) Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "volume")) {
		m_volume = Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else
		CBaseToggle::KeyValue(pkvd);
}

#define noiseMoving noise
#define noiseArrived noise1

void CBasePlatTrain::Precache()
{
	// set the plat's "in-motion" sound
	switch (m_bMoveSnd) {
		case 0:
			pev->noiseMoving = MAKE_STRING("common/null.wav");
			break;
		case 1:
			PRECACHE_SOUND("plats/bigmove1.wav");
			pev->noiseMoving = MAKE_STRING("plats/bigmove1.wav");
			break;
		case 2:
			PRECACHE_SOUND("plats/bigmove2.wav");
			pev->noiseMoving = MAKE_STRING("plats/bigmove2.wav");
			break;
		case 3:
			PRECACHE_SOUND("plats/elevmove1.wav");
			pev->noiseMoving = MAKE_STRING("plats/elevmove1.wav");
			break;
		case 4:
			PRECACHE_SOUND("plats/elevmove2.wav");
			pev->noiseMoving = MAKE_STRING("plats/elevmove2.wav");
			break;
		case 5:
			PRECACHE_SOUND("plats/elevmove3.wav");
			pev->noiseMoving = MAKE_STRING("plats/elevmove3.wav");
			break;
		case 6:
			PRECACHE_SOUND("plats/freightmove1.wav");
			pev->noiseMoving = MAKE_STRING("plats/freightmove1.wav");
			break;
		case 7:
			PRECACHE_SOUND("plats/freightmove2.wav");
			pev->noiseMoving = MAKE_STRING("plats/freightmove2.wav");
			break;
		case 8:
			PRECACHE_SOUND("plats/heavymove1.wav");
			pev->noiseMoving = MAKE_STRING("plats/heavymove1.wav");
			break;
		case 9:
			PRECACHE_SOUND("plats/rackmove1.wav");
			pev->noiseMoving = MAKE_STRING("plats/rackmove1.wav");
			break;
		case 10:
			PRECACHE_SOUND("plats/railmove1.wav");
			pev->noiseMoving = MAKE_STRING("plats/railmove1.wav");
			break;
		case 11:
			PRECACHE_SOUND("plats/squeekmove1.wav");
			pev->noiseMoving = MAKE_STRING("plats/squeekmove1.wav");
			break;
		case 12:
			PRECACHE_SOUND("plats/talkmove1.wav");
			pev->noiseMoving = MAKE_STRING("plats/talkmove1.wav");
			break;
		case 13:
			PRECACHE_SOUND("plats/talkmove2.wav");
			pev->noiseMoving = MAKE_STRING("plats/talkmove2.wav");
			break;
		default:
			pev->noiseMoving = MAKE_STRING("common/null.wav");
			break;
	}

	// set the plat's 'reached destination' stop sound
	switch (m_bStopSnd) {
		case 0:
			pev->noiseArrived = MAKE_STRING("common/null.wav");
			break;
		case 1:
			PRECACHE_SOUND("plats/bigstop1.wav");
			pev->noiseArrived = MAKE_STRING("plats/bigstop1.wav");
			break;
		case 2:
			PRECACHE_SOUND("plats/bigstop2.wav");
			pev->noiseArrived = MAKE_STRING("plats/bigstop2.wav");
			break;
		case 3:
			PRECACHE_SOUND("plats/freightstop1.wav");
			pev->noiseArrived = MAKE_STRING("plats/freightstop1.wav");
			break;
		case 4:
			PRECACHE_SOUND("plats/heavystop2.wav");
			pev->noiseArrived = MAKE_STRING("plats/heavystop2.wav");
			break;
		case 5:
			PRECACHE_SOUND("plats/rackstop1.wav");
			pev->noiseArrived = MAKE_STRING("plats/rackstop1.wav");
			break;
		case 6:
			PRECACHE_SOUND("plats/railstop1.wav");
			pev->noiseArrived = MAKE_STRING("plats/railstop1.wav");
			break;
		case 7:
			PRECACHE_SOUND("plats/squeekstop1.wav");
			pev->noiseArrived = MAKE_STRING("plats/squeekstop1.wav");
			break;
		case 8:
			PRECACHE_SOUND("plats/talkstop1.wav");
			pev->noiseArrived = MAKE_STRING("plats/talkstop1.wav");
			break;

		default:
			pev->noiseArrived = MAKE_STRING("common/null.wav");
			break;
	}
}

void CFuncPlat::CallGoDown()
{
	GoDown();
}

void CFuncPlat::CallHitTop()
{
	HitTop();
}

void CFuncPlat::CallHitBottom()
{
	HitBottom();
}

LINK_ENTITY_TO_CLASS(func_plat, CFuncPlat);

#define noiseMovement noise
#define noiseStopMoving noise1

// QUAKED func_plat (0 .5 .8) ? PLAT_LOW_TRIGGER
// speed	default 150

// Plats are always drawn in the extended position, so they will light correctly.

// If the plat is the target of another trigger or button, it will start out disabled in
// the extended position until it is trigger, when it will lower and become a normal plat.

// If the "height" key is set, that will determine the amount the plat moves, instead of
// being implicitly determined by the model's height.

// Set "sounds" to one of the following:
// 1) base fast
// 2) chain slow

void CFuncPlat::Setup()
{
	if (m_flTLength == 0)
		m_flTLength = 80;

	if (m_flTWidth == 0)
		m_flTWidth = 10;

	pev->angles = g_vecZero;
	pev->solid = SOLID_BSP;
	pev->movetype = MOVETYPE_PUSH;

	// set size and link into world
	UTIL_SetOrigin(pev, pev->origin);
	UTIL_SetSize(pev, pev->mins, pev->maxs);
	SET_MODEL(ENT(pev), STRING(pev->model));

	// vecPosition1 is the top position, vecPosition2 is the bottom
	m_vecPosition1 = pev->origin;
	m_vecPosition2 = pev->origin;

	if (m_flHeight != 0) {
		m_vecPosition2.z = pev->origin.z - m_flHeight;
	} else {
		m_vecPosition2.z = pev->origin.z - pev->size.z + 8;
	}

	if (pev->speed == 0) {
		pev->speed = 150;
	}

	if (m_volume == 0) {
		m_volume = 0.85;
	}
}

void CFuncPlat::Precache()
{
	CBasePlatTrain::Precache();

	if (!IsTogglePlat()) {
		// the "start moving" trigger
		PlatSpawnInsideTrigger(pev);
	}
}

void CFuncPlat::Spawn()
{
	Setup();
	Precache();

	// If this platform is the target of some button, it starts at the TOP position,
	// and is brought down by that button. Otherwise, it starts at BOTTOM.
	if (!FStringNull(pev->targetname)) {
		UTIL_SetOrigin(pev, m_vecPosition1);
		m_toggle_state = TS_AT_TOP;
		SetUse(&CFuncPlat::PlatUse);
	} else {
		UTIL_SetOrigin(pev, m_vecPosition2);
		m_toggle_state = TS_AT_BOTTOM;
	}
}

void PlatSpawnInsideTrigger(entvars_t *pevPlatform)
{
	CreateClassPtr<CPlatTrigger>()->SpawnInsideTrigger(GetClassPtr<CFuncPlat>(pevPlatform));
}

// Create a trigger entity for a platform.

void CPlatTrigger::SpawnInsideTrigger(CFuncPlat *pPlatform)
{
	m_pPlatform = pPlatform;

	// Create trigger entity, "point" it at the owning platform, give it a touch method
	pev->solid = SOLID_TRIGGER;
	pev->movetype = MOVETYPE_NONE;
	pev->origin = pPlatform->pev->origin;

	// Establish the trigger field's size
	Vector vecTMin = m_pPlatform->pev->mins + Vector(25, 25, 0);
	Vector vecTMax = m_pPlatform->pev->maxs + Vector(25, 25, 8);

	vecTMin.z = vecTMax.z - (m_pPlatform->m_vecPosition1.z - m_pPlatform->m_vecPosition2.z + 8);

	if (m_pPlatform->pev->size.x <= 50) {
		vecTMin.x = (m_pPlatform->pev->mins.x + m_pPlatform->pev->maxs.x) / 2;
		vecTMax.x = vecTMin.x + 1;
	}

	if (m_pPlatform->pev->size.y <= 50) {
		vecTMin.y = (m_pPlatform->pev->mins.y + m_pPlatform->pev->maxs.y) / 2;
		vecTMax.y = vecTMin.y + 1;
	}

	UTIL_SetSize(pev, vecTMin, vecTMax);
}

void CPlatTrigger::Touch(CBaseEntity *pOther)
{
	// Ignore touches by non-players
	entvars_t *pevToucher = pOther->pev;

	if (!FClassnameIs(pevToucher, "player")) {
		return;
	}

	// Ignore touches by corpses
	if (!pOther->IsAlive())
		return;

	// Make linked platform go up/down.
	if (m_pPlatform->m_toggle_state == TS_AT_BOTTOM) {
		m_pPlatform->GoUp();
	} else if (m_pPlatform->m_toggle_state == TS_AT_TOP) {
		// delay going down
		m_pPlatform->pev->nextthink = m_pPlatform->pev->ltime + 1s;
	}
}

// Used by SUB_UseTargets, when a platform is the target of a button.
// Start bringing platform down.

void CFuncPlat::PlatUse(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	if (IsTogglePlat()) {
		// Top is off, bottom is on
		BOOL on = (m_toggle_state == TS_AT_BOTTOM) ? TRUE : FALSE;

		if (!ShouldToggle(useType, on))
			return;

		if (m_toggle_state == TS_AT_TOP) {
			GoDown();
		} else if (m_toggle_state == TS_AT_BOTTOM) {
			GoUp();
		}
	} else {
		SetUse(NULL);

		if (m_toggle_state == TS_AT_TOP) {
			GoDown();
		}
	}
}

// Platform is at top, now starts moving down.

void CFuncPlat::GoDown()
{
	if (pev->noiseMovement) {
		EMIT_SOUND(ENT(pev), CHAN_STATIC, (char *) STRING(pev->noiseMovement), m_volume, ATTN_NORM);
	}

	assert(m_toggle_state == TS_AT_TOP || m_toggle_state == TS_GOING_UP);

	m_toggle_state = TS_GOING_DOWN;
	SetMoveDone(&CFuncPlat::CallHitBottom);
	LinearMove(m_vecPosition2, pev->speed);
}

// Platform has hit bottom.  Stops and waits forever.

void CFuncPlat::HitBottom()
{
	if (pev->noiseMovement) {
		STOP_SOUND(ENT(pev), CHAN_STATIC, (char *) STRING(pev->noiseMovement));
	}

	if (pev->noiseStopMoving) {
		EMIT_SOUND(ENT(pev), CHAN_WEAPON, (char *) STRING(pev->noiseStopMoving), m_volume, ATTN_NORM);
	}

	assert(m_toggle_state == TS_GOING_DOWN);

	m_toggle_state = TS_AT_BOTTOM;
}

// Platform is at bottom, now starts moving up

void CFuncPlat::GoUp()
{
	if (pev->noiseMovement) {
		EMIT_SOUND(ENT(pev), CHAN_STATIC, (char *) STRING(pev->noiseMovement), m_volume, ATTN_NORM);
	}

	assert(m_toggle_state == TS_AT_BOTTOM || m_toggle_state == TS_GOING_DOWN);

	m_toggle_state = TS_GOING_UP;
	SetMoveDone(&CFuncPlat::CallHitTop);
	LinearMove(m_vecPosition1, pev->speed);
}

// Platform has hit top.  Pauses, then starts back down again.

void CFuncPlat::HitTop()
{
	if (pev->noiseMovement) {
		STOP_SOUND(ENT(pev), CHAN_STATIC, (char *) STRING(pev->noiseMovement));
	}

	if (pev->noiseStopMoving) {
		EMIT_SOUND(ENT(pev), CHAN_WEAPON, (char *) STRING(pev->noiseStopMoving), m_volume, ATTN_NORM);
	}

	assert(m_toggle_state == TS_GOING_UP);

	m_toggle_state = TS_AT_TOP;

	if (!IsTogglePlat()) {
		// After a delay, the platform will automatically start going down again.
		SetThink(&CFuncPlat::CallGoDown);
		pev->nextthink = pev->ltime + 3s;
	}
}

void CFuncPlat::Blocked(CBaseEntity *pOther)
{
	ALERT(at_aiconsole, "%s Blocked by %s\n", STRING(pev->classname), STRING(pOther->pev->classname));

	// Hurt the blocker a little
	pOther->TakeDamage(pev, pev, 1, DMG_CRUSH);

	if (pev->noiseMovement) {
		STOP_SOUND(ENT(pev), CHAN_STATIC, (char *) STRING(pev->noiseMovement));
	}

	// Send the platform back where it came from
	assert(m_toggle_state == TS_GOING_UP || m_toggle_state == TS_GOING_DOWN);

	if (m_toggle_state == TS_GOING_UP) {
		GoDown();
	} else if (m_toggle_state == TS_GOING_DOWN) {
		GoUp();
	}
}

LINK_ENTITY_TO_CLASS(func_platrot, CFuncPlatRot);

IMPLEMENT_SAVERESTORE(CFuncPlatRot, CFuncPlat);

void CFuncPlatRot::SetupRotation()
{
	// This plat rotates too!
	if (m_vecFinalAngle.x != 0) {
		CBaseToggle::AxisDir(pev);
		m_start = pev->angles;
		m_end = pev->angles + pev->movedir * m_vecFinalAngle.x;
	} else {
		m_start = g_vecZero;
		m_end = g_vecZero;
	}

	// Start at top
	if (!FStringNull(pev->targetname)) {
		pev->angles = m_end;
	}
}

void CFuncPlatRot::Spawn()
{
	CFuncPlat::Spawn();
	SetupRotation();
}

void CFuncPlatRot::GoDown()
{
	CFuncPlat::GoDown();
	RotMove(m_start, pev->nextthink - pev->ltime);
}

// Platform has hit bottom.  Stops and waits forever.

void CFuncPlatRot::HitBottom()
{
	CFuncPlat::HitBottom();
	pev->avelocity = g_vecZero;
	pev->angles = m_start;
}

// Platform is at bottom, now starts moving up

void CFuncPlatRot::GoUp()
{
	CFuncPlat::GoUp();
	RotMove(m_end, pev->nextthink - pev->ltime);
}

// Platform has hit top.  Pauses, then starts back down again.

void CFuncPlatRot::HitTop()
{
	CFuncPlat::HitTop();
	pev->avelocity = g_vecZero;
	pev->angles = m_end;
}

void CFuncPlatRot::RotMove(Vector &destAngle, duration_t time)
{
	// set destdelta to the vector needed to move
	Vector vecDestDelta = destAngle - pev->angles;

	// Travel time is so short, we're practically there already;  so make it so.
	if (time >= 0.1s) {
		pev->avelocity = vecDestDelta / (time / 1s);
	} else {
		pev->avelocity = vecDestDelta;
		pev->nextthink = pev->ltime + 1s;
	}
}

LINK_ENTITY_TO_CLASS(func_train, CFuncTrain);

IMPLEMENT_SAVERESTORE(CFuncTrain, CBasePlatTrain);

void CFuncTrain::KeyValue(KeyValueData *pkvd)
{
	if (FStrEq(pkvd->szKeyName, "sounds")) {
		m_sounds = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else
		CBasePlatTrain::KeyValue(pkvd);
}

void CFuncTrain::Blocked(CBaseEntity *pOther)
{
	if (gpGlobals->time < m_flActivateFinished)
		return;

	m_flActivateFinished = gpGlobals->time + 0.5s;

	pOther->TakeDamage(pev, pev, pev->dmg, DMG_CRUSH);
}

void CFuncTrain::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	if (pev->spawnflags & SF_TRAIN_WAIT_RETRIGGER) {
		// Move toward my target
		pev->spawnflags &= ~SF_TRAIN_WAIT_RETRIGGER;
		Next();
	} else {
		pev->spawnflags |= SF_TRAIN_WAIT_RETRIGGER;
		// Pop back to last target if it's available
		if (pev->enemy) {
			pev->target = pev->enemy->v.targetname;
		}

		pev->nextthink = invalid_time_point;
		pev->velocity = g_vecZero;
		if (pev->noiseStopMoving) {
			EMIT_SOUND(ENT(pev), CHAN_VOICE, (char *) STRING(pev->noiseStopMoving), m_volume, ATTN_NORM);
		}
	}
}

void CFuncTrain::Wait()
{
	if (m_pevCurrentTarget != NULL) {
		// Fire the pass target if there is one
		if (m_pevCurrentTarget->message) {
			FireTargets(STRING(m_pevCurrentTarget->message), this, this, USE_TOGGLE, 0);
			if (m_pevCurrentTarget->spawnflags & SF_CORNER_FIREONCE) {
				m_pevCurrentTarget->message = 0;
			}
		}

		// need pointer to LAST target.
		if ((m_pevCurrentTarget->spawnflags & SF_TRAIN_WAIT_RETRIGGER) || (pev->spawnflags & SF_TRAIN_WAIT_RETRIGGER)) {
			pev->spawnflags |= SF_TRAIN_WAIT_RETRIGGER;

			// clear the sound channel.
			if (pev->noiseMovement) {
				STOP_SOUND(edict(), CHAN_STATIC, (char *) STRING(pev->noiseMovement));
			}
			if (pev->noiseStopMoving) {
				EMIT_SOUND(ENT(pev), CHAN_VOICE, (char *) STRING(pev->noiseStopMoving), m_volume, ATTN_NORM);
			}

			pev->nextthink = invalid_time_point;
			return;
		}
	}

	if (m_flWait != 0s) {
		// -1 wait will wait forever!
		pev->nextthink = pev->ltime + m_flWait;
		if (pev->noiseMovement) {
			STOP_SOUND(edict(), CHAN_STATIC, (char *) STRING(pev->noiseMovement));
		}
		if (pev->noiseStopMoving) {
			EMIT_SOUND(ENT(pev), CHAN_VOICE, (char *) STRING(pev->noiseStopMoving), m_volume, ATTN_NORM);
		}
		SetThink(&CFuncTrain::Next);
	} else {
		// do it RIGHT now!
		Next();
	}
}

// Train next - path corner needs to change to next target

void CFuncTrain::Next()
{
	CBaseEntity *pTarg;

	// now find our next target
	pTarg = GetNextTarget();

	if (!pTarg) {
		if (pev->noiseMovement) {
			STOP_SOUND(edict(), CHAN_STATIC, (char *) STRING(pev->noiseMovement));
		}

		// Play stop sound
		if (pev->noiseStopMoving) {
			EMIT_SOUND(ENT(pev), CHAN_VOICE, (char *) STRING(pev->noiseStopMoving), m_volume, ATTN_NORM);
		}

		return;
	}

	// Save last target in case we need to find it again
	pev->message = pev->target;

	pev->target = pTarg->pev->target;
	m_flWait = pTarg->GetDelay();

	if (m_pevCurrentTarget != NULL && m_pevCurrentTarget->speed != 0) {
		// don't copy speed from target if it is 0 (uninitialized)
		pev->speed = m_pevCurrentTarget->speed;
		ALERT(at_aiconsole, "Train %s speed to %4.2f\n", STRING(pev->targetname), pev->speed);
	}

	// keep track of this since path corners change our target for us.
	m_pevCurrentTarget = pTarg->pev;

	//hack
	pev->enemy = pTarg->edict();

	if (m_pevCurrentTarget->spawnflags & SF_CORNER_TELEPORT) {
		// Path corner has indicated a teleport to the next corner.
		pev->effects |= EF_NOINTERP;

		UTIL_SetOrigin(pev, pTarg->pev->origin - (pev->mins + pev->maxs) * 0.5);

		// Get on with doing the next path corner.
		Wait();
	} else {
		// Normal linear move.
		// CHANGED this from CHAN_VOICE to CHAN_STATIC around OEM beta time because trains should
		// use CHAN_STATIC for their movement sounds to prevent sound field problems.
		// this is not a hack or temporary fix, this is how things should be. (sjb).
		if (pev->noiseMovement) {
			STOP_SOUND(edict(), CHAN_STATIC, (char *) STRING(pev->noiseMovement));
		}
		if (pev->noiseMovement) {
			EMIT_SOUND(ENT(pev), CHAN_STATIC, (char *) STRING(pev->noiseMovement), m_volume, ATTN_NORM);
		}

		pev->effects &= ~EF_NOINTERP;
		SetMoveDone(&CFuncTrain::Wait);
		LinearMove(pTarg->pev->origin - (pev->mins + pev->maxs) * 0.5, pev->speed);
	}
}

void CFuncTrain::Activate()
{
	// Not yet active, so teleport to first target
	if (!m_activated) {
		m_activated = TRUE;
		entvars_t *pevTarg = VARS(FIND_ENTITY_BY_TARGETNAME(NULL, STRING(pev->target)));

		pev->target = pevTarg->target;

		// keep track of this since path corners change our target for us.
		m_pevCurrentTarget = pevTarg;

		UTIL_SetOrigin(pev, pevTarg->origin - (pev->mins + pev->maxs) * 0.5);

		if (FStringNull(pev->targetname)) {
			// not triggered, so start immediately
			pev->nextthink = pev->ltime + 0.1s;
			SetThink(&CFuncTrain::Next);
		} else
			pev->spawnflags |= SF_TRAIN_WAIT_RETRIGGER;
	}
}

// QUAKED func_train (0 .5 .8) ?
// Trains are moving platforms that players can ride.
// The targets origin specifies the min point of the train at each corner.
// The train spawns at the first target it is pointing at.
// If the train is the target of a button or trigger, it will not begin moving until activated.
// speed	default 100
// dmg		default	2
// sounds
// 1) ratchet metal

void CFuncTrain::Spawn()
{
	Precache();

	if (pev->speed == 0)
		pev->speed = 100;

	if (FStringNull(pev->target)) {
		ALERT(at_console, "FuncTrain with no target");
	}

	m_pevFirstTarget = m_pevCurrentTarget;
	m_vStartPosition = pev->origin;

	if (pev->dmg == 0)
		pev->dmg = 2;

	pev->movetype = MOVETYPE_PUSH;

	if (pev->spawnflags & SF_TRACKTRAIN_PASSABLE)
		pev->solid = SOLID_NOT;
	else
		pev->solid = SOLID_BSP;

	SET_MODEL(ENT(pev), STRING(pev->model));
	UTIL_SetSize(pev, pev->mins, pev->maxs);
	UTIL_SetOrigin(pev, pev->origin);

	m_activated = FALSE;

	if (m_volume == 0)
		m_volume = 0.85;
}

void CFuncTrain::Restart()
{
	if (pev->speed == 0)
		pev->speed = 100;

	if (pev->dmg == 0)
		pev->dmg = 2;

	pev->movetype = MOVETYPE_PUSH;
	m_pevCurrentTarget = m_pevFirstTarget;

	UTIL_SetOrigin(pev, m_vStartPosition);

	m_activated = FALSE;

	if (m_volume == 0)
		m_volume = 0.85;
}

void CFuncTrain::Precache()
{
	CBasePlatTrain::Precache();
}

void CFuncTrain::OverrideReset()
{
	CBaseEntity *pTarg;

	// Are we moving?
	if (pev->velocity != g_vecZero && pev->nextthink != invalid_time_point) {
		pev->target = pev->message;

		// now find our next target
		pTarg = GetNextTarget();

		if (!pTarg) {
			pev->nextthink = invalid_time_point;
			pev->velocity = g_vecZero;
		} else    // Keep moving for 0.1 secs, then find path_corner again and restart
		{
			SetThink(&CFuncTrain::Next);
			pev->nextthink = pev->ltime + 0.1s;
		}
	}
}

IMPLEMENT_SAVERESTORE(CFuncTrackTrain, CBaseEntity);

LINK_ENTITY_TO_CLASS(func_tracktrain, CFuncTrackTrain);

void CFuncTrackTrain::KeyValue(KeyValueData *pkvd)
{
	if (FStrEq(pkvd->szKeyName, "wheels")) {
		m_length = Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "height")) {
		m_height = Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "startspeed")) {
		m_startSpeed = Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "sounds")) {
		m_sounds = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "volume")) {
		m_flVolume = (float) Q_atoi(pkvd->szValue);
		m_flVolume *= 0.1;
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "bank")) {
		m_flBank = Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else
		CBaseEntity::KeyValue(pkvd);
}

void CFuncTrackTrain::NextThink(time_point_t thinkTime, BOOL alwaysThink)
{
	if (alwaysThink)
		pev->flags |= FL_ALWAYSTHINK;
	else
		pev->flags &= ~FL_ALWAYSTHINK;

	pev->nextthink = thinkTime;
}

void CFuncTrackTrain::Blocked(CBaseEntity *pOther)
{
	entvars_t *pevOther = pOther->pev;

	// Blocker is on-ground on the train
	if ((pevOther->flags & FL_ONGROUND) && VARS(pevOther->groundentity) == pev) {
		float deltaSpeed = fabs((float) pev->speed);

		if (deltaSpeed > 50) {
			deltaSpeed = 50;
		}

		if (!pevOther->velocity.z) {
			pevOther->velocity.z += deltaSpeed;
		}

		return;
	} else
		pevOther->velocity = (pevOther->origin - pev->origin).Normalize() * pev->dmg;

	ALERT(at_aiconsole, "TRAIN(%s): Blocked by %s (dmg:%.2f)\n", STRING(pev->targetname),
	      STRING(pOther->pev->classname), pev->dmg);

	// TODO: is missing?
#if 0
	if (pev->dmg <= 0)
		return;

	// we can't hurt this thing, so we're not concerned with it
	pOther->TakeDamage(pev, pev, pev->dmg, DMG_CRUSH);

#endif
}

void CFuncTrackTrain::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	if (useType != USE_SET) {
		if (!ShouldToggle(useType, (pev->speed != 0)))
			return;

		if (pev->speed == 0) {
			pev->speed = m_speed * m_dir;
			Next();
		} else {
			pev->speed = 0;
			pev->velocity = g_vecZero;
			pev->avelocity = g_vecZero;
			StopSound();
			SetThink(NULL);
		}
	} else {
		float delta = ((int) (pev->speed * 4) / (int) m_speed) * 0.25 + 0.25 * value;

		if (delta > 1)
			delta = 1;

		else if (delta < -1)
			delta = -1;

		if (pev->spawnflags & SF_TRACKTRAIN_FORWARDONLY) {
			if (delta < 0)
				delta = 0;
		}

		pev->speed = m_speed * delta;
		Next();
		ALERT(at_aiconsole, "TRAIN(%s), speed to %.2f\n", STRING(pev->targetname), pev->speed);
	}
}

float Fix(float v)
{
	float angle = v;

	while (angle < 0)
		angle += 360;

	while (angle > 360)
		angle -= 360;

	return angle;
}

void FixupAngles(Vector &v)
{
	v.x = Fix(v.x);
	v.y = Fix(v.y);
	v.z = Fix(v.z);
}

void CFuncTrackTrain::StopSound()
{
	// if sound playing, stop it
	if (m_soundPlaying && pev->noise) {
		unsigned short us_encode;
		unsigned short us_sound = ((unsigned short) (m_sounds) & 0x0007) << 12;

		us_encode = us_sound;

		PLAYBACK_EVENT_FULL(FEV_RELIABLE | FEV_UPDATE, edict(), m_usAdjustPitch, 0.0, g_vecZero,
		                    g_vecZero, 0.0, 0.0, us_encode, 0, 1, 0);
		EMIT_SOUND_DYN(ENT(pev), CHAN_ITEM, "plats/ttrain_brake1.wav", m_flVolume, ATTN_NORM, 0, 100);
	}

	m_soundPlaying = 0;
}

void CFuncTrackTrain::UpdateSound()
{
	float flpitch;

	if (!pev->noise)
		return;

	flpitch = TRAIN_STARTPITCH + (abs(pev->speed) * (TRAIN_MAXPITCH - TRAIN_STARTPITCH) / TRAIN_MAXSPEED);

	if (!m_soundPlaying) {
		// play startup sound for train
		EMIT_SOUND_DYN(ENT(pev), CHAN_ITEM, "plats/ttrain_start1.wav", m_flVolume, ATTN_NORM, 0, 100);
		EMIT_SOUND_DYN(ENT(pev), CHAN_STATIC, (char *) STRING(pev->noise), m_flVolume, ATTN_NORM, 0, (int) flpitch);
		m_soundPlaying = 1;
	} else {
		// update pitch
		// EMIT_SOUND_DYN(ENT(pev), CHAN_STATIC, (char *)STRING(pev->noise), m_flVolume, ATTN_NORM, SND_CHANGE_PITCH, (int)flpitch);

		// volume 0.0 - 1.0 - 6 bits
		// m_sounds 3 bits
		// flpitch = 6 bits
		// 15 bits total

		unsigned short us_encode;
		unsigned short us_sound = ((unsigned short) (m_sounds) & 0x0007) << 12;
		unsigned short us_pitch = ((unsigned short) (flpitch / 10.0) & 0x003f) << 6;
		unsigned short us_volume = ((unsigned short) (m_flVolume * 40.0) & 0x003f);

		us_encode = us_sound | us_pitch | us_volume;

		PLAYBACK_EVENT_FULL(FEV_RELIABLE | FEV_UPDATE, edict(), m_usAdjustPitch, 0.0, g_vecZero,
		                    g_vecZero, 0.0, 0.0, us_encode, 0, 0, 0);
	}
}

void CFuncTrackTrain::Next()
{
	auto time = 0.5s;

	if (!pev->speed) {
		ALERT(at_aiconsole, "TRAIN(%s): Speed is 0\n", STRING(pev->targetname));
		StopSound();
		return;
	}

	if (!m_ppath) {
		ALERT(at_aiconsole, "TRAIN(%s): Lost path\n", STRING(pev->targetname));
		StopSound();
		return;
	}

	UpdateSound();

	Vector nextPos = pev->origin;

	nextPos.z -= m_height;
	CPathTrack *pnext = m_ppath->LookAhead(&nextPos, pev->speed * 0.1, 1);
	nextPos.z += m_height;

	pev->velocity = (nextPos - pev->origin) * 10;

	Vector nextFront = pev->origin;
	nextFront.z -= m_height;

	if (m_length > 0)
		m_ppath->LookAhead(&nextFront, m_length, 0);
	else
		m_ppath->LookAhead(&nextFront, 100, 0);

	nextFront.z += m_height;

	Vector delta = nextFront - pev->origin;
	Vector angles = UTIL_VecToAngles(delta);

	float fixAngleY = angles.y + 180.0f;

	// The train actually points west
	angles.y += fixAngleY;

	// TODO: All of this crap has to be done to make the angles not wrap around, revisit this.
	FixupAngles(angles);
	FixupAngles(pev->angles);

	if (!pnext || (delta.x == 0 && delta.y == 0))
		angles = pev->angles;

	float vy, vx;
	if (!(pev->spawnflags & SF_TRACKTRAIN_NOPITCH))
		vx = UTIL_AngleDistance(angles.x, pev->angles.x);
	else
		vx = 0;

	vy = UTIL_AngleDistance(angles.y, pev->angles.y);

	pev->avelocity.y = vy * 10;
	pev->avelocity.x = vx * 10;

	if (m_flBank != 0) {
		if (pev->avelocity.y < -5) {
			pev->avelocity.z = UTIL_AngleDistance(UTIL_ApproachAngle(-m_flBank, pev->angles.z, m_flBank * 2),
			                                      pev->angles.z);
		} else if (pev->avelocity.y > 5) {
			pev->avelocity.z = UTIL_AngleDistance(UTIL_ApproachAngle(m_flBank, pev->angles.z, m_flBank * 2),
			                                      pev->angles.z);
		} else {
			pev->avelocity.z =
					UTIL_AngleDistance(UTIL_ApproachAngle(0, pev->angles.z, m_flBank * 4), pev->angles.z) * 4;
		}
	}

	if (pnext != NULL) {
		if (pnext != m_ppath) {
			CPathTrack *pFire;
			if (pev->speed >= 0)
				pFire = pnext;
			else
				pFire = m_ppath;

			m_ppath = pnext;

			// Fire the pass target if there is one
			if (!FStringNull(pFire->pev->message)) {
				FireTargets(STRING(pFire->pev->message), this, this, USE_TOGGLE, 0);

				if (pFire->pev->spawnflags & SF_PATH_FIREONCE) {
					pFire->pev->message = 0;
				}
			}

			if (pFire->pev->spawnflags & SF_PATH_DISABLE_TRAIN) {
				pev->spawnflags |= SF_TRACKTRAIN_NOCONTROL;
			}

			// Don't override speed if under user control
			if (pev->spawnflags & SF_TRACKTRAIN_NOCONTROL) {
				if (pFire->pev->speed != 0) {
					// don't copy speed from target if it is 0 (uninitialized)
					pev->speed = pFire->pev->speed;
					ALERT(at_aiconsole, "TrackTrain %s speed to %4.2f\n", STRING(pev->targetname), pev->speed);
				}
			}
		}

		SetThink(&CFuncTrackTrain::Next);
		NextThink(pev->ltime + time, TRUE);
	} else    // end of path, stop
	{
		StopSound();
		pev->velocity = (nextPos - pev->origin);
		pev->avelocity = g_vecZero;

		float distance = pev->velocity.Length();
		m_oldSpeed = pev->speed;
		pev->speed = 0;

		// Move to the dead end

		// Are we there yet?
		if (distance > 0) {
			// no, how long to get there?
			time = distance / m_oldSpeed * 1s;
			pev->velocity = pev->velocity * (m_oldSpeed / distance);
			SetThink(&CFuncTrackTrain::DeadEnd);
			NextThink(pev->ltime + time, FALSE);
		} else {
			DeadEnd();
		}
	}
}

void CFuncTrackTrain::DeadEnd()
{
	// Fire the dead-end target if there is one
	CPathTrack *pTrack, *pNext;

	pTrack = m_ppath;

	ALERT(at_aiconsole, "TRAIN(%s): Dead end ", STRING(pev->targetname));

	// Find the dead end path node
	// HACKHACK -- This is bugly, but the train can actually stop moving at a different node depending on it's speed
	// so we have to traverse the list to it's end.
	if (pTrack != NULL) {
		if (m_oldSpeed < 0) {
			do {
				pNext = pTrack->ValidPath(pTrack->GetPrevious(), TRUE);

				if (pNext) {
					pTrack = pNext;
				}
			} while (pNext != NULL);
		} else {
			do {
				pNext = pTrack->ValidPath(pTrack->GetNext(), TRUE);

				if (pNext) {
					pTrack = pNext;
				}
			} while (pNext != NULL);
		}
	}

	pev->velocity = g_vecZero;
	pev->avelocity = g_vecZero;

	if (pTrack != NULL) {
		ALERT(at_aiconsole, "at %s\n", STRING(pTrack->pev->targetname));

		if (!FStringNull(pTrack->pev->netname)) {
			FireTargets(STRING(pTrack->pev->netname), this, this, USE_TOGGLE, 0);
		}
	} else
		ALERT(at_aiconsole, "\n");
}

void CFuncTrackTrain::SetControls(entvars_t *pevControls)
{
	Vector offset = pevControls->origin - pev->oldorigin;

	m_controlMins = pevControls->mins + offset;
	m_controlMaxs = pevControls->maxs + offset;
}

BOOL CFuncTrackTrain::OnControls(entvars_t *pevTest)
{
	Vector offset = pevTest->origin - pev->origin;

	if (pev->spawnflags & SF_TRACKTRAIN_NOCONTROL)
		return FALSE;

	// Transform offset into local coordinates
	UTIL_MakeVectors(pev->angles);
	Vector local;

	local.x = DotProduct(offset, gpGlobals->v_forward);
	local.y = -DotProduct(offset, gpGlobals->v_right);
	local.z = DotProduct(offset, gpGlobals->v_up);

	if (local.x >= m_controlMins.x && local.y >= m_controlMins.y && local.z >= m_controlMins.z &&
	    local.x <= m_controlMaxs.x && local.y <= m_controlMaxs.y && local.z <= m_controlMaxs.z)
		return TRUE;

	return FALSE;
}

void CFuncTrackTrain::Find()
{
	m_ppath = CPathTrack::Instance(FIND_ENTITY_BY_TARGETNAME(NULL, STRING(pev->target)));

	if (!m_ppath) {
		return;
	}

	entvars_t *pevTarget = m_ppath->pev;
	if (!FClassnameIs(pevTarget, "path_track")) {
		ALERT(at_error, "func_track_train must be on a path of path_track\n");
		m_ppath = NULL;
		return;
	}

	Vector nextPos = pevTarget->origin;
	nextPos.z += m_height;

	Vector look = nextPos;
	look.z -= m_height;

	m_ppath->LookAhead(&look, m_length, 0);
	look.z += m_height;

	pev->angles = UTIL_VecToAngles(look - nextPos);
	// The train actually points west
	pev->angles.y += 180;

	if (pev->spawnflags & SF_TRACKTRAIN_NOPITCH) {
		pev->angles.x = 0;
	}

	UTIL_SetOrigin(pev, nextPos);
	NextThink(pev->ltime + 0.1s, FALSE);
	SetThink(&CFuncTrackTrain::Next);
	pev->speed = m_startSpeed;

	UpdateSound();
}

void CFuncTrackTrain::NearestPath()
{
	CBaseEntity *pTrack = NULL;
	CBaseEntity *pNearest = NULL;
	float dist;
	float closest;

	closest = 1024;

	while ((pTrack = UTIL_FindEntityInSphere(pTrack, pev->origin, 1024)) != NULL) {
		// filter out non-tracks
		if (!(pTrack->pev->flags & (FL_CLIENT | FL_MONSTER)) && FClassnameIs(pTrack->pev, "path_track")) {
			dist = (pev->origin - pTrack->pev->origin).Length();

			if (dist < closest) {
				closest = dist;
				pNearest = pTrack;
			}
		}
	}

	if (!pNearest) {
		ALERT(at_console, "Can't find a nearby track !!!\n");
		SetThink(NULL);
		return;
	}

	ALERT(at_aiconsole, "TRAIN: %s, Nearest track is %s\n", STRING(pev->targetname), STRING(pNearest->pev->targetname));

	// If I'm closer to the next path_track on this path, then it's my real path
	pTrack = ((CPathTrack *) pNearest)->GetNext();

	if (pTrack != NULL) {
		if ((pev->origin - pTrack->pev->origin).Length() < (pev->origin - pNearest->pev->origin).Length()) {
			pNearest = pTrack;
		}
	}

	m_ppath = static_cast<CPathTrack *>(pNearest);

	if (pev->speed != 0) {
		NextThink(pev->ltime + 0.1s, FALSE);
		SetThink(&CFuncTrackTrain::Next);
	}
}

void CFuncTrackTrain::OverrideReset()
{
	NextThink(pev->ltime + 0.1s, FALSE);
	SetThink(&CFuncTrackTrain::NearestPath);
}

CFuncTrackTrain *CFuncTrackTrain::Instance(edict_t *pent)
{
	if (FClassnameIs(pent, "func_tracktrain")) {
		return (CFuncTrackTrain *) GET_PRIVATE(pent);
	}

	return NULL;
}

void CFuncTrackTrain::Spawn()
{
	if (pev->speed == 0)
		m_speed = 165;
	else
		m_speed = pev->speed;

	ALERT(at_console, "M_speed = %f\n", m_speed);

	pev->speed = 0;
	pev->velocity = g_vecZero;
	pev->avelocity = g_vecZero;
	pev->impulse = (int) m_speed;

	m_dir = 1;

	if (FStringNull(pev->target)) {
		ALERT(at_console, "FuncTrain with no target");
	}

	if (pev->spawnflags & SF_TRACKTRAIN_PASSABLE)
		pev->solid = SOLID_NOT;
	else
		pev->solid = SOLID_BSP;

	pev->movetype = MOVETYPE_PUSH;
	SET_MODEL(ENT(pev), STRING(pev->model));

	UTIL_SetSize(pev, pev->mins, pev->maxs);
	UTIL_SetOrigin(pev, pev->origin);

	// Cache off placed origin for train controls
	pev->oldorigin = pev->origin;

	m_controlMins = pev->mins;
	m_controlMaxs = pev->maxs;
	m_controlMaxs.z += 72;

	// start trains on the next frame, to make sure their targets have had
	// a chance to spawn/activate
	NextThink(pev->ltime + 0.1s, FALSE);
	SetThink(&CFuncTrackTrain::Find);
	Precache();
}

void CFuncTrackTrain::Restart()
{
	ALERT(at_console, "M_speed = %f\n", m_speed);

	pev->speed = 0;
	pev->velocity = g_vecZero;
	pev->avelocity = g_vecZero;
	pev->impulse = (int) m_speed;
	m_dir = 1;

	if (FStringNull(pev->target)) {
		ALERT(at_console, "FuncTrain with no target");
	}

	UTIL_SetOrigin(pev, pev->oldorigin);
	NextThink(pev->ltime + 0.1s, FALSE);
	SetThink(&CFuncTrackTrain::Find);
}

void CFuncTrackTrain::Precache()
{
	if (m_flVolume == 0.0)
		m_flVolume = 1.0;

	switch (m_sounds) {
		default:
			// no sound
			pev->noise = 0;
			break;
		case 1:
			PRECACHE_SOUND("plats/ttrain1.wav");
			pev->noise = MAKE_STRING("plats/ttrain1.wav");
			break;
		case 2:
			PRECACHE_SOUND("plats/ttrain2.wav");
			pev->noise = MAKE_STRING("plats/ttrain2.wav");
			break;
		case 3:
			PRECACHE_SOUND("plats/ttrain3.wav");
			pev->noise = MAKE_STRING("plats/ttrain3.wav");
			break;
		case 4:
			PRECACHE_SOUND("plats/ttrain4.wav");
			pev->noise = MAKE_STRING("plats/ttrain4.wav");
			break;
		case 5:
			PRECACHE_SOUND("plats/ttrain6.wav");
			pev->noise = MAKE_STRING("plats/ttrain6.wav");
			break;
		case 6:
			PRECACHE_SOUND("plats/ttrain7.wav");
			pev->noise = MAKE_STRING("plats/ttrain7.wav");
			break;
	}

	PRECACHE_SOUND("plats/ttrain_brake1.wav");
	PRECACHE_SOUND("plats/ttrain_start1.wav");

	m_usAdjustPitch = PRECACHE_EVENT(1, "events/train.sc");
}

LINK_ENTITY_TO_CLASS(func_traincontrols, CFuncTrainControls);

void CFuncTrainControls::Find()
{
	edict_t *pTarget = NULL;

	do {
		pTarget = FIND_ENTITY_BY_TARGETNAME(pTarget, STRING(pev->target));
	} while (!FNullEnt(pTarget) && !FClassnameIs(pTarget, "func_tracktrain"));

	if (FNullEnt(pTarget)) {
		ALERT(at_console, "No train %s\n", STRING(pev->target));
		return;
	}

	CFuncTrackTrain *ptrain = CFuncTrackTrain::Instance(pTarget);
	ptrain->SetControls(pev);
	UTIL_Remove(this);
}

void CFuncTrainControls::Spawn()
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	SET_MODEL(ENT(pev), STRING(pev->model));

	UTIL_SetSize(pev, pev->mins, pev->maxs);
	UTIL_SetOrigin(pev, pev->origin);

	SetThink(&CFuncTrainControls::Find);
	pev->nextthink = gpGlobals->time;
}

typedef enum
{
	TRAIN_SAFE, TRAIN_BLOCKING, TRAIN_FOLLOWING
} TRAIN_CODE;

class CFuncTrackChange : public CFuncPlatRot
{
public:
	virtual void Spawn(void);
	virtual void Precache(void);
	virtual void EXPORT GoUp(void);
	virtual void EXPORT GoDown(void);
	virtual void KeyValue(KeyValueData *pkvd);
	virtual void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	virtual void HitBottom(void);
	virtual void HitTop(void);
	virtual void UpdateAutoTargets(int toggleState);
	virtual BOOL IsTogglePlat(void) { return TRUE; }
	virtual void Touch(CBaseEntity *pOther);
	virtual int Save(CSave &save);
	virtual int Restore(CRestore &restore);
	virtual void OverrideReset(void);

public:
	TRAIN_CODE EvaluateTrain(CPathTrack *pcurrent);
	void UpdateTrain(Vector &dest);
	void EXPORT Find(void);
	void DisableUse(void) { m_use = 0; }
	void EnableUse(void) { m_use = 1; }
	int UseEnabled(void) { return m_use; }

public:
	static TYPEDESCRIPTION m_SaveData[];

public:
	CPathTrack *m_trackTop;
	CPathTrack *m_trackBottom;
	CFuncTrackTrain *m_train;
	int m_trackTopName;
	int m_trackBottomName;
	int m_trainName;
	TRAIN_CODE m_code;
	int m_targetState;
	int m_use;
};

LINK_ENTITY_TO_CLASS(func_trackchange, CFuncTrackChange);

TYPEDESCRIPTION CFuncTrackChange::m_SaveData[] =
		{
				DEFINE_GLOBAL_FIELD(CFuncTrackChange, m_trackTop, FIELD_CLASSPTR),
				DEFINE_GLOBAL_FIELD(CFuncTrackChange, m_trackBottom, FIELD_CLASSPTR),
				DEFINE_GLOBAL_FIELD(CFuncTrackChange, m_train, FIELD_CLASSPTR),
				DEFINE_GLOBAL_FIELD(CFuncTrackChange, m_trackTopName, FIELD_STRING),
				DEFINE_GLOBAL_FIELD(CFuncTrackChange, m_trackBottomName, FIELD_STRING),
				DEFINE_GLOBAL_FIELD(CFuncTrackChange, m_trainName, FIELD_STRING),
				DEFINE_FIELD(CFuncTrackChange, m_code, FIELD_INTEGER),
				DEFINE_FIELD(CFuncTrackChange, m_targetState, FIELD_INTEGER),
				DEFINE_FIELD(CFuncTrackChange, m_use, FIELD_INTEGER),
		};

IMPLEMENT_SAVERESTORE(CFuncTrackChange, CFuncPlatRot);

void CFuncTrackChange::Spawn()
{
	Setup();
	if (pev->spawnflags & SF_TRACK_DONT_MOVE) {
		m_vecPosition2.z = pev->origin.z;
	}

	SetupRotation();

	if (pev->spawnflags & SF_TRACK_STARTBOTTOM) {
		UTIL_SetOrigin(pev, m_vecPosition2);
		m_toggle_state = TS_AT_BOTTOM;
		pev->angles = m_start;
		m_targetState = TS_AT_TOP;
	} else {
		UTIL_SetOrigin(pev, m_vecPosition1);
		m_toggle_state = TS_AT_TOP;
		pev->angles = m_end;
		m_targetState = TS_AT_BOTTOM;
	}

	EnableUse();
	pev->nextthink = pev->ltime + 2.0s;
	SetThink(&CFuncTrackChange::Find);
	Precache();
}

void CFuncTrackChange::Precache()
{
	// Can't trigger sound
	PRECACHE_SOUND("buttons/button11.wav");

	CFuncPlatRot::Precache();
}

// UNDONE: Filter touches before re-evaluating the train.

void CFuncTrackChange::Touch(CBaseEntity *pOther)
{
#if 0
	TRAIN_CODE code;
	entvars_t *pevToucher = pOther->pev;
#endif
}

void CFuncTrackChange::KeyValue(KeyValueData *pkvd)
{
	if (FStrEq(pkvd->szKeyName, "train")) {
		m_trainName = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "toptrack")) {
		m_trackTopName = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else if (FStrEq(pkvd->szKeyName, "bottomtrack")) {
		m_trackBottomName = ALLOC_STRING(pkvd->szValue);
		pkvd->fHandled = TRUE;
	} else {
		// Pass up to base class
		CFuncPlatRot::KeyValue(pkvd);
	}
}

void CFuncTrackChange::OverrideReset()
{
	pev->nextthink = pev->ltime + 1.0s;
	SetThink(&CFuncTrackChange::Find);
}

void CFuncTrackChange::Find()
{
	// Find track entities
	edict_t *target;

	target = FIND_ENTITY_BY_TARGETNAME(NULL, STRING(m_trackTopName));
	if (!FNullEnt(target)) {
		m_trackTop = CPathTrack::Instance(target);
		target = FIND_ENTITY_BY_TARGETNAME(NULL, STRING(m_trackBottomName));

		if (!FNullEnt(target)) {
			m_trackBottom = CPathTrack::Instance(target);
			target = FIND_ENTITY_BY_TARGETNAME(NULL, STRING(m_trainName));

			if (!FNullEnt(target)) {
				m_train = CFuncTrackTrain::Instance(FIND_ENTITY_BY_TARGETNAME(NULL, STRING(m_trainName)));

				if (!m_train) {
					ALERT(at_error, "Can't find train for track change! %s\n", STRING(m_trainName));
					return;
				}

				Vector center = (pev->absmin + pev->absmax) * 0.5;
				m_trackBottom = m_trackBottom->Nearest(center);
				m_trackTop = m_trackTop->Nearest(center);
				UpdateAutoTargets(m_toggle_state);
				SetThink(NULL);
				return;
			} else {
				ALERT(at_error, "Can't find train for track change! %s\n", STRING(m_trainName));
				target = FIND_ENTITY_BY_TARGETNAME(NULL, STRING(m_trainName));
			}
		} else
			ALERT(at_error, "Can't find bottom track for track change! %s\n", STRING(m_trackBottomName));
	} else
		ALERT(at_error, "Can't find top track for track change! %s\n", STRING(m_trackTopName));
}

TRAIN_CODE CFuncTrackChange::EvaluateTrain(CPathTrack *pcurrent)
{
	// Go ahead and work, we don't have anything to switch, so just be an elevator
	if (!pcurrent || !m_train)
		return TRAIN_SAFE;

	if (m_train->m_ppath == pcurrent || (pcurrent->m_pprevious && m_train->m_ppath == pcurrent->m_pprevious) ||
	    (pcurrent->m_pnext && m_train->m_ppath == pcurrent->m_pnext)) {
		if (m_train->pev->speed != 0)
			return TRAIN_BLOCKING;

		Vector dist = pev->origin - m_train->pev->origin;
		float length = dist.Length2D();

		// Empirically determined close distance
		if (length < m_train->m_length)
			return TRAIN_FOLLOWING;

		else if (length > (150 + m_train->m_length))
			return TRAIN_SAFE;

		return TRAIN_BLOCKING;
	}

	return TRAIN_SAFE;

}

void CFuncTrackChange::UpdateTrain(Vector &dest)
{
	duration_t time = (pev->nextthink - pev->ltime);

	m_train->pev->velocity = pev->velocity;
	m_train->pev->avelocity = pev->avelocity;
	m_train->NextThink(m_train->pev->ltime + time, FALSE);

	// Attempt at getting the train to rotate properly around the origin of the trackchange
	if (time <= 0s)
		return;

	Vector offset = m_train->pev->origin - pev->origin;
	Vector delta = dest - pev->angles;

	// Transform offset into local coordinates
	UTIL_MakeInvVectors(delta, gpGlobals);

	Vector local;
	local.x = DotProduct(offset, gpGlobals->v_forward);
	local.y = DotProduct(offset, gpGlobals->v_right);
	local.z = DotProduct(offset, gpGlobals->v_up);

	local = local - offset;
	m_train->pev->velocity = pev->velocity + (local * (1.0s / time));
}

void CFuncTrackChange::GoDown()
{
	if (m_code == TRAIN_BLOCKING)
		return;

	// HitBottom may get called during CFuncPlat::GoDown(), so set up for that
	// before you call GoDown()
	UpdateAutoTargets(TS_GOING_DOWN);

	// If ROTMOVE, move & rotate
	if (pev->spawnflags & SF_TRACK_DONT_MOVE) {
		SetMoveDone(&CFuncTrackChange::CallHitBottom);
		m_toggle_state = TS_GOING_DOWN;
		AngularMove(m_start, pev->speed);
	} else {
		CFuncPlat::GoDown();
		SetMoveDone(&CFuncTrackChange::CallHitBottom);
		RotMove(m_start, pev->nextthink - pev->ltime);
	}

	// Otherwise, rotate first, move second
	// If the train is moving with the platform, update it
	if (m_code == TRAIN_FOLLOWING) {
		UpdateTrain(m_start);
		m_train->m_ppath = NULL;
	}
}

// Platform is at bottom, now starts moving up

void CFuncTrackChange::GoUp()
{
	if (m_code == TRAIN_BLOCKING)
		return;

	// HitTop may get called during CFuncPlat::GoUp(), so set up for that
	// before you call GoUp();

	UpdateAutoTargets(TS_GOING_UP);
	if (pev->spawnflags & SF_TRACK_DONT_MOVE) {
		m_toggle_state = TS_GOING_UP;
		SetMoveDone(&CFuncTrackChange::CallHitTop);
		AngularMove(m_end, pev->speed);
	} else {
		// If ROTMOVE, move & rotate
		CFuncPlat::GoUp();
		SetMoveDone(&CFuncTrackChange::CallHitTop);
		RotMove(m_end, pev->nextthink - pev->ltime);
	}

	// Otherwise, move first, rotate second

	// If the train is moving with the platform, update it
	if (m_code == TRAIN_FOLLOWING) {
		UpdateTrain(m_end);
		m_train->m_ppath = NULL;
	}
}

// Normal track change

void CFuncTrackChange::UpdateAutoTargets(int toggleState)
{
	if (!m_trackTop || !m_trackBottom)
		return;

	if (toggleState == TS_AT_TOP)
		m_trackTop->pev->spawnflags &= ~SF_PATH_DISABLED;
	else
		m_trackTop->pev->spawnflags |= SF_PATH_DISABLED;

	if (toggleState == TS_AT_BOTTOM)
		m_trackBottom->pev->spawnflags &= ~SF_PATH_DISABLED;
	else
		m_trackBottom->pev->spawnflags |= SF_PATH_DISABLED;
}

void CFuncTrackChange::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	if (m_toggle_state != TS_AT_TOP && m_toggle_state != TS_AT_BOTTOM)
		return;

	// If train is in "safe" area, but not on the elevator, play alarm sound
	switch (m_toggle_state) {
		case TS_AT_TOP:
			m_code = EvaluateTrain(m_trackTop);
			break;
		case TS_AT_BOTTOM:
			m_code = EvaluateTrain(m_trackBottom);
			break;
		default:
			m_code = TRAIN_BLOCKING;
			break;
	}

	if (m_code == TRAIN_BLOCKING) {
		// Play alarm and return
		EMIT_SOUND(ENT(pev), CHAN_VOICE, "buttons/button11.wav", VOL_NORM, ATTN_NORM);
		return;
	}

	// Otherwise, it's safe to move
	// If at top, go down
	// at bottom, go up

	DisableUse();
	if (m_toggle_state == TS_AT_TOP)
		GoDown();
	else
		GoUp();
}

// Platform has hit bottom.  Stops and waits forever.

void CFuncTrackChange::HitBottom()
{
	CFuncPlatRot::HitBottom();
	if (m_code == TRAIN_FOLLOWING) {
		//UpdateTrain();
		m_train->SetTrack(m_trackBottom);
	}
	SetThink(NULL);
	pev->nextthink = invalid_time_point;

	UpdateAutoTargets(m_toggle_state);
	EnableUse();
}

// Platform has hit bottom.  Stops and waits forever.

void CFuncTrackChange::HitTop()
{
	CFuncPlatRot::HitTop();
	if (m_code == TRAIN_FOLLOWING) {
		//UpdateTrain();
		m_train->SetTrack(m_trackTop);
	}

	// Don't let the plat go back down
	SetThink(NULL);
	pev->nextthink = invalid_time_point;

	UpdateAutoTargets(m_toggle_state);
	EnableUse();
}

class CFuncTrackAuto : public CFuncTrackChange
{
public:
	virtual void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	virtual void UpdateAutoTargets(int toggleState);
};

LINK_ENTITY_TO_CLASS(func_trackautochange, CFuncTrackAuto);

// Auto track change

void CFuncTrackAuto::UpdateAutoTargets(int toggleState)
{
	CPathTrack *pTarget, *pNextTarget;

	if (!m_trackTop || !m_trackBottom)
		return;

	if (m_targetState == TS_AT_TOP) {
		pTarget = m_trackTop->GetNext();
		pNextTarget = m_trackBottom->GetNext();
	} else {
		pTarget = m_trackBottom->GetNext();
		pNextTarget = m_trackTop->GetNext();
	}

	if (pTarget != NULL) {
		pTarget->pev->spawnflags &= ~SF_PATH_DISABLED;

		if (m_code == TRAIN_FOLLOWING && m_train && m_train->pev->speed == 0) {
			m_train->Use(this, this, USE_ON, 0);
		}
	}

	if (pNextTarget != NULL) {
		pNextTarget->pev->spawnflags |= SF_PATH_DISABLED;
	}
}

void CFuncTrackAuto::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	CPathTrack *pTarget;

	if (!UseEnabled())
		return;

	switch (m_toggle_state) {
		case TS_AT_TOP:
			pTarget = m_trackTop;
			break;
		case TS_AT_BOTTOM:
			pTarget = m_trackBottom;
			break;
		default:
			pTarget = NULL;
			break;
	}

	if (FClassnameIs(pActivator->pev, "func_tracktrain")) {
		m_code = EvaluateTrain(pTarget);

		// Safe to fire?
		if (m_code == TRAIN_FOLLOWING && m_toggle_state != m_targetState) {
			DisableUse();

			if (m_toggle_state == TS_AT_TOP)
				GoDown();
			else
				GoUp();
		}
	} else {
		if (pTarget != NULL) {
			pTarget = pTarget->GetNext();
		}

		if (pTarget != NULL && m_train->m_ppath != pTarget && ShouldToggle(useType, m_targetState)) {
			if (m_targetState == TS_AT_TOP)
				m_targetState = TS_AT_BOTTOM;
			else
				m_targetState = TS_AT_TOP;
		}

		UpdateAutoTargets(m_targetState);
	}
}

LINK_ENTITY_TO_CLASS(func_guntarget, CGunTarget);

IMPLEMENT_SAVERESTORE(CGunTarget, CBaseMonster);

void CGunTarget::Spawn()
{
	pev->solid = SOLID_BSP;
	pev->movetype = MOVETYPE_PUSH;

	UTIL_SetOrigin(pev, pev->origin);
	SET_MODEL(ENT(pev), STRING(pev->model));

	if (pev->speed == 0) {
		pev->speed = 100;
	}

	// Don't take damage until "on"
	pev->takedamage = DAMAGE_NO;
	pev->flags |= FL_MONSTER;

	m_on = FALSE;
	pev->max_health = pev->health;

	if (pev->spawnflags & FGUNTARGET_START_ON) {
		SetThink(&CGunTarget::Start);
		pev->nextthink = pev->ltime + 0.3s;
	}
}

void CGunTarget::Activate()
{
	CBaseEntity *pTarg;

	// now find our next target
	pTarg = GetNextTarget();

	if (pTarg != NULL) {
		m_hTargetEnt = pTarg;
		UTIL_SetOrigin(pev, pTarg->pev->origin - (pev->mins + pev->maxs) * 0.5);
	}
}

void CGunTarget::Start()
{
	Use(this, this, USE_ON, 0);
}

void CGunTarget::Next()
{
	SetThink(NULL);

	m_hTargetEnt = GetNextTarget();
	CBaseEntity *pTarget = m_hTargetEnt;

	if (!pTarget) {
		Stop();
		return;
	}

	SetMoveDone(&CGunTarget::Wait);
	LinearMove(pTarget->pev->origin - (pev->mins + pev->maxs) * 0.5, pev->speed);
}

void CGunTarget::Wait()
{
	CBaseEntity *pTarget = m_hTargetEnt;

	if (!pTarget) {
		Stop();
		return;
	}

	// Fire the pass target if there is one
	if (pTarget->pev->message) {
		FireTargets(STRING(pTarget->pev->message), this, this, USE_TOGGLE, 0);

		if (pTarget->pev->spawnflags & SF_CORNER_FIREONCE) {
			pTarget->pev->message = 0;
		}
	}

	m_flWait = pTarget->GetDelay();
	pev->target = pTarget->pev->target;
	SetThink(&CGunTarget::Next);

	if (m_flWait != 0s) {
		// -1 wait will wait forever!
		pev->nextthink = pev->ltime + m_flWait;
	} else {
		// do it RIGHT now!
		Next();
	}
}

void CGunTarget::Stop()
{
	pev->velocity = g_vecZero;
	pev->nextthink = invalid_time_point;
	pev->takedamage = DAMAGE_NO;
}

int CGunTarget::TakeDamage(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType)
{
	if (pev->health > 0) {
		pev->health -= flDamage;

		if (pev->health <= 0) {
			pev->health = 0;
			Stop();

			if (pev->message) {
				FireTargets(STRING(pev->message), this, this, USE_TOGGLE, 0);
			}
		}
	}

	return 0;
}

void CGunTarget::Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	if (!ShouldToggle(useType, m_on))
		return;

	if (m_on) {
		Stop();
	} else {
		pev->takedamage = DAMAGE_AIM;
		m_hTargetEnt = GetNextTarget();
		if (m_hTargetEnt == nullptr)
			return;

		pev->health = pev->max_health;
		Next();
	}
}

}
