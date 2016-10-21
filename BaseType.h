#pragma once

typedef unsigned long long uint64;
typedef long long int64;
typedef char Char;
typedef float f32;
typedef double f64;

typedef unsigned char uint8;
typedef signed char	int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned long SM_KEY;

#define SrvID_NULL	-1
#define IPLEN		32

#define SAFE_DELETE(p) if(p) { delete (p); p = NULL; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] (p); p = NULL; }
#define SAFE_RELEASE(p) if(p) { p.Release(); p = NULL; }

#define MAKE_TYPE_ID( paramid, dataid) \
	( (paramid) << 24 + (dataid) & 0xffffff )

#define MAKE_LOGIC_ID( paramid, dataid) \
	( (paramid) << 8 + (dataid) & 0xff )

#define SERVER_TICK_TIME_SHOW		1000
#define GAME_THREAD_WATCHDOG_ID		2
#define SCENE_ID_NULL				0
#define SCENE_MAIN_LINE_ID			1
#define SERVERID_NULL				-1
#define	INVALID_TEAM_ID				-1
#define SCENE_ID_DEFAULT			1

#define MAX_AVATAR_COUNT_ONE_USER	3

#define _WINSOCKAPI_
