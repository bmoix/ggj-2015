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
        GameBackground,
        Player1,
        PlayerAnimation,
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
