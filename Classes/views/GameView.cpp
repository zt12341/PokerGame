#include "GameView.h"

USING_NS_CC;

// 布局常量定义 - 底牌区域整体右移
const Vec2 GameView::kStackPosition = Vec2(250, 400);  // 底牌备用牌区 - 右移
const Vec2 GameView::kTrayPosition = Vec2(550, 400);   // 底牌区 - 右移
const Vec2 GameView::kUndoButtonPosition = Vec2(600, 300);

GameView::GameView()
    : _gameModel(nullptr)
    , _playfieldNode(nullptr)
    , _stackNode(nullptr)
    , _trayNode(nullptr)
    , _undoButton(nullptr)
    , _currentTrayCardId(-1)
{
}

GameView::~GameView()
{
    _cardViews.clear();
}

GameView* GameView::create(const GameModel* gameModel)
{
    GameView* ret = new GameView();
    if (ret && ret->init(gameModel))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GameView::init(const GameModel* gameModel)
{
    if (!Layer::init())
        return false;
    
    if (!gameModel)
        return false;
    
    _gameModel = gameModel;
    
    createUI();
    updateDisplay(gameModel);
    
    return true;
}

void GameView::createUI()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    createBackgroundAreas();
    createGameAreas();
    createUndoButton();
}

void GameView::createBackgroundAreas()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 计算背景区域尺寸，上方70%，下方30%
    float totalHeight = visibleSize.height;
    float upperHeight = totalHeight * 0.7f;
    float lowerHeight = totalHeight * 0.3f;
    
    // 创建上方手牌区域背景（黄色）
    auto upperBackground = LayerColor::create(Color4B(255, 255, 150, 255)); // 淡黄色
    upperBackground->setContentSize(Size(visibleSize.width, upperHeight));
    upperBackground->setPosition(Vec2(origin.x, origin.y + lowerHeight));
    this->addChild(upperBackground, -2);
    
    // 创建下方底牌区域背景（紫色）
    auto lowerBackground = LayerColor::create(Color4B(200, 150, 255, 255)); // 淡紫色
    lowerBackground->setContentSize(Size(visibleSize.width, lowerHeight));
    lowerBackground->setPosition(Vec2(origin.x, origin.y));
    this->addChild(lowerBackground, -2);
    
    // 创建分隔线
    auto separatorLine = DrawNode::create();
    Vec2 lineStart = Vec2(origin.x, origin.y + lowerHeight);
    Vec2 lineEnd = Vec2(origin.x + visibleSize.width, origin.y + lowerHeight);
    separatorLine->drawLine(lineStart, lineEnd, Color4F(0, 0, 0, 1)); // 黑色分割线
    this->addChild(separatorLine, -1);
}

void GameView::createGameAreas()
{
    // 创建游戏区域节点
    _playfieldNode = Node::create();
    this->addChild(_playfieldNode);
    
    // 创建手牌堆节点
    _stackNode = Node::create();
    _stackNode->setPosition(kStackPosition);
    this->addChild(_stackNode);
    
    // 创建底牌节点
    _trayNode = Node::create();
    _trayNode->setPosition(kTrayPosition);
    this->addChild(_trayNode);
}

void GameView::createUndoButton()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 尝试使用系统字体创建标签
    auto undoLabel = Label::createWithSystemFont("回退", "Arial", 64);
    if (!undoLabel) {
        // 如果系统字体失败，使用ASCII文本
        undoLabel = Label::createWithSystemFont("UNDO", "Arial", 64);
    }
    
    undoLabel->setColor(Color3B::WHITE);
    undoLabel->enableOutline(Color4B::BLACK, 2);
    
    auto undoMenuItem = MenuItemLabel::create(undoLabel, [this](Ref* sender) {
        if (_onUndoClickCallback)
            _onUndoClickCallback();
    });
    
    _undoButton = Menu::create(undoMenuItem, nullptr);
    
    // 计算底牌区域的高度（30%区域）并将按钮放在底牌区域右侧
    float lowerAreaHeight = visibleSize.height * 0.3f;
    float buttonY = origin.y + lowerAreaHeight * 0.5f; // 底牌区域中央高度
    float buttonX = origin.x + visibleSize.width * 0.85f; // 右侧位置
    
    Vec2 buttonPos = Vec2(buttonX, buttonY);
    _undoButton->setPosition(buttonPos);
    this->addChild(_undoButton, 10);
}

