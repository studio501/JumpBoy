#import "AppController.h"
#import "EAGLView.h"
#import "cocos2d.h"
#import "AppDelegate.h"
#import "RootViewController.h"
#import "Flurry.h"
#import "Chartboost.h"
#import "GADInterstitialDelegate.h"
#import "appirater/Appirater.h"

#define FLURRY_KEY @"PQSZ65X7JQZXGS87BRPY"
#define ADMOB_ID @"a1531edba247f36"
#define ADMOB_INTER_ID @"a1531edc2755f5e"
#define CHARTBOOST_KEY @"531edafaf8975c06c7041158"
#define CHARTBOOST_SIGNATURE @"bc491b045e08817438708a9a8ac6d4e65780e25b"
#define APP_ID @"838814215"
#define GAMECENTER_LEADERBOARD_CATEGORY @"com.coolgame.jumpyboyhighscore"

@interface AppController () <ChartboostDelegate, GADInterstitialDelegate, AppiraterDelegate>
@end

@implementation AppController

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;
static id instance;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    
    instance = self;
    
    m_showingRate = NO;
    m_toShowLeaderboard = NO;
    highScore = -1;

    [Flurry startSession:FLURRY_KEY];
    [Flurry logEvent:@"Start of Jumpy Boy"];
    
    // Override point for customization after application launch.
    
    // Add the view controller's view to the window and display.
    window = [[UIWindow alloc] initWithFrame: [[UIScreen mainScreen] bounds]];
    
    // Init the EAGLView
    EAGLView *__glView = [EAGLView viewWithFrame: [window bounds]
                                     pixelFormat: kEAGLColorFormatRGB565
                                     depthFormat: GL_DEPTH24_STENCIL8_OES
                              preserveBackbuffer: NO
                                      sharegroup: nil
                                   multiSampling: NO
                                 numberOfSamples: 0];
    
    // Use RootViewController manage EAGLView
    viewController = [[RootViewController alloc] initWithNibName:nil bundle:nil];
    viewController.wantsFullScreenLayout = YES;
    viewController.view = __glView;
    
    highScore = -1;
    
    self.adBanner = [[GADBannerView alloc] initWithAdSize:kGADAdSizeBanner];
    self.adBanner.adUnitID = ADMOB_ID;
    self.adBanner.rootViewController = viewController;
    {
        CGRect frame = self.adBanner.frame;
        // landscape mode
        frame.origin.x = (__glView.frame.size.height - self.adBanner.frame.size.width) / 2;
        frame.origin.x = MAX(frame.origin.x, __glView.frame.size.height * 0.25f);
        frame.origin.y = __glView.frame.origin.x + __glView.frame.size.width - frame.size.height;
        self.adBanner.frame = frame;
        GADRequest *request = [GADRequest request];
        [self.adBanner loadRequest:request];
    }
    [viewController.view addSubview:self.adBanner];
    
    self.adMRect = [[GADBannerView alloc] initWithAdSize:kGADAdSizeMediumRectangle];
    self.adMRect.adUnitID = ADMOB_ID;
    self.adMRect.rootViewController = viewController;
    self.adMRect.hidden = YES;
    {
        CGRect frame = self.adMRect.frame;
        float width = __glView.frame.size.height, height = __glView.frame.size.width;
        frame.origin.x = (width * 0.33f + 34);
        frame.size.width = MIN(frame.size.width, width * 0.67f - 30);
        frame.origin.y = (height - self.adMRect.frame.size.height) / 2;
        self.adMRect.frame = frame;
        GADRequest *request = [GADRequest request];
        [self.adMRect loadRequest:request];
    }
    [viewController.view addSubview:self.adMRect];
    
    Chartboost *cb = [Chartboost sharedChartboost];
    cb.appId = CHARTBOOST_KEY;
    cb.appSignature = CHARTBOOST_SIGNATURE;
    cb.delegate = self;
    [cb startSession];
    [cb showInterstitial];
    [cb cacheMoreApps];

    [Appirater setAppId:APP_ID];
    [Appirater setDebug:YES];
    [Appirater setDelegate:self];
    
    // Set RootViewController to window
    if ( [[UIDevice currentDevice].systemVersion floatValue] < 6.0)
    {
        // warning: addSubView doesn't work on iOS6
        [window addSubview: viewController.view];
    }
    else
    {
        // use this method on ios6
        [window setRootViewController:viewController];
    }
    
    [window makeKeyAndVisible];
    
    [[UIApplication sharedApplication] setStatusBarHidden:true];
    
    cocos2d::CCApplication::sharedApplication()->run();
    
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    /*
     Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
     Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
     */
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    /*
     Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     */
    
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    /*
     Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
     If your application supports background execution, called instead of applicationWillTerminate: when the user quits.
     */
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    /*
     Called as part of  transition from the background to the inactive state: here you can undo many of the changes made on entering the background.
     */
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
    [Appirater appEnteredForeground:m_showingRate];
}

