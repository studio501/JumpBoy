#include "MainScene.h"
#include "GameScene.h"
#include "QuitScene.h"
#include "NativeBridge.h"

#include <string>
#include "LGMenuItemImage.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

CCScene *MainScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    MainScene *layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

MainScene *MainScene::create()
{
    MainScene *pRet = new MainScene();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    } 
    else 
    { 
        delete pRet;
        pRet = NULL;
        return NULL;
    } 
}

MainScene::MainScene()
    : m_Sound(false)
    , m_pSound(NULL)
    , m_pRecord(NULL)
    , m_pBoy(NULL)
{
}

MainScene::~MainScene()
{
    CC_SAFE_RELEASE_NULL(m_pSound);
    CC_SAFE_RELEASE_NULL(m_pRecord);
    CC_SAFE_RELEASE_NULL(m_pBoy);
}

bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    setKeypadEnabled(true);

    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visibleCenter = visibleOrigin + visibleSize / 2;

    CCSprite *sprite = CCSprite::create("bg1.jpg");
    sprite->setPosition(visibleCenter);
    addChild(sprite);

    sprite = CCSprite::create("title.png");
    sprite->setPosition(ccp(visibleCenter.x, visibleOrigin.y + 0.7f * visibleSize.height));
    addChild(sprite);

    m_Sound = CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true);
    CCUserDefault::sharedUserDefault()->setBoolForKey("sound", m_Sound);
    m_pSound = CCMenuItemImage::create("sound_off.png", "sound_on.png", this, menu_selector(MainScene::soundCallback));
    m_pSound->retain();
    m_pSound->setPosition(visibleOrigin.x + visibleSize.width * 0.065f, visibleOrigin.y + visibleSize.height * 0.9125f);
    if (m_Sound)
    {
        m_pSound->selected();
    }
    else
    {
        m_pSound->unselected();
    }

    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SFX_FADE);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SFX_IMPACT);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SFX_JUMP);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SFX_NEWITEM);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SFX_POINT);
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(SFX_RECORD);

    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("boys.plist");
    cache->addSpriteFramesWithFile("props.plist");

    m_pBoy = CCSprite::createWithSpriteFrameName("boy1");
    m_pBoy->retain();
    m_pBoy->setPosition(ccp(visibleOrigin.x + visibleSize.width * 0.1675f, visibleOrigin.y + 86 + m_pBoy->getContentSize().height / 2));
    addChild(m_pBoy);
    

    sprite = CCSprite::create("best.png");
    sprite->setPosition(ccp(visibleOrigin.x + visibleSize.width * 0.7125f, m_pSound->getPositionY()));
    addChild(sprite);

    m_pRecord = CCLabelAtlas::create(CCString::createWithFormat("%d", CCUserDefault::sharedUserDefault()->getIntegerForKey("record", 0))->getCString(), "fonts/font1.png", 42, 52, '0');
    m_pRecord->retain();
    m_pRecord->setZOrder(1);
    m_pRecord->setAnchorPoint(ccp(0.5, 0.5));
    m_pRecord->setPosition(visibleOrigin.x + visibleSize.width * 0.8975f, m_pSound->getPositionY());
    addChild(m_pRecord);

    CCSprite *hint = CCSprite::create("hint.png");
    hint->setPosition(ccp(visibleOrigin.x + visibleSize.width * 0.64f, visibleOrigin.y + visibleSize.height * 0.36875f));
    addChild(hint);

    CCMenuItemImage *pRank = LGMenuItemImage::create("rank.png", NULL, this, menu_selector(MainScene::rankCallback));
    pRank->setPosition(ccp(visibleOrigin.x + 0.5f * visibleSize.width, m_pSound->getPositionY()));

    CCMenuItemImage *pMore = LGMenuItemImage::create("more.png", NULL, this, menu_selector(MainScene::moreCallback));
    pMore->setScale(0.72f);
    pMore->setPosition(ccp(visibleOrigin.x + 0.075f * visibleSize.width, visibleOrigin.y + 0.1f * visibleSize.height));

    CCMenu *pMenu = CCMenu::create(m_pSound, pRank, pMore, NULL);
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu);

    return true;
}

void MainScene::soundCallback(CCObject *sender)
{
    m_Sound = !m_Sound;
    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }
    CCUserDefault::sharedUserDefault()->setBoolForKey("sound", m_Sound);
    if (m_Sound)
    {
        m_pSound->selected();
    }
    else
    {
        m_pSound->unselected();
    }
}

void MainScene::rankCallback(CCObject *sender)
{
    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }
    NativeBridge::showLeaderboard();
}

void MainScene::moreCallback(CCObject *sender)
{
    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }
    NativeBridge::onMoreClicked();
}

void MainScene::onEnter()
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);

    m_Sound = CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true);
    m_pRecord->setString(CCString::createWithFormat("%d", CCUserDefault::sharedUserDefault()->getIntegerForKey("record", 0))->getCString());

    m_pBoy->removeAllChildren();
    int propIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey("prop_index", -1);
    for (int i = 0; i <= propIndex; i++)
    {
        CCSprite *propFull = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("item%d.png", i + 1)->getCString());
        propFull->setPosition(ccp(m_pBoy->getContentSize().width / 2, m_pBoy->getContentSize().height / 2));
        m_pBoy->addChild(propFull);
    }

    CCLayer::onEnter();

    NativeBridge::setAdMobBannerPosition(false);
    //NativeBridge::showAdMobInterstitial();
}

void MainScene::onExit()
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCLayer::onExit();
}

void MainScene::keyBackClicked()
{
    CCLayer::keyBackClicked();
    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }
    CCDirector::sharedDirector()->pushScene(QuitScene::scene());
}

bool MainScene::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }
    CCDirector::sharedDirector()->pushScene(GameScene::scene());

    return true;
}