#include "UndoManager.h"

USING_NS_CC;

UndoManager::UndoManager()
    : _undoModel(nullptr)
    , _gameModel(nullptr)
{
}

UndoManager::~UndoManager()
{
}

void UndoManager::init(UndoModel* undoModel, GameModel* gameModel)
{
    _undoModel = undoModel;
    _gameModel = gameModel;
}

void UndoManager::recordMoveAction(int cardId, const Vec2& fromPosition, const Vec2& toPosition,
                                  std::shared_ptr<CardModel> previousTrayCard)
{
    if (!_undoModel)
        return;
    
    UndoAction action(UAT_MOVE_CARD, cardId, fromPosition, toPosition);
    action.previousTrayCard = previousTrayCard;
    
    _undoModel->addUndoAction(action);
}

void UndoManager::recordStackToTrayAction(int cardId, std::shared_ptr<CardModel> previousTrayCard)
{
    if (!_undoModel)
        return;
    
    UndoAction action(UAT_STACK_TO_TRAY, cardId, Vec2::ZERO, Vec2::ZERO);
    action.previousTrayCard = previousTrayCard;
    
    _undoModel->addUndoAction(action);
}

bool UndoManager::executeUndo(const std::function<void()>& onAnimationComplete)
{
    if (!canUndo())
        return false;
    
    auto lastAction = _undoModel->getLastUndoAction();
    if (!lastAction)
        return false;
    
    _undoModel->removeLastUndoAction();
    
    switch (lastAction->actionType)
    {
        case UAT_MOVE_CARD:
            undoMoveAction(lastAction, onAnimationComplete);
            break;
        case UAT_REPLACE_TRAY:
            undoReplaceTrayAction(lastAction, onAnimationComplete);
            break;
        case UAT_STACK_TO_TRAY:
            undoStackToTrayAction(lastAction, onAnimationComplete);
            break;
        default:
            return false;
    }
    
    return true;
}

void UndoManager::undoMoveAction(std::shared_ptr<UndoAction> action, const std::function<void()>& onComplete)
{
    if (!_gameModel)
        return;
    
    // 找到需要移动的卡牌（当前在底牌位置）
    auto card = _gameModel->getTrayCard();
    if (!card || card->getCardId() != action->cardId)
    {
        // 如果底牌不是要回退的卡牌，尝试从所有卡牌中查找
        card = _gameModel->findCard(action->cardId);
        if (!card)
            return;
    }
    
    // 将卡牌从底牌位置移回游戏场地
    card->setPosition(action->fromPosition);
    _gameModel->addPlayfieldCard(card); // 重新添加到游戏场地
    
    // 恢复之前的底牌
    if (action->previousTrayCard)
    {
        _gameModel->setTrayCard(action->previousTrayCard);
        action->previousTrayCard->setPosition(action->toPosition);
        action->previousTrayCard->setVisible(true); // 确保可见
    }
    else
    {
        _gameModel->setTrayCard(nullptr); // 如果之前没有底牌，清空底牌位置
    }
    
    // 播放撤销动画
    if (_undoAnimationCallback)
    {
        _undoAnimationCallback(action->cardId, action->fromPosition, onComplete);
    }
    else if (onComplete)
    {
        onComplete();
    }
}

void UndoManager::undoReplaceTrayAction(std::shared_ptr<UndoAction> action, const std::function<void()>& onComplete)
{
    if (!_gameModel)
        return;
    
    // 恢复之前的底牌
    if (action->previousTrayCard)
    {
        _gameModel->setTrayCard(action->previousTrayCard);
        action->previousTrayCard->setPosition(action->toPosition);
    }
    
    // 将当前卡牌移回原位置
    auto card = _gameModel->findCard(action->cardId);
    if (card)
    {
        card->setPosition(action->fromPosition);
        
        // 播放撤销动画
        if (_undoAnimationCallback)
        {
            _undoAnimationCallback(action->cardId, action->fromPosition, onComplete);
        }
        else if (onComplete)
        {
            onComplete();
        }
    }
    else if (onComplete)
    {
        onComplete();
    }
}

void UndoManager::undoStackToTrayAction(std::shared_ptr<UndoAction> action, const std::function<void()>& onComplete)
{
    if (!_gameModel)
        return;
    
    // 将当前底牌放回手牌堆
    auto currentTrayCard = _gameModel->getTrayCard();
    if (currentTrayCard && currentTrayCard->getCardId() == action->cardId)
    {
        // 设置正确的手牌堆位置（右移后的位置）
        currentTrayCard->setPosition(Vec2(250, 400)); // 右移后的备用牌区位置
        currentTrayCard->setVisible(true); // 确保可见
        _gameModel->addStackCard(currentTrayCard);
    }
    
    // 恢复之前的底牌
    if (action->previousTrayCard)
    {
        action->previousTrayCard->setPosition(Vec2(550, 400)); // 右移后的底牌位置
        action->previousTrayCard->setVisible(true); // 确保可见
        _gameModel->setTrayCard(action->previousTrayCard);
    }
    else
    {
        _gameModel->setTrayCard(nullptr);
    }
    
    if (onComplete)
    {
        onComplete();
    }
}

bool UndoManager::canUndo() const
{
    return _undoModel && _undoModel->hasUndoActions();
}

void UndoManager::clearUndoHistory()
{
    if (_undoModel)
    {
        _undoModel->clear();
    }
}

void UndoManager::setMaxUndoSteps(size_t maxSteps)
{
    if (_undoModel)
    {
        _undoModel->setMaxUndoSteps(maxSteps);
    }
}

size_t UndoManager::getUndoCount() const
{
    return _undoModel ? _undoModel->getUndoCount() : 0;
}

void UndoManager::setUndoAnimationCallback(const std::function<void(int, const Vec2&, std::function<void()>)>& callback)
{
    _undoAnimationCallback = callback;
}
