#include "UndoModel.h"

USING_NS_CC;

UndoModel::UndoModel()
    : _maxUndoSteps(0)  // 0表示无限制
{
}

UndoModel::~UndoModel()
{
    clear();
}

void UndoModel::addUndoAction(const UndoAction& action)
{
    auto undoAction = std::make_shared<UndoAction>(action);
    _undoActions.push_back(undoAction);
    
    // 如果超出最大步数限制，移除最早的操作
    if (_maxUndoSteps > 0 && _undoActions.size() > _maxUndoSteps)
    {
        _undoActions.erase(_undoActions.begin());
    }
}

std::shared_ptr<UndoAction> UndoModel::getLastUndoAction()
{
    if (_undoActions.empty())
        return nullptr;
    
    return _undoActions.back();
}

void UndoModel::removeLastUndoAction()
{
    if (!_undoActions.empty())
    {
        _undoActions.pop_back();
    }
}

bool UndoModel::hasUndoActions() const
{
    return !_undoActions.empty();
}

size_t UndoModel::getUndoCount() const
{
    return _undoActions.size();
}

void UndoModel::clear()
{
    _undoActions.clear();
}

void UndoModel::setMaxUndoSteps(size_t maxSteps)
{
    _maxUndoSteps = maxSteps;
    
    // 如果当前操作超过新的限制，移除多余的操作
    if (_maxUndoSteps > 0 && _undoActions.size() > _maxUndoSteps)
    {
        size_t removeCount = _undoActions.size() - _maxUndoSteps;
        _undoActions.erase(_undoActions.begin(), _undoActions.begin() + removeCount);
    }
}