- (void)applicationWillTerminate:(UIApplication *)application {
    /*
     Called when the application is about to terminate.
     See also applicationDidEnterBackground:.
     */
    
    [Flurry logEvent:@"Termination of Jumpy Boy"];
}


#pragma mark -
#pragma mark Memory management

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application {
    /*
     Free up as much memory as possible by purging cached data objects that can be recreated (or reloaded from disk) later.
     */
}

- (void)dealloc {
    [window release];
    [super dealloc];
}

- (int)getAdMobBannerHeight {
    return self.adBanner.frame.size.height * [[UIScreen mainScreen] scale];
}

- (int)getAdMobMRectLeft {
    return self.adMRect.frame.origin.x * [[UIScreen mainScreen] scale];
}

- (void)onMoreClicked {
    [[Chartboost sharedChartboost] showMoreApps];
}

- (void)setAdMobBannerPosition:(bool)top {
    CGRect frame = self.adBanner.frame;
    frame.origin.y = (top ? viewController.view.frame.origin.x : viewController.view.frame.origin.x + viewController.view.frame.size.width - frame.size.height);
    self.adBanner.frame = frame;
}

- (void)showAdMobBanner:(bool)show {
    [self.adBanner setHidden:(show ? NO : YES)];
}

- (void)showAdMobMRect:(bool)show {
    [self.adMRect setHidden:(show ? NO : YES)];
}

- (void)showAdMobInterstitial {
    self.adInterstitial = [[GADInterstitial alloc] init];
    self.adInterstitial.adUnitID = ADMOB_INTER_ID;
    self.adInterstitial.delegate = self;
    [self.adInterstitial loadRequest:[GADRequest request]];
}

- (void)interstitialDidReceiveAd:(GADInterstitial *)interstitial {
    [self.adInterstitial presentFromRootViewController:viewController];
}

- (void)interstitial:(GADInterstitial *)ad didFailToReceiveAdWithError:(GADRequestError *)error {
    [NSTimer scheduledTimerWithTimeInterval:3.0f target:self selector:@selector(showAdMobInterstitial) userInfo:nil repeats:NO];
}

- (void)showInterstitial {
    [[Chartboost sharedChartboost] showInterstitial];
}

- (void)showRateAlert:(cocos2d::CCObject *)object rateCallback:(cocos2d::SEL_MenuHandler)rateCallback laterCallback:(cocos2d::SEL_MenuHandler)laterCallback declineCallback:(cocos2d::SEL_MenuHandler)declineCallback {
    if (object && rateCallback && laterCallback && declineCallback) {
        m_pListener = object;
        m_pfnRateSelector = rateCallback;
        m_pfnLaterSelector = laterCallback;
        m_pfnDeclineSelector = declineCallback;
        [Appirater forceShowPrompt:YES];
    }
}

- (void)submitScore:(int)score {
    if (score > highScore) {
        highScore = score;
        if ([self isGameCenterAvailable] && [GKLocalPlayer localPlayer].authenticated) {
            [self submitHighScore];
        }
    }
}

- (void)showLeaderboard {
    if ([self isGameCenterAvailable]) {
        if ([GKLocalPlayer localPlayer].authenticated == NO) {
            m_toShowLeaderboard = YES;
            [self authenticateLocalUser];
        } else {
            GKLeaderboardViewController *leaderboardController = [[GKLeaderboardViewController alloc] init];
            if (leaderboardController != nil) {
                leaderboardController.leaderboardDelegate = self;
                
                [viewController presentModalViewController:leaderboardController animated:YES];
            }
        }
    }
}

+ (AppController *)getCurrentInstance {
    return (AppController *)instance;
}

/*
 * Chartboost Delegate Methods
 *
 * Recommended for everyone: shouldDisplayInterstitial
 */

- (BOOL)shouldRequestInterstitialsInFirstSession {
    return YES;
}

- (BOOL)shouldRequestInterstitial:(NSString *)location {
    return YES;
}

