/**
 * @file LevelConfig.h
 * @brief 关卡配置头文件
 * @author OUC-Zhou Tao
 * @date 2024
 * 
 * 关卡配置类的定义
 * 用于描述游戏关卡中卡牌的初始布局和属性
 * 支持多区域的卡牌配置管理
 */

#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include "../../utils/CardTypes.h"
#include <vector>

/**
 * @class LevelConfig
 * @brief 关卡配置类
 * 
 * 功能概述：
 * - 定义卡牌在游戏区域的初始布局
 * - 管理主游戏区、托盘区、备牌区的卡牌配置
 * - 提供关卡数据的序列化和反序列化
 * - 支持动态的关卡内容修改
 * 
 * 配置结构：
 * - 主游戏区卡牌：分布在上方70%黄色区域
 * - 备牌堆卡牌：位于下方30%紫色区域右侧
 * - 托盘初始卡牌：位于下方30%紫色区域左侧
 */
class LevelConfig
{
public:
    /**
     * @struct CardConfig
     * @brief 单张卡牌的配置信息
     * 
     * 包含卡牌的完整属性：点数、花色和位置坐标
     * 用于描述关卡中每张卡牌的初始状态
     */
    struct CardConfig
    {
        CardFaceType cardFace;      /**< 卡牌点数（A、2-10、J、Q、K） */
        CardSuitType cardSuit;      /**< 卡牌花色（草花、方块、红心、黑桃） */
        cocos2d::Vec2 position;     /**< 卡牌在游戏场景中的位置坐标 */
        
        /**
         * @brief 默认构造函数
         * 创建一个无效的卡牌配置，所有值设为默认
         */
        CardConfig() : cardFace(CFT_NONE), cardSuit(CST_NONE), position(cocos2d::Vec2::ZERO) {}
        
        /**
         * @brief 参数化构造函数
         * @param face 卡牌点数
         * @param suit 卡牌花色
         * @param pos 卡牌位置坐标
         * 
         * 根据指定参数创建完整的卡牌配置
         */
        CardConfig(CardFaceType face, CardSuitType suit, const cocos2d::Vec2& pos) 
            : cardFace(face), cardSuit(suit), position(pos) {}
    };
    
    // ==================== 构造与析构 ====================
    
    /**
     * @brief 默认构造函数
     * 创建空的关卡配置，所有区域都没有卡牌
     */
    LevelConfig();
    ~LevelConfig();
    
    // 获取游戏区域卡牌配置
    const std::vector<CardConfig>& getPlayfieldCards() const { return _playfieldCards; }
    
    // 获取手牌堆卡牌配置
    const std::vector<CardConfig>& getStackCards() const { return _stackCards; }
    
    // 设置游戏区域卡牌配置
    void setPlayfieldCards(const std::vector<CardConfig>& cards) { _playfieldCards = cards; }
    
    // 设置手牌堆卡牌配置
    void setStackCards(const std::vector<CardConfig>& cards) { _stackCards = cards; }

private:
    std::vector<CardConfig> _playfieldCards;    // 游戏区域卡牌配置
    std::vector<CardConfig> _stackCards;        // 手牌堆卡牌配置
};

#endif // __LEVEL_CONFIG_H__
