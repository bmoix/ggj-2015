#ifndef RESOURCE_IDENTIFIERS_H
#define RESOURCE_IDENTIFIERS_H


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
        Cursor
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

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;

#endif // RESOURCE_IDENTIFIERS_H
