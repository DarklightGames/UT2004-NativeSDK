/*=============================================================================
	AVolume.h.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

// Constructor
AVolume();

// AActor interface.
virtual void SetVolumes(const TArray<class AVolume*>& Volumes);
virtual void SetVolumes();
virtual void PostBeginPlay();

virtual UBOOL ShouldTrace(AActor *SourceActor, DWORD TraceFlags);

virtual UBOOL IsAVolume();

// AVolume interface.
INT Encompasses(FVector point);
