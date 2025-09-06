#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include "../models/CardModel.h"
#include "../configs/models/CardResConfig.h"
#include <functional>

/**
 * 卡牌视图类
 * 负责单张卡牌的显示和交互
 */
class CardView : public cocos2d::Node
{
public:
    CardView();
    virtual ~CardView();
    
    /**
     * 创建卡牌视图
     * @param cardModel 卡牌数据模型（只读）
     * @return 创建的卡牌视图
     */
    static CardView* create(const CardModel* cardModel);
    
    /**
     * 初始化卡牌视图
     * @param cardModel 卡牌数据模型（只读）
     * @return 初始化是否成功
     */
    bool init(const CardModel* cardModel);
    
    /**
     * 更新卡牌显示状态
     * @param cardModel 更新后的卡牌数据模型（只读）
     */
    void updateDisplay(const CardModel* cardModel);
    
    /**
     * 播放移动到指定位置的动画
     * @param targetPosition 目标位置
     * @param duration 动画持续时间
     * @param callback 动画完成回调
     */
    void playMoveAnimation(const cocos2d::Vec2& targetPosition, 
                          float duration = 0.3f, 
                          const std::function<void()>& callback = nullptr);
    
    /**
     * 设置卡牌点击回调
     * @param callback 点击回调函数，参数为卡牌ID
     */
    void setOnClickCallback(const std::function<void(int)>& callback);
    
    /**
     * 获取卡牌ID
     * @return 卡牌唯一标识
     */
    int getCardId() const { return _cardId; }
    
    /**
     * 设置卡牌是否可点击
     * @param enabled true表示可点击
     */
    void setTouchEnabled(bool enabled);

private:
    /**
     * 创建卡牌UI组件
     */
    void createCardUI();
    
    /**
     * 设置触摸事件监听
     */
    void setupTouchListener();
    
    /**
     * 处理触摸事件
     */
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
    const CardModel* _cardModel;                    // 卡牌数据模型（只读引用）
    int _cardId;                                    // 卡牌ID（缓存）
    
    // UI组件
    cocos2d::Sprite* _backgroundSprite;             // 背景精灵
    cocos2d::Sprite* _bigNumberSprite;              // 大数字精灵（中下部）
    cocos2d::Sprite* _smallNumberSprite;            // 小数字精灵（左上角）
    cocos2d::Sprite* _suitSprite;                   // 花色精灵（右上角）
    
    // 交互
    cocos2d::EventListenerTouchOneByOne* _touchListener;  // 触摸监听器
    std::function<void(int)> _onClickCallback;            // 点击回调
    bool _touchEnabled;                                   // 是否可点击
};

#endif // __CARD_VIEW_H__
