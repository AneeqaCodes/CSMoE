
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "player.h"
#include "talkmonster.h"
#include "gamerules.h"

#include "sound.h"

#include <ctype.h>
#include <chrono>

namespace sv {

// presets for runtime pitch and vol modulation of ambient sounds
dynpitchvol_t rgdpvpreset[CDPVPRESETMAX] =
{
	// pitch prun	pstart	spinup	spindwn	volrun	volstrt	fadein	fadeout	lfotype	lforate	lfomodp modvol	cspnup	cspnct	pitch	spupsv	spdwnsv	pfrac	vol	fdinsv	fdotsv	volfrac	lfofrac	lfomult
	{ 1,	255,	75,	95,	95,	10,	1,	50,	95,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 2,	255,	85,	70,	88,	10,	1,	20,	88,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 3,	255,	100,	50,	75,	10,	1,	10,	75,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 4,	100,	100,	0,	0,	10,	1,	90,	90,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 5,	100,	100,	0,	0,	10,	1,	80,	80,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 6,	100,	100,	0,	0,	10,	1,	50,	70,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 7,	100,	100,	0,	0,	5,	1,	40,	50,	1,	50,	0,	10,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 8,	100,	100,	0,	0,	5,	1,	40,	50,	1,	150,	0,	10,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 9,	100,	100,	0,	0,	5,	1,	40,	50,	1,	750,	0,	10,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 10,	128,	100,	50,	75,	10,	1,	30,	40,	2,	8,	20,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 11,	128,	100,	50,	75,	10,	1,	30,	40,	2,	25,	20,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 12,	128,	100,	50,	75,	10,	1,	30,	40,	2,	70,	20,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 13,	50,	50,	0,	0,	10,	1,	20,	50,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 14,	70,	70,	0,	0,	10,	1,	20,	50,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 15,	90,	90,	0,	0,	10,	1,	20,	50,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 16,	120,	120,	0,	0,	10,	1,	20,	50,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 17,	180,	180,	0,	0,	10,	1,	20,	50,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 18,	255,	255,	0,	0,	10,	1,	20,	50,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 19,	200,	75,	90,	90,	10,	1,	50,	90,	2,	100,	20,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 20,	255,	75,	97,	90,	10,	1,	50,	90,	1,	40,	50,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 21,	100,	100,	0,	0,	10,	1,	30,	50,	3,	15,	20,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 22,	160,	160,	0,	0,	10,	1,	50,	50,	3,	500,	25,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 23,	255,	75,	88,	0,	10,	1,	40,	0,	0,	0,	0,	0,	5,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 24,	200,	20,	95,	70,	10,	1,	70,	70,	3,	20,	50,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 25,	180,	100,	50,	60,	10,	1,	40,	60,	2,	90,	100,	100,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 26,	60,	60,	0,	0,	10,	1,	40,	70,	3,	80,	20,	50,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 },
	{ 27,	128,	90,	10,	10,	10,	1,	20,	40,	1,	5,	10,	20,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0 }
};

int gcallsentences = 0;
int fSentencesInit = FALSE;

TYPEDESCRIPTION CAmbientGeneric::m_SaveData[] =
{
	DEFINE_FIELD(CAmbientGeneric, m_flAttenuation, FIELD_FLOAT),
	DEFINE_FIELD(CAmbientGeneric, m_fActive, FIELD_BOOLEAN),
	DEFINE_FIELD(CAmbientGeneric, m_fLooping, FIELD_BOOLEAN),

	// HACKHACK - This is not really in the spirit of the save/restore design, but save this
	// out as a binary data block.  If the dynpitchvol_t is changed, old saved games will NOT
	// load these correctly, so bump the save/restore version if you change the size of the struct
	// The right way to do this is to split the input parms (read in keyvalue) into members and re-init this
	// struct in Precache(), but it's unlikely that the struct will change, so it's not worth the time right now.
	DEFINE_ARRAY(CAmbientGeneric, m_dpv, FIELD_CHARACTER, sizeof(dynpitchvol_t)),
};

TYPEDESCRIPTION CEnvSound::m_SaveData[] =
{
	DEFINE_FIELD(CEnvSound, m_flRadius, FIELD_FLOAT),
	DEFINE_FIELD(CEnvSound, m_flRoomtype, FIELD_FLOAT),
};

TYPEDESCRIPTION CSpeaker::m_SaveData[] =
{
	DEFINE_FIELD(CSpeaker, m_preset, FIELD_INTEGER),
};

// time delay until it's ok to speak: used so that two NPCs don't talk at once
time_point_t CTalkMonster::g_talkWaitTime;

char gszallsentencenames[ CVOXFILESENTENCEMAX ][ CBSENTENCENAME_MAX ];
sentenceg rgsentenceg[ CSENTENCEG_MAX ];

#define CTEXTURESMAX		512			// max number of textures loaded

char grgszTextureName[ CTEXTURESMAX ][ CBTEXTURENAMEMAX ];
char grgchTextureType[ CTEXTURESMAX ];
int fTextureTypeInit;
int gcTextures;

LINK_ENTITY_TO_CLASS(ambient_generic, CAmbientGeneric);

IMPLEMENT_SAVERESTORE(CAmbientGeneric, CBaseEntity);

// -1 : "Default"
// 0  : "Everywhere"
// 200 : "Small Radius"
// 125 : "Medium Radius"
// 80  : "Large Radius"

void CAmbientGeneric::Spawn()
{
	if (pev->spawnflags & AMBIENT_SOUND_EVERYWHERE)
	{
		m_flAttenuation = ATTN_NONE;
	}
	else if (pev->spawnflags & AMBIENT_SOUND_SMALLRADIUS)
	{
		m_flAttenuation = ATTN_IDLE;
	}
	else if (pev->spawnflags & AMBIENT_SOUND_MEDIUMRADIUS)
	{
		m_flAttenuation = ATTN_STATIC;
	}
	else if (pev->spawnflags & AMBIENT_SOUND_LARGERADIUS)
	{
		m_flAttenuation = ATTN_NORM;
	}
	else
	{
		// if the designer didn't set a sound attenuation, default to one.
		m_flAttenuation = ATTN_STATIC;
	}

	char *szSoundFile = (char *)STRING(pev->message);

	if (FStringNull(pev->message) || Q_strlen(szSoundFile) < 1)
	{
		ALERT(at_error, "EMPTY AMBIENT AT: %f, %f, %f\n", pev->origin.x, pev->origin.y, pev->origin.z);
		pev->nextthink = gpGlobals->time + 0.1s;
		SetThink(&CAmbientGeneric::SUB_Remove);
		return;
	}

	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;

	// Set up think function for dynamic modification
	// of ambient sound's pitch or volume. Don't
	// start thinking yet.

	SetThink(&CAmbientGeneric::RampThink);
	pev->nextthink = invalid_time_point;

	// allow on/off switching via 'use' function.
	SetUse(&CAmbientGeneric::ToggleUse);

	m_fActive = FALSE;

	if (pev->spawnflags & AMBIENT_SOUND_NOT_LOOPING)
		m_fLooping = FALSE;
	else
		m_fLooping = TRUE;

	Precache();
}

void CAmbientGeneric::Restart()
{
	if (pev->spawnflags & AMBIENT_SOUND_EVERYWHERE)
	{
		m_flAttenuation = ATTN_NONE;
	}
	else if (pev->spawnflags & AMBIENT_SOUND_SMALLRADIUS)
	{
		m_flAttenuation = ATTN_IDLE;
	}
	else if (pev->spawnflags & AMBIENT_SOUND_MEDIUMRADIUS)
	{
		m_flAttenuation = ATTN_STATIC;
	}
	else if (pev->spawnflags & AMBIENT_SOUND_LARGERADIUS)
	{
		m_flAttenuation = ATTN_NORM;
	}
	else
	{
		// if the designer didn't set a sound attenuation, default to one.
		m_flAttenuation = ATTN_STATIC;
	}

	char *szSoundFile = (char *)STRING(pev->message);

	if (FStringNull(pev->message) || Q_strlen(szSoundFile) < 1)
	{
		ALERT(at_error, "EMPTY AMBIENT AT: %f, %f, %f\n", pev->origin.x, pev->origin.y, pev->origin.z);
		pev->nextthink = gpGlobals->time + 0.1s;
		SetThink(&CBaseEntity::SUB_Remove);
		return;
	}

	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;

	// Set up think function for dynamic modification
	// of ambient sound's pitch or volume. Don't
	// start thinking yet.
	SetThink(&CAmbientGeneric::RampThink);
	pev->nextthink = invalid_time_point;

	// allow on/off switching via 'use' function.
	SetUse(&CAmbientGeneric::ToggleUse);

	m_fActive = FALSE;

	UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile, 0, 0, SND_STOP, 0);
	InitModulationParms();
	pev->nextthink = gpGlobals->time + 0.1s;

