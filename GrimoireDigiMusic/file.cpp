#include <string>
#include <unordered_map>
#include <filesystem>
#include <system_error>

bool DirExists(std::string const &path)
{
    namespace fs = std::filesystem;
    std::error_code ec;

    if (!fs::is_directory(path, ec) || !fs::exists(path, ec))
    {
        return false;
    }

    return true;
}

std::string MIDILenToFileName(int midiLen)
{
    static std::unordered_map<int, std::string> umMIDILenToFileName = {
        {   249, "!VISIONS.mp3" }, // Grimoire V2
        {   364, "!ENDTIME.mp3" }, // Grimoire V2
        {   459, "!PATLVUP.mp3" }, // Grimoire V2
        {   840, "!LUCID.mp3"   }, // Grimoire V2
        {  2242, "!CUCAROK.mp3" }, // Grimoire V2
        {  2439, "!PRESENT.mp3" },
        {  3846, "SLEEP.mp3"    },
        {  4236, "GRAVEROB.mp3" },
        {  4921, "GHOSTLY.mp3"  },
        {  5495, "HOARD.mp3"    },
        {  5537, "CREATION.mp3" },
        {  5669, "JOURNEY.mp3"  },
        {  6112, "!EERIE.mp3"   },
        {  6133, "ADVANCE.mp3"  },
        {  6564, "LABYRITH.mp3" },
        {  7233, "!UPBEAT.mp3"  },
        {  7785, "ENTOMBED.mp3" },
        {  7808, "CITYWALK.mp3" },
        {  7925, "THEQUEST.mp3" },
        {  8102, "LANTERN.mp3"  },
        {  8170, "NAUTILUS.mp3" },
        {  8348, "!BELDARK.mp3" }, // Grimoire V2
        {  8682, "CALYPSO.mp3"  },
        {  8743, "ORDERS.mp3"   },
        {  8839, "DUNJON.mp3"   },
        {  8908, "ENIGMA.mp3"   },
        {  9378, "HEROFEAT.mp3" },
        {  9679, "!EPIC.mp3"    },
        {  9907, "BOSS.mp3"     },
        { 10637, "ELFMAGIC.mp3" },
        { 10648, "FAIRYEYE.mp3" },
        { 10771, "CREDITS.mp3"  },
        { 11184, "BATTLE.mp3"   },
        { 11832, "HYPNOTIC.mp3" },
        { 12370, "FEARLESS.mp3" },
        { 12597, "DARKHUNT.mp3" },
        { 12790, "FORTUNE.mp3"  },
        { 18575, "SUNSCOUT.mp3" },
        { 19687, "MAIN.mp3"     },
        { 31447, "NOMAD.mp3"    },
        { 51070, "SPHINX.mp3"   }
    };

    auto it = umMIDILenToFileName.find(midiLen);

    if (it == umMIDILenToFileName.end())
    {
        return {};
    }

    return it->second;
}

