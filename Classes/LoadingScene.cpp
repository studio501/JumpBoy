#include "LoadingScene.h"
#include "MainScene.h"

USING_NS_CC;

CCScene *LoadingScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    LoadingScene *layer = LoadingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

LoadingScene *LoadingScene::create()
{
    LoadingScene *pRet = new LoadingScene();
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

LoadingScene::LoadingScene()
{
}

LoadingScene::~LoadingScene()
{
}

bool LoadingScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    setKeypadEnabled(true);

    CCPoint visibleCenter = CCDirector::sharedDirector()->getVisibleOrigin() + CCDirector::sharedDirector()->getVisibleSize() / 2;

    CCSprite *sprite = CCSprite::create("bg1.jpg");
    sprite->setPosition(visibleCenter);
    addChild(sprite);

    sprite = CCSprite::create("loading.png");
    sprite->setPosition(visibleCenter);
    addChild(sprite);

#if COCOS2D_DEBUG
    scheduleOnce(schedule_selector(LoadingScene::start), 0.1f);
#else
    scheduleOnce(schedule_selector(LoadingScene::start), 5);
#endif

    return true;
}

void LoadingScene::start(float dt)
{
    CCDirector::sharedDirector()->replaceScene(MainScene::scene());
}
