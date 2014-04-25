//
//  PlatformCallback.c
//  flypig
//
//  Created by fu xiaoling on 13-11-25.
//
//

#import "IOSPlatformCallback.h"
#import "AppController.h"

int ios_getAdMobBannerHeight()
{
    return [[AppController getCurrentInstance] getAdMobBannerHeight];
}

int ios_getAdMobMRectLeft()
{
    return [[AppController getCurrentInstance] getAdMobMRectLeft];
}

void ios_onMoreClicked()
{
    [[AppController getCurrentInstance] onMoreClicked];
}

void ios_setAdMobBannerPosition(bool top)
{
    [[AppController getCurrentInstance] setAdMobBannerPosition:top];
}

void ios_showAdMobBanner(bool show)
{
    [[AppController getCurrentInstance] showAdMobBanner:show];
}

void ios_showAdMobMRect(bool show)
{
    
    [[AppController getCurrentInstance] showAdMobMRect:show];
}

void ios_showAdMobInterstitial()
{
    [[AppController getCurrentInstance] showAdMobInterstitial];
}

void ios_showInterstitial()
{
    [[AppController getCurrentInstance] showInterstitial];
}

void ios_showRateAlert(cocos2d::CCObject *target, cocos2d::SEL_MenuHandler rateSelector, cocos2d::SEL_MenuHandler laterSelector, cocos2d::SEL_MenuHandler declineSelector)
{
    [[AppController getCurrentInstance] showRateAlert:target rateCallback:rateSelector laterCallback:laterSelector declineCallback:declineSelector];
}

void ios_submitScore(int score)
{
    [[AppController getCurrentInstance] submitScore:score];
}

void ios_showLeaderboard()
{
    [[AppController getCurrentInstance] showLeaderboard];
}