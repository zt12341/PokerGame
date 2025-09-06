#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>
#include <memory>

/**
 * 回退操作类型
 */
enum UndoActionType
{
    UAT_MOVE_CARD,          // 移动卡牌
    UAT_REPLACE_TRAY,       // 替换底牌
    UAT_STACK_TO_TRAY       // 手牌堆到底牌
};

/**
 * 单个回退操作记录
 */
struct UndoAction
{
    UndoActionType actionType;                  // 操作类型
    int cardId;                                 // 操作的卡牌ID
    cocos2d::Vec2 fromPosition;                 // 起始位置
    cocos2d::Vec2 toPosition;                   // 目标位置
    std::shared_ptr<CardModel> previousTrayCard; // 之前的底牌（用于恢复）
    
    UndoAction(UndoActionType type, int id, const cocos2d::Vec2& from, const cocos2d::Vec2& to)
        : actionType(type), cardId(id), fromPosition(from), toPosition(to) {}
};

/**
 * 回退数据模型
 * 管理游戏的撤销操作历史记录
 */
class UndoModel
{
public:
    UndoModel();
    ~UndoModel();
    
    /**
     * 添加撤销操作记录
     * @param action 操作记录
     */
    void addUndoAction(const UndoAction& action);
    
    /**
     * 获取最近的撤销操作
     * @return 最近的操作，如果无操作则返回nullptr
     */
    std::shared_ptr<UndoAction> getLastUndoAction();
    
    /**
     * 移除最近的撤销操作
     */
    void removeLastUndoAction();
    
    /**
     * 检查是否有可撤销的操作
     * @return true表示有操作可以撤销
     */
    bool hasUndoActions() const;
    
    /**
     * 获取撤销栈的大小
     * @return 撤销操作的数量
     */
    size_t getUndoCount() const;
    
    /**
     * 清空所有撤销操作
     */
    void clear();
    
    /**
     * 设置最大撤销步数
     * @param maxSteps 最大步数，0表示无限制
     */
    void setMaxUndoSteps(size_t maxSteps);

private:
    std::vector<std::shared_ptr<UndoAction>> _undoActions;  // 撤销操作栈
    size_t _maxUndoSteps;                                   // 最大撤销步数限制
};

#endif // __UNDO_MODEL_H__
