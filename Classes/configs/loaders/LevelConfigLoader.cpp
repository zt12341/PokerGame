/**
 * @file LevelConfigLoader.cpp
 * @brief 关卡配置加载器实现文件
 * @author OUC-Zhou Tao
 * @date 2024
 * 
 * 关卡配置加载器的具体实现
 * 负责从各种数据源加载游戏关卡的配置信息
 * 支持JSON格式配置文件和硬编码的测试关卡
 */

#include "LevelConfigLoader.h"
#include "../models/LevelConfig.h"

USING_NS_CC;

/**
 * @brief 根据关卡ID加载关卡配置
 * @param levelId 关卡唯一标识符
 * @return 加载的关卡配置对象，失败时返回nullptr
 * 
 * 当前实现返回默认测试关卡配置
 * TODO: 后续可扩展支持多关卡配置文件
 */
LevelConfig* LevelConfigLoader::loadLevelConfig(int levelId)
{
    // 目前加载默认测试关卡，后续可根据levelId加载不同关卡
    return loadDefaultTestLevel();
}

/**
 * @brief 从JSON字符串加载关卡配置
 * @param jsonString 包含关卡数据的JSON格式字符串
 * @return 解析生成的关卡配置对象，失败时返回nullptr
 * 
 * TODO: 实现完整的JSON解析逻辑
 * 当前版本返回默认测试关卡作为占位实现
 */
LevelConfig* LevelConfigLoader::loadFromJsonString(const std::string& jsonString)
{
    // TODO: 实现JSON解析功能
    // 目前返回默认测试关卡
    return loadDefaultTestLevel();
}

/**
 * @brief 创建默认测试关卡配置
 * @return 预设的测试关卡配置对象
 * 
 * 生成用于测试的卡牌布局配置，包含：
 * - 主游戏区域的卡牌排列（上方黄色70%区域）
 * - 托盘区域的初始卡牌（下方紫色30%区域左侧）
 * - 备牌堆的卡牌配置（下方紫色30%区域右侧）
 * 
 * 卡牌排列规则：
 * - 主牌堆位置已优化，整体右移至X坐标400-750范围
 * - 选择具有匹配关系的卡牌组合（点数差值为1）
 * - 布局考虑视觉平衡和操作便利性
 */
LevelConfig* LevelConfigLoader::loadDefaultTestLevel()
{
    LevelConfig* config = new LevelConfig();
    
    // 创建主游戏区域卡牌配置
    std::vector<LevelConfig::CardConfig> playfieldCards;
    
    // 根据需求文档示例创建卡牌布局 - 主牌堆整体右移，更好利用上方空间
    playfieldCards.push_back(LevelConfig::CardConfig(CFT_QUEEN, CST_CLUBS, Vec2(400, 1500)));    // Q♣ - 右上角
    playfieldCards.push_back(LevelConfig::CardConfig(CFT_TWO, CST_DIAMONDS, Vec2(450, 1300)));   // 2♦ - 中右上
    playfieldCards.push_back(LevelConfig::CardConfig(CFT_TWO, CST_HEARTS, Vec2(500, 1100)));     // 2♥ - 右中
    playfieldCards.push_back(LevelConfig::CardConfig(CFT_THREE, CST_DIAMONDS, Vec2(850, 1500))); // 3♦ - 最右上
    playfieldCards.push_back(LevelConfig::CardConfig(CFT_TWO, CST_SPADES, Vec2(800, 1300)));     // 2♠ - 右上中
    playfieldCards.push_back(LevelConfig::CardConfig(CFT_ACE, CST_SPADES, Vec2(750, 1100)));     // A♠ - 右中下
    
    config->setPlayfieldCards(playfieldCards);
    
    // 创建备牌堆卡牌配置（下方右侧区域）
    std::vector<LevelConfig::CardConfig> stackCards;
    stackCards.push_back(LevelConfig::CardConfig(CFT_FOUR, CST_CLUBS, Vec2(0, 0)));    // 4♣ - 托盘底牌
    stackCards.push_back(LevelConfig::CardConfig(CFT_ACE, CST_HEARTS, Vec2(0, 0)));    // A♥ - 备牌1
    stackCards.push_back(LevelConfig::CardConfig(CFT_THREE, CST_CLUBS, Vec2(0, 0)));   // 3♣ - 备牌2
    
    config->setStackCards(stackCards);
    
    return config;
}

/**
 * @brief 将整数值转换为卡牌点数枚举
 * @param faceValue 整数形式的点数值
 * @return 对应的卡牌点数枚举，无效值时返回CFT_ACE
 * 
 * 用于从配置文件或外部数据源解析卡牌点数
 */
CardFaceType LevelConfigLoader::intToCardFace(int faceValue)
{
    if (faceValue >= 0 && faceValue < CFT_NUM_CARD_FACE_TYPES)
    {
        return static_cast<CardFaceType>(faceValue);
    }
    return CFT_ACE;  // 默认返回A作为安全值
}

/**
 * @brief 将整数值转换为卡牌花色枚举
 * @param suitValue 整数形式的花色值
 * @return 对应的卡牌花色枚举，无效值时返回CST_CLUBS
 * 
 * 用于从配置文件或外部数据源解析卡牌花色
 */
CardSuitType LevelConfigLoader::intToCardSuit(int suitValue)
{
    if (suitValue >= 0 && suitValue < CST_NUM_CARD_SUIT_TYPES)
    {
        return static_cast<CardSuitType>(suitValue);
    }
    return CST_CLUBS;  // 默认返回草花作为安全值
}
