#include "GameScene.h"
#include "LGMenuItemImage.h"
#include "LGLayer.h"

#include "SimpleAudioEngine.h"
#include "MainScene.h"
#include "GameManager.h"
#include "Util.h"
#include "NativeBridge.h"
#include "CCStrokeLabel.h"

USING_NS_CC;

#define kTagFlutter 1
#define kTagRaise 2
#define kTagOverScoreBoard 3
#define kTagOverMenu 4

const int PROP_SHOW_NUM[] = {5, 10, 15, 25, 35, 45, 55, 65, 75, 85};
//const int PROP_SHOW_NUM[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();

    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

GameScene::GameScene()
    : m_VisibleOrigin(CCPointZero)
    , m_VisibleSize(CCSizeZero)
    , m_pBackground1(NULL)
    , m_pBackground2(NULL)
    , m_pSound(NULL)
    , m_ScoreLabel(NULL)
    , m_RecordLabel(NULL)
    , m_fForwordVelocity(-650)
    , m_fFallVelocity(0)
    , m_fMaxVelocity(-938)
    , m_pBoy(NULL)
    , m_PassedTime(0)
    , m_CurrentScore(0)
    , m_Record(0)
    , m_LastPassedPillarTag(-1)
    , m_GroundPosY(0)
    , m_Gap(250)
    , m_Started(false)
    , m_Crashed(false)
    , m_pOverLayer(NULL)
    , m_pRateLayer(NULL)
    , m_pCurrentLayer(NULL)
    , m_Touched(false)
    , m_Fall(true)
    , m_TouchBeganInOverLayer(false)
    , m_PillarNumber(0)
    , m_PropIndex(-1)
{
    memset(m_Pillars, 0, sizeof(m_Pillars));
    m_VisibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    m_VisibleSize = CCDirector::sharedDirector()->getVisibleSize();
    m_Sound = CCUserDefault::sharedUserDefault()->getBoolForKey("sound");
}

