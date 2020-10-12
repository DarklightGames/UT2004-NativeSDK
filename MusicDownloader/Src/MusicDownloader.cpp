/*===========================================================================
    Music Downloader Main Source File
===========================================================================*/

#include <cstdio>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "MusicDownloaderPrivate.h"

/*-----------------------------------------------------------------------------
	Declarations.
-----------------------------------------------------------------------------*/

// Package
IMPLEMENT_PACKAGE(MusicDownloader);

// Class
IMPLEMENT_CLASS(UMusicDownloaderInteraction);

// Register things.
#define NAMES_ONLY
#define AUTOGENERATE_NAME(name) MUSICDOWNLOADER_API FName MUSICDOWNLOADER_##name;
#define AUTOGENERATE_FUNCTION(cls,idx,name) IMPLEMENT_FUNCTION(cls,idx,name)
#include "MusicDownloaderClasses.h"
#undef AUTOGENERATE_FUNCTION
#undef AUTOGENERATE_NAME
#undef NAMES_ONLY

// Import natives
#define NATIVES_ONLY
#define NAMES_ONLY
#define AUTOGENERATE_NAME(name)
#define AUTOGENERATE_FUNCTION(cls,idx,name)
#include "MusicDownloaderClasses.h"
#undef AUTOGENERATE_FUNCTION
#undef AUTOGENERATE_NAME
#undef NATIVES_ONLY
#undef NAMES_ONLY

void RegisterNamesMusicDownloader(void)
{
	#define NAMES_ONLY
	#define AUTOGENERATE_NAME(name) extern MUSICDOWNLOADER_API FName MUSICDOWNLOADER_##name; MUSICDOWNLOADER_##name=FName(TEXT(#name),FNAME_Intrinsic);
	#define AUTOGENERATE_FUNCTION(cls,idx,name)
	#include "MusicDownloaderClasses.h"
	#undef AUTOGENERATE_FUNCTION
	#undef AUTOGENERATE_NAME
	#undef NAMES_ONLY
}

#if __STATIC_LINK
struct FMusicDownloaderInitManager
{
	FMusicDownloaderInitManager()
	{
		UMusicDownloaderInteraction::StaticClass();
		RegisterNamesMusicDownloader();
	}
} FMusicDownloaderInitManager;
#else
struct FMusicDownloaderInitManager
{
	FMusicDownloaderInitManager()
	{
		RegisterNamesMusicDownloader();
	}
} FMusicDownloaderInitManager;
#endif

/*-----------------------------------------------------------------------------
	Implementation.
-----------------------------------------------------------------------------*/

enum NetworkError
{
	NE_RESET = -1,
	NE_OK = 0,
	NE_INVALID = 1,
	NE_CANCELLED = 2,
	NE_URL_NOT_RESOLVED = 3,
	NE_CONNECTION_PROBLEM = 4,
	NE_FILE_NOT_FOUND = 5,
	NE_IO_ERROR = 6,
	NE_ALREADY_DOWNLOADED = 7,
	NE_OTHER_ERROR = 8
};

// WSA
static UBOOL IsWSAInit = false;
static WSAData MyWSAData;

// Mutex
static HANDLE SyncMutex = CreateMutex(NULL, false, NULL);
static UBOOL IsDownloading = false;

// Data
static UBOOL SignalAbort = true;
static NetworkError SignalOutput = NE_OK;

static INT BytesDownloaded = 0;
static INT BytesTotal = -1;

static FString CurrentURL = TEXT("");
static FString CurrentMusicName = TEXT("");

static FString Host = TEXT("");
static FString Path = TEXT("");
static INT Port = -1;