	if (!(pev->spawnflags & AMBIENT_SOUND_NOT_LOOPING))
	{
		m_fLooping = TRUE;
		m_fActive = TRUE;
	}
	else
		m_fLooping = FALSE;

	if (m_fActive)
	{
		UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile, (m_dpv.vol * 0.01f), m_flAttenuation, 0, m_dpv.pitch);
	}
}

void CAmbientGeneric::Precache()
{
	char *szSoundFile = (char *)STRING(pev->message);

	if (!FStringNull(pev->message) && Q_strlen(szSoundFile) > 1)
	{
		if (*szSoundFile != '!')
		{
			PRECACHE_SOUND(szSoundFile);
		}
	}

	// init all dynamic modulation parms
	InitModulationParms();

	if (!(pev->spawnflags & AMBIENT_SOUND_START_SILENT))
	{
		// start the sound ASAP
		if (m_fLooping)
		{
			m_fActive = TRUE;
		}
	}

	if (m_fActive)
	{
		UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile, (m_dpv.vol * 0.01), m_flAttenuation, SND_SPAWNING, m_dpv.pitch);
		pev->nextthink = gpGlobals->time + 0.1s;
	}
}

// RampThink - Think at 5hz if we are dynamically modifying
// pitch or volume of the playing sound.  This function will
// ramp pitch and/or volume up or down, modify pitch/volume
// with lfo if active.

void CAmbientGeneric::RampThink()
{
	char *szSoundFile = (char *)STRING(pev->message);
	int pitch = m_dpv.pitch;
	int vol = m_dpv.vol;
	int flags = 0;
	int fChanged = 0;		// FALSE if pitch and vol remain unchanged this round
	int prev;

	if (!m_dpv.spinup && !m_dpv.spindown && !m_dpv.fadein && !m_dpv.fadeout && !m_dpv.lfotype)
	{
		// no ramps or lfo, stop thinking
		return;
	}

	// pitch envelope
	if (m_dpv.spinup || m_dpv.spindown)
	{
		prev = m_dpv.pitchfrac >> 8;

		if (m_dpv.spinup > 0)
		{
			m_dpv.pitchfrac += m_dpv.spinup;
		}
		else if (m_dpv.spindown > 0)
		{
			m_dpv.pitchfrac -= m_dpv.spindown;
		}

		pitch = m_dpv.pitchfrac >> 8;

		if (pitch > m_dpv.pitchrun)
		{
			pitch = m_dpv.pitchrun;
			m_dpv.spinup = 0;	// done with ramp up
		}

		if (pitch < m_dpv.pitchstart)
		{
			pitch = m_dpv.pitchstart;
			m_dpv.spindown = 0;	// done with ramp down

			// shut sound off
			UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile, 0, 0, SND_STOP, 0);

			// return without setting nextthink
			return;
		}

		// pitch clamp
		if (pitch > 255)
			pitch = 255;
		if (pitch < 1)
			pitch = 1;

		//pitch = Q_max(1, Q_min(255, pitch));

		m_dpv.pitch = pitch;

		fChanged |= (prev != pitch);
		flags |= SND_CHANGE_PITCH;
	}

	// amplitude envelope
	if (m_dpv.fadein || m_dpv.fadeout)
	{
		prev = m_dpv.volfrac >> 8;

		if (m_dpv.fadein > 0)
		{
			m_dpv.volfrac += m_dpv.fadein;
		}
		else if (m_dpv.fadeout > 0)
		{
			m_dpv.volfrac -= m_dpv.fadeout;
		}

		vol = m_dpv.volfrac >> 8;

		if (vol > m_dpv.volrun)
		{
			vol = m_dpv.volrun;
			m_dpv.fadein = 0;	// done with ramp up
		}

		if (vol < m_dpv.volstart)
		{
			vol = m_dpv.volstart;
			m_dpv.fadeout = 0;	// done with ramp down

			// shut sound off
			UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile, 0, 0, SND_STOP, 0);

			// return without setting nextthink
			return;
		}

		// volume clamp
		if (vol > 100)
			vol = 100;
		if (vol < 1)
			vol = 1;

		//vol = Q_max(1, Q_min(100, vol));

		m_dpv.vol = vol;

		fChanged |= (prev != vol);
		flags |= SND_CHANGE_VOL;
	}

	// pitch/amplitude LFO
	if (m_dpv.lfotype)
	{
		int pos;

		if (m_dpv.lfofrac > 0x6fffffff)
			m_dpv.lfofrac = 0;

		// update lfo, lfofrac/255 makes a triangle wave 0-255
		m_dpv.lfofrac += m_dpv.lforate;
		pos = m_dpv.lfofrac >> 8;

		if (m_dpv.lfofrac < 0)
		{
			m_dpv.lfofrac = 0;
			m_dpv.lforate = abs(m_dpv.lforate);
			pos = 0;
		}
		else if (pos > 255)
		{
			pos = 255;
			m_dpv.lfofrac = (255 << 8);
			m_dpv.lforate = -abs(m_dpv.lforate);
		}

		switch (m_dpv.lfotype)
		{
		case LFO_SQUARE:
			if (pos < 128)
				m_dpv.lfomult = 255;
			else
				m_dpv.lfomult = 0;

			break;
		case LFO_RANDOM:
			if (pos == 255)
				m_dpv.lfomult = RANDOM_LONG(0, 255);
			break;
		case LFO_TRIANGLE:
		default:
			m_dpv.lfomult = pos;
			break;
		}

		if (m_dpv.lfomodpitch)
		{
			prev = pitch;

			// pitch 0-255
			pitch += ((m_dpv.lfomult - 128) * m_dpv.lfomodpitch) / 100;

			// pitch clamp
			if (pitch > 255)
				pitch = 255;
			if (pitch < 1)
				pitch = 1;

			//pitch = Q_max(1, Q_min(255, pitch));

			fChanged |= (prev != pitch);
			flags |= SND_CHANGE_PITCH;
		}

		if (m_dpv.lfomodvol)
		{
			// vol 0-100
			prev = vol;

			vol += ((m_dpv.lfomult - 128) * m_dpv.lfomodvol) / 100;

			// volume clamp
			if (vol > 100)
				vol = 100;
			if (vol < 0)
				vol = 0;

			fChanged |= (prev != vol);
			flags |= SND_CHANGE_VOL;
		}
	}

	// Send update to playing sound only if we actually changed
	// pitch or volume in this routine.
	if (flags && fChanged)
	{
		if (pitch == PITCH_NORM)
		{
			// don't send 'no pitch' !
			pitch = PITCH_NORM + 1;
		}

		UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile, (vol * 0.01), m_flAttenuation, flags, pitch);
	}

	// update ramps at 5hz
	pev->nextthink = gpGlobals->time + 0.2s;
	return;
}

