#include "GameController.h"
#include "../configs/loaders/LevelConfigLoader.h"
#include "../services/GameModelFromLevelGenerator.h"

USING_NS_CC;

GameController::GameController()
    : _gameView(nullptr)
    , _parentNode(nullptr)
    , _isGameActive(false)
    , _isProcessingAction(false)
{
}

GameController::~GameController()
{
    stopGame();
}

bool GameController::init(Node* parentNode)
{
    if (!parentNode)
        return false;
    
    _parentNode = parentNode;
    
    // 初始化数据模型
    _undoModel = std::make_unique<UndoModel>();
    _undoManager = std::make_unique<UndoManager>();
    
    return true;
}

bool GameController::startGame(int levelId)
{
    // 加载关卡配置
    LevelConfig* levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    if (!levelConfig)
    {
        CCLOG("Failed to load level config for level %d", levelId);
        return false;
    }
    
    // 生成游戏模型
    _gameModel.reset(GameModelFromLevelGenerator::generateGameModel(*levelConfig));
    delete levelConfig; // 释放临时配置对象
    
    if (!_gameModel)
    {
        CCLOG("Failed to generate game model");
        return false;
    }
    
    // 初始化撤销管理器
    _undoManager->init(_undoModel.get(), _gameModel.get());
    
    // 创建游戏视图
    if (_gameView)
    {
        _gameView->removeFromParent();
        _gameView = nullptr;
    }
    
    _gameView = GameView::create(_gameModel.get());
    if (!_gameView)
    {
        CCLOG("Failed to create game view");
        return false;
    }
    
    _parentNode->addChild(_gameView);
    
    // 设置回调函数
    _gameView->setOnCardClickCallback([this](int cardId) {
        this->handleCardClick(cardId);
    });
    
    _gameView->setOnUndoClickCallback([this]() {
        this->handleUndoClick();
    });
    
    // 设置撤销动画回调
    _undoManager->setUndoAnimationCallback([this](int cardId, const Vec2& targetPos, std::function<void()> callback) {
        _gameView->playUndoAnimation(cardId, targetPos, callback);
    });
    
    _isGameActive = true;
    _isProcessingAction = false;
    
    CCLOG("Game started successfully!");
    return true;
}

bool GameController::handleCardClick(int cardId)
{
    if (!_isGameActive || _isProcessingAction || !_gameModel)
        return false;
    
    _isProcessingAction = true;
    
    auto card = _gameModel->findCard(cardId);
    if (!card)
    {
        _isProcessingAction = false;
        return false;
    }
    
    bool success = false;
    
    // 检查卡牌是在游戏区还是牌堆中
    auto playfieldCard = _gameModel->getPlayfieldCard(cardId);
    if (playfieldCard)
    {
        // 游戏区卡牌被点击
        success = handlePlayfieldCardClick(cardId);
    }
    else
    {
        // 牌堆卡牌被点击
        success = handleStackCardClick(cardId);
    }
    
    if (success)
    {
        updateGameView();
        
        // 检查游戏结束条件
        if (checkWinCondition())
        {
            CCLOG("Congratulations! You won!");
            _isGameActive = false;
        }
    }
    
    _isProcessingAction = false;
    return success;
}

bool GameController::handlePlayfieldCardClick(int cardId)
{
    auto card = _gameModel->getPlayfieldCard(cardId);
    auto trayCard = _gameModel->getTrayCard();
    
    if (!card || !trayCard)
        return false;
    
    // 检查卡牌是否可以匹配
    if (!card->canMatch(*trayCard))
    {
        CCLOG("Card cannot match with tray card");
        return false;
    }
    
    return executeCardMatch(cardId);
}

bool GameController::handleStackCardClick(int cardId)
{
    // 只有最顶层的牌堆卡牌可以被点击
    auto topCard = _gameModel->getTopStackCard();
    if (!topCard || topCard->getCardId() != cardId)
    {
        CCLOG("Only top stack card can be clicked");
        return false;
    }
    
    return executeStackCardReplace(cardId);
}

bool GameController::executeCardMatch(int cardId)
{
    auto card = _gameModel->getPlayfieldCard(cardId);
    auto trayCard = _gameModel->getTrayCard();
    
    if (!card || !trayCard)
        return false;
    
    // 记录撤销操作
    Vec2 fromPos = card->getPosition();
    Vec2 toPos = trayCard->getPosition();
    auto previousTrayCard = std::make_shared<CardModel>(*trayCard); // 克隆当前托盘卡牌
    
    _undoManager->recordMoveAction(cardId, fromPos, toPos, previousTrayCard);
    
    // 将匹配的卡牌移动到托盘位置并替换
    card->setPosition(toPos);
    _gameModel->setTrayCard(card);
    
    // 从游戏区移除卡牌
    _gameModel->removePlayfieldCard(cardId);
    
    // 播放匹配动画
    _gameView->playMatchAnimation(cardId, toPos, [this]() {
        // 动画完成回调
        CCLOG("Match animation completed");
    });
    
    // 增加分数
    _gameModel->addScore(10);
    
    return true;
}

bool GameController::executeStackCardReplace(int cardId)
{
    auto stackCard = _gameModel->popStackCard();
    auto currentTrayCard = _gameModel->getTrayCard();
    
    if (!stackCard || stackCard->getCardId() != cardId)
        return false;
    
    // 记录撤销操作
    _undoManager->recordStackToTrayAction(cardId, currentTrayCard);
    
    // 将牌堆卡牌移动到托盘位置
    Vec2 trayPos = currentTrayCard ? currentTrayCard->getPosition() : Vec2(400, 300);
    stackCard->setPosition(trayPos);
    _gameModel->setTrayCard(stackCard);
    
    // 播放移动动画
    _gameView->playMatchAnimation(cardId, trayPos, [this]() {
        CCLOG("Stack to tray animation completed");
    });
    
    return true;
}

bool GameController::handleUndoClick()
{
    if (!_isGameActive || _isProcessingAction || !_undoManager)
        return false;
    
    if (!_undoManager->canUndo())
    {
        CCLOG("No actions to undo");
        return false;
    }
    
    _isProcessingAction = true;
    
    bool success = _undoManager->executeUndo([this]() {
        // 撤销动画完成回调
        CCLOG("Undo animation completed");
    });
    
    if (success)
    {
        // 立即更新视图，不等待动画完成
        updateGameView();
        _isProcessingAction = false; // 重置处理状态
    }
    else
    {
        _isProcessingAction = false; // 失败时也要重置状态
    }
    
    return success;
}

void GameController::updateGameView()
{
    if (_gameView && _gameModel)
    {
        _gameView->updateDisplay(_gameModel.get());
    }
}

bool GameController::checkWinCondition() const
{
    if (!_gameModel)
        return false;
    
    // 胜利条件：游戏区没有剩余卡牌
    return _gameModel->getPlayfieldCards().empty();
}

void GameController::stopGame()
{
    _isGameActive = false;
    _isProcessingAction = false;
    
    if (_gameView)
    {
        _gameView->removeFromParent();
        _gameView = nullptr;
    }
    
    _gameModel.reset();
    _undoModel.reset();
    _undoManager.reset();
}

void GameController::pauseGame()
{
    _isGameActive = false;
}

void GameController::resumeGame()
{
    _isGameActive = true;
}

bool GameController::isGameOver() const
{
    return !_isGameActive || checkWinCondition();
}

int GameController::getCurrentScore() const
{
    return _gameModel ? _gameModel->getScore() : 0;
}