GameScene::~GameScene()
{
    CC_SAFE_RELEASE_NULL(m_pBackground1);
    CC_SAFE_RELEASE_NULL(m_pBackground2);
    CC_SAFE_RELEASE_NULL(m_pSound);
    CC_SAFE_RELEASE_NULL(m_ScoreLabel);
    CC_SAFE_RELEASE_NULL(m_RecordLabel);
    CC_SAFE_RELEASE_NULL(m_pBoy);
    for (int i = 0; i < sizeof(m_Pillars) / sizeof(m_Pillars[0]); i++)
    {
        CC_SAFE_RELEASE_NULL(m_Pillars[i]);
    }

    CC_SAFE_RELEASE_NULL(m_pOverLayer);
    CC_SAFE_RELEASE_NULL(m_pRateLayer);
    CC_SAFE_RELEASE_NULL(m_pCurrentLayer);

    for (std::list<CCSprite *>::iterator iter = m_Props.begin(); iter != m_Props.end();)
    {
        (*iter)->release();
        iter = m_Props.erase(iter);
    }
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    setTouchEnabled(true);
    setKeypadEnabled(true);

    m_GroundPosY = 86/* ground position in bg1.jpg(bg2.jpg) */ - m_VisibleOrigin.y;

    m_pBackground1 = CCSprite::create("bg1.jpg");
    m_pBackground1->retain();
    m_pBackground1->setPosition(m_VisibleOrigin + m_VisibleSize / 2);
    addChild(m_pBackground1);

    m_pBackground2 = CCSprite::create("bg1.jpg");
    m_pBackground2->retain();
    m_pBackground2->setPosition(ccp(m_VisibleOrigin.x + m_VisibleSize.width * 1.5f, m_VisibleOrigin.y + m_VisibleSize.height * 0.5f));
    addChild(m_pBackground2);

    m_Sound = CCUserDefault::sharedUserDefault()->getBoolForKey("sound", true);
    m_Record = CCUserDefault::sharedUserDefault()->getIntegerForKey("record", -1);
    //m_pSound = CCMenuItemImage::create("sound_off.png", "sound_on.png", this, menu_selector(GameScene::soundCallback));
    //m_pSound->retain();
    //m_pSound->setPosition(m_VisibleOrigin.x + m_VisibleSize.width * 0.065f, m_VisibleOrigin.y + m_VisibleSize.height * 0.9125f);
    //if (m_Sound)
    //{
    //    m_pSound->selected();
    //}
    //else
    //{
    //    m_pSound->unselected();
    //}

    //CCMenu *pMenu = CCMenu::createWithItem(m_pSound);
    //pMenu->setZOrder(2);
    //pMenu->setAnchorPoint(CCPointZero);
    //pMenu->setPosition(CCPointZero);
    //addChild(pMenu);

    m_ScoreLabel = CCLabelAtlas::create("0", "fonts/font1.png", 42, 52, '0');
    m_ScoreLabel->retain();
    m_ScoreLabel->setZOrder(2);
    m_ScoreLabel->setAnchorPoint(ccp(0.5, 0.5));
    m_ScoreLabel->setPosition(ccp(m_VisibleOrigin.x + m_VisibleSize.width * 0.15f, m_VisibleOrigin.y + m_VisibleSize.height * 0.9125f));
    addChild(m_ScoreLabel);

    CCSprite *sprite = CCSprite::create("best.png");
    sprite->setZOrder(2);
    sprite->setPosition(ccp(m_VisibleOrigin.x + m_VisibleSize.width * 0.7125f, m_ScoreLabel->getPositionY()));
    addChild(sprite);

    m_RecordLabel = CCLabelAtlas::create(CCString::createWithFormat("%d", m_Record < 0 ? 0 : m_Record)->getCString(), "fonts/font1.png", 42, 52, '0');
    m_RecordLabel->retain();
    m_RecordLabel->setZOrder(2);
    m_RecordLabel->setAnchorPoint(ccp(0.5, 0.5));
    m_RecordLabel->setPosition(m_VisibleOrigin.x + m_VisibleSize.width * 0.8975f, m_ScoreLabel->getPositionY());
    addChild(m_RecordLabel);

    addBoy();

    if (m_Sound)
    {
        CocosDenshion:: SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(SFX_SKATING);
    }

    scheduleUpdate();
    schedule(schedule_selector(GameScene::updatePillar), 0.1f, kCCRepeatForever, 0);

    return true;
}

void GameScene::onEnter()
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCLayer::onEnter();

    NativeBridge::showAdMobBanner(true);

    if (m_Sound)
    {
        CocosDenshion:: SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(SFX_SKATING);
    }

    scheduleOnce(schedule_selector(GameScene::startSchedule), 0.1f);
}

void GameScene::onExit()
{
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);

    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }

    CCLayer::onExit();

    NativeBridge::showAdMobBanner(true);
}  

bool GameScene::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    if (m_pCurrentLayer && m_pCurrentLayer == m_pOverLayer)
    {
        m_TouchBeganInOverLayer = true;
    }
    else
    {
        m_TouchBeganInOverLayer = false;
        if (!m_Started)
        {
            start();

            if (m_Sound)
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
            }
        }
        else if (!m_Crashed)
        {
            if (m_Sound)
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_JUMP);
            }
            m_Touched = true;
            m_Fall = false;
        }
    }

    return true;
}

void GameScene::ccTouchEnded(CCTouch *touch, CCEvent *event)
{
    if (m_pCurrentLayer && m_pCurrentLayer == m_pOverLayer)
    {
        if (m_TouchBeganInOverLayer)
        {
            CCDirector::sharedDirector()->popScene();
        }
    }
    else
    {
        m_Touched = false;
        m_Fall = true;
    }
}

void GameScene::ccTouchCancelled(CCTouch *touch, CCEvent *event)
{
    if (m_pCurrentLayer && m_pCurrentLayer == m_pOverLayer)
    {
        if (m_TouchBeganInOverLayer)
        {
            CCDirector::sharedDirector()->popScene();
        }
    }
    else
    {
        m_Touched = false;
        m_Fall = true;
    }
}