// Init all ramp params in preparation to
// play a new sound

void CAmbientGeneric::InitModulationParms()
{
	int pitchinc;

	// 0 - 100
	m_dpv.volrun = pev->health * 10;

	if (m_dpv.volrun > 100)
		m_dpv.volrun = 100;
	if (m_dpv.volrun < 0)
		m_dpv.volrun = 0;

	// get presets
	if (m_dpv.preset != 0 && m_dpv.preset <= CDPVPRESETMAX)
	{
		// load preset values
		m_dpv = rgdpvpreset[m_dpv.preset - 1];

		// fixup preset values, just like
		// fixups in KeyValue routine.
		if (m_dpv.spindown > 0)
		{
			m_dpv.spindown = (101 - m_dpv.spindown) * 64;
		}
		if (m_dpv.spinup > 0)
		{
			m_dpv.spinup = (101 - m_dpv.spinup) * 64;
		}

		m_dpv.volstart *= 10;
		m_dpv.volrun *= 10;

		if (m_dpv.fadein > 0)
		{
			m_dpv.fadein = (101 - m_dpv.fadein) * 64;
		}

		if (m_dpv.fadeout > 0)
		{
			m_dpv.fadeout = (101 - m_dpv.fadeout) * 64;
		}

		m_dpv.lforate *= 256;

		m_dpv.fadeinsav = m_dpv.fadein;
		m_dpv.fadeoutsav = m_dpv.fadeout;
		m_dpv.spinupsav = m_dpv.spinup;
		m_dpv.spindownsav = m_dpv.spindown;
	}

	m_dpv.fadein = m_dpv.fadeinsav;
	m_dpv.fadeout = 0;

	if (m_dpv.fadein)
		m_dpv.vol = m_dpv.volstart;
	else
		m_dpv.vol = m_dpv.volrun;

	m_dpv.spinup = m_dpv.spinupsav;
	m_dpv.spindown = 0;

	if (m_dpv.spinup)
		m_dpv.pitch = m_dpv.pitchstart;
	else
		m_dpv.pitch = m_dpv.pitchrun;

	if (m_dpv.pitch == 0)
		m_dpv.pitch = PITCH_NORM;

	m_dpv.pitchfrac = m_dpv.pitch << 8;
	m_dpv.volfrac = m_dpv.vol << 8;

	m_dpv.lfofrac = 0;
	m_dpv.lforate = abs(m_dpv.lforate);

	m_dpv.cspincount = 1;

	if (m_dpv.cspinup)
	{
		pitchinc = (255 - m_dpv.pitchstart) / m_dpv.cspinup;

		m_dpv.pitchrun = m_dpv.pitchstart + pitchinc;
		if (m_dpv.pitchrun > 255)
		{
			m_dpv.pitchrun = 255;
		}
	}

	if ((m_dpv.spinupsav || m_dpv.spindownsav || (m_dpv.lfotype && m_dpv.lfomodpitch)) && m_dpv.pitch == PITCH_NORM)
	{
		// must never send 'no pitch' as first pitch
		// if we intend to pitch shift later!
		m_dpv.pitch = PITCH_NORM + 1;
	}
}

// ToggleUse - turns an ambient sound on or off.  If the
// ambient is a looping sound, mark sound as active (m_fActive)
// if it's playing, innactive if not.  If the sound is not
// a looping sound, never mark it as active.

void CAmbientGeneric::ToggleUse(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	char *szSoundFile = (char *)STRING(pev->message);
	float fraction;

	if (useType != USE_TOGGLE)
	{
		if ((m_fActive && useType == USE_ON) || (!m_fActive && useType == USE_OFF))
			return;
	}

	// Directly change pitch if arg passed. Only works if sound is already playing.
	// Momentary buttons will pass down a float in here
	if (useType == USE_SET && m_fActive)
	{
		fraction = value;

		if (fraction > 1.0f)
			fraction = 1.0f;

		if (fraction < 0.0f)
			fraction = 0.01f;

		m_dpv.pitch = fraction * 255;
		UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile, 0, 0, SND_CHANGE_PITCH, m_dpv.pitch);
		return;
	}

	// m_fActive is TRUE only if a looping sound is playing.
	// turn sound off
	if (m_fActive)
	{
		if (m_dpv.cspinup)
		{
			// Don't actually shut off. Each toggle causes
			// incremental spinup to max pitch
			if (m_dpv.cspincount <= m_dpv.cspinup)
			{
				int pitchinc;

				// start a new spinup
				m_dpv.cspincount++;

				pitchinc = (255 - m_dpv.pitchstart) / m_dpv.cspinup;

				m_dpv.spinup = m_dpv.spinupsav;
				m_dpv.spindown = 0;

				m_dpv.pitchrun = m_dpv.pitchstart + pitchinc * m_dpv.cspincount;
				if (m_dpv.pitchrun > 255)
				{
					m_dpv.pitchrun = 255;
				}
				pev->nextthink = gpGlobals->time + 0.1s;
			}
		}
		else
		{
			m_fActive = FALSE;

			// HACKHACK - this makes the code in Precache() work properly after a save/restore
			pev->spawnflags |= AMBIENT_SOUND_START_SILENT;
			if (m_dpv.spindownsav || m_dpv.fadeoutsav)
			{
				// spin it down (or fade it) before shutoff if spindown is set
				m_dpv.spindown = m_dpv.spindownsav;
				m_dpv.spinup = 0;

				m_dpv.fadeout = m_dpv.fadeoutsav;
				m_dpv.fadein = 0;
				pev->nextthink = gpGlobals->time + 0.1s;
			}
			else
				UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile, 0, 0, SND_STOP, 0);
		}
	}
	// turn sound on
	else
	{
		// only toggle if this is a looping sound.  If not looping, each
		// trigger will cause the sound to play.  If the sound is still
		// playing from a previous trigger press, it will be shut off
		// and then restarted.
		if (m_fLooping)
			m_fActive = TRUE;
		else
		{
			// shut sound off now - may be interrupting a long non-looping sound
			UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile, 0, 0, SND_STOP, 0);
		}

		// init all ramp params for startup
		InitModulationParms();
		UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile, (m_dpv.vol * 0.01), m_flAttenuation, 0, m_dpv.pitch);
		pev->nextthink = gpGlobals->time + 0.1s;
	}
}

// KeyValue - load keyvalue pairs into member data of the
// ambient generic. NOTE: called BEFORE spawn!

