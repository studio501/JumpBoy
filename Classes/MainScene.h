#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

// android effect only support ogg
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define SFX_FADE             "fade.ogg"
#define SFX_GAMEOVER             "game_over.ogg"
#define SFX_IMPACT         "impact.ogg"
#define SFX_IMPACTGROUND "impact_ground.ogg"
#define SFX_JUMP          "jump.ogg"
#define SFX_NEWITEM          "new_item.ogg"
#define SFX_POINT          "point.ogg"
#define SFX_RECORD          "record.ogg"
#define SFX_SKATING          "skating.ogg"
#else
#define SFX_FADE             "fade.mp3"
#define SFX_GAMEOVER             "game_over.mp3"
#define SFX_IMPACT         "impact.mp3"
#define SFX_IMPACTGROUND "impact_ground.mp3"
#define SFX_JUMP          "jump.mp3"
#define SFX_NEWITEM          "new_item.mp3"
#define SFX_POINT          "point.mp3"
#define SFX_RECORD          "record.mp3"
#define SFX_SKATING          "skating.mp3"
#endif // CC_PLATFORM_ANDROID

class MainScene : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    MainScene();
    virtual ~MainScene();
    virtual bool init();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static CCScene *scene();

    // implement the "static node()" method manually
    static MainScene *create();

    void soundCallback(CCObject *sender);
    void rankCallback(CCObject *sender);
    void moreCallback(CCObject *sender);

    virtual void onEnter();
    virtual void onExit();
    virtual void keyBackClicked();

    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);

private:
    CCMenuItem *m_pSound;
    CCLabelAtlas *m_pRecord;
    bool m_Sound;
    CCSprite *m_pBoy;
};

#endif // __MAIN_SCENE_H__