void GameScene::draw()
{
    CCLayer::draw();

    //glLineWidth(1);
    //ccDrawColor4B(255, 100, 100, 255);
    //for (int i = 0; i < 4; i++)
    //{
    //    if (m_Pillars[i] != NULL)
    //    {
    //        ccDrawRect(m_Pillars[i]->boundingBox().origin, m_Pillars[i]->boundingBox().origin + m_Pillars[i]->boundingBox().size);
    //    }
    //}

    //if (m_pBoy)
    //{
    //    float w = m_pBoy->getContentSize().width * m_pBoy->getScaleX(), h = m_pBoy->getContentSize().height * m_pBoy->getScaleY();
    //    CCRect boyRect = CCRectMake(m_pBoy->getPositionX() - w / 2, m_pBoy->getPositionY() - h / 2, w, h);

    //    ccDrawRect(boyRect.origin, boyRect.origin + boyRect.size);
    //}
}

void GameScene::update(float delta)
{
    float deltaX = m_fForwordVelocity * delta;
    
    
    CCLog("this postion ----->   %f,%f",this->getPositionX(),this->getPositionY());
    if (!m_Crashed)
    {
        // move background
        m_pBackground1->setPositionX(m_pBackground1->getPositionX() + deltaX);
        m_pBackground2->setPositionX(m_pBackground1->getPositionX() + m_pBackground1->getContentSize().width);

        if (m_pBackground1->getPositionX() + m_pBackground1->getContentSize().width / 2 <= m_VisibleOrigin.x)
        {
            CCSprite *pTemp = m_pBackground1;
            m_pBackground1 = m_pBackground2;
            m_pBackground2 = pTemp;

            m_pBackground2->setPositionX(m_pBackground1->getPositionX() + m_pBackground1->getContentSize().width);
        }

        updateProps(delta);
    }

    // floor boundary
    float floorBoundary = m_VisibleOrigin.y + m_GroundPosY + m_pBoy->getContentSize().height / 2;

    bool oldCrashed = m_Crashed;
    if (m_Started)
    {
        if (!m_Crashed)
        {
            // move pillars
            for (int i = 0; i < 8; i++)
            {
                if (m_Pillars[i] != NULL)
                {
                    m_Pillars[i]->setPositionX(m_Pillars[i]->getPositionX() + deltaX);
                    float w = m_pBoy->getContentSize().width * m_pBoy->getScaleX(), h = m_pBoy->getContentSize().height * m_pBoy->getScaleY();
                    CCRect boyRect = CCRectMake(m_pBoy->getPositionX() - w / 2, m_pBoy->getPositionY() - h / 2, w, h);
                    if (m_Pillars[i]->boundingBox().intersectsRect(boyRect))
                    {
                        m_Crashed = true;
                    }
                }
            }
        }

        if (m_Crashed)
        {
            if (!oldCrashed)
            {
                m_pBoy->stopAllActions();

                GameManager *gameManager = GameManager::getInstance();
                gameManager->addGoalToday();
                if (gameManager->getGoalsToday() >= 2 && m_CurrentScore >= 5 && gameManager->getRated() == false && gameManager->getLaterTS() != Util::getCurrentDays())
                {
                    showRateView();
                }
                else
                {
                    if (gameManager->getGoalsToday() % 10 == 0)
                    {
                        NativeBridge::showAdMobInterstitial();
                    }
                    showOverView();
                }

                if (m_Sound)
                {
                    CocosDenshion:: SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
                    CocosDenshion:: SimpleAudioEngine::sharedEngine()->playEffect(SFX_IMPACT);
                    scheduleOnce(schedule_selector(GameScene::playDieEffect), 0.5f);
                }

                NativeBridge::showAdMobBanner(true);
            }
        }
        else if (m_pBoy)
        {
            const static float ACCEL = -2500;
            // fall boy
            if (!m_Touched || m_Fall)
            {
                if (m_fFallVelocity > 0)
                {
                    m_fFallVelocity = 0;
                }
                m_fFallVelocity = m_fFallVelocity + ACCEL * delta;
            }
            else
            {
                m_fFallVelocity = -m_fMaxVelocity;
            }
            m_fFallVelocity = MAX(m_fMaxVelocity, MIN(m_fFallVelocity, -m_fMaxVelocity));

            float oldPosY = m_pBoy->getPositionY();
            float newPosY = oldPosY +m_fFallVelocity * delta;

            if (m_Fall)
            // skate on the boxes
            {
                float minBoyX = m_pBoy->getPositionX() - m_pBoy->getContentSize().width / 2, maxBoyX = m_pBoy->getPositionX() + m_pBoy->getContentSize().width / 2;
                for (int i = 1; i < 8; i += 2)
                {
                    if (m_Pillars[i] != NULL)
                    {
                        float minPillarX = m_Pillars[i]->getPositionX() - m_Pillars[i]->getContentSize().width / 2, maxPillarX = m_Pillars[i]->getPositionX() + m_Pillars[i]->getContentSize().width / 2;
                        if (!(minBoyX > maxPillarX || maxBoyX < minPillarX))
                        {
                            float pillarTop = m_Pillars[i]->getPositionY() + m_Pillars[i]->getContentSize().height / 2 + m_pBoy->getContentSize().height / 2 + 0.01f;
                            if (oldPosY >= pillarTop && newPosY < pillarTop)
                            {
                                newPosY = pillarTop;
                            }
                            break;
                        }
                    }
                }
            }

            float skyBoundary = m_VisibleOrigin.y + m_VisibleSize.height;
            // floor boundary
            if (newPosY < floorBoundary)
            {
                newPosY = floorBoundary;
            }
            // up boundary
            else if (newPosY > skyBoundary)
            {
                newPosY = skyBoundary;
                m_Fall = true;
            }

            m_pBoy->setPositionY(newPosY);

            // score
            int lastPassedPillarTag = -1;
            float boyX = m_pBoy->getPositionX();
            for (int i = 0; i < 8 && m_Pillars[i] && m_Pillars[i]->getPositionX() < boyX; i += 2)
            {
                lastPassedPillarTag = m_Pillars[i]->getTag();
            }

            if (lastPassedPillarTag >= 0 && lastPassedPillarTag != m_LastPassedPillarTag)
            {
                m_CurrentScore++;
                m_LastPassedPillarTag = lastPassedPillarTag;

                m_ScoreLabel->setString(CCString::createWithFormat("%d", m_CurrentScore)->getCString());

                if (m_CurrentScore > m_Record)
                {
                    m_Record = m_CurrentScore;

                    m_RecordLabel->setString(CCString::createWithFormat("%d", m_Record)->getCString());

                    if (m_Sound)
                    {
                        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_RECORD);
                    }
                }
                else
                {
                    if (m_Sound)
                    {
                        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_POINT);
                    }
                }
            }
        }
    }

    if (m_Sound && m_pBoy)
    {
        if (m_pBoy->getPositionY() <= floorBoundary + 0.01 && !m_Crashed)
        {
            if (!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(SFX_SKATING);
            }
        }
        else
        {
            if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
            {
                CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            }
        }
    }
}