void CAmbientGeneric::KeyValue(KeyValueData *pkvd)
{
	// NOTE: changing any of the modifiers in this code
	// NOTE: also requires changing InitModulationParms code.

	// preset
	if (FStrEq(pkvd->szKeyName, "preset"))
	{
		m_dpv.preset = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}

	// pitchrun
	else if (FStrEq(pkvd->szKeyName, "pitch"))
	{
		m_dpv.pitchrun = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;

		if (m_dpv.pitchrun > 255)
			m_dpv.pitchrun = 255;

		if (m_dpv.pitchrun < 0)
			m_dpv.pitchrun = 0;
	}

	// pitchstart
	else if (FStrEq(pkvd->szKeyName, "pitchstart"))
	{
		m_dpv.pitchstart = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;

		if (m_dpv.pitchstart > 255)
			m_dpv.pitchstart = 255;

		if (m_dpv.pitchstart < 0)
			m_dpv.pitchstart = 0;
	}

	// spinup
	else if (FStrEq(pkvd->szKeyName, "spinup"))
	{
		m_dpv.spinup = Q_atoi(pkvd->szValue);

		if (m_dpv.spinup > 100)
			m_dpv.spinup = 100;
		if (m_dpv.spinup < 0)
			m_dpv.spinup = 0;

		if (m_dpv.spinup > 0)
			m_dpv.spinup = (101 - m_dpv.spinup) * 64;

		m_dpv.spinupsav = m_dpv.spinup;
		pkvd->fHandled = TRUE;
	}

	// spindown
	else if (FStrEq(pkvd->szKeyName, "spindown"))
	{
		m_dpv.spindown = Q_atoi(pkvd->szValue);

		if (m_dpv.spindown > 100)
			m_dpv.spindown = 100;

		if (m_dpv.spindown < 0)
			m_dpv.spindown = 0;

		if (m_dpv.spindown > 0)
			m_dpv.spindown = (101 - m_dpv.spindown) * 64;

		m_dpv.spindownsav = m_dpv.spindown;
		pkvd->fHandled = TRUE;
	}

	// volstart
	else if (FStrEq(pkvd->szKeyName, "volstart"))
	{
		m_dpv.volstart = Q_atoi(pkvd->szValue);

		if (m_dpv.volstart > 10)
			m_dpv.volstart = 10;

		if (m_dpv.volstart < 0)
			m_dpv.volstart = 0;

		// 0 - 100
		m_dpv.volstart *= 10;

		pkvd->fHandled = TRUE;
	}

	// fadein
	else if (FStrEq(pkvd->szKeyName, "fadein"))
	{
		m_dpv.fadein = Q_atoi(pkvd->szValue);

		if (m_dpv.fadein > 100)
			m_dpv.fadein = 100;

		if (m_dpv.fadein < 0)
			m_dpv.fadein = 0;

		if (m_dpv.fadein > 0)
			m_dpv.fadein = (101 - m_dpv.fadein) * 64;

		m_dpv.fadeinsav = m_dpv.fadein;
		pkvd->fHandled = TRUE;
	}

	// fadeout
	else if (FStrEq(pkvd->szKeyName, "fadeout"))
	{
		m_dpv.fadeout = Q_atoi(pkvd->szValue);

		if (m_dpv.fadeout > 100)
			m_dpv.fadeout = 100;

		if (m_dpv.fadeout < 0)
			m_dpv.fadeout = 0;

		if (m_dpv.fadeout > 0)
			m_dpv.fadeout = (101 - m_dpv.fadeout) * 64;

		m_dpv.fadeoutsav = m_dpv.fadeout;
		pkvd->fHandled = TRUE;
	}

	// lfotype
	else if (FStrEq(pkvd->szKeyName, "lfotype"))
	{
		m_dpv.lfotype = Q_atoi(pkvd->szValue);

		if (m_dpv.lfotype > 4)
			m_dpv.lfotype = LFO_TRIANGLE;

		pkvd->fHandled = TRUE;
	}

	// lforate
	else if (FStrEq(pkvd->szKeyName, "lforate"))
	{
		m_dpv.lforate = Q_atoi(pkvd->szValue);

		if (m_dpv.lforate > 1000)
			m_dpv.lforate = 1000;

		if (m_dpv.lforate < 0)
			m_dpv.lforate = 0;

		m_dpv.lforate *= 256;
		pkvd->fHandled = TRUE;
	}
	// lfomodpitch
	else if (FStrEq(pkvd->szKeyName, "lfomodpitch"))
	{
		m_dpv.lfomodpitch = Q_atoi(pkvd->szValue);

		if (m_dpv.lfomodpitch > 100)
			m_dpv.lfomodpitch = 100;

		if (m_dpv.lfomodpitch < 0)
			m_dpv.lfomodpitch = 0;

		pkvd->fHandled = TRUE;
	}

	// lfomodvol
	else if (FStrEq(pkvd->szKeyName, "lfomodvol"))
	{
		m_dpv.lfomodvol = Q_atoi(pkvd->szValue);

		if (m_dpv.lfomodvol > 100)
			m_dpv.lfomodvol = 100;

		if (m_dpv.lfomodvol < 0)
			m_dpv.lfomodvol = 0;

		pkvd->fHandled = TRUE;
	}

	// cspinup
	else if (FStrEq(pkvd->szKeyName, "cspinup"))
	{
		m_dpv.cspinup = Q_atoi(pkvd->szValue);

		if (m_dpv.cspinup > 100)
			m_dpv.cspinup = 100;

		if (m_dpv.cspinup < 0)
			m_dpv.cspinup = 0;

		pkvd->fHandled = TRUE;
	}
	else
		CBaseEntity::KeyValue(pkvd);
}

LINK_ENTITY_TO_CLASS(env_sound, CEnvSound);

IMPLEMENT_SAVERESTORE(CEnvSound, CBaseEntity);

