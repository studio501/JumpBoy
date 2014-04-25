#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class LGMenuItemImage;

class GameScene : public cocos2d::CCLayer
{
public:
    GameScene();
    ~GameScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();

    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);

    virtual void onEnter();
    virtual void onExit();

    virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void ccTouchCancelled(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);
    virtual void draw();
    virtual void update(float delta);
    virtual void keyBackClicked();

    void soundCallback(CCObject *sender);
    void rateCallback(CCObject *pSender);
    void laterCallback(CCObject *pSender);
    void declineCallback(CCObject *pSender);

private:
    void addBoy();
    void updatePillar(float delta);
    void updateProps(float delta);
    void showOverView();
    void showRateView();
    void playDieEffect(float delta);
    void showOverViewSchedule(float delta);
    void startSchedule(float delta);
    void start();

private:
    cocos2d::CCPoint m_VisibleOrigin;
    cocos2d::CCSize m_VisibleSize;
    cocos2d::CCSprite *m_pBackground1;
    cocos2d::CCSprite *m_pBackground2;
    cocos2d::CCMenuItem *m_pSound;
    cocos2d::CCLabelAtlas *m_ScoreLabel;
    cocos2d::CCLabelAtlas *m_RecordLabel;
    float m_fForwordVelocity;
    float m_fFallVelocity;
    float m_fMaxVelocity;

    cocos2d::CCSprite *m_pBoy;
    cocos2d::CCSprite *m_Pillars[8];
    std::list<cocos2d::CCSprite *> m_Props;
    float m_PassedTime;
    int m_CurrentScore;
    int m_Record;
    int m_LastPassedPillarTag;
    int m_GroundPosY; // ground y position relative to visible origin
    int m_Gap;
    bool m_Started;
    bool m_Crashed;

    cocos2d::CCLayer *m_pOverLayer;
    cocos2d::CCLayer *m_pRateLayer;
    cocos2d::CCLayer *m_pCurrentLayer;

    bool m_Sound;
    bool m_Touched;
    bool m_Fall;
    bool m_TouchBeganInOverLayer;
    int m_PillarNumber;
    int m_PropIndex;
};

#endif // __GAME_SCENE_H__
