#ifndef __LG_LAYER__
#define __LG_LAYER__

#include "cocos2d.h"

USING_NS_CC;

class LGLayerColor : public CCLayerColor
{
public:
    LGLayerColor();
    virtual ~LGLayerColor();

    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

    void setTouchDelegate(CCTouchDelegate *delegate) { m_TouchDelegate = delegate; };

    static LGLayerColor *create(const ccColor4B &color);

private:
    CCTouchDelegate *m_TouchDelegate;
};

#endif /* defined(__LG_LAYER__) */
