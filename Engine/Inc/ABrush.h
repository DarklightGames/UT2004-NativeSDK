/*=============================================================================
	ABrush.h.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

// Constructors.
ABrush();

// UObject interface.
virtual void PostLoad();
virtual void PostEditChange();

// AActor interface.
virtual UPrimitive* GetPrimitive();

virtual FCoords ToLocal() const;
virtual FCoords ToWorld() const;
FLOAT BuildCoords(FModelCoords* Coords, FModelCoords* Uncoords);

virtual UBOOL IsABrush();

virtual void CheckForErrors();

// ABrush interface.
virtual void CopyPosRotScaleFrom(ABrush* Other);
virtual void InitPosRotScale();

FCoords OldToLocal() const;
FCoords OldToWorld() const;
FLOAT OldBuildCoords(FModelCoords* Coords, FModelCoords* Uncoords);