void GameScene::keyBackClicked()
{
    CCDirector::sharedDirector()->popScene();

    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }
}

void GameScene::soundCallback(CCObject *sender)
{
    m_Sound = !m_Sound;
    CCUserDefault::sharedUserDefault()->setBoolForKey("sound", m_Sound);
    if (m_Sound)
    {
        m_pSound->selected();
    }
    else
    {
        m_pSound->unselected();
        if (CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        {
            CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
        }
    }
}

void GameScene::rateCallback(CCObject *pSender)
{
    GameManager::getInstance()->saveRated(true);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    NativeBridge::rateApp();
#endif
    if (m_pRateLayer != NULL)
    {
        m_pRateLayer->removeFromParent();

        CC_SAFE_RELEASE_NULL(m_pCurrentLayer);
    }

    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }

    scheduleOnce(schedule_selector(GameScene::showOverViewSchedule), 2.0f);
}

void GameScene::laterCallback(CCObject *pSender)
{
    GameManager::getInstance()->saveLaterTS(Util::getCurrentDays());

    if (m_pRateLayer != NULL)
    {
        m_pRateLayer->removeFromParent();

        CC_SAFE_RELEASE_NULL(m_pCurrentLayer);
    }

    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }

    showOverView();
}

void GameScene::declineCallback(CCObject *pSender)
{
    GameManager::getInstance()->saveRated(true);

    if (m_pRateLayer != NULL)
    {
        m_pRateLayer->removeFromParent();

        CC_SAFE_RELEASE_NULL(m_pCurrentLayer);
    }

    if (m_Sound)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_FADE);
    }

    showOverView();
}

