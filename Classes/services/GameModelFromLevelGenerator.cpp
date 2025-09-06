#include "GameModelFromLevelGenerator.h"

USING_NS_CC;

int GameModelFromLevelGenerator::s_nextCardId = 1;

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig& levelConfig)
{
    GameModel* gameModel = new GameModel();
    
    // 生成游戏区域卡牌
    generatePlayfieldCards(gameModel, levelConfig.getPlayfieldCards());
    
    // 生成手牌堆卡牌
    generateStackCards(gameModel, levelConfig.getStackCards());
    
    // 设置初始底牌（从手牌堆取第一张）
    if (!gameModel->isStackEmpty())
    {
        auto firstCard = gameModel->popStackCard();
        if (firstCard)
        {
            // 将底牌位置调整到右移后的新位置
            firstCard->setPosition(Vec2(550, 400)); // 底牌区右移后位置
            gameModel->setTrayCard(firstCard);
        }
    }
    
    // 设置游戏为活跃状态
    gameModel->setGameActive(true);
    
    return gameModel;
}

void GameModelFromLevelGenerator::generatePlayfieldCards(GameModel* gameModel, const std::vector<LevelConfig::CardConfig>& cardConfigs)
{
    for (const auto& config : cardConfigs)
    {
        int cardId = generateCardId();
        auto card = std::make_shared<CardModel>(cardId, config.cardFace, config.cardSuit, config.position);
        card->setOriginalPosition(config.position);
        gameModel->addPlayfieldCard(card);
    }
}

void GameModelFromLevelGenerator::generateStackCards(GameModel* gameModel, const std::vector<LevelConfig::CardConfig>& cardConfigs)
{
    Vec2 baseStackPosition(250, 400); // 底牌备用牌区右移后位置
    
    for (size_t i = 0; i < cardConfigs.size(); ++i)
    {
        const auto& config = cardConfigs[i];
        int cardId = generateCardId();
        
        // 备用牌水平摊开显示，保持同一高度，增加重叠效果
        Vec2 cardPosition = baseStackPosition + Vec2(i * 30, 0); // 水平间距30像素，Y坐标相同
        
        auto card = std::make_shared<CardModel>(cardId, config.cardFace, config.cardSuit, cardPosition);
        card->setOriginalPosition(cardPosition);
        gameModel->addStackCard(card);
    }
}

int GameModelFromLevelGenerator::generateCardId()
{
    return s_nextCardId++;
}