void GameView::updateDisplay(const GameModel* gameModel)
{
    if (!gameModel)
        return;
    
    _gameModel = gameModel;
    
    // 清除所有现有卡牌视图，强制重新创建
    for (auto& pair : _cardViews)
    {
        pair.second->removeFromParent();
    }
    _cardViews.clear();
    _currentTrayCardId = -1;
    
    // 重新创建游戏区域卡牌
    const auto& playfieldCards = gameModel->getPlayfieldCards();
    for (const auto& card : playfieldCards)
    {
        addCardView(card.get());
    }
    
    // 重新创建手牌堆卡牌
    const auto& stackCards = gameModel->getStackCards();
    for (const auto& card : stackCards)
    {
        addCardView(card.get());
    }
    
    // 重新创建底牌
    auto trayCard = gameModel->getTrayCard();
    if (trayCard)
    {
        addCardView(trayCard.get());
        _currentTrayCardId = trayCard->getCardId();
    }
}

void GameView::addCardView(const CardModel* cardModel)
{
    if (!cardModel || _cardViews.find(cardModel->getCardId()) != _cardViews.end())
        return;
    
    CardView* cardView = CardView::create(cardModel);
    if (cardView)
    {
        cardView->setOnClickCallback(_onCardClickCallback);
        
        // 检查是否是底牌，如果是底牌则设置更高的z-order
        if (_gameModel && _gameModel->getTrayCard() && _gameModel->getTrayCard()->getCardId() == cardModel->getCardId())
        {
            _playfieldNode->addChild(cardView, 5); // 底牌使用更高的层级
        }
        else
        {
            _playfieldNode->addChild(cardView, 1); // 普通卡牌使用较低层级
        }
        
        _cardViews[cardModel->getCardId()] = cardView;
    }
}

void GameView::removeCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        it->second->removeFromParent();
        _cardViews.erase(it);
    }
}

CardView* GameView::getCardView(int cardId) const
{
    auto it = _cardViews.find(cardId);
    return (it != _cardViews.end()) ? it->second : nullptr;
}

void GameView::playMatchAnimation(int cardId, const Vec2& targetPosition, const std::function<void()>& callback)
{
    CardView* cardView = getCardView(cardId);
    if (cardView)
    {
        cardView->playMoveAnimation(targetPosition, 0.3f, callback);
    }
}

void GameView::setOnCardClickCallback(const std::function<void(int)>& callback)
{
    _onCardClickCallback = callback;
    
    // 更新所有现有卡牌视图的回调
    for (auto& pair : _cardViews)
    {
        pair.second->setOnClickCallback(callback);
    }
}

void GameView::setOnUndoClickCallback(const std::function<void()>& callback)
{
    _onUndoClickCallback = callback;
}

void GameView::playMoveAnimation(int cardId, const cocos2d::Vec2& targetPosition, 
                                const std::function<void()>& callback)
{
    CardView* cardView = getCardView(cardId);
    if (cardView)
    {
        cardView->playMoveAnimation(targetPosition, 0.3f, callback);
    }
}

void GameView::playUndoAnimation(int cardId, const cocos2d::Vec2& targetPosition,
                                const std::function<void()>& callback)
{
    CardView* cardView = getCardView(cardId);
    if (cardView)
    {
        cardView->playMoveAnimation(targetPosition, 0.3f, callback);
    }
}
