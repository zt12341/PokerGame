#include "GameModel.h"

USING_NS_CC;

GameModel::GameModel()
    : _isGameActive(false)
    , _score(0)
{
}

GameModel::~GameModel()
{
    clear();
}

void GameModel::removePlayfieldCard(int cardId)
{
    auto it = std::find_if(_playfieldCards.begin(), _playfieldCards.end(),
        [cardId](const std::shared_ptr<CardModel>& card) {
            return card->getCardId() == cardId;
        });
    
    if (it != _playfieldCards.end())
    {
        _playfieldCards.erase(it);
    }
}

std::shared_ptr<CardModel> GameModel::getPlayfieldCard(int cardId) const
{
    auto it = std::find_if(_playfieldCards.begin(), _playfieldCards.end(),
        [cardId](const std::shared_ptr<CardModel>& card) {
            return card->getCardId() == cardId;
        });
    
    return (it != _playfieldCards.end()) ? *it : nullptr;
}

std::shared_ptr<CardModel> GameModel::popStackCard()
{
    if (_stackCards.empty())
        return nullptr;
    
    auto card = _stackCards.back();
    _stackCards.pop_back();
    return card;
}

std::shared_ptr<CardModel> GameModel::getTopStackCard() const
{
    return _stackCards.empty() ? nullptr : _stackCards.back();
}

std::shared_ptr<CardModel> GameModel::findCard(int cardId) const
{
    // 在游戏区域查找
    auto card = getPlayfieldCard(cardId);
    if (card)
        return card;
    
    // 在手牌堆查找
    auto it = std::find_if(_stackCards.begin(), _stackCards.end(),
        [cardId](const std::shared_ptr<CardModel>& card) {
            return card->getCardId() == cardId;
        });
    
    if (it != _stackCards.end())
        return *it;
    
    // 检查底牌
    if (_trayCard && _trayCard->getCardId() == cardId)
        return _trayCard;
    
    return nullptr;
}

void GameModel::clear()
{
    _playfieldCards.clear();
    _stackCards.clear();
    _trayCard.reset();
    _isGameActive = false;
    _score = 0;
}