void CEnvSound::KeyValue(KeyValueData *pkvd)
{
	if (FStrEq(pkvd->szKeyName, "radius"))
	{
		m_flRadius = Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	if (FStrEq(pkvd->szKeyName, "roomtype"))
	{
		m_flRoomtype = Q_atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
}

// returns TRUE if the given sound entity (pev) is in range
// and can see the given player entity (pevTarget)

BOOL FEnvSoundInRange(entvars_t *pev, entvars_t *pevTarget, float *pflRange)
{
	CEnvSound *pSound = GetClassPtr<CEnvSound>(pev);
	Vector vecSpot1 = pev->origin + pev->view_ofs;
	Vector vecSpot2 = pevTarget->origin + pevTarget->view_ofs;
	Vector vecRange;
	float flRange;
	TraceResult tr;

	UTIL_TraceLine(vecSpot1, vecSpot2, ignore_monsters, ENT(pev), &tr);

	// check if line of sight crosses water boundary, or is blocked
	if ((tr.fInOpen && tr.fInWater) || tr.flFraction != 1.0f)
	{
		return FALSE;
	}

	// calc range from sound entity to player
	vecRange = tr.vecEndPos - vecSpot1;
	flRange = vecRange.Length();

	if (pSound->m_flRadius < flRange)
		return FALSE;

	if (pflRange)
	{
		*pflRange = flRange;
	}

	return TRUE;
}

// A client that is visible and in range of a sound entity will
// have its room_type set by that sound entity.  If two or more
// sound entities are contending for a client, then the nearest
// sound entity to the client will set the client's room_type.
// A client's room_type will remain set to its prior value until
// a new in-range, visible sound entity resets a new room_type.

// CONSIDER: if player in water state, autoset roomtype to 14,15 or 16.

void CEnvSound::Think()
{
	// get pointer to client if visible; FIND_CLIENT_IN_PVS will
	// cycle through visible clients on consecutive calls.
	edict_t *pentPlayer = FIND_CLIENT_IN_PVS(edict());
	CBasePlayer *pPlayer = NULL;

	if (FNullEnt(pentPlayer))
	{
		// no player in pvs of sound entity, slow it down
		goto env_sound_Think_slow;
	}

	pPlayer = GetClassPtr<CBasePlayer>(VARS(pentPlayer));
	float flRange;

	// check to see if this is the sound entity that is
	// currently affecting this player

	if (!FNullEnt(pPlayer->m_pentSndLast) && pPlayer->m_pentSndLast == ENT(pev))
	{
		// this is the entity currently affecting player, check
		// for validity
		if (pPlayer->m_flSndRoomtype != 0 && pPlayer->m_flSndRange != 0)
		{
			// we're looking at a valid sound entity affecting
			// player, make sure it's still valid, update range
			if (FEnvSoundInRange(pev, VARS(pentPlayer), &flRange))
			{
				pPlayer->m_flSndRange = flRange;
				goto env_sound_Think_fast;
			}
			else
			{
				// current sound entity affecting player is no longer valid,
				// flag this state by clearing room_type and range.
				// NOTE: we do not actually change the player's room_type
				// NOTE: until we have a new valid room_type to change it to.

				pPlayer->m_flSndRange = 0;
				pPlayer->m_flSndRoomtype = 0;
				goto env_sound_Think_slow;
			}
		}
		else
		{
			// entity is affecting player but is out of range,
			// wait passively for another entity to usurp it...
			goto env_sound_Think_slow;
		}
	}

	// if we got this far, we're looking at an entity that is contending
	// for current player sound. the closest entity to player wins.
	if (FEnvSoundInRange(pev, VARS(pentPlayer), &flRange))
	{
		if (flRange < pPlayer->m_flSndRange || pPlayer->m_flSndRange == 0)
		{
			// new entity is closer to player, so it wins.
			pPlayer->m_pentSndLast = ENT(pev);
			pPlayer->m_flSndRoomtype = m_flRoomtype;
			pPlayer->m_flSndRange = flRange;

			// send room_type command to player's server.
			// this should be a rare event - once per change of room_type
			// only!

			//CLIENT_COMMAND(pentPlayer, "room_type %f", m_flRoomtype);
			MESSAGE_BEGIN(MSG_ONE, SVC_ROOMTYPE, NULL, pentPlayer);	// use the magic #1 for "one client"
				WRITE_SHORT((short)m_flRoomtype);			// sequence number
			MESSAGE_END();

			// crank up nextthink rate for new active sound entity
			// by falling through to think_fast...
		}
		// player is not closer to the contending sound entity,
		// just fall through to think_fast. this effectively
		// cranks up the think_rate of entities near the player.
	}

	// player is in pvs of sound entity, but either not visible or
	// not in range. do nothing, fall through to think_fast...

env_sound_Think_fast:
	pev->nextthink = gpGlobals->time + 0.25s;
	return;

env_sound_Think_slow:
	pev->nextthink = gpGlobals->time + 0.75s;
	return;
}

// env_sound - spawn a sound entity that will set player roomtype
// when player moves in range and sight.

void CEnvSound::Spawn()
{
	// spread think times
	pev->nextthink = gpGlobals->time + RandomDuration(0.0s, 0.5s);
}

// randomize list of sentence name indices

void USENTENCEG_InitLRU(unsigned char *plru, int count)
{
	int i, j, k;
	unsigned char temp;

	if (!fSentencesInit)
		return;

	if (count > CSENTENCE_LRU_MAX)
		count = CSENTENCE_LRU_MAX;

	for (i = 0; i < count; ++i)
		plru[i] = (unsigned char)i;

	// randomize array
	for (i = 0; i < (count * 4); ++i)
	{
		j = RANDOM_LONG(0, count - 1);
		k = RANDOM_LONG(0, count - 1);

		temp = plru[j];
		plru[j] = plru[k];
		plru[k] = temp;
	}
}

// ignore lru. pick next sentence from sentence group. Go in order until we hit the last sentence,
// then repeat list if freset is true.  If freset is false, then repeat last sentence.
// ipick is passed in as the requested sentence ordinal.
// ipick 'next' is returned.
// return of -1 indicates an error.

int USENTENCEG_PickSequential(int isentenceg, char *szfound, int ipick, int freset)
{
	char *szgroupname;
	unsigned char count;
	char sznum[8];

	if (!fSentencesInit)
		return -1;

	if (isentenceg < 0)
		return -1;

	szgroupname = rgsentenceg[isentenceg].szgroupname;
	count = rgsentenceg[isentenceg].count;

	if (count == 0)
		return -1;

	if (ipick >= count)
		ipick = count - 1;

	Q_strcpy(szfound, "!");
	Q_strcat(szfound, szgroupname);
	Q_sprintf(sznum, "%d", ipick);
	Q_strcat(szfound, sznum);

	if (ipick >= count)
	{
		if (freset)
			return 0; // reset at end of list
		else
			return count;
	}

	return ipick + 1;
}

// pick a random sentence from rootname0 to rootnameX.
// picks from the rgsentenceg[isentenceg] least
// recently used, modifies lru array. returns the sentencename.
// note, lru must be seeded with 0-n randomized sentence numbers, with the
// rest of the lru filled with -1. The first integer in the lru is
// actually the size of the list.  Returns ipick, the ordinal
// of the picked sentence within the group.

int USENTENCEG_Pick(int isentenceg, char *szfound)
{
	char *szgroupname;
	unsigned char *plru;
	unsigned char i;
	unsigned char count;
	char sznum[8];
	unsigned char ipick = 0xFF;
	BOOL ffound = FALSE;

	if (!fSentencesInit)
		return -1;

	if (isentenceg < 0)
		return -1;

	szgroupname = rgsentenceg[isentenceg].szgroupname;
	count = rgsentenceg[isentenceg].count;
	plru = rgsentenceg[isentenceg].rgblru;

	while (!ffound)
	{
		for (i = 0; i < count; ++i)
		{
			if (plru[i] != 0xFF)
			{
				ipick = plru[i];
				plru[i] = 0xFF;
				ffound = TRUE;
				break;
			}
		}

		if (ffound)
		{
			Q_strcpy(szfound, "!");
			Q_strcat(szfound, szgroupname);
			Q_sprintf(sznum, "%d", ipick);
			Q_strcat(szfound, sznum);

			return ipick;
		}
		else
			USENTENCEG_InitLRU(plru, count);
	}

	return -1;
}

// Given sentence group rootname (name without number suffix),
// get sentence group index (isentenceg). Returns -1 if no such name.

int SENTENCEG_GetIndex(const char *szgroupname)
{
	int i;

	if (!fSentencesInit || !szgroupname)
		return -1;

	// search rgsentenceg for match on szgroupname

	i = 0;
	while (rgsentenceg[i].count != 0)
	{
		if (!Q_strcmp(szgroupname, rgsentenceg[i].szgroupname))
			return i;

		++i;
	}

	return -1;
}

// given sentence group index, play random sentence for given entity.
// returns ipick - which sentence was picked to
// play from the group. Ipick is only needed if you plan on stopping
// the sound before playback is done (see SENTENCEG_Stop).

int SENTENCEG_PlayRndI(edict_t *entity, int isentenceg, float volume, float attenuation, int flags, int pitch)
{
	char name[64];
	int ipick;

	if (!fSentencesInit)
		return -1;

	name[0] = '\0';

	ipick = USENTENCEG_Pick(isentenceg, name);
	if( ipick > 0 )
		EMIT_SOUND_DYN(entity, CHAN_VOICE, name, volume, attenuation, flags, pitch);

	return ipick;
}

// same as above, but takes sentence group name instead of index

int SENTENCEG_PlayRndSz(edict_t *entity, const char *szgroupname, float volume, float attenuation, int flags, int pitch)
{
	char name[64];
	int ipick;
	int isentenceg;

	if (!fSentencesInit)
		return -1;

	name[0] = '\0';

	isentenceg = SENTENCEG_GetIndex(szgroupname);
	if (isentenceg < 0)
	{
		ALERT(at_console, "No such sentence group %s\n", szgroupname);
		return -1;
	}

	ipick = USENTENCEG_Pick(isentenceg, name);

	if (ipick >= 0 && name[0] != '\0')
	{
		EMIT_SOUND_DYN(entity, CHAN_VOICE, name, volume, attenuation, flags, pitch);
	}

	return ipick;
}

// play sentences in sequential order from sentence group.  Reset after last sentence.

int SENTENCEG_PlaySequentialSz(edict_t *entity, const char *szgroupname, float volume, float attenuation, int flags, int pitch, int ipick, int freset)
{
	char name[64];
	int ipicknext;
	int isentenceg;

	if (!fSentencesInit)
		return -1;

	name[0] = '\0';

	isentenceg = SENTENCEG_GetIndex(szgroupname);
	if (isentenceg < 0)
		return -1;

	ipicknext = USENTENCEG_PickSequential(isentenceg, name, ipick, freset);
	if (ipicknext >= 0 && name[0] != '\0')
	{
		EMIT_SOUND_DYN(entity, CHAN_VOICE, name, volume, attenuation, flags, pitch);
	}

	return ipicknext;
}

// for this entity, for the given sentence within the sentence group, stop
// the sentence.

NOXREF void SENTENCEG_Stop(edict_t *entity, int isentenceg, int ipick)
{
	char buffer[64];
	char sznum[8];

	if (!fSentencesInit)
		return;

	if (isentenceg < 0 || ipick < 0)
		return;

	Q_strcpy(buffer, "!");
	Q_strcat(buffer, rgsentenceg[isentenceg].szgroupname);
	Q_sprintf(sznum, "%d", ipick);
	Q_strcat(buffer, sznum);

	STOP_SOUND(entity, CHAN_VOICE, buffer);
}

// open sentences.txt, scan for groups, build rgsentenceg
// Should be called from world spawn, only works on the
// first call and is ignored subsequently.

void SENTENCEG_Init()
{
	char buffer[512];
	char szgroup[64];
	int i, j;
	int isentencegs;

	if (fSentencesInit)
		return;

	Q_memset(gszallsentencenames, 0, sizeof(gszallsentencenames));
	gcallsentences = 0;

	Q_memset(rgsentenceg, 0, CSENTENCEG_MAX * sizeof(sentenceg));
	Q_memset(buffer, 0, sizeof(buffer));
	Q_memset(szgroup, 0, sizeof(szgroup));

	isentencegs = -1;

	int filePos = 0, fileSize;
	byte *pMemFile = LOAD_FILE_FOR_ME("sound/sentences.txt", &fileSize);
	if (!pMemFile)
		return;

	// for each line in the file...
	while (memfgets(pMemFile, fileSize, filePos, buffer, sizeof(buffer) - 1) != NULL)
	{
		// skip whitespace
		i = 0;
		while (buffer[i] && buffer[i] == ' ')
			++i;

		if (!buffer[i])
			continue;

		if (buffer[i] == '/' || !isalpha(buffer[i]))
			continue;

		// get sentence name
		j = i;
		while (buffer[j] && buffer[j] != ' ')
			j++;

		if (!buffer[j])
			continue;

		if (gcallsentences > CVOXFILESENTENCEMAX)
		{
			ALERT(at_error, "Too many sentences in sentences.txt!\n");
			break;
		}

		// null-terminate name and save in sentences array
		buffer[j] = 0;
		const char *pString = buffer + i;

		if (Q_strlen(pString) >= CBSENTENCENAME_MAX)
		{
			ALERT(at_warning, "Sentence %s longer than %d letters\n", pString, CBSENTENCENAME_MAX - 1);
		}

		Q_strcpy(gszallsentencenames[gcallsentences++], pString);

		j--;
		if (j <= i)
			continue;

		if (!isdigit(buffer[j]))
			continue;

		// cut out suffix numbers
		while (j > i && isdigit(buffer[j]))
			j--;

		if (j <= i)
			continue;

		buffer[j + 1] = 0;

		// if new name doesn't match previous group name,
		// make a new group.

		if (Q_strcmp(szgroup, &(buffer[i])))
		{
			// name doesn't match with prev name,
			// copy name into group, init count to 1
			isentencegs++;
			if (isentencegs >= CSENTENCEG_MAX)
			{
				ALERT(at_error, "Too many sentence groups in sentences.txt!\n");
				break;
			}

			Q_strcpy(rgsentenceg[isentencegs].szgroupname, &(buffer[i]));
			rgsentenceg[isentencegs].count = 1;

			Q_strcpy(szgroup, &(buffer[i]));

			continue;
		}
		else
		{
			//name matches with previous, increment group count
			if (isentencegs >= 0)
				rgsentenceg[isentencegs].count++;
		}
	}

	FREE_FILE(pMemFile);

	fSentencesInit = TRUE;

	// init lru lists

	i = 0;

	while (rgsentenceg[i].count && i < CSENTENCEG_MAX)
	{
		USENTENCEG_InitLRU(&(rgsentenceg[i].rgblru[0]), rgsentenceg[i].count);
		++i;
	}
}

// convert sentence (sample) name to !sentencenum, return !sentencenum

int SENTENCEG_Lookup(const char *sample, char *sentencenum)
{
	char sznum[8];
	int i;

	// this is a sentence name; lookup sentence number
	// and give to engine as string.
	for (i = 0; i < gcallsentences; ++i)
	{
		if (!Q_stricmp(gszallsentencenames[i], sample + 1))
		{
			if (sentencenum)
			{
				Q_strcpy(sentencenum, "!");
				Q_sprintf(sznum, "%d", i);
				Q_strcat(sentencenum, sznum);
			}
			return i;
		}
	}

	// sentence name not found!
	return -1;
}

void EMIT_SOUND_DYN(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int flags, int pitch)
{
	if (sample && *sample == '!')
	{
		char name[32];
		if (SENTENCEG_Lookup(sample, name) >= 0)
			EMIT_SOUND_DYN2(entity, channel, name, volume, attenuation, flags, pitch);
		else
			ALERT(at_aiconsole, "Unable to find %s in sentences.txt\n", sample);
	}
	else
		EMIT_SOUND_DYN2(entity, channel, sample, volume, attenuation, flags, pitch);
}

// play a specific sentence over the HEV suit speaker - just pass player entity, and !sentencename

void EMIT_SOUND_SUIT(edict_t *entity, const char *sample)
{
	float fvol;
	int pitch = PITCH_NORM;

	fvol = CVAR_GET_FLOAT("suitvolume");
	if (RANDOM_LONG(0, 1))
		pitch = RANDOM_LONG(0, 6) + 98;

	if (fvol > 0.05f)
		EMIT_SOUND_DYN(entity, CHAN_STATIC, sample, fvol, ATTN_NORM, 0, pitch);
}

// play a sentence, randomly selected from the passed in group id, over the HEV suit speaker

void EMIT_GROUPID_SUIT(edict_t *entity, int isentenceg)
{
	float fvol;
	int pitch = PITCH_NORM;

	fvol = CVAR_GET_FLOAT("suitvolume");
	if (RANDOM_LONG(0, 1))
		pitch = RANDOM_LONG(0, 6) + 98;

	if (fvol > 0.05f)
	{
		SENTENCEG_PlayRndI(entity, isentenceg, fvol, ATTN_NORM, 0, pitch);
	}
}

// play a sentence, randomly selected from the passed in groupname

NOXREF void EMIT_GROUPNAME_SUIT(edict_t *entity, const char *groupname)
{
	float fvol;
	int pitch = PITCH_NORM;

	fvol = CVAR_GET_FLOAT("suitvolume");

	if (RANDOM_LONG(0, 1))
		pitch = RANDOM_LONG(0, 6) + 98;

	if (fvol > 0.05f)
	{
		SENTENCEG_PlayRndSz(entity, groupname, fvol, ATTN_NORM, 0, pitch);
	}
}

char *memfgets(byte *pMemFile, int fileSize, int &filePos, char *pBuffer, int bufferSize)
{
	// Bullet-proofing
	if (!pMemFile || !pBuffer)
		return NULL;

	if (filePos >= fileSize)
		return NULL;

	int i = filePos;
	int last = fileSize;

	// fgets always NULL terminates, so only read bufferSize-1 characters
	if (last - filePos > (bufferSize - 1))
		last = filePos + (bufferSize - 1);

	int stop = 0;

	// Stop at the next newline (inclusive) or end of buffer
	while (i < last && !stop)
	{
		if (pMemFile[i] == '\n')
			stop = 1;
		++i;
	}

	// If we actually advanced the pointer, copy it over
	if (i != filePos)
	{
		// We read in size bytes
		int size = i - filePos;
		// copy it out
		Q_memcpy(pBuffer, pMemFile + filePos, sizeof(byte) * size);

		// If the buffer isn't full, terminate (this is always true)
		if (size < bufferSize)
			pBuffer[size] = 0;

		// Update file pointer
		filePos = i;
		return pBuffer;
	}

	// No data read, bail
	return NULL;
}

void TEXTURETYPE_Init()
{
	char buffer[512];
	int i, j;
	byte *pMemFile;
	int fileSize, filePos = 0;

	if (fTextureTypeInit)
		return;

	Q_memset(&(grgszTextureName[0][0]), 0, sizeof(grgszTextureName));
	Q_memset(grgchTextureType, 0, sizeof(grgchTextureType));

	gcTextures = 0;
	Q_memset(buffer, 0, sizeof(buffer));

	pMemFile = LOAD_FILE_FOR_ME("sound/materials.txt", &fileSize);

	if (!pMemFile)
		return;

	// for each line in the file...
	while (memfgets(pMemFile, fileSize, filePos, buffer, sizeof(buffer) - 1) != NULL && (gcTextures < CTEXTURESMAX))
	{
		// skip whitespace
		i = 0;
		while (buffer[i] && isspace(buffer[i]))
			++i;

		if (!buffer[i])
			continue;

		// skip comment lines
		if (buffer[i] == '/' || !isalpha(buffer[i]))
			continue;

		// get texture type
		grgchTextureType[gcTextures] = toupper(buffer[i++]);

		// skip whitespace
		while (buffer[i] && isspace(buffer[i]))
			++i;

		if (!buffer[i])
			continue;

		// get sentence name
		j = i;
		while (buffer[j] && !isspace(buffer[j]))
			j++;

		if (!buffer[j])
			continue;

		// null-terminate name and save in sentences array
		j = Q_min(j, CBTEXTURENAMEMAX - 1 + i);
		buffer[j] = 0;
		Q_strcpy(&(grgszTextureName[gcTextures++][0]), &(buffer[i]));
	}

	FREE_FILE(pMemFile);

	fTextureTypeInit = TRUE;
}

// given texture name, find texture type
// if not found, return type 'concrete'

// NOTE: this routine should ONLY be called if the
// current texture under the player changes!

char TEXTURETYPE_Find(char *name)
{
	// CONSIDER: pre-sort texture names and perform faster binary search here

	for (int i = 0; i < gcTextures; ++i)
	{
		if (!Q_strnicmp(name, &(grgszTextureName[ i ][0]), CBTEXTURENAMEMAX - 1))
			return (grgchTextureType[ i ]);
	}

	return CHAR_TEX_CONCRETE;
}

// play a strike sound based on the texture that was hit by the attack traceline.  VecSrc/VecEnd are the
// original traceline endpoints used by the attacker, iBulletType is the type of bullet that hit the texture.
// returns volume of strike instrument (crowbar) to play

float TEXTURETYPE_PlaySound(TraceResult *ptr, Vector vecSrc, Vector vecEnd, int iBulletType)
{
	// hit the world, try to play sound based on texture material type

	char chTextureType;
	float fvol;
	float fvolbar;
	char szbuffer[64];
	const char *pTextureName;
	const char *rgsz[4];
	int cnt;
	float fattn = ATTN_NORM;

	if (!g_pGameRules->PlayTextureSounds())
		return 0.0;

	CBaseEntity *pEntity = CBaseEntity::Instance(ptr->pHit);

	chTextureType = 0;

	if (pEntity && pEntity->Classify() != CLASS_NONE && pEntity->Classify() != CLASS_MACHINE)
	{
		// hit body
		chTextureType = CHAR_TEX_FLESH;
	}
	else
	{
		// hit world
		// find texture under strike, get material type
		// copy trace vector into array for trace_texture

		// get texture from entity or world (world is ent(0))

		if (pEntity)
			pTextureName = TRACE_TEXTURE(ENT(pEntity->pev), vecSrc, vecEnd);
		else
			pTextureName = TRACE_TEXTURE(ENT(0), vecSrc, vecEnd);

		if (pTextureName)
		{
			// strip leading '-0' or '+0~' or '{' or '!'
			if (*pTextureName == '-' || *pTextureName == '+')
				pTextureName += 2;

			if (*pTextureName == '{' || *pTextureName == '!' || *pTextureName == '~' || *pTextureName == ' ')
				pTextureName++;

			// '}}'
			Q_strcpy(szbuffer, pTextureName);
			szbuffer[CBTEXTURENAMEMAX - 1] = '\0';

			// get texture type
			chTextureType = TEXTURETYPE_Find(szbuffer);
		}
	}

	switch (chTextureType)
	{
	default:
	case CHAR_TEX_CONCRETE:
		fvol = 0.9f;
		fvolbar = 0.6f;

		rgsz[0] = "player/pl_step1.wav";
		rgsz[1] = "player/pl_step2.wav";
		cnt = 2;
		break;
	case CHAR_TEX_METAL:
		fvol = 0.9f;
		fvolbar = 0.3f;

		rgsz[0] = "player/pl_metal1.wav";
		rgsz[1] = "player/pl_metal2.wav";
		cnt = 2;
		break;
	case CHAR_TEX_DIRT:
		fvol = 0.9f;
		fvolbar = 0.1f;

		rgsz[0] = "player/pl_dirt1.wav";
		rgsz[1] = "player/pl_dirt2.wav";
		rgsz[2] = "player/pl_dirt3.wav";
		cnt = 3;
		break;
	case CHAR_TEX_VENT:
		fvol = 0.5f;
		fvolbar = 0.3f;

		rgsz[0] = "player/pl_duct1.wav";
		rgsz[1] = "player/pl_duct1.wav";
		cnt = 2;
		break;
	case CHAR_TEX_GRATE:
		fvol = 0.9f;
		fvolbar = 0.5f;

		rgsz[0] = "player/pl_grate1.wav";
		rgsz[1] = "player/pl_grate4.wav";
		cnt = 2;
		break;
	case CHAR_TEX_TILE:
		fvol = 0.8f;
		fvolbar = 0.2f;

		rgsz[0] = "player/pl_tile1.wav";
		rgsz[1] = "player/pl_tile3.wav";
		rgsz[2] = "player/pl_tile2.wav";
		rgsz[3] = "player/pl_tile4.wav";
		cnt = 4;
		break;
	case CHAR_TEX_SLOSH:
		fvol = 0.9f;
		fvolbar = 0.0f;

		rgsz[0] = "player/pl_slosh1.wav";
		rgsz[1] = "player/pl_slosh3.wav";
		rgsz[2] = "player/pl_slosh2.wav";
		rgsz[3] = "player/pl_slosh4.wav";
		cnt = 4;
		break;
	case CHAR_TEX_SNOW:
		fvol = 0.7f;
		fvolbar = 0.4f;

		rgsz[0] = "player/pl_snow1.wav";
		rgsz[1] = "player/pl_snow2.wav";
		rgsz[2] = "player/pl_snow3.wav";
		rgsz[3] = "player/pl_snow4.wav";
		cnt = 4;
		break;
	case CHAR_TEX_WOOD:
		fvol = 0.9f;
		fvolbar = 0.2f;

		rgsz[0] = "debris/wood1.wav";
		rgsz[1] = "debris/wood2.wav";
		rgsz[2] = "debris/wood3.wav";
		cnt = 3;
		break;
	case CHAR_TEX_GLASS:
	case CHAR_TEX_COMPUTER:
		fvol = 0.8f;
		fvolbar = 0.2f;

		rgsz[0] = "debris/glass1.wav";
		rgsz[1] = "debris/glass2.wav";
		rgsz[2] = "debris/glass3.wav";
		cnt = 3;
		break;
	case CHAR_TEX_FLESH:
		if (iBulletType == BULLET_PLAYER_CROWBAR)
			return 0.0f; // crowbar already makes this sound

		fvol = 1.0f;
		fvolbar = 0.2f;

		rgsz[0] = "weapons/bullet_hit1.wav";
		rgsz[1] = "weapons/bullet_hit2.wav";
		fattn = 1.0f;
		cnt = 2;
		break;
	}

	// did we hit a breakable?
	if (pEntity && (FClassnameIs(pEntity->pev, "func_breakable") || FClassnameIs(pEntity->pev, "func_door_rotating")))
	{
		// drop volumes, the object will already play a damaged sound
		fvol /= 1.5f;
		fvolbar /= 2.0f;
	}
	else if (chTextureType == CHAR_TEX_COMPUTER)
	{
		// play random spark if computer
		if (ptr->flFraction != 1.0f && RANDOM_LONG(0, 1))
		{
			UTIL_Sparks(ptr->vecEndPos);

			//random volume range
			float flVolume = RANDOM_FLOAT(0.7 , 1.0);

			switch (RANDOM_LONG(0, 1))
			{
			case 0:
				UTIL_EmitAmbientSound(ENT(0), ptr->vecEndPos, "buttons/spark5.wav", flVolume, ATTN_NORM, 0, 100);
				break;
			case 1:
				UTIL_EmitAmbientSound(ENT(0), ptr->vecEndPos, "buttons/spark6.wav", flVolume, ATTN_NORM, 0, 100);
				break;
			}
		}
	}

	// play material hit sound
	UTIL_EmitAmbientSound(ENT(0), ptr->vecEndPos, rgsz[RANDOM_LONG(0, cnt - 1)], fvol, fattn, 0, 96 + RANDOM_LONG(0, 0xf));

	return fvolbar;
}

LINK_ENTITY_TO_CLASS(speaker, CSpeaker);

IMPLEMENT_SAVERESTORE(CSpeaker, CBaseEntity);

// ambient_generic - general-purpose user-defined static sound

void CSpeaker::Spawn()
{
	char *szSoundFile = (char *)STRING(pev->message);

	if (!m_preset && (FStringNull(pev->message) || Q_strlen(szSoundFile) < 1))
	{
		ALERT(at_error, "SPEAKER with no Level/Sentence! at: %f, %f, %f\n", pev->origin.x, pev->origin.y, pev->origin.z);
		pev->nextthink = gpGlobals->time + 0.1s;
		SetThink(&CSpeaker::SUB_Remove);
		return;
	}

	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;

	SetThink(&CSpeaker::SpeakerThink);
	pev->nextthink = invalid_time_point;

	// allow on/off switching via 'use' function.
	SetUse(&CSpeaker::ToggleUse);

	Precache();
}

void CSpeaker::Precache()
{
	if (!(pev->spawnflags & SPEAKER_START_SILENT))
	{
		// set first announcement time for random n second
		pev->nextthink = gpGlobals->time + RandomDuration(5.0s, 15.0s);
	}
}

void CSpeaker::SpeakerThink()
{
	const char *szSoundFile = NULL;
	float flvolume = pev->health * 0.1f;
	float flattenuation = 0.3f;
	int flags = 0;
	int pitch = 100;

	// Wait for the talkmonster to finish first.
	if (gpGlobals->time <= CTalkMonster::g_talkWaitTime)
	{
		pev->nextthink = CTalkMonster::g_talkWaitTime + duration_t(RANDOM_FLOAT(5, 10));
		return;
	}

	if (m_preset)
	{
		// go lookup preset text, assign szSoundFile
		switch (m_preset)
		{
		case 1: szSoundFile =  "C1A0_"; break;
		case 2: szSoundFile =  "C1A1_"; break;
		case 3: szSoundFile =  "C1A2_"; break;
		case 4: szSoundFile =  "C1A3_"; break;
		case 5: szSoundFile =  "C1A4_"; break;
		case 6: szSoundFile =  "C2A1_"; break;
		case 7: szSoundFile =  "C2A2_"; break;
		case 8: szSoundFile =  "C2A3_"; break;
		case 9: szSoundFile =  "C2A4_"; break;
		case 10: szSoundFile = "C2A5_"; break;
		case 11: szSoundFile = "C3A1_"; break;
		case 12: szSoundFile = "C3A2_"; break;
		}
	}
	else
		szSoundFile = (char *)STRING(pev->message);

	if (szSoundFile == NULL)
	{
		// if is null - return;
		return;
	}

	if (szSoundFile[0] == '!')
	{
		// play single sentence, one shot
		UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile, flvolume, flattenuation, flags, pitch);

		// shut off and reset
		pev->nextthink = invalid_time_point;
	}
	else
	{
		// make random announcement from sentence group

		if (SENTENCEG_PlayRndSz(ENT(pev), szSoundFile, flvolume, flattenuation, flags, pitch) < 0)
		{
			ALERT(at_console, "Level Design Error!\nSPEAKER has bad sentence group name: %s\n", szSoundFile);
		}

		// set next announcement time for random 5 to 10 minute delay
		pev->nextthink = gpGlobals->time + RandomDuration(ANNOUNCE_MINUTES_MIN * 60.0s, ANNOUNCE_MINUTES_MAX * 60.0s);

		// time delay until it's ok to speak: used so that two NPCs don't talk at once
		CTalkMonster::g_talkWaitTime = gpGlobals->time + 5.0s;
	}

	return;
}

// ToggleUse - if an announcement is pending, cancel it.  If no announcement is pending, start one.

void CSpeaker::ToggleUse(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value)
{
	int fActive = (pev->nextthink > invalid_time_point);

	// fActive is TRUE only if an announcement is pending
	if (useType != USE_TOGGLE)
	{
		// ignore if we're just turning something on that's already on, or
		// turning something off that's already off.
		if ((fActive && useType == USE_ON) || (!fActive && useType == USE_OFF))
		{
			return;
		}
	}

	if (useType == USE_ON)
	{
		// turn on announcements
		pev->nextthink = gpGlobals->time + 0.1s;
		return;
	}

	if (useType == USE_OFF)
	{
		// turn off announcements
		pev->nextthink = invalid_time_point;
		return;

	}

	// Toggle announcements
	if (fActive)
	{
		// turn off announcements
		pev->nextthink = invalid_time_point;
	}
	else
	{
		// turn on announcements
		pev->nextthink = gpGlobals->time + 0.1s;
	}
}

// KeyValue - load keyvalue pairs into member data
// NOTE: called BEFORE spawn!

void CSpeaker::KeyValue(KeyValueData *pkvd)
{
	// preset
	if (FStrEq(pkvd->szKeyName, "preset"))
	{
		m_preset = Q_atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseEntity::KeyValue(pkvd);
}

}
