#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "cocos2d.h"
#include "../configs/models/LevelConfig.h"
#include "../models/GameModel.h"

/**
 * 游戏模型生成服务
 * 将静态关卡配置转换为运行时游戏数据模型
 */
class GameModelFromLevelGenerator
{
public:
    /**
     * 从关卡配置生成游戏模型
     * @param levelConfig 关卡配置
     * @return 生成的游戏模型，调用方负责内存管理
     */
    static GameModel* generateGameModel(const LevelConfig& levelConfig);
    
private:
    /**
     * 生成游戏区域卡牌
     * @param gameModel 游戏模型
     * @param cardConfigs 卡牌配置列表
     */
    static void generatePlayfieldCards(GameModel* gameModel, const std::vector<LevelConfig::CardConfig>& cardConfigs);
    
    /**
     * 生成手牌堆卡牌
     * @param gameModel 游戏模型
     * @param cardConfigs 卡牌配置列表
     */
    static void generateStackCards(GameModel* gameModel, const std::vector<LevelConfig::CardConfig>& cardConfigs);
    
    /**
     * 生成唯一的卡牌ID
     * @return 唯一的卡牌ID
     */
    static int generateCardId();
    
private:
    static int s_nextCardId;    // 下一个卡牌ID
};

#endif // __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
