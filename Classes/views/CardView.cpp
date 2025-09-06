#include "CardView.h"

USING_NS_CC;

CardView::CardView()
    : _cardModel(nullptr)
    , _cardId(-1)
    , _backgroundSprite(nullptr)
    , _bigNumberSprite(nullptr)
    , _smallNumberSprite(nullptr)
    , _suitSprite(nullptr)
    , _touchListener(nullptr)
    , _touchEnabled(true)
{
}

CardView::~CardView()
{
    if (_touchListener)
    {
        _eventDispatcher->removeEventListener(_touchListener);
        _touchListener = nullptr;
    }
}

CardView* CardView::create(const CardModel* cardModel)
{
    CardView* ret = new CardView();
    if (ret && ret->init(cardModel))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool CardView::init(const CardModel* cardModel)
{
    if (!Node::init())
        return false;
    
    if (!cardModel)
        return false;
    
    _cardModel = cardModel;
    _cardId = cardModel->getCardId();
    
    createCardUI();
    setupTouchListener();
    updateDisplay(cardModel);
    
    return true;
}

void CardView::createCardUI()
{
    Size cardSize = CardResConfig::getCardSize();
    
    // 设置锚点为中心，确保触摸检测正确
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    // 创建卡牌背景
    _backgroundSprite = Sprite::create(CardResConfig::getCardBackgroundPath());
    if (_backgroundSprite)
    {
        _backgroundSprite->setContentSize(cardSize);
        _backgroundSprite->setAnchorPoint(Vec2(0.5f, 0.5f)); // 背景也设置中心锚点
        _backgroundSprite->setPosition(Vec2(0, 0)); // 背景位置应该在节点中心，即(0,0)
        this->addChild(_backgroundSprite);
    }
    
    // 创建大数字精灵（中下部位置）- 相对于中心锚点
    _bigNumberSprite = Sprite::create();
    if (_bigNumberSprite)
    {
        _bigNumberSprite->setPosition(Vec2(0, -cardSize.height * 0.15f)); // 相对于中心向下偏移
        _bigNumberSprite->setScale(0.5f);
        this->addChild(_bigNumberSprite, 1);
    }
    
    // 创建小数字精灵（左上角）- 相对于中心锚点
    _smallNumberSprite = Sprite::create();
    if (_smallNumberSprite)
    {
        _smallNumberSprite->setPosition(Vec2(-cardSize.width * 0.35f, cardSize.height * 0.35f)); // 相对于中心向左上
        _smallNumberSprite->setScale(0.25f);
        this->addChild(_smallNumberSprite, 1);
    }
    
    // 创建花色精灵（右上角）- 相对于中心锚点
    _suitSprite = Sprite::create();
    if (_suitSprite)
    {
        _suitSprite->setPosition(Vec2(cardSize.width * 0.35f, cardSize.height * 0.35f)); // 相对于中心向右上
        _suitSprite->setScale(0.7f);
        this->addChild(_suitSprite, 1);
    }
    
    this->setContentSize(cardSize);
}

void CardView::setupTouchListener()
{
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);
    
    _touchListener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        return this->onTouchBegan(touch, event);
    };
    
    _touchListener->onTouchEnded = [this](Touch* touch, Event* event) {
        this->onTouchEnded(touch, event);
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);
}

bool CardView::onTouchBegan(Touch* touch, Event* event)
{
    if (!_touchEnabled || !_cardModel || !_cardModel->isVisible())
        return false;
    
    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
    Size size = this->getContentSize();
    // 使用以锚点为中心的边界框
    Rect rect = Rect(-size.width * 0.5f, -size.height * 0.5f, size.width, size.height);
    
    return rect.containsPoint(locationInNode);
}

void CardView::onTouchEnded(Touch* touch, Event* event)
{
    if (!_touchEnabled || !_cardModel || !_cardModel->isVisible())
        return;
    
    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
    Size size = this->getContentSize();
    // 使用以锚点为中心的边界框
    Rect rect = Rect(-size.width * 0.5f, -size.height * 0.5f, size.width, size.height);
    
    if (rect.containsPoint(locationInNode) && _onClickCallback)
    {
        _onClickCallback(_cardId);
    }
}

void CardView::updateDisplay(const CardModel* cardModel)
{
    if (!cardModel)
        return;
    
    _cardModel = cardModel;
    _cardId = cardModel->getCardId();
    
    // 更新位置
    this->setPosition(cardModel->getPosition());
    
    // 更新可见性
    this->setVisible(cardModel->isVisible());
    
    bool isRed = CardResConfig::isRedSuit(cardModel->getSuit());
    std::string numberPath = CardResConfig::getNumberImagePath(cardModel->getFace(), isRed);
    
    // 更新大数字显示（中下部）
    if (_bigNumberSprite)
    {
        _bigNumberSprite->setTexture(numberPath);
    }
    
    // 更新小数字显示（左上角）
    if (_smallNumberSprite)
    {
        _smallNumberSprite->setTexture(numberPath);
    }
    
    // 更新花色显示（右上角）
    if (_suitSprite)
    {
        std::string suitPath = CardResConfig::getSuitImagePath(cardModel->getSuit());
        _suitSprite->setTexture(suitPath);
    }
}

void CardView::playMoveAnimation(const Vec2& targetPosition, float duration, const std::function<void()>& callback)
{
    if (!_cardModel)
        return;
    
    auto moveAction = MoveTo::create(duration, targetPosition);
    
    if (callback)
    {
        auto sequence = Sequence::create(moveAction, CallFunc::create(callback), nullptr);
        this->runAction(sequence);
    }
    else
    {
        this->runAction(moveAction);
    }
}

void CardView::setOnClickCallback(const std::function<void(int)>& callback)
{
    _onClickCallback = callback;
}

void CardView::setTouchEnabled(bool enabled)
{
    _touchEnabled = enabled;
}
