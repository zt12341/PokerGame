/**
 * @file UndoManager.h
 * @brief 撤销管理器头文件
 * @author OUC-Zhou Tao
 * @date 2024
 * 
 * 撤销管理器类定义
 * 实现游戏操作的撤销和重做功能
 * 采用命令模式记录和回放用户操作
 */

#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "cocos2d.h"
#include "../models/UndoModel.h"
#include "../models/GameModel.h"
#include <functional>

/**
 * @class UndoManager
 * @brief 撤销管理器类
 * 
 * 功能概述：
 * - 记录用户的游戏操作历史
 * - 提供撤销和重做功能
 * - 管理操作栈的存储和检索
 * - 支持批量操作的撤销
 * - 维护游戏状态的一致性
 * 
 * 设计模式：
 * - 命令模式：将每个操作封装为命令对象
 * - 备忘录模式：保存游戏状态的快照
 * - 单例模式：全局唯一的撤销管理实例
 * 
 * 使用场景：
 * - 作为GameController的成员变量使用
 * - 在每次卡牌操作后记录状态
 * - 响应用户的撤销请求
 */
class UndoManager
{
public:
    // ==================== 构造与析构 ====================
    
    /**
     * @brief 默认构造函数
     * 初始化撤销管理器的基本状态
     */
    UndoManager();
    
    /**
     * @brief 析构函数
     * 清理撤销历史和占用的内存资源
     */
    ~UndoManager();
    
    // ==================== 初始化方法 ====================
    
    /**
     * @brief 初始化撤销管理器
     * @param undoModel 撤销数据模型指针，用于存储操作历史
     * @param gameModel 游戏数据模型指针，用于执行状态恢复
     * 
     * 建立管理器与模型的关联关系，准备接收操作记录
     */
    void init(UndoModel* undoModel, GameModel* gameModel);
    
    // ==================== 操作记录方法 ====================
    
    /**
     * @brief 记录卡牌移动操作
     * @param cardId 移动卡牌的唯一标识符
     * @param fromPosition 卡牌的起始位置坐标
     * @param toPosition 卡牌的目标位置坐标
     * @param previousTrayCard 之前的底牌（如果有替换）
     */
    void recordMoveAction(int cardId, const cocos2d::Vec2& fromPosition, const cocos2d::Vec2& toPosition,
                         std::shared_ptr<CardModel> previousTrayCard = nullptr);
    
    /**
     * 记录手牌堆到底牌的操作
     * @param cardId 移动的卡牌ID
     * @param previousTrayCard 之前的底牌
     */
    void recordStackToTrayAction(int cardId, std::shared_ptr<CardModel> previousTrayCard);
    
    /**
     * 执行撤销操作
     * @param onAnimationComplete 动画完成回调
     * @return 是否成功执行撤销
     */
    bool executeUndo(const std::function<void()>& onAnimationComplete = nullptr);
    
    /**
     * 检查是否可以撤销
     * @return true表示有操作可以撤销
     */
    bool canUndo() const;
    
    /**
     * 清空所有撤销记录
     */
    void clearUndoHistory();
    
    /**
     * 设置最大撤销步数
     * @param maxSteps 最大步数，0表示无限制
     */
    void setMaxUndoSteps(size_t maxSteps);
    
    /**
     * 获取当前撤销步数
     * @return 撤销步数
     */
    size_t getUndoCount() const;
    
    /**
     * 设置撤销动画回调
     * @param callback 动画回调函数，参数为(cardId, targetPosition, animationCallback)
     */
    void setUndoAnimationCallback(const std::function<void(int, const cocos2d::Vec2&, std::function<void()>)>& callback);

private:
    /**
     * 撤销移动卡牌操作
     * @param action 撤销操作记录
     * @param onComplete 完成回调
     */
    void undoMoveAction(std::shared_ptr<UndoAction> action, const std::function<void()>& onComplete);
    
    /**
     * 撤销替换底牌操作
     * @param action 撤销操作记录
     * @param onComplete 完成回调
     */
    void undoReplaceTrayAction(std::shared_ptr<UndoAction> action, const std::function<void()>& onComplete);
    
    /**
     * 撤销手牌堆到底牌操作
     * @param action 撤销操作记录
     * @param onComplete 完成回调
     */
    void undoStackToTrayAction(std::shared_ptr<UndoAction> action, const std::function<void()>& onComplete);

private:
    UndoModel* _undoModel;                  // 撤销数据模型
    GameModel* _gameModel;                  // 游戏数据模型
    
    // 动画回调
    std::function<void(int, const cocos2d::Vec2&, std::function<void()>)> _undoAnimationCallback;
};

#endif // __UNDO_MANAGER_H__
