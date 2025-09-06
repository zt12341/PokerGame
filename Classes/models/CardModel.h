/**
 * @file CardModel.h
 * @brief 卡牌数据模型类定义
 * 
 * 本文件定义了CardModel类，用于表示游戏中单张卡牌的所有数据信息，
 * 包括卡牌的花色、点数、位置坐标、可见性状态等属性。
 * 该类是整个卡牌游戏系统中最基础的数据单元。
 * 
 * @author OUC-Zhou Tao
 * @date 2025-09-06
 * @version 1.0
 */

#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "../utils/CardTypes.h"

/**
 * @class CardModel
 * @brief 卡牌数据模型类
 * 
 * 该类封装了游戏中单张卡牌的所有核心属性和操作方法。
 * 每张卡牌都有唯一的ID、花色、点数、位置等信息。
 * 支持位置管理、可见性控制、移动状态管理等基本操作。
 * 
 * 主要功能：
 * - 存储卡牌的基本属性（ID、花色、点数）
 * - 管理卡牌的位置信息（当前位置、原始位置）
 * - 控制卡牌的显示状态（可见性、移动状态）
 * - 提供属性访问和修改接口
 */
class CardModel
{
public:
    /**
     * @brief 默认构造函数
     * 创建一个空的卡牌模型实例，所有属性都设置为默认值
     */
    CardModel();
    
    /**
     * @brief 参数化构造函数
     * @param cardId 卡牌的唯一标识符
     * @param face 卡牌点数（A、2-10、J、Q、K）
     * @param suit 卡牌花色（黑桃、红心、方块、草花）
     * @param position 卡牌在游戏场景中的初始位置坐标
     * 
     * 根据指定参数创建卡牌模型实例，同时将初始位置设置为原始位置
     */
    CardModel(int cardId, CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position);
    
    /**
     * @brief 析构函数
     * 清理卡牌模型占用的资源
     */
    ~CardModel();
    
    // ==================== 属性获取方法 ====================
    
    /**
     * @brief 获取卡牌唯一标识符
     * @return 卡牌ID，用于在游戏中区分不同的卡牌实例
     */
    int getCardId() const { return _cardId; }
    
    /**
     * @brief 获取卡牌点数
     * @return 卡牌的点数值（A、2-10、J、Q、K）
     */
    CardFaceType getFace() const { return _face; }
    
    /**
     * @brief 获取卡牌花色
     * @return 卡牌的花色（黑桃、红心、方块、草花）
     */
    CardSuitType getSuit() const { return _suit; }
    
    /**
     * @brief 获取卡牌当前位置
     * @return 卡牌在游戏场景中的当前二维坐标
     */
    const cocos2d::Vec2& getPosition() const { return _position; }
    
    /**
     * @brief 获取卡牌原始位置
     * @return 卡牌的初始位置坐标，用于回退操作
     */
    const cocos2d::Vec2& getOriginalPosition() const { return _originalPosition; }
    
    /**
     * @brief 获取卡牌可见性状态
     * @return true表示卡牌可见，false表示卡牌隐藏
     */
    bool isVisible() const { return _isVisible; }
    
    /**
     * @brief 获取卡牌移动状态
     * @return true表示卡牌正在移动中，false表示卡牌静止
     */
    bool isMoving() const { return _isMoving; }
    
    // ==================== 属性设置方法 ====================
    
    /**
     * @brief 设置卡牌ID
     * @param cardId 新的卡牌唯一标识符
     * 
     * 更新卡牌的ID值，通常在初始化或重新分配时使用
     */
    void setCardId(int cardId) { _cardId = cardId; }
    
    /**
     * @brief 设置卡牌点数
     * @param face 新的卡牌点数值
     * 
     * 更改卡牌的点数，通常在游戏初始化时使用
     */
    void setFace(CardFaceType face) { _face = face; }
    
    /**
     * @brief 设置卡牌花色
     * @param suit 新的卡牌花色
     * 
     * 更改卡牌的花色，通常在游戏初始化时使用
     */
    void setSuit(CardSuitType suit) { _suit = suit; }
    
    /**
     * @brief 设置卡牌位置
     * @param position 新的位置坐标
     * 
     * 更新卡牌在游戏场景中的位置，通常用于卡牌移动或重排
     */
    void setPosition(const cocos2d::Vec2& position) { _position = position; }
    
    /**
     * @brief 设置卡牌原始位置
     * @param position 原始位置坐标
     * 
     * 保存卡牌的初始位置，用于撤销操作或重置
     */
    void setOriginalPosition(const cocos2d::Vec2& position) { _originalPosition = position; }
    
    /**
     * @brief 设置卡牌可见性
     * @param visible 可见性状态：true显示卡牌，false隐藏卡牌
     * 
     * 控制卡牌在界面上的显示状态，常用于动画效果或游戏逻辑
     */
    void setVisible(bool visible) { _isVisible = visible; }
    
    /**
     * @brief 设置卡牌移动状态
     * @param moving 移动状态：true表示正在移动，false表示静止
     * 
     * 用于标记卡牌的移动状态，防止移动过程中的重复操作
     */
    void setMoving(bool moving) { _isMoving = moving; }
    
    // ==================== 工具方法 ====================
    
    /**
     * @brief 检查两张卡牌是否可以匹配
     * @param other 要比较的另一张卡牌
     * @return true表示可以匹配（点数差值为1），false表示不可匹配
     * 
     * 游戏规则：两张卡牌的点数差值为1即可匹配，不限制花色
     * 例如：A可以和2匹配，5可以和4或6匹配，K可以和Q匹配
     */
    bool canMatch(const CardModel& other) const;
    
    /**
     * @brief 获取卡牌点数的数值
     * @return 卡牌点数对应的整数值
     * 
     * 将卡牌点数枚举转换为数值用于匹配计算：
     * A=1, 2-10=对应数值, J=11, Q=12, K=13
     */
    int getFaceValue() const;
    
    /**
     * @brief 克隆卡牌对象
     * @return 新创建的卡牌副本指针
     * 
     * 创建当前卡牌的完整副本，包含所有属性值
     * 主要用于撤销功能的状态保存
     */
    CardModel* clone() const;

private:
    // ==================== 私有成员变量 ====================
    int _cardId;                        // 卡牌唯一标识符
    CardFaceType _face;                 // 卡牌点数值
    CardSuitType _suit;                 // 卡牌花色
    cocos2d::Vec2 _position;            // 当前位置坐标
    cocos2d::Vec2 _originalPosition;    // 原始位置坐标（用于撤销）
    bool _isVisible;                    // 可见性状态
    bool _isMoving;                     // 移动状态标记
};

#endif // __CARD_MODEL_H__
