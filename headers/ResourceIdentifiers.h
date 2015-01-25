#ifndef RESOURCE_IDENTIFIERS_H
#define RESOURCE_IDENTIFIERS_H

#include <SFML/Audio.hpp>

// Forward declaration of SFML classes
namespace sf
{
	class Texture;
	class Font;
}

namespace Textures
{
	enum ID
	{
        GameBackground1,
        GameBackground2,
        HelpBackground,
        CountdownBackground,
        Player1,
        Player2,
        PlayerAnimation1,
        PlayerAnimation2,
        Blue,
        Red,
        Platform1,
        TrapBox,
        SpikesBall,
        PlatformWood,
        PlatformStone,
        IconBox,
        IconBall,
        IconSpikes,
        IconSwitch,
        Spikes,
        Title,
        Start,
        Help,
        Exit,
        RaccoonWins,
        FoxWins,
        Cursor,
        Return,
        AButton,
        BButton,
        XButton,
        YButton
	};
}

namespace Fonts
{
	enum ID
	{
        AlluraRegular,
        Sansation,
        Gomo
	};
}

namespace Music
{
        enum ID
        {
                MenuTheme,
                GameTheme,
                StartTheme,
                FinishTheme,
                WinningTheme,
        };
}

namespace SoundEffect {
        enum ID
        {
                Cage,
                Gong,
                Jump,
                Spikeball,
                Spikes,
                Switch,
        };
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;

#endif // RESOURCE_IDENTIFIERS_H
