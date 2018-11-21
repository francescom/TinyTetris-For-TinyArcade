# Tinytris-For-TinyArcade

Adaptation of my TinyTetris (on codebender.cc and hackster.io):

https://www.hackster.io/francescomm/tinytetris-06ce50


for the TinyArcade:
 
https://www.kickstarter.com/projects/kenburns/tiny-arcade-a-retro-tiny-playable-game-cabinet


At the moment it runs, has background music and a splash screen.
It looks for a "Tinytris" folder on the SD card with inside a "music.raw" file.
Three alternatives music are available, choose the one you like most and rename it music.raw

Music is a personal arrangement, made with Garageband, derived from a classic "Korobeiniki" (Russian: Коробейники) midi file.
https://en.wikipedia.org/wiki/Korobeiniki



To use your own music, export audio to this format.

Set frequency to 11025Hz, mono

Export to format (names are from free software Audacity):

    Other uncompressed formats
        Raw (header-less)
            Unsigned 8-bit PCM


and call it "music.raw".


**Help**

- Joystick moves pieces left + right + down=fast
- Buttons rotate in two directions (right button is "classic" top-goes-right rotation)

**Push the 2 buttons together** to reset the game and toggle blocks size. Larger blocks are easier to see but leave less vertical space to play.


**Possible extensions**

- Hi scores +prefs on µSD: at the moment a blank prefs file is saved inside the app folder
- a global options menu (start, pause, restart, mute effects, mute music, change size, show scores)


**Possible fixes**

If it doesn't link correctly reporting a lot of "undefined reference to" all files into subfolders
- keep the original folders (files are needed for includes "#include TinyAudio/TinyAudioLib/somefile.h")
- drag duplicate copies of all the source files inside TinyAudio/TinyAudioLib amd inside audio to the main folder (both .cpp and .h files, .cpp files expect .h files to be on the same level)
- close and reopen the .ino sketch
- tell the IDE people to fix compilation of subfolders