void GameScene::addBoy()
{
    const static int fameCount = 3;
    const static float animationDuration = 0.15f;

    m_PropIndex = CCUserDefault::sharedUserDefault()->getIntegerForKey("prop_index", -1);

    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("boys.plist");
    cache->addSpriteFramesWithFile("props.plist");

    m_pBoy = CCSprite::createWithSpriteFrameName("boy1");
    m_pBoy->retain();
    m_pBoy->setZOrder(1);
    m_pBoy->setPosition(ccp(m_VisibleOrigin.x + m_VisibleSize.width * 0.36875f, m_VisibleOrigin.y + m_GroundPosY + m_pBoy->getContentSize().height / 2));
    for (int i = 0; i <= m_PropIndex; i++)
    {
        CCSprite *propFull = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("item%d.png", i + 1)->getCString());
        propFull->setPosition(ccp(m_pBoy->getContentSize().width / 2, m_pBoy->getContentSize().height / 2));
        m_pBoy->addChild(propFull);
    }
    addChild(m_pBoy);

    CCArray *animFrames = CCArray::createWithCapacity(fameCount);
    for(int i = 1; i <= fameCount; i++)
    {
        CCSpriteFrame* frame = cache->spriteFrameByName(CCString::createWithFormat("boy%d", i)->getCString());
        animFrames->addObject(frame);
    }

    CCAction *action = CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(animFrames, animationDuration)));
    action->setTag(kTagFlutter);
    m_pBoy->runAction(action);
}

