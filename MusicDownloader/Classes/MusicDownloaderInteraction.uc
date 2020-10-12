class MusicDownloaderInteraction extends Interaction
	native;

cpptext
{
    UMusicDownloaderInteraction();
    void DlReset();
}

var int LastBroadcastedPct;
var float TimeSinceLastBroadcast;
var string LastBroadcastedName;

var MusicDownloaderLoader Loader;

// Download the music - note, these properties are global, so only one download allowed
// 32-bit version can bind by name, but 64-bit version can only use numbers (perhaps due to static linking?)
native(3200) final function NativeTick(float DeltaTime);
native(3201) final function bool IsMusicDownloaded(string MusicName);
native(3202) final function bool IsDownloading();
native(3203) final function bool DownloadMusic(string URL, string MusicName);
native(3204) final function bool CancelDownload();
native(3205) final function GetDownloadStatus(out string MusicName, out int DownloadedBytes, out int DownloadSize);

// Initialized
event Initialized()
{
	local MusicDownloaderLoader Temp;
	
	if (ViewportOwner == None || ViewportOwner.Actor == None)
		return;
		
	foreach ViewportOwner.Actor.AllActors(class'MusicDownloaderLoader', Temp)
	{
		if (Temp.Owner == ViewportOwner.Actor)
		{
			Loader = Temp;
			break;
		}
	}
		
	if (Loader != None)
	{
		Loader.IsMusicDownloaded = IsMusicDownloaded;
		Loader.DownloadMusic = DownloadMusic;
		Loader.MusicDownloadInitialized();
	}
}

// Reports
event DownloadError(string URL, string MusicName)
{
	LastBroadcastedPct = -2;
	LastBroadcastedName = MusicName $ ".ogg";
	
	if (ViewportOwner != None && ViewportOwner.Actor != None)
		ViewportOwner.Actor.ReceiveLocalizedMessage(class'MusicDownloaderStatusMessage', 1001,,, self);
		
	if (Loader != None)
		Loader.DownloadError(URL, MusicName);
}

event DownloadCancelled(string URL, string MusicName)
{	
	LastBroadcastedPct = -2;
	LastBroadcastedName = MusicName $ ".ogg";
	
	if (ViewportOwner != None && ViewportOwner.Actor != None)
		ViewportOwner.Actor.ReceiveLocalizedMessage(class'MusicDownloaderStatusMessage', 1002,,, self);
		
	if (Loader != None)
		Loader.DownloadCancelled(URL, MusicName);
}

event DownloadComplete(string URL, string MusicName)
{	
	LastBroadcastedPct = -2;
	LastBroadcastedName = MusicName $ ".ogg";
	
	if (ViewportOwner != None && ViewportOwner.Actor != None)
		ViewportOwner.Actor.ReceiveLocalizedMessage(class'MusicDownloaderStatusMessage', 1000,,, self);
		
	if (Loader != None)
		Loader.DownloadComplete(URL, MusicName);
}

event NotifyLevelChange()
{	
	if (IsDownloading())
		CancelDownload();
		
	Master.RemoveInteraction(self);
}

function Tick(float deltaTime)
{
	local string MusicName;
	local int DlSize, DlBytes, DlPct;

	NativeTick(deltaTime);

	if (ViewportOwner == None || ViewportOwner.Actor == None || !IsDownloading())
		return;
	
	GetDownloadStatus(MusicName, DlBytes, DlSize);	
	
	if (DlSize == -1)
		DlPct = -1;
	else
		DlPct = 100 * DlBytes / DlSize;
	
	if (LastBroadcastedPct != DlPct || TimeSinceLastBroadcast > 1.0)
	{	
		LastBroadcastedPct = DlPct;
		LastBroadcastedName = MusicName $".ogg";
		ViewportOwner.Actor.ReceiveLocalizedMessage(class'MusicDownloaderStatusMessage', DlPct,,, self);
		TimeSinceLastBroadcast = 0.0;
	}
	else
		TimeSinceLastBroadcast += deltaTime;
}

exec function CmdDownloadMusic(string Cmd)
{
	local string URL;
	local string MusicName;
	local int i;
	
	i = InStr(Cmd, " ");
	if (i < 0)
		return;
		
	URL = Left(Cmd, i);
	MusicName = Mid(Cmd, i + 1);
	DownloadMusic(URL, MusicName);
}

exec function CmdCancelDownload()
{
	if (IsDownloading())
		CancelDownload();
}

defaultproperties
{
	bVisible=true
	bRequiresTick=true
	LastBroadcastedPct=-2
	LastBroadcastedName=""
}