static DWORD STDCALL DownloadThreadEntry(void* Arg)
{
	// One download at time!
	DWORD result = WaitForSingleObject(SyncMutex, INFINITE);
	if (result != WAIT_OBJECT_0)
	{
		SignalOutput = NE_INVALID;
		return 0;
	}

	FILE* f = NULL;
	SOCKET sd = INVALID_SOCKET;
	FString filename = TEXT(""), tempFilename = TEXT("");

	try
	{
		if (Host.Len() == 0 || Path.Len() == 0 || Port <= 0)
		{
			SignalOutput = NE_INVALID;
			ReleaseMutex(SyncMutex);
			return 0;
		}

		// Check if we already have the file
		filename = FString::Printf(TEXT("..\\Music\\%s.ogg"), (TCHAR*)*CurrentMusicName);
		tempFilename = FString::Printf(TEXT("..\\Music\\%s.ogg.tmp"), (TCHAR*)*CurrentMusicName);

		DWORD attrib = GetFileAttributes(*filename);
		if (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY))
		{
			SignalOutput = NE_ALREADY_DOWNLOADED;
			ReleaseMutex(SyncMutex);
			return 0;
		}

		// Make sure WSA is initialized
		if (!IsWSAInit)
		{
			SignalOutput = NE_CONNECTION_PROBLEM;
			ReleaseMutex(SyncMutex);
			return 0;
		}

		ADDRINFOA hints;
		PADDRINFOA serverInfo;

		memset(&hints, 0, sizeof(ADDRINFOA));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		
		for (INT i = 0; i < 3; i++)
		{
			if (getaddrinfo(TCHAR_TO_ANSI(*Host), "http", &hints, &serverInfo) == 0)
				break;

			INT e = WSAGetLastError();
			if (e == WSAHOST_NOT_FOUND || e == WSANO_DATA)
				break;
		}

		SOCKADDR_IN serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(Port);

		bool resolved = false;
		for (PADDRINFOA p = serverInfo; p != NULL; p = p->ai_next)
		{
			if (p->ai_family == AF_INET && p->ai_socktype == SOCK_STREAM)
			{
				serverAddr.sin_addr = ((SOCKADDR_IN*)(p->ai_addr))->sin_addr;
				resolved = true;
				break;
			}
		}

		if (!resolved || SignalAbort)
		{
			SignalOutput = SignalAbort ? NE_CANCELLED : NE_URL_NOT_RESOLVED;
			freeaddrinfo(serverInfo);
			ReleaseMutex(SyncMutex);
			return 0;
		}	

		freeaddrinfo(serverInfo);

		sd = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
		if (sd == INVALID_SOCKET)
		{
			SignalOutput = NE_CONNECTION_PROBLEM;
			ReleaseMutex(SyncMutex);
			return 0;
		}

		// No more than 5 seconds
		DWORD timeout = 5000;
		setsockopt(sd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));
		setsockopt(sd, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout, sizeof(timeout));

		if (connect(sd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) || SignalAbort)
		{
			closesocket(sd);
			SignalOutput = SignalAbort ? NE_CANCELLED : NE_CONNECTION_PROBLEM;
			ReleaseMutex(SyncMutex);
			return 0;
		}
	
		f = fopen(TCHAR_TO_ANSI(*tempFilename), "wb+");
		if (!f)
		{
			closesocket(sd);
			SignalOutput = NE_IO_ERROR;
			ReleaseMutex(SyncMutex);
			return 0;
		}

		FString request = FString::Printf(TEXT("GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: Unreal\r\nConnection: close\r\n\r\n"), *Path, *Host);
		send(sd, TCHAR_TO_ANSI(*request), request.Len(), 0);

		int bytesExpected = -1;
		int bytesReceived;
		int dataOffset = 0;
		char buffer[4096];

		while (BytesDownloaded != BytesTotal)
		{
			// Cancel?
			if (SignalAbort)
			{
				closesocket(sd);
				fclose(f);
				DeleteFile(*tempFilename);
				SignalOutput = NE_CANCELLED;
				ReleaseMutex(SyncMutex);
				return 0;
			}

			// Anything left if parsing header
			if (sizeof(buffer) - dataOffset <= 0)
			{
				closesocket(sd);
				fclose(f);
				DeleteFile(*tempFilename);		
				SignalOutput = NE_FILE_NOT_FOUND;
				ReleaseMutex(SyncMutex);
				return 0;
			}

			bytesReceived = recv(sd, &(buffer[dataOffset]), sizeof(buffer) - dataOffset, 0);

			if (bytesReceived <= 0)
			{
				closesocket(sd);
				fclose(f);
				DeleteFile(*tempFilename);
				SignalOutput = NE_CONNECTION_PROBLEM;
				ReleaseMutex(SyncMutex);
				return 0;
			}

			// Parse the header
			if (BytesTotal == -1)
			{
				int dataStart = -1;

				for (int i = 0; i < sizeof(buffer) - 3; i++)
				{
					if (buffer[i] == '\r' && buffer[i + 1] == '\n' && buffer[i + 2] == '\r' && buffer[i + 3] == '\n')
					{
						buffer[i] = '\0';
						dataStart = i + 4;
						break;
					}
				}

				if (dataStart == -1)
				{
					dataOffset += bytesReceived;
					continue;
				}

				FString header = ANSI_TO_TCHAR(buffer);
				FString responseOK = TEXT("200 OK");

				if (header.InStr(responseOK) == -1)
				{
					closesocket(sd);
					fclose(f);
					DeleteFile(*tempFilename);		
					SignalOutput = NE_FILE_NOT_FOUND;
					ReleaseMutex(SyncMutex);
					return 0;
				}
				
				FString contentLength = TEXT("Content-Length: ");
				INT lengthPos = header.InStr(contentLength);
				if (lengthPos == -1)
				{
					closesocket(sd);
					fclose(f);
					DeleteFile(*tempFilename);		
					SignalOutput = NE_FILE_NOT_FOUND;
					ReleaseMutex(SyncMutex);
					return 0;
				}

				BytesTotal = appAtoi(&((*header)[lengthPos]) + contentLength.Len());
				if (BytesTotal <= 0)
				{
					closesocket(sd);
					fclose(f);
					DeleteFile(*tempFilename);
					SignalOutput = NE_FILE_NOT_FOUND;
					ReleaseMutex(SyncMutex);
					return 0;
				}
				
				fwrite(&buffer[dataStart], bytesReceived - dataStart, 1, f);
				BytesDownloaded += bytesReceived - dataStart;
				dataOffset = 0;
			}
			else
			{
				fwrite(buffer, bytesReceived, 1, f);
				BytesDownloaded += bytesReceived;
			}
		}
	}
	catch (...)
	{
		// Ensure cleanup
		if (f)
		{
			fclose(f);
			DeleteFile(*tempFilename);
		}
		closesocket(sd);
		SignalOutput = NE_OTHER_ERROR;
		ReleaseMutex(SyncMutex);
		return 0;
	}

	// We're good here, move the file from tmp to final and close it
	fclose(f);
  closesocket(sd);
	SignalOutput = MoveFile(*tempFilename, *filename) ? NE_OK : NE_IO_ERROR;
	ReleaseMutex(SyncMutex);
	return 0;
}

static bool IsThreadActive()
{
	DWORD result = WaitForSingleObject(SyncMutex, 0);
	bool waiting = true;

	if (result == WAIT_OBJECT_0)
	{
		ReleaseMutex(SyncMutex);
		waiting = false;
	}

	return waiting;
}

UMusicDownloaderInteraction::UMusicDownloaderInteraction()
{
	// Must use same as IpDrv!!!
	if (!IsWSAInit)
	{
		if (WSAStartup(MAKEWORD(1, 1), &MyWSAData) == 0)
			IsWSAInit = true;
	}

	// Verify classes
	UBOOL Mismatch = false;
	#define VERIFY_CLASS_SIZES
	#define NAMES_ONLY
	#define AUTOGENERATE_NAME(name)
	#define AUTOGENERATE_FUNCTION(cls,idx,name)
	#include "MusicDownloaderClasses.h"
	#undef AUTOGENERATE_FUNCTION
	#undef AUTOGENERATE_NAME
	#undef NAMES_ONLY
	#undef VERIFY_CLASS_SIZES
	if(Mismatch)
		appErrorf(TEXT("C++/ Script class size mismatch"));
}

void UMusicDownloaderInteraction::DlReset()
{
	IsDownloading = false;
	CurrentURL = TEXT("");
	CurrentMusicName = TEXT("");

	Host = TEXT("");
	Path = TEXT("");
	Port = -1;
}

void UMusicDownloaderInteraction::execNativeTick(FFrame &Stack, RESULT_DECL)
{
	guard(UMusicDownloaderInteraction::execNativeTick);
	P_GET_FLOAT(DeltaTime);
	P_FINISH;

	if (IsDownloading && SignalOutput != NE_RESET)
	{
		FString URL = CurrentURL;
		FString MusicName = CurrentMusicName;
		DlReset();

		if (SignalOutput)
		{
			GLog->Logf(TEXT("Downloading %s to %s.ogg failed! Error code: %d"), *URL, *MusicName, SignalOutput);
			eventDownloadError(URL, MusicName);
		}
		else
		{
			GLog->Logf(TEXT("Downloading %s to %s.ogg was successful!"), *URL, *MusicName);
			eventDownloadComplete(URL, MusicName);		
		}
	}

	unguard;
}

