/**
 * @file CardTypes.h
 * @brief 卡牌类型定义头文件
 * @author OUC-Zhou Tao
 * @date 2024
 * 
 * 定义游戏中使用的卡牌类型枚举
 * 包含花色类型和点数类型的完整定义
 * 为整个卡牌系统提供类型安全保障
 */

#ifndef __CARD_TYPES_H__
#define __CARD_TYPES_H__

/**
 * @enum CardSuitType
 * @brief 卡牌花色类型枚举
 * 
 * 定义标准扑克牌的四种花色
 * 按照国际通用顺序排列
 */
enum CardSuitType
{
    CST_NONE = -1,                  /**< 无效花色，用于错误处理 */
    CST_CLUBS,                      /**< 草花 ♣ */
    CST_DIAMONDS,                   /**< 方块 ♦ */
    CST_HEARTS,                     /**< 红心 ♥ */
    CST_SPADES,                     /**< 黑桃 ♠ */
    CST_NUM_CARD_SUIT_TYPES         /**< 花色类型总数，用于数组大小和循环边界 */
};

/**
 * @enum CardFaceType
 * @brief 卡牌点数类型枚举
 * 
 * 定义标准扑克牌的13种点数
 * 按照A、2-10、J、Q、K的顺序排列
 * 数值可用于匹配逻辑计算（A=1, 2=2, ..., K=13）
 */
enum CardFaceType
{
    CFT_NONE = -1,                  /**< 无效点数，用于错误处理 */
    CFT_ACE,                        /**< A，数值为1 */
    CFT_TWO,                        /**< 2，数值为2 */
    CFT_THREE,                      /**< 3，数值为3 */
    CFT_FOUR,                       /**< 4，数值为4 */
    CFT_FIVE,                       /**< 5，数值为5 */
    CFT_SIX,                        /**< 6，数值为6 */
    CFT_SEVEN,                      /**< 7，数值为7 */
    CFT_EIGHT,                      /**< 8，数值为8 */
    CFT_NINE,                       /**< 9，数值为9 */
    CFT_TEN,                        /**< 10，数值为10 */
    CFT_JACK,                       /**< J，数值为11 */
    CFT_QUEEN,                      /**< Q，数值为12 */
    CFT_KING,                       /**< K，数值为13 */
    CFT_NUM_CARD_FACE_TYPES         /**< 点数类型总数，用于数组大小和循环边界 */
};

#endif // __CARD_TYPES_H__