- (BOOL)shouldDisplayInterstitial:(NSString *)location {
    NSLog(@"about to display interstitial at location %@", location);
    return YES;
}

- (BOOL)shouldDisplayMoreApps {
    return YES;
}

- (BOOL)shouldDisplayLoadingViewForMoreApps {
    return YES;
}

- (void)didCloseInterstitial:(NSString *)location {
    
}

- (void)didFailToLoadInterstitial:(NSString *)location {
    NSLog(@"failure to load interstitial at location %@", location);
    
    // Show a house ad or do something else when a chartboost interstitial fails to load
}

/*
 * didCacheInterstitial
 *
 * Passes in the location name that has successfully been cached.
 *
 * Is fired on:
 * - All assets loaded
 * - Triggered by cacheInterstitial
 *
 * Notes:
 * - Similar to this is: cb.hasCachedInterstitial(String location)
 * Which will return true if a cached interstitial exists for that location
 */

- (void)didCacheInterstitial:(NSString *)location {
    NSLog(@"interstitial cached at location %@", location);
    
}

/*
 * didFailToLoadMoreApps
 *
 * This is called when the more apps page has failed to load for any reason
 *
 * Is fired on:
 * - No network connection
 * - No more apps page has been created (add a more apps page in the dashboard)
 * - No publishing campaign matches for that user (add more campaigns to your more apps page)
 *  -Find this inside the App > Edit page in the Chartboost dashboard
 */

- (void)didFailToLoadMoreApps {
    NSLog(@"failure to load more apps");
}


/*
 * didDismissInterstitial
 *
 * This is called when an interstitial is dismissed
 *
 * Is fired on:
 * - Interstitial click
 * - Interstitial close
 *
 * #Pro Tip: Use the delegate method below to immediately re-cache interstitials
 */

- (void)didDismissInterstitial:(NSString *)location {
    NSLog(@"dismissed interstitial at location %@", location);
    
    [[Chartboost sharedChartboost] cacheInterstitial:location];
}


/*
 * didDismissMoreApps
 *
 * This is called when the more apps page is dismissed
 *
 * Is fired on:
 * - More Apps click
 * - More Apps close
 *
 * #Pro Tip: Use the delegate method below to immediately re-cache the more apps page
 */

- (void)didDismissMoreApps {
    NSLog(@"dismissed more apps page, re-caching now");
    
    [[Chartboost sharedChartboost] cacheMoreApps];
}

- (bool)isGameCenterAvailable {
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    
    return (gcClass && osVersionSupported);
}

- (void) authenticateLocalUser {
    [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error) {
        if (error != nil) {
            UIAlertView *alert= [[[UIAlertView alloc] initWithTitle:@"Game Center Account Required" message:[NSString stringWithFormat:@"Reason: %@", [error localizedDescription]] delegate:self cancelButtonTitle:@"Close" otherButtonTitles:nil] autorelease];
            [alert show];
        } else {
            [self submitHighScore];
            if (m_toShowLeaderboard == YES)
            {
                m_toShowLeaderboard = NO;
                [self showLeaderboard];
            }
        }
    }];
}

- (void)submitHighScore {
    if (highScore >= 0) {
        GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:GAMECENTER_LEADERBOARD_CATEGORY] autorelease];
        if (scoreReporter != nil) {
            scoreReporter.value = highScore;
            [scoreReporter reportScoreWithCompletionHandler: ^(NSError *error) {
                if(error != nil) {
                    //处理错误
                }
            }];
        }
    }
}

- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)leaderboardViewController {
    [viewController dismissModalViewControllerAnimated:YES];
}

- (void)appiraterDidDisplayAlert:(Appirater *)appirater {
    m_showingRate = YES;
}

- (void)appiraterDidDeclineToRate:(Appirater *)appirater {
    if (m_pListener && m_pfnDeclineSelector) {
        (m_pListener->*m_pfnDeclineSelector)(NULL);
    }
    m_showingRate = NO;
}

- (void)appiraterDidOptToRate:(Appirater *)appirater {
    if (m_pListener && m_pfnRateSelector) {
        (m_pListener->*m_pfnRateSelector)(NULL);
    }
    m_showingRate = NO;
}

- (void)appiraterDidOptToRemindLater:(Appirater *)appirater {
    if (m_pListener && m_pfnLaterSelector) {
        (m_pListener->*m_pfnLaterSelector)(NULL);
    }
    m_showingRate = NO;
}
@end
