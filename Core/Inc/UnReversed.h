/*-----------------------------------------------------------------------------
	Defintions.
-----------------------------------------------------------------------------*/

enum ECamOrientation
{
	CAMORIENT_None=0,
	CAMORIENT_LookAtActor=1,
	CAMORIENT_FacePath=2,
	CAMORIENT_Interpolate=3,
	CAMORIENT_Dolly=4,
};

enum EAxis
{
	AXIS_X=0,
	AXIS_Y=1,
	AXIS_Z=2
};

struct FCompressedPosition
{
	FVector Location;
	FRotator Rotation;
	FVector Velocity;
};

struct FInterpCurvePoint
{
	FLOAT InVal;
	FLOAT OutVal;
};

struct FIntBox
{
	INT X1, Y1, X2, Y2;
};

struct FFloatBox
{
	FLOAT X1, Y1, X2, Y2;
};

class ENGINE_API FRange
{
public:
	FLOAT Min;
	FLOAT Max;
};

class ENGINE_API FRangeVector
{
public:
	FRange X;
	FRange Y;
	FRange Z;
};

class ENGINE_API FInterpCurve
{
public:
	TArrayNoInit<FInterpCurvePoint> Points;
	
	void AddPoint(FLOAT InVal, FLOAT OutVal);
	FLOAT Eval(FLOAT InVal);
	UBOOL operator==(const FInterpCurve &Other);
	void operator=(const FInterpCurve &Other);
};
