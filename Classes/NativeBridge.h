#ifndef _NATIVE_BRIDGE_H
#define _NATIVE_BRIDGE_H

#include "cocos2d.h"

class NativeBridge
{
public:
    static int getAdMobBannerHeight();
    static int getAdMobMRectLeft();
    static void setAdMobBannerPosition(bool top);
    static void showAdMobBanner(bool show);
    static void onMoreClicked();
    static void showAdMobMRect(bool show);
    static void showInterstitial();
    static void showAdMobInterstitial();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    static void rateApp();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    static void showRateAlert(cocos2d::CCObject *target, cocos2d::SEL_MenuHandler rateSelector, cocos2d::SEL_MenuHandler laterSelector, cocos2d::SEL_MenuHandler declineSelector);
#endif
    static void submitScore(int score);
    static void showLeaderboard();
};

#endif /*_LAYOUT_MANAGER_H*/
