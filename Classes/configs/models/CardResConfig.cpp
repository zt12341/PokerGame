#include "CardResConfig.h"

USING_NS_CC;

const Size CardResConfig::kCardSize = Size(160, 220);

CardResConfig::CardResConfig()
{
}

CardResConfig::~CardResConfig()
{
}

std::string CardResConfig::getNumberImagePath(CardFaceType face, bool isRed, bool isBig)
{
    std::string basePath = "res/number/";
    std::string size = isBig ? "big_" : "small_";
    std::string color = isRed ? "red_" : "black_";
    
    std::string faceStr;
    switch (face)
    {
        case CFT_ACE: faceStr = "A"; break;
        case CFT_TWO: faceStr = "2"; break;
        case CFT_THREE: faceStr = "3"; break;
        case CFT_FOUR: faceStr = "4"; break;
        case CFT_FIVE: faceStr = "5"; break;
        case CFT_SIX: faceStr = "6"; break;
        case CFT_SEVEN: faceStr = "7"; break;
        case CFT_EIGHT: faceStr = "8"; break;
        case CFT_NINE: faceStr = "9"; break;
        case CFT_TEN: faceStr = "10"; break;
        case CFT_JACK: faceStr = "J"; break;
        case CFT_QUEEN: faceStr = "Q"; break;
        case CFT_KING: faceStr = "K"; break;
        default: faceStr = "A"; break;
    }
    
    return basePath + size + color + faceStr + ".png";
}

std::string CardResConfig::getSuitImagePath(CardSuitType suit)
{
    std::string basePath = "res/suits/";
    
    switch (suit)
    {
        case CST_CLUBS: return basePath + "club.png";
        case CST_DIAMONDS: return basePath + "diamond.png";
        case CST_HEARTS: return basePath + "heart.png";
        case CST_SPADES: return basePath + "spade.png";
        default: return basePath + "heart.png";
    }
}

std::string CardResConfig::getCardBackgroundPath()
{
    return "res/card_general.png";
}

Size CardResConfig::getCardSize()
{
    return kCardSize;
}

bool CardResConfig::isRedSuit(CardSuitType suit)
{
    return (suit == CST_HEARTS || suit == CST_DIAMONDS);
}
