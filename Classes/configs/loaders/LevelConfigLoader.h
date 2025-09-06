#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "cocos2d.h"
#include "../models/LevelConfig.h"

/**
 * 关卡配置加载器
 * 负责从JSON文件或数据中加载关卡配置
 */
class LevelConfigLoader
{
public:
    /**
     * 加载关卡配置
     * @param levelId 关卡ID
     * @return 加载的关卡配置，失败返回nullptr
     */
    static LevelConfig* loadLevelConfig(int levelId);
    
    /**
     * 从JSON字符串加载关卡配置
     * @param jsonString JSON配置字符串
     * @return 加载的关卡配置，失败返回nullptr
     */
    static LevelConfig* loadFromJsonString(const std::string& jsonString);
    
    /**
     * 加载默认测试关卡
     * @return 默认测试关卡配置
     */
    static LevelConfig* loadDefaultTestLevel();

private:
    /**
     * Parse card configuration from JSON value
     * @param cardValue JSON value
     * @return Parsed card configuration
     */
    // static LevelConfig::CardConfig parseCardConfig(const rapidjson::Value& cardValue);
    
    /**
     * 将数值转换为卡牌点数
     * @param faceValue 点数值
     * @return 卡牌点数枚举
     */
    static CardFaceType intToCardFace(int faceValue);
    
    /**
     * 将数值转换为卡牌花色
     * @param suitValue 花色值
     * @return 卡牌花色枚举
     */
    static CardSuitType intToCardSuit(int suitValue);
};

#endif // __LEVEL_CONFIG_LOADER_H__
