#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>
#include <memory>

/**
 * 游戏数据模型
 * 管理整个游戏的运行时数据状态
 */
class GameModel
{
public:
    GameModel();
    ~GameModel();
    
    // 游戏区域卡牌管理
    const std::vector<std::shared_ptr<CardModel>>& getPlayfieldCards() const { return _playfieldCards; }
    void setPlayfieldCards(const std::vector<std::shared_ptr<CardModel>>& cards) { _playfieldCards = cards; }
    void addPlayfieldCard(std::shared_ptr<CardModel> card) { _playfieldCards.push_back(card); }
    void removePlayfieldCard(int cardId);
    std::shared_ptr<CardModel> getPlayfieldCard(int cardId) const;
    
    // 手牌堆卡牌管理
    const std::vector<std::shared_ptr<CardModel>>& getStackCards() const { return _stackCards; }
    void setStackCards(const std::vector<std::shared_ptr<CardModel>>& cards) { _stackCards = cards; }
    void addStackCard(std::shared_ptr<CardModel> card) { _stackCards.push_back(card); }
    std::shared_ptr<CardModel> popStackCard();
    std::shared_ptr<CardModel> getTopStackCard() const;
    bool isStackEmpty() const { return _stackCards.empty(); }
    
    // 底牌管理
    std::shared_ptr<CardModel> getTrayCard() const { return _trayCard; }
    void setTrayCard(std::shared_ptr<CardModel> card) { _trayCard = card; }
    
    // 游戏状态
    bool isGameActive() const { return _isGameActive; }
    void setGameActive(bool active) { _isGameActive = active; }
    
    // 得分管理
    int getScore() const { return _score; }
    void addScore(int points) { _score += points; }
    void setScore(int score) { _score = score; }
    
    // 根据ID查找卡牌
    std::shared_ptr<CardModel> findCard(int cardId) const;
    
    // 清空所有卡牌
    void clear();

private:
    std::vector<std::shared_ptr<CardModel>> _playfieldCards;    // 游戏区域卡牌
    std::vector<std::shared_ptr<CardModel>> _stackCards;        // 手牌堆卡牌
    std::shared_ptr<CardModel> _trayCard;                       // 当前底牌
    
    bool _isGameActive;                                         // 游戏是否进行中
    int _score;                                                 // 当前得分
};

#endif // __GAME_MODEL_H__