void GameScene::updatePillar(float delta)
{
    m_PassedTime += delta;
    const static int pillarWidth = 88;
    const static int minBottomPillarHeight = 40;
    const static int minTopPillarHeight = 20;

    int firstEmptyIndex = 0;
    float lastPillarPosX = -1000;

    if (m_Pillars[0] != NULL)
    {
        // not visible
        if (m_Pillars[0]->getPositionX() + m_Pillars[0]->getContentSize().width / 2 <= 0)
        {
            m_Pillars[0]->removeFromParent();
            m_Pillars[1]->removeFromParent();
            CC_SAFE_RELEASE_NULL(m_Pillars[0]);
            CC_SAFE_RELEASE_NULL(m_Pillars[1]);

            for (int i = 0; i < 6; i++)
            {
                m_Pillars[i] = m_Pillars[i + 2];
                m_Pillars[i + 1] = m_Pillars[i + 3];
            }
            m_Pillars[6] = NULL;
            m_Pillars[7] = NULL;
        }

        while (firstEmptyIndex < 8 && m_Pillars[firstEmptyIndex] != NULL)
        {
            firstEmptyIndex += 2;
        }
        if (firstEmptyIndex > 1)
        {
            lastPillarPosX = m_Pillars[firstEmptyIndex - 2]->getPositionX();
        }
    }

    static int pillarId = 0;
    // add new pillar
    if ((m_Pillars[0] == NULL && m_PassedTime > 1.0f) || (m_Pillars[0] != NULL && lastPillarPosX + m_VisibleSize.width + m_Pillars[0]->getContentSize().width * 1.0f <= m_VisibleOrigin.x + m_VisibleSize.width && firstEmptyIndex < 8))
    {
        int bottomPillar_Height = minBottomPillarHeight + CCRANDOM_0_1() * (m_VisibleSize.height - m_GroundPosY - minBottomPillarHeight - minTopPillarHeight - m_Gap);
        int bottomPillar_BottomY = m_VisibleOrigin.y + m_GroundPosY;
        int topPillar_BottomY = bottomPillar_BottomY + bottomPillar_Height + m_Gap;
        int topPillar_Height = CCDirector::sharedDirector()->getWinSize().height - topPillar_BottomY + m_VisibleOrigin.y;
        m_Pillars[firstEmptyIndex] = CCSprite::create("pillar.png", CCRectMake(0, 0, pillarWidth, topPillar_Height));
        m_Pillars[firstEmptyIndex]->retain();
        m_Pillars[firstEmptyIndex]->setTag(pillarId);
        m_Pillars[firstEmptyIndex]->setPosition(ccp(m_VisibleOrigin.x + m_VisibleSize.width + pillarWidth / 2, topPillar_BottomY + topPillar_Height / 2));
        m_Pillars[firstEmptyIndex]->setRotation(180);
        addChild(m_Pillars[firstEmptyIndex]);

        m_Pillars[firstEmptyIndex + 1] = CCSprite::create("pillar.png", CCRectMake(0, 0, pillarWidth, bottomPillar_Height));
        m_Pillars[firstEmptyIndex + 1]->retain();
        m_Pillars[firstEmptyIndex + 1]->setTag(pillarId++);
        m_Pillars[firstEmptyIndex + 1]->setPosition(ccp(m_VisibleOrigin.x + m_VisibleSize.width + pillarWidth / 2, bottomPillar_BottomY + bottomPillar_Height / 2));
        addChild(m_Pillars[firstEmptyIndex + 1]);

        m_PillarNumber++;

        {
            int index = -1;
            for (int i = m_PropIndex + 1; i < sizeof(PROP_SHOW_NUM) / sizeof(PROP_SHOW_NUM[0]) && m_PillarNumber >= PROP_SHOW_NUM[i] && index < 0; i++)
            {
                if (m_PillarNumber == PROP_SHOW_NUM[i])
                {
                    index = i + 1;
                }
            }

            if (index > 0)
            {
                CCSprite *prop = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("i%d.png", index)->getCString());
                prop->setPosition(ccp(m_Pillars[firstEmptyIndex]->getPositionX(), (m_Pillars[firstEmptyIndex]->getPositionY() + m_Pillars[firstEmptyIndex + 1]->getPositionY()) / 2));
                prop->setTag(index - 1);
                addChild(prop);

                m_Props.push_back(prop);
                prop->retain();
            }
        }
    }
}

void GameScene::updateProps(float delta)
{
    float deltaX = m_fForwordVelocity * delta;
    for (std::list<CCSprite *>::iterator iter = m_Props.begin(); iter != m_Props.end(); )
    {
        CCSprite *prop = *iter;
        float newX = prop->getPositionX() + deltaX;
        if (newX < m_VisibleOrigin.x - 50)
        {
            prop->release();
            iter = m_Props.erase(iter);

            prop->removeFromParent();
        }
        else
        {
            prop->setPositionX(newX);

            if (prop->boundingBox().intersectsRect(m_pBoy->boundingBox()))
            {
                m_PropIndex = prop->getTag();
                CCUserDefault::sharedUserDefault()->setIntegerForKey("prop_index", m_PropIndex);

                CCSprite *propFull = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("item%d.png", m_PropIndex + 1)->getCString());
                propFull->setPosition(ccp(m_pBoy->getContentSize().width / 2, m_pBoy->getContentSize().height / 2));
                m_pBoy->addChild(propFull);

                prop->release();
                iter = m_Props.erase(iter);

                prop->removeFromParent();

                if (m_Sound)
                {
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_NEWITEM);
                }
            }
            else
            {
                iter++;
            }
        }
    }
}

