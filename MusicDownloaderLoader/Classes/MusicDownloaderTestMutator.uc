class MusicDownloaderTestMutator extends Mutator;

var bool bHasLoader;
var MusicDownloaderLoader MyLoader;
var PlayerController MyPC;

function Tick(float DeltaTime)
{
	/* just in case */
	if(bHasLoader)
		return;
		
  MyPC = Level.GetLocalPlayerController();

  if (MyPC != None)
  {
		MyLoader = Spawn(class'MusicDownloaderLoader', MyPC);
		bHasLoader = true;
		
		/* Set delegates */
		MyLoader.MusicDownloadInitialized = MusicDownloadInitialized;
		MyLoader.DownloadError = DownloadError;
		MyLoader.DownloadCancelled = DownloadCancelled;
		MyLoader.DownloadComplete = DownloadComplete;
		
		if (!MyLoader.LoadDownloader(MyPC))
			MyPC.ClientMessage("Failed to initialize music downloader!");
		
		Disable('Tick');
  }
}

function MusicDownloadInitialized()
{
	MyPC.ClientMessage("Music downloader was initialized!");
}

function DownloadError(string URL, string MusicName)
{
	MyPC.ClientMessage("Failed to download " $ MusicName $ " from: " $ URL);
}

function DownloadCancelled(string URL, string MusicName)
{
	MyPC.ClientMessage("Cancelled download: " $ MusicName);
}

delegate DownloadComplete(string URL, string MusicName)
{
	MyPC.ClientMessage("Completed download: " $ MusicName);
}

defaultproperties
{
	FriendlyName="Music Downloader Test Mutator"
}
