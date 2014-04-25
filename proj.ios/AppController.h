#import <UIKit/UIKit.h>
#import <GameKit/GameKit.h>
#import "GADBannerView.h"
#import "GADInterstitial.h"

#import "cocos2d.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, GKLeaderboardViewControllerDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
    int highScore;
    cocos2d::CCObject *m_pListener;
    cocos2d::SEL_MenuHandler m_pfnRateSelector;
    cocos2d::SEL_MenuHandler m_pfnLaterSelector;
    cocos2d::SEL_MenuHandler m_pfnDeclineSelector;
    bool m_showingRate;
    bool m_toShowLeaderboard;
}

@property(nonatomic, strong) GADBannerView *adBanner;
@property(nonatomic, strong) GADBannerView *adMRect;
@property(nonatomic, strong) GADInterstitial *adInterstitial;

- (int)getAdMobBannerHeight;
- (int)getAdMobMRectLeft;
- (void)onMoreClicked;
- (void)setAdMobBannerPosition:(bool)top;
- (void)showAdMobBanner:(bool)show;
- (void)showAdMobMRect:(bool)show;
- (void)showAdMobInterstitial;
- (void)showInterstitial;
- (void)showRateAlert:(cocos2d::CCObject *)object rateCallback:(cocos2d::SEL_MenuHandler)rateCallback laterCallback:(cocos2d::SEL_MenuHandler)laterCallback declineCallback:(cocos2d::SEL_MenuHandler)declineCallback;
- (void)submitScore:(int)score;
- (void)showLeaderboard;

+ (AppController *)getCurrentInstance;
@end

