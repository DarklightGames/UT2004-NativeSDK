/*=============================================================================
	AActor.h.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

	// Constructors.
	AActor();

	// UObject interface.
	virtual void ProcessEvent( UFunction* Function, void* Parms, void* Result=NULL );
	virtual void ProcessState( FLOAT DeltaSeconds );
	virtual UBOOL ProcessRemoteFunction( UFunction* Function, void* Parms, FFrame* Stack );
	virtual void PostLoad();
	virtual void Destroy();
	virtual void Serialize( FArchive& Ar );
	virtual UBOOL IsPendingKill();
	virtual void ScriptInit(ALevelInfo* arg0);
	virtual void InitExecution();
	virtual void PostEditChange();
	virtual void PreEditUndo();
	virtual void PostEditUndo();
	virtual void NetDirty(UProperty* property);
	virtual void ExecutingBadStateCode(FFrame& Stack);

	// AActor interface.
	virtual INT* GetOptimizedRepList( BYTE* InDefault, FPropertyRetirement* Retire, INT* Ptr, UPackageMap* Map, UActorChannel* Channel );
	virtual INT IsJoinedTo(const AActor* Other) const;
	virtual UBOOL ShouldUpdateRelativeLocation();
	virtual void DoCleanup();
	virtual FLOAT GetNetPriority( FVector& ViewPos, FVector& ViewDir, AActor* Sent, FLOAT Time, FLOAT Lag );
	virtual FLOAT WorldLightRadius() const;
	virtual UBOOL Tick( FLOAT DeltaTime, enum ELevelTick TickType );
	virtual void PostEditMove();
	virtual void PostEditLoad();
	virtual void PreRaytrace();
	virtual void PostRaytrace();
	virtual void Spawned();
	virtual void PreNetReceive();
	virtual void PostNetReceive();
	virtual void PostNetReceiveLocation();
	virtual UMaterial* GetSkin( INT Index );
	virtual UBOOL ShouldTickInEntry();
	virtual FCoords ToLocal() const;
	virtual FCoords ToWorld() const;
	virtual FMatrix LocalToWorld() const;
	virtual FMatrix WorldToLocal() const;
	virtual void PostScriptDestroyed();
	virtual void CleanupSubObjects();
	virtual UBOOL ShouldTrace(AActor *SourceActor, DWORD TraceFlags);
	virtual UPrimitive* GetPrimitive();
	virtual void NotifyBump(AActor *Other);
	virtual void SetBase(AActor *NewBase, FVector NewFloor = FVector(0,0,1), int bNotifyActor=1);
	virtual void NotifyAnimEnd(int arg0);
#ifdef _WIN32
	virtual void NotifyLIPSincAnimEnd();
#endif
	virtual void UpdateAnimation(float arg0);
	virtual void StartAnimPoll();
	virtual UBOOL CheckAnimFinished(int arg0);
	virtual UBOOL CheckOwnerUpdated();
	virtual void TickAuthoritative( FLOAT DeltaSeconds );
	virtual void TickSimulated( FLOAT DeltaSeconds );
	virtual void TickSpecial( FLOAT DeltaSeconds );
	virtual UBOOL PlayerControlled();
	virtual UBOOL IsNetRelevantFor(APlayerController* RealViewer, AActor* Viewer, FVector SrcLocation);
	virtual UBOOL DelayScriptReplication(FLOAT LastFullUpdateTime);
	virtual void RenderEditorInfo(FLevelSceneNode* arg0, FRenderInterface* arg1, FDynamicActor* arg2);
	virtual void RenderEditorSelected(FLevelSceneNode* arg0, FRenderInterface* arg1 ,FDynamicActor* arg2);
	virtual FLOAT GetAmbientVolume(FLOAT arg0);
	virtual void SetZone( UBOOL bTest, UBOOL bForceRefresh );
	virtual void SetVolumes(const TArray<class AVolume*>& Volumes);
	virtual void SetVolumes();
	virtual void PostBeginPlay();
	virtual void setPhysics(BYTE NewPhysics, AActor *NewFloor = NULL, FVector NewFloorV = FVector(0,0,1) );
	virtual void performPhysics(FLOAT DeltaSeconds);
	virtual void BoundProjectileVelocity();
	virtual void processHitWall(FVector HitNormal, AActor *HitActor);
	virtual void processLanded(FVector HitNormal, AActor *HitActor, FLOAT remainingTime, INT Iterations);
	virtual void physFalling(FLOAT deltaTime, INT Iterations);
	virtual void physWalking(FLOAT deltaTime, INT Iterations);
	virtual void physFlying(FLOAT deltaTime, INT Iterations);
	virtual FRotator FindSlopeRotation(FVector FloorNormal, FRotator NewRotation);
	virtual void SmoothHitWall(FVector HitNormal, AActor *HitActor);
	virtual void stepUp(FVector GravDir, FVector DesiredDir, FVector Delta, FCheckResult& Hit);
	virtual UBOOL ShrinkCollision(AActor *HitActor, const FVector &StartLocation);
	virtual _McdModel* getKModel(void) const;
	virtual void physKarma(FLOAT arg0);
	virtual void preKarmaStep(FLOAT arg0);
	virtual void postKarmaStep();
	virtual void preContactUpdate();
	virtual UBOOL KRepulsorsShouldHit(AActor* arg0);
	virtual UBOOL ReachedBy(APawn* arg0, FVector arg1);
	virtual UBOOL NoReachDistance();
	virtual INT AddMyMarker(AActor *S);
	virtual void ClearMarker();
	virtual AActor* AssociatedLevelGeometry();
	virtual UBOOL HasAssociatedLevelGeometry(AActor *Other);
	virtual FVector GetDestination(AController* Controller);
	virtual INT PlayAnim(INT arg0, FName arg1, FLOAT arg2, FLOAT arg3, INT arg4);
	virtual FVector GetRootLocation();
	virtual void CheckForErrors();
	virtual void PrePath();
	virtual void PostPath();
	virtual AActor* GetProjectorBase();
	virtual UBOOL IsABrush();
	virtual UBOOL IsAMover();
	virtual UBOOL IsAVolume();
	virtual void PostRender(FLevelSceneNode* arg0, FRenderInterface* arg1);
	virtual APlayerController* GetAPlayerController();
	virtual APawn* GetAPawn();
	virtual AProjectile* GetAProjectile();
	virtual AMover* GetAMover();
	virtual APlayerController* GetTopPlayerController();

	void ProcessDemoRecFunction( UFunction* Function, void* Parms, FFrame* Stack );

	UBOOL IsPlayer() const;
	UBOOL IsOwnedBy( const AActor *TestOwner ) const;
	UBOOL IsBlockedBy( const AActor* Other ) const;
	UBOOL IsInZone( const AZoneInfo* Other ) const;
	UBOOL IsBasedOn( const AActor *Other ) const;
	FLOAT LifeFraction();
	FVector GetCylinderExtent();
	AActor* GetTopOwner();

	// AActor collision functions.
	UPrimitive* GetPrimitive() const;
	UBOOL IsOverlapping(AActor *Other, FCheckResult* arg1) const;

	// AActor general functions.
	void BeginTouch(AActor *Other);
	void EndTouch(AActor *Other, UBOOL NoNotifySelf);
	void SetOwner( AActor *Owner );
	UBOOL IsBrush()       const;
	UBOOL IsStaticBrush() const;
	UBOOL IsMovingBrush() const;
	UBOOL IsVolumeBrush() const;
	UBOOL IsAnimating(INT arg0) const;
	void SetCollision( UBOOL NewCollideActors, UBOOL NewBlockActors, UBOOL NewBlockPlayers);
	void SetCollisionSize( FLOAT NewRadius, FLOAT NewHeight );
	FRotator GetViewRotation();

	// Physics functions.
	void setPhysics(BYTE NewPhysics, AActor *NewFloor = NULL, INT arg0 = 0);
	void FindBase();

	void physProjectile(FLOAT deltaTime, INT Iterations);
	void physicsRotation(FLOAT deltaTime);
	int fixedTurn(int current, int desired, int deltaRate); 
	void TwoWallAdjust(FVector &DesiredDir, FVector &Delta, FVector &HitNormal, FVector &OldHitNormal, FLOAT HitTime);
	void physTrailer(FLOAT DeltaTime);
	int moveSmooth(FVector Delta);

	// AI functions.
	void CheckNoiseHearing(FLOAT Loudness);
	int TestCanSeeMe(APlayerController *Viewer);

	// Special editor behavior
	AActor* GetHitActor();

	// Natives.
	DECLARE_FUNCTION(execPollSleep)
	DECLARE_FUNCTION(execPollFinishAnim)
	DECLARE_FUNCTION(execPollFinishInterpolation)

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
