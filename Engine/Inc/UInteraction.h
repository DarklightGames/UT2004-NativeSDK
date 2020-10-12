/*=============================================================================
	UnInteraction.h: Unreal UModel definition.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Joe Wilcox
=============================================================================*/

virtual void  NativeMessage(const FString Msg, FLOAT MsgLife);
virtual UBOOL NativeKeyType(BYTE& iKey, TCHAR Unicode );
virtual UBOOL NativeKeyEvent(BYTE& iKey, BYTE& State, FLOAT Delta );
virtual void  NativeTick(FLOAT DeltaTime);
virtual void  NativePreRender(UCanvas* Canvas);
virtual void  NativePostRender(UCanvas* Canvas);
