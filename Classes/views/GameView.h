/**
 * @file GameView.h
 * @brief 游戏主视图头文件
 * @author OUC-Zhou Tao
 * @date 2024
 * 
 * 游戏主视图类定义
 * 负责管理整个游戏的UI显示、用户交互、卡牌布局和视觉效果
 * 提供完整的MVC架构中的视图层实现
 */

#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include "../models/GameModel.h"
#include <map>
#include <functional>

/**
 * @class GameView
 * @brief 游戏主视图类
 * 
 * 功能概述：
 * - 管理整个游戏界面的显示和布局
 * - 处理用户的触摸输入和交互事件
 * - 控制卡牌视图的创建、更新和销毁
 * - 提供游戏状态的可视化反馈
 * - 支持撤销操作的视觉效果
 * - 管理分层背景和区域划分
 * 
 * 设计模式：
 * - 使用观察者模式响应模型变化
 * - 采用组合模式管理子视图
 * - 实现策略模式处理不同的交互
 */
class GameView : public cocos2d::Layer
{
public:
    // ==================== 构造与析构 ====================
    
    /**
     * @brief 默认构造函数
     * 初始化游戏视图的基本属性
     */
    GameView();
    
    /**
     * @brief 析构函数
     * 清理游戏视图占用的资源和监听器
     */
    virtual ~GameView();
    
    // ==================== 创建与初始化 ====================
    
    /**
     * @brief 创建游戏视图实例
     * @param gameModel 游戏数据模型指针（只读）
     * @return 成功创建的游戏视图实例，失败返回nullptr
     * 
     * 静态工厂方法，创建并初始化游戏视图
     * 自动调用initWithGameModel进行完整初始化
     */
    static GameView* create(const GameModel* gameModel);
    
    /**
     * @brief 初始化游戏视图
     * @param gameModel 游戏数据模型指针（只读）
     * @return true表示初始化成功，false表示初始化失败
     * 
     * 完成以下初始化工作：
     * - 创建背景层（上方黄色70%，下方紫色30%）
     * - 根据模型数据创建所有卡牌视图
     * - 设置触摸事件监听器
     * - 创建撤销按钮
     * - 建立视图与模型的映射关系
     */
    bool init(const GameModel* gameModel);
    
    // ==================== 显示更新方法 ====================
    
    /**
     * @brief 更新所有卡牌显示
     * @param gameModel 更新后的游戏数据模型指针（只读）
     * 
     * 根据模型数据的变化更新视图显示：
     * - 同步卡牌位置和可见性
     * - 更新卡牌层级关系
     * - 刷新撤销按钮状态
     * - 触发必要的动画效果
     */
    void updateDisplay(const GameModel* gameModel);
    
