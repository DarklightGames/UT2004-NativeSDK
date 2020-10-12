# UT2004-NativeSDK

This is a mirror of a file posted at [ceonss.net](http://ceonss.net/viewtopic.php?t=1157) which allows you to compile native code for the Unreal Tournament 2004 engine.

Verbatim quote from the original post:

> [Coding] Native coding for UT2004
>
> Post by Izumo_CZ » Mon 7. Jan 2019, 04:52
>
> Hi there,
>
> I thought I'd share this for people to play with albeit it's useful only for coders (I was doing coding stuff for LDG before retiring from UT2k4). It is possible to create native packages for UT2004, albeit severely limited since the headers weren't published and are instead based off UT v432 headers and modified further to work with UT2004 (but only Object, Interaction, Actor classes (actually Volume as well) - I wasn't bothered adapting it for other classes or to compile onslaught etc.).
>
> We've used this on LDG for our music downloader mod (which is IMO pretty much the only feature of the game that's worth to implement as a client-side native mod) and I included a heavily stripped version of it in the source code (which you can compile). You can compile it for 32-bit version using Microsoft Visual Studio .NET 2003 or for 64-bit version using Microsoft Visual Studio 2005. Both of them work fine (64-bit version executable is statically linked but it still exports required symbols and you can link against it - the only limitation I noticed is that you have to use numbered native functions, otherwise it has the same capability to load the corresponding DLL). To test, compile the packages & DLL, place it to your System directory and add the mutator. Then use in-game console "CmdDownloadMusic http://www.someurl.com/somemusic.ogg somemusic" to interactively download music.
>
> The source code I've stripped from LDG version is the in-game installer of the DLL for 32-bit version (kinda obvious reasoning here) and Anti-TCC "graceful" bypass (= will only whitelist itself without disabling any checks). Everything is also renamed to prevent any collisions. The attached code is only however an example (yes, you WILL get kicked by Anti-TCC if you have it loaded :) ).
>
> While there's not much to do for client-side it may however serve more purpose if there's something to improve on the server, provided it's running Windows (Linux is a whole different story ._.) .
