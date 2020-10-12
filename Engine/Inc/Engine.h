/*=============================================================================
	Engine.h: Unreal engine public header file.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

#ifndef _INC_ENGINE
#define _INC_ENGINE

/*----------------------------------------------------------------------------
	API.
----------------------------------------------------------------------------*/

#ifndef ENGINE_API
	#define ENGINE_API DLL_IMPORT
#endif

/*-----------------------------------------------------------------------------
	Dependencies.
-----------------------------------------------------------------------------*/

#include "Core.h"

/*-----------------------------------------------------------------------------
	Global variables.
-----------------------------------------------------------------------------*/

ENGINE_API extern class FMemStack	GEngineMem;
ENGINE_API extern class FMemCache	GCache;

/*-----------------------------------------------------------------------------
	Engine compiler specific includes.
-----------------------------------------------------------------------------*/

#if __GNUG__
	#include "UnEngineGnuG.h"
#endif

/*-----------------------------------------------------------------------------
	Definitions
-----------------------------------------------------------------------------*/

struct FKRBVec
{
	FLOAT x,y,z;
};

struct FKRigidBodyState
{
	FKRBVec	Position;
	FQuat	Quaternion;
	FKRBVec	LinVel;
	FKRBVec	AngVel;
};

struct FBatchReference
{
	INT BatchIndex;
	INT ElementIndex;
};

struct FPointRegion
{
	class AZoneInfo* Zone;
	INT              iLeaf;
	BYTE             ZoneNumber;
};

struct FAnimRep
{
	FName AnimSequence;
	UBOOL bAnimLoop;
	BYTE AnimRate;
	BYTE AnimFrame;
	BYTE TweenRate;
};

struct FColor
{
	BYTE B, G, R, A;
};

class UCanvas;
class UActorChannel;
class UTexture;
class UMaterial;
class UPrimitive;
class AController;
class APlayerController;
class FLevelSceneNode;
class FRenderInterface;
struct FCheckResult;
class FDynamicActor;
class AProjectile;
struct _McdModel;
class AMover;
typedef PTRINT FActorRenderDataPtr;
typedef PTRINT FLightRenderDataPtr;
typedef PTRINT FProjectorRenderInfoPtr;
typedef PTRINT FStaticMeshProjectorRenderInfoPtr;

#include "EngineClasses.h"

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
#endif
