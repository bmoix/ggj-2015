#ifndef RESOURCEIDENTIFIERS_H
#define RESOURCEIDENTIFIERS_H


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
        Llibre,
        Player,
        Caixa,
        Fons1,
        MFIFons,
        MFIPlayer,
        MG1Fons,
        MG1Player,
        WMBackground,
        WMPlayer,
        WMPerson,
        WMObject,
        FightBackground,
        FightPlayer,
        FightGroundEnemy,
        FightSmallAirEnemy,
        FightBigAirEnemy
	};

    namespace SplashImage {
        enum ID {
            Fons,
            Fletxa
        };
    }
}

namespace Fonts
{
	enum ID
	{
        AlluraRegular,
        Sansation
	};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>	FontHolder;

#endif // RESOURCEIDENTIFIERS_H
