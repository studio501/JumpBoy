//
//  PlatformCallback.h
//  flypig
//
//  Created by fu xiaoling on 13-11-25.
//
//

#ifndef __IOS_PLATFORM_CALLBACK_H__
#define __IOS_PLATFORM_CALLBACK_H__

#include "cocos2d.h"

#ifdef __cplusplus
extern "C" {
#endif
    int ios_getAdMobBannerHeight();
    int ios_getAdMobMRectLeft();
    void ios_onMoreClicked();
    void ios_setAdMobBannerPosition(bool top);
    void ios_showAdMobBanner(bool show);
    void ios_showAdMobMRect(bool show);
    void ios_showAdMobInterstitial();
    void ios_showInterstitial();
    void ios_showRateAlert(cocos2d::CCObject *target, cocos2d::SEL_MenuHandler rateSelector, cocos2d::SEL_MenuHandler laterSelector, cocos2d::SEL_MenuHandler declineSelector);
    void ios_submitScore(int score);
    void ios_showLeaderboard();
#ifdef __cplusplus
} 
#endif

#endif
