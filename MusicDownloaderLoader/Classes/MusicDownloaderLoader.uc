class MusicDownloaderLoader extends Actor;

const PACKAGE_NAME = "MusicDownloader";

var Interaction DownloaderInteraction;

function bool IsDownloaderInstalled()
{
	// Make sure we have the files in the first place
	if (ConsoleCommand("dir MusicDownloader.dll") == "")
		return false;
		
	if (ConsoleCommand("dir MusicDownloader.u") == "")
		return false;
		
	return true;
}

function bool LoadDownloader(PlayerController PC)
{
	if (!IsDownloaderInstalled())
		return false;
	
	// The interaction will retrieve the loader and bind itself through all actors
	// This can crash in some scenarios (like missing / incompatible DLL etc.)
	DownloaderInteraction = PC.Player.InteractionMaster.AddInteraction("MusicDownloader.MusicDownloaderInteraction", PC.Player);
	return DownloaderInteraction != None;
}

delegate MusicDownloadInitialized()
{
}

delegate bool IsMusicDownloaded(string MusicName)
{
	return true;
}

delegate bool DownloadMusic(string URL, string MusicName)
{
	return false;
}

delegate DownloadError(string URL, string MusicName)
{
}

delegate DownloadCancelled(string URL, string MusicName)
{
}

delegate DownloadComplete(string URL, string MusicName)
{
}

defaultproperties
{
	RemoteRole=ROLE_None
	bHidden=true
}
