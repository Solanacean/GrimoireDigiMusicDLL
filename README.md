## GrimoireDigiMusicDLL
GrimoireDigiMusicDLL is a wrapper which adds support for digital music (MP3) to Grimoire: Heralds of the Winged Exemplar. 
It is compatible with both Grimoire V1 and V2. 

### Building from source code
To build the wrapper from source code you need Microsoft Visual Studio Community 2017. The following libraries are another requirement:

[Microsoft Detours Express 3.0](https://www.microsoft.com/en-us/download/details.aspx?id=52586)  
[BASS audio library 2.4](http://www.un4seen.com/bass.html)

Building the project will produce file `dsound.dll` (which is not to be confused with dsound.dll residing in your Windows system folder).
    
### Installation
Copy `dsound.dll` and `bass.dll` to the game's installation folder. The wrapper will look for the tracks in MP3 format
in a subfolder named `Music` which must contain all the required files (see details below).

The game's folder should look like this (note the `Music` folder and its content and the files `bass.dll` and `dsound.dll`):<br>

<pre>
|-- 001.SVG
|-- BUNDLE-Z.RSF
|-- Grimoire.exe
|-- Music 
|   |-- !BELDARK.mp3
|   |-- !CUCAROK.mp3
|   |-- !EERIE.mp3
|   |-- !ENDTIME.mp3
|   |-- !EPIC.mp3
|   |-- !LUCID.mp3
|   |-- !PATLVUP.mp3
|   |-- !PRESENT.mp3
|   |-- !UPBEAT.mp3
|   |-- !VISIONS.mp3
|   |-- ADVANCE.mp3
|   |-- BATTLE.mp3
|   |-- BOSS.mp3
|   |-- CALYPSO.mp3
|   |-- CITYWALK.mp3
|   |-- CREATION.mp3
|   |-- CREDITS.mp3
|   |-- DARKHUNT.mp3
|   |-- DUNJON.mp3
|   |-- ELFMAGIC.mp3
|   |-- ENIGMA.mp3
|   |-- ENTOMBED.mp3
|   |-- FAIRYEYE.mp3
|   |-- FEARLESS.mp3
|   |-- FORTUNE.mp3
|   |-- GHOSTLY.mp3
|   |-- GRAVEROB.mp3
|   |-- HEROFEAT.mp3
|   |-- HOARD.mp3
|   |-- HYPNOTIC.mp3
|   |-- JOURNEY.mp3
|   |-- LABYRITH.mp3
|   |-- LANTERN.mp3
|   |-- MAIN.mp3
|   |-- NAUTILUS.mp3
|   |-- NOMAD.mp3
|   |-- ORDERS.mp3
|   |-- SLEEP.mp3
|   |-- SPHINX.mp3
|   |-- SUNSCOUT.mp3
|   `-- THEQUEST.mp3
|-- RPG100.LIB
|-- _CommonRedist
|   `-- vcredist
|       `-- 2010
|           |-- installscript.vdf
|           |-- vcredist_x64.exe
|           `-- vcredist_x86.exe
|-- bass.dll
|-- dsound.dll
|-- fast.qst
|-- game.cfg
`-- grimoire.log
</pre>

Each time the game is launched (provided you installed the wrapper correctly) a `DIGIMUSIC_LOG.txt` file will be created 
in the game’s folder. In case something goes wrong please check this file for possible error messages.

### Uninstallation
If you'd rather disable the wrapper instead of removing it, go to the game's folder and simply rename `dsound.dll` to e.g. `_dsound.dll` 

To uninstall it, just delete the wrapper's `dsound.dll` from the game's folder. You can also delete the files `bass.dll` and 
`DIGIMUSIC_LOG.txt` as well as the `Music` folder.
