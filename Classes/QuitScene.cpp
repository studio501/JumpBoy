#include "QuitScene.h"

#include "NativeBridge.h"
#include "LGMenuItemImage.h"

#include "MainScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

CCScene *QuitScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    QuitScene *layer = QuitScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

QuitScene *QuitScene::create()
{
    QuitScene *pRet = new QuitScene();
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

QuitScene::QuitScene()
    : m_Sound(false)
{
}

QuitScene::~QuitScene()
{
}

bool QuitScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    m_Sound = CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true);

    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visibleCenter = visibleOrigin + visibleSize / 2;

    CCSprite *sprite = CCSprite::create("bg1.jpg");
    sprite->setPosition(visibleCenter);
    addChild(sprite);

    float yesX = visibleOrigin.x + visibleSize.width * 0.08f;
    CCMenuItemImage *pYes = LGMenuItemImage::create("quit_yes.png", NULL, this, menu_selector(QuitScene::yesCallback));
    pYes->setPosition(ccp(yesX, visibleOrigin.y + visibleSize.height * 0.225f));

    int adMRectLeft = NativeBridge::getAdMobMRectLeft();
    CCMenuItemImage *pNo = LGMenuItemImage::create("quit_no.png", NULL, this, menu_selector(QuitScene::noCallback));
    float noX = visibleOrigin.x + visibleSize.width * 0.31875f;
    noX = MIN(noX, visibleOrigin.x + adMRectLeft - 42);
    pNo->setPosition(ccp(noX, pYes->getPositionY()));

    sprite = CCSprite::create("quit_title.png");
    sprite->setScale(0.72f);
    sprite->setPosition(ccp((yesX + noX) / 2, visibleOrigin.y + visibleSize.height * 0.7875f));
    addChild(sprite);

    CCMenuItemImage *pMore = LGMenuItemImage::create("more.png", NULL, this, menu_selector(QuitScene::moreCallback));
    pMore->setPosition(ccp(sprite->getPositionX(), visibleOrigin.y + visibleSize.height * 0.50625f));

    CCMenu *pMenu = CCMenu::create(pYes, pNo, pMore, NULL);
    pMenu->setAnchorPoint(CCPointZero);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu);

    return true;
}

void QuitScene::onEnter()
{
    CCLayer::onEnter();

    NativeBridge::showAdMobMRect(true);
    NativeBridge::showAdMobBanner(false);
}

void QuitScene::onExit()
{
    CCLayer::onExit();
    NativeBridge::showAdMobMRect(false);
    NativeBridge::showAdMobBanner(true);
}

void QuitScene::yesCallback(CCObject *sender)
{
    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void QuitScene::noCallback(CCObject *sender)
{
    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }
    CCDirector::sharedDirector()->popScene();
}

void QuitScene::moreCallback(CCObject *sender)
{
    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }
    NativeBridge::onMoreClicked();
}