void UMusicDownloaderInteraction::execGetDownloadStatus(FFrame &Stack, RESULT_DECL)
{
	guard(UMusicDownloaderInteraction::execGetDownloadStatus);
	P_GET_STR_REF(MusicName);
	P_GET_INT_REF(DownloadedBytes);
	P_GET_INT_REF(DownloadSize);
	P_FINISH;
	
	*MusicName = CurrentMusicName;
	*DownloadedBytes = BytesDownloaded;
	*DownloadSize = BytesTotal;

	unguard;
}

void UMusicDownloaderInteraction::execIsDownloading(FFrame &Stack, RESULT_DECL)
{
	guard(UMusicDownloaderInteraction::execIsDownloading);
	P_FINISH;
	*((UBOOL*)Result) = IsDownloading;
	unguard;
}

void UMusicDownloaderInteraction::execIsMusicDownloaded(FFrame &Stack, RESULT_DECL)
{
	guard(UMusicDownloaderInteraction::execIsMusicDownloaded);
	P_GET_STR(MusicName);
	P_FINISH;

	FString filename = FString::Printf(TEXT("..\\Music\\%s.ogg"), (TCHAR*)*MusicName);
	DWORD attrib = GetFileAttributes(*filename);
	if (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY))
		*((UBOOL*)Result) = true;
	else
		*((UBOOL*)Result) = false;

	unguard;
}

void UMusicDownloaderInteraction::execDownloadMusic(FFrame &Stack, RESULT_DECL)
{ 
	guard(UMusicDownloaderInteraction::execDownloadMusic);
	P_GET_STR(URL);
	P_GET_STR(MusicName);
	P_FINISH;

	INT hostEnd = -1;
	INT portStart = -1;
	INT pathStart = -1;
	FString host;
	INT port;
	FString path;

	// If a thread is holding the mutex, it hasn't finished processing.
	// Check our flag but also if the mutex is locked and thus the DL thread isn't finished ...
	if (IsDownloading || IsThreadActive())
	{
		GLog->Logf(TEXT("Trying to download a new music while downloading another!"));
		*((UBOOL*)Result) = false;
		goto exit;
	}

	// Parse URL
	INT hostStart = URL.InStr(TEXT("://"));
	if (hostStart < 0)
	{
		GLog->Logf(TEXT("URL %s is not valid!"), *URL);
		*((UBOOL*)Result) = false;
		goto exit;
	}

	for (INT i = hostStart + 3; i < URL.Len(); i++)
	{
		if ((*URL)[i] == TEXT(':') && hostEnd == -1)
		{
			if (hostEnd == -1)
			{
				hostEnd = i;
				portStart = i + 1;
			}
		}
		else if ((*URL)[i] == TEXT('/'))
		{
			if (hostEnd == -1)
				hostEnd = i;
            pathStart = i;
			break;
		}
	}

	if (hostEnd == -1 || pathStart == -1)
	{
		GLog->Logf(TEXT("URL %s has invalid hostname!"), *URL);
		*((UBOOL*)Result) = false;
		goto exit;
	}

	host = URL.Mid(hostStart + 3, hostEnd - (hostStart + 3));
	port = 80;

	if (portStart != -1)
	{
		FString portStr = URL.Mid(portStart, pathStart - (hostStart + 3));
		port = appAtoi(*portStr);
	}

	path = URL.Mid(pathStart);

	GLog->Logf(TEXT("Starting download %s to %s.ogg ..."), *URL, *MusicName);

	// Setup download
	IsDownloading = true;
	Host = host;
	Path = path;
	Port = port;
	CurrentURL = URL;
	CurrentMusicName = MusicName;
	SignalAbort = false;
	SignalOutput = NE_RESET;
	BytesTotal = -1;
	BytesDownloaded = 0;

	// Spawn the thread
	HANDLE hThread = CreateThread(NULL, 0, DownloadThreadEntry, NULL, 0, NULL);
	check(hThread);
	CloseHandle(hThread);

	// Return
	*((UBOOL*)Result) = true;
exit:
	// NOP - (prevents compiler error since the label is the last statement)
	do {} while (0);

	unguard;
	return;
}

void UMusicDownloaderInteraction::execCancelDownload(FFrame &Stack, RESULT_DECL)
{
	guard(UMusicDownloaderInteraction::execCancelDownload);
	P_FINISH;

	if (IsDownloading)
	{
		FString URL = CurrentURL;
		FString MusicName = CurrentMusicName;
		SignalAbort = true;
		DlReset();

		eventDownloadCancelled(URL, MusicName);
		*((UBOOL*)Result) = true;
	}
	else
		*((UBOOL*)Result) = false;

	unguard;
}
