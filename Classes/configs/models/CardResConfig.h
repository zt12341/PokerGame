#ifndef __CARD_RES_CONFIG_H__
#define __CARD_RES_CONFIG_H__

#include "cocos2d.h"
#include "../../utils/CardTypes.h"

/**
 * 卡牌UI资源配置类
 * 提供卡牌资源路径映射和UI配置
 */
class CardResConfig
{
public:
    CardResConfig();
    ~CardResConfig();
    
    /**
     * 获取卡牌数字资源路径
     * @param face 卡牌点数
     * @param isRed 是否为红色（红桃/方块）
     * @param isBig 是否为大号字体
     * @return 资源文件路径
     */
    static std::string getNumberImagePath(CardFaceType face, bool isRed, bool isBig = true);
    
    /**
     * 获取花色资源路径
     * @param suit 花色类型
     * @return 花色图片路径
     */
    static std::string getSuitImagePath(CardSuitType suit);
    
    /**
     * 获取卡牌背景资源路径
     * @return 卡牌背景图片路径
     */
    static std::string getCardBackgroundPath();
    
    /**
     * 获取卡牌尺寸
     * @return 卡牌的标准尺寸
     */
    static cocos2d::Size getCardSize();
    
    /**
     * 判断花色是否为红色
     * @param suit 花色类型
     * @return true表示红色(红桃/方块), false表示黑色(黑桃/梅花)
     */
    static bool isRedSuit(CardSuitType suit);

private:
    static const cocos2d::Size kCardSize;
};

#endif // __CARD_RES_CONFIG_H__
