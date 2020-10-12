class MusicDownloaderStatusMessage extends CriticalEventPlus;

var color SuccessColor, FailColor;
var localized string Downloading;

static function color GetColor(optional int Switch, optional PlayerReplicationInfo RelatedPRI_1, optional PlayerReplicationInfo RelatedPRI_2)
{
	if (Switch == 1000)
		return Default.SuccessColor;
	if (Switch == 1001 || Switch == 1002)
    return Default.FailColor;
    
  return Default.DrawColor;
}

static function string GetString(optional int Switch,	optional PlayerReplicationInfo RelatedPRI_1, optional PlayerReplicationInfo RelatedPRI_2,	optional Object OptionalObject)
{
	local string MusicName;
	
	MusicName = MusicDownloaderInteraction(OptionalObject).LastBroadcastedName;
	if (Switch == -1)
		return Default.Downloading @ MusicName @ "...";
	if (Switch >= 0 && Switch <= 100)
		return Default.Downloading @ MusicName @ "(" $ Switch $ "%)";
	if (Switch == 1000)
		return Default.Downloading @ MusicName @ "succeeded!";
	if (Switch == 1001)
		return Default.Downloading @ MusicName @ "failed!";
	if (Switch == 1002)
		return Default.Downloading @ MusicName @ "was cancelled!";
		
	return "";
}


defaultproperties
{
	PosY=0.90
	DrawColor=(R=225,G=225,B=0,A=255)
	SuccessColor=(R=0,G=225,B=0,A=255)
	FailColor=(R=225,G=0,B=0,A=255)
	Lifetime=2.5
	FontSize=2
	Downloading="Downloading"
}