    /**
     * @brief 播放卡牌匹配动画
     * @param card1 第一张匹配的卡牌模型
     * @param card2 第二张匹配的卡牌模型
     * 
     * 为成功匹配的两张卡牌播放消除动画效果
     * 动画包括缩放、淡出和粒子特效
     */
    void playMatchAnimation(int cardId, const cocos2d::Vec2& targetPosition, 
                           const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 播放卡牌移动动画
     * @param cardId 要移动的卡牌唯一标识符
     * @param targetPosition 目标位置坐标
     * @param callback 动画完成后的回调函数（可选）
     * 
     * 播放平滑的卡牌移动动画，支持异步回调
     * 用于卡牌拖拽、自动排列等场景
     */
    void playMoveAnimation(int cardId, const cocos2d::Vec2& targetPosition, 
                           const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 播放撤销动画
     * @param cardId 要回退的卡牌唯一标识符
     * @param targetPosition 回退目标位置坐标
     * @param callback 动画完成后的回调函数（可选）
     * 
     * 播放撤销操作的动画效果，将卡牌平滑移动到之前的位置
     * 配合撤销系统使用，提供良好的视觉反馈
     */
    void playUndoAnimation(int cardId, const cocos2d::Vec2& targetPosition,
                          const std::function<void()>& callback = nullptr);
    
    // ==================== 事件回调设置 ====================
    
    /**
     * @brief 设置卡牌点击事件回调函数
     * @param callback 点击回调函数，接收被点击卡牌的ID作为参数
     * 
     * 注册卡牌点击事件的处理函数，当用户点击卡牌时触发
     * 通常连接到控制器的卡牌选择逻辑
     */
    void setOnCardClickCallback(const std::function<void(int)>& callback);
    
    /**
     * @brief 设置撤销按钮点击事件回调函数
     * @param callback 撤销按钮点击回调函数
     * 
     * 注册撤销按钮的点击处理函数，当用户点击撤销按钮时触发
     * 通常连接到控制器的撤销操作逻辑
     */
    void setOnUndoClickCallback(const std::function<void()>& callback);
    
    // ==================== 卡牌视图管理 ====================
    
    /**
     * @brief 添加卡牌视图到场景中
     * @param cardModel 卡牌数据模型指针
     * 
     * 根据卡牌模型创建对应的视图，并添加到游戏场景中
     * 自动设置卡牌的位置、层级和触摸响应
     */
    void addCardView(const CardModel* cardModel);
    
    /**
     * @brief 从场景中移除卡牌视图
     * @param cardId 要移除的卡牌唯一标识符
     * 
     * 移除指定ID的卡牌视图，清理相关资源和映射关系
     * 通常在卡牌匹配消除时调用
     */
    void removeCardView(int cardId);
    
    /**
     * @brief 根据ID获取卡牌视图
     * @param cardId 卡牌唯一标识符
     * @return 对应的卡牌视图指针，找不到时返回nullptr
     * 
     * 从内部映射表中查找指定ID的卡牌视图
     * 用于更新特定卡牌的显示状态
     */
    CardView* getCardView(int cardId) const;

private:
    // ==================== 私有初始化方法 ====================
    
    /**
     * @brief 创建用户界面元素
     * 
     * 初始化游戏界面的各个组件：
     * - 创建分层背景（上方黄色区域、下方紫色区域）
     * - 设置区域边界和布局参数
     * - 创建撤销按钮和其他UI控件
     */
    void createUI();
    
    /**
     * @brief 创建背景区域
     * 
     * 设置游戏的视觉背景：
     * - 上方70%区域设为黄色（主游戏区域）
     * - 下方30%区域设为紫色（托盘和备牌区域）
     * - 确保区域划分清晰明确
     */
    void createBackgroundAreas();
    
    /**
     * @brief 创建撤销按钮
     * 
     * 在界面右下角创建撤销按钮
     * 设置按钮的外观、位置和点击响应
     */
    void createUndoButton();
    
    /**
     * @brief 创建游戏区域容器
     * 
     * 初始化各个游戏区域的节点容器：
     * - 主游戏区域节点（上方黄色区域）
     * - 托盘区域节点（下方左侧）
     * - 备牌堆节点（下方右侧）
     */
    void createGameAreas();

private:
    // ==================== 私有成员变量 ====================
    
    const GameModel* _gameModel;                                // 游戏数据模型（只读引用）
    
    // 卡牌视图管理
    std::map<int, CardView*> _cardViews;                        // 卡牌ID到CardView指针的映射表
    int _currentTrayCardId;                                     // 当前托盘卡牌ID，用于跟踪托盘状态变化
    
    // UI组件节点
    cocos2d::Node* _playfieldNode;                              // 主游戏区域容器节点
    cocos2d::Node* _stackNode;                                  // 备牌堆容器节点
    cocos2d::Node* _trayNode;                                   // 托盘区域容器节点
    cocos2d::Menu* _undoButton;                                 // 撤销按钮菜单组件
    
    // 事件回调函数
    std::function<void(int)> _onCardClickCallback;              // 卡牌点击事件回调函数
    std::function<void()> _onUndoClickCallback;                 // 撤销按钮点击事件回调函数
    
    // 布局常量定义
    static const cocos2d::Vec2 kStackPosition;                  // 备牌堆的固定位置坐标
    static const cocos2d::Vec2 kTrayPosition;                   // 托盘区域的固定位置坐标
    static const cocos2d::Vec2 kUndoButtonPosition;             // 回退按钮位置
};

#endif // __GAME_VIEW_H__
