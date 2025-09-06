#include "CardModel.h"
#include <cmath>

USING_NS_CC;

CardModel::CardModel()
    : _cardId(-1)
    , _face(CFT_NONE)
    , _suit(CST_NONE)
    , _position(Vec2::ZERO)
    , _originalPosition(Vec2::ZERO)
    , _isVisible(true)
    , _isMoving(false)
{
}

CardModel::CardModel(int cardId, CardFaceType face, CardSuitType suit, const Vec2& position)
    : _cardId(cardId)
    , _face(face)
    , _suit(suit)
    , _position(position)
    , _originalPosition(position)
    , _isVisible(true)
    , _isMoving(false)
{
}

CardModel::~CardModel()
{
}

bool CardModel::canMatch(const CardModel& other) const
{
    int myValue = getFaceValue();
    int otherValue = other.getFaceValue();
    
    // A可以和K匹配（循环），其他按差1匹配
    if ((myValue == 1 && otherValue == 13) || (myValue == 13 && otherValue == 1))
    {
        return true;
    }
    
    return std::abs(myValue - otherValue) == 1;
}

int CardModel::getFaceValue() const
{
    switch (_face)
    {
        case CFT_ACE: return 1;
        case CFT_TWO: return 2;
        case CFT_THREE: return 3;
        case CFT_FOUR: return 4;
        case CFT_FIVE: return 5;
        case CFT_SIX: return 6;
        case CFT_SEVEN: return 7;
        case CFT_EIGHT: return 8;
        case CFT_NINE: return 9;
        case CFT_TEN: return 10;
        case CFT_JACK: return 11;
        case CFT_QUEEN: return 12;
        case CFT_KING: return 13;
        default: return 1;
    }
}

CardModel* CardModel::clone() const
{
    CardModel* card = new CardModel();
    card->_cardId = _cardId;
    card->_face = _face;
    card->_suit = _suit;
    card->_position = _position;
    card->_originalPosition = _originalPosition;
    card->_isVisible = _isVisible;
    card->_isMoving = _isMoving;
    return card;
}