void GameScene::showOverView()
{
    if (m_pOverLayer == NULL)
    {
        LGLayerColor *pOverLayer = LGLayerColor::create(ccc4(0, 0, 0, 175));
        pOverLayer->setTouchDelegate(this);
        pOverLayer->setTouchMode(kCCTouchesOneByOne);
        pOverLayer->setTouchEnabled(true);
        pOverLayer->setZOrder(3);
        m_pOverLayer = pOverLayer;
        m_pOverLayer->retain();

        CCSprite *title = CCSprite::create("title_over.png");
        title->setPosition(ccp(m_VisibleOrigin.x + m_VisibleSize.width / 2, m_VisibleOrigin.y + m_VisibleSize.height * 0.6625f));
        m_pOverLayer->addChild(title);

        CCSprite *retryHint = CCSprite::create("retry_hint.png");
        retryHint->setPosition(ccp(m_VisibleOrigin.x + m_VisibleSize.width / 2, m_VisibleOrigin.y + m_VisibleSize.height * 0.44375f));
        m_pOverLayer->addChild(retryHint);
    }

    m_Record = MAX(0, m_Record);
    NativeBridge::submitScore(m_Record);
    CCUserDefault::sharedUserDefault()->setIntegerForKey("record", m_Record);

    if (m_pOverLayer && m_pOverLayer->getParent() == NULL)
    {
        getParent()->addChild(m_pOverLayer);
        CC_SAFE_RELEASE_NULL(m_pCurrentLayer);
        m_pCurrentLayer = m_pOverLayer;
        m_pCurrentLayer->retain();
    }
}

void GameScene::showRateView()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (m_pRateLayer == NULL)
    {
        m_pRateLayer = LGLayerColor::create(ccc4(0, 0, 0, 0));
        m_pRateLayer->retain();
        m_pRateLayer->setTouchMode(kCCTouchesOneByOne);
        m_pRateLayer->setTouchEnabled(true);
        m_pRateLayer->setZOrder(3);

        CCSprite *pBg = CCSprite::create("rate_bg.png");
        pBg->setPosition(ccp(m_VisibleOrigin.x + m_VisibleSize.width / 2, m_VisibleOrigin.y + m_VisibleSize.height * 0.6125f));
        m_pRateLayer->addChild(pBg);

        float bgW = pBg->getContentSize().width, bgH = pBg->getContentSize().height;
        CCMenuItemImage *pRate = LGMenuItemImage::create("rate_rate.png", NULL, this, menu_selector(GameScene::rateCallback));
        pRate->setScale(0.9f);

        CCMenuItemImage *pLater = LGMenuItemImage::create("rate_later.png", NULL, this, menu_selector(GameScene::laterCallback));
        pLater->setScale(0.9f);

        pRate->setPosition(ccp(pBg->getPositionX() - bgW * 0.277778f, pBg->getPositionY() - bgH * 0.49f));
        pLater->setPosition(ccp(pBg->getPositionX() + bgW * 0.277778f, pRate->getPositionY()));

        CCMenu *pMenu = CCMenu::create(pRate, pLater, NULL);
        pMenu->setAnchorPoint(CCPointZero);
        pMenu->setPosition(CCPointZero);
        m_pRateLayer->addChild(pMenu);
    }

    if (m_pRateLayer && m_pRateLayer->getParent() == NULL)
    {
        getParent()->addChild(m_pRateLayer);
        CC_SAFE_RELEASE_NULL(m_pCurrentLayer);
        m_pCurrentLayer = m_pRateLayer;
        m_pCurrentLayer->retain();
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    NativeBridge::showRateAlert(this, menu_selector(GameScene::rateCallback), menu_selector(GameScene::laterCallback), menu_selector(GameScene::declineCallback));
#endif
}

void GameScene::playDieEffect(float delta)
{
    //if (m_Sound)
    //{
    //    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(SFX_DIE);
    //}
}

void GameScene::showOverViewSchedule(float delta)
{
    showOverView();
}

void GameScene::startSchedule(float delta)
{
    start();
}

void GameScene::start()
{
    if (!m_Started)
    {
        m_Started = true;

        NativeBridge::showAdMobBanner(false);
    }
}