/**
 * @file GameController.h
 * @brief 游戏控制器头文件
 * @author OUC-Zhou Tao
 * @date 2024
 * 
 * 游戏控制器类定义
 * 作为MVC架构中的控制器层，协调模型和视图之间的交互
 * 处理游戏逻辑、用户输入和数据更新
 */

#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../models/UndoModel.h"
#include "../views/GameView.h"
#include "../managers/UndoManager.h"
#include <memory>

/**
 * @class GameController
 * @brief 游戏控制器类
 * 
 * 功能概述：
 * - 协调游戏模型和视图之间的数据流转
 * - 处理用户输入事件并转换为游戏逻辑
 * - 管理游戏状态转换和生命周期
 * - 控制撤销系统和历史记录
 * - 实现游戏规则和匹配逻辑
 * - 提供关卡管理和进度控制
 * 
 * 设计模式：
 * - MVC控制器模式：分离业务逻辑和表示层
 * - 命令模式：支持撤销和重做操作
 * - 观察者模式：响应模型变化通知视图更新
 */
class GameController
{
public:
    // ==================== 构造与析构 ====================
    
    /**
     * @brief 默认构造函数
     * 初始化游戏控制器的基本状态
     */
    GameController();
    
    /**
     * @brief 析构函数
     * 清理控制器占用的资源和内存
     */
    ~GameController();
    
    // ==================== 初始化方法 ====================
    
    /**
     * @brief 初始化游戏控制器
     * @param parentNode 父节点，用于添加视图组件
     * @return true表示初始化成功，false表示初始化失败
     * 
     * 完成以下初始化工作：
     * - 创建游戏模型和视图实例
     * - 建立控制器与视图的事件绑定
     * - 初始化撤销管理器
     * - 设置游戏的初始状态
     */
    bool init(cocos2d::Node* parentNode);
    
    // ==================== 游戏管理方法 ====================
    
    /**
     * @brief 启动指定关卡的游戏
     * @param levelId 关卡唯一标识符（默认为第1关）
     * @return true表示游戏启动成功，false表示启动失败
     * 
     * 执行以下操作：
     * - 加载指定关卡的配置数据
     * - 初始化卡牌布局和位置
     * - 重置游戏状态和计分
     * - 更新视图显示
     */
    bool startGame(int levelId = 1);
    
    // ==================== 事件处理方法 ====================
    
    /**
     * @brief 处理卡牌点击事件
     * @param cardId 被点击卡牌的唯一标识符
     * @return true表示处理成功，false表示处理失败
     * 
     * 根据游戏状态处理卡牌点击：
     * - 检查卡牌是否可以被选中
     * - 执行卡牌匹配逻辑判断
     * - 处理匹配成功的卡牌消除
     * - 更新游戏状态和视图显示
     * - 保存操作到撤销历史
     */
    bool handleCardClick(int cardId);
    
    /**
     * @brief 处理撤销按钮点击事件
     * @return true表示撤销成功，false表示撤销失败
     * 
     * 执行撤销操作：
     * - 检查是否有可撤销的操作历史
     * - 恢复上一步操作前的游戏状态
     * - 更新模型数据和视图显示
     * - 播放相应的撤销动画效果
     */
    bool handleUndoClick();
    
    // ==================== 游戏状态控制 ====================
    
    /**
     * @brief 停止当前游戏
     * 
     * 清理游戏资源，重置所有状态
     * 通常在退出游戏或切换关卡时调用
     */
    void stopGame();
    
    /**
     * @brief 暂停游戏
     * 
     * 暂停游戏逻辑和动画更新
     * 保持当前状态，可随时恢复
     */
    void pauseGame();
    
    /**
     * @brief 恢复游戏
     * 
     * 从暂停状态恢复游戏运行
     */
    void resumeGame();
    
    /**
     * Check if game is over
     * @return true if game is over
     */
    bool isGameOver() const;
    
    /**
     * Get current score
     * @return Current score
     */
    int getCurrentScore() const;

private:
    /**
     * Handle playfield card click
     * @param cardId Card ID
     * @return Whether processing was successful
     */
    bool handlePlayfieldCardClick(int cardId);
    
    /**
     * Handle stack card click
     * @param cardId Card ID
     * @return Whether processing was successful
     */
    bool handleStackCardClick(int cardId);
    
    /**
     * Execute card matching logic
     * @param cardId Card ID to match
     * @return Whether matching was successful
     */
    bool executeCardMatch(int cardId);
    
    /**
     * Execute stack card flip logic
     * @param cardId Card ID in stack
     * @return Whether flipping was successful
     */
    bool executeStackCardReplace(int cardId);
    
    /**
     * Update game view
     */
    void updateGameView();
    
    /**
     * Check win condition
     * @return true if won
     */
    bool checkWinCondition() const;

private:
    // 数据模型
    std::unique_ptr<GameModel> _gameModel;          // 游戏数据模型
    std::unique_ptr<UndoModel> _undoModel;          // 撤销数据模型
    
    // 视图组件
    GameView* _gameView;                            // 游戏视图
    cocos2d::Node* _parentNode;                     // 父节点
    
    // 管理器
    std::unique_ptr<UndoManager> _undoManager;      // 撤销管理器
    
    // 游戏状态
    bool _isGameActive;                             // 游戏是否激活
    bool _isProcessingAction;                       // 是否正在处理操作（防止双击）
};

#endif // __GAME_CONTROLLER_H__
