/*===========================================================================
    Music Downloader private header file
===========================================================================*/

#ifndef MUSICDOWNLOADERPRIVATE_H
#define MUSICDOWNLOADERPRIVATE_H

/*----------------------------------------------------------------------------
	API.
----------------------------------------------------------------------------*/

extern "C" {
    void RegisterNamesMusicDownloader(void);
}

/*-----------------------------------------------------------------------------
	Includes.
-----------------------------------------------------------------------------*/

#include "Engine.h"
#include "MusicDownloaderClasses.h"

#if __STATIC_LINK
#define NAMES_ONLY
#define NATIVE_DEFS_ONLY
#define AUTOGENERATE_NAME(name)
#define AUTOGENERATE_FUNCTION(cls,idx,name)
#include "MusicDownloaderClasses.h"
#undef AUTOGENERATE_FUNCTION
#undef AUTOGENERATE_NAME
#undef NATIVE_DEFS_ONLY
#undef NAMES_ONLY
#endif

#endif

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
