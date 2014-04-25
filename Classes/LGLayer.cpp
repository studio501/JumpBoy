#include "LGLayer.h"


LGLayerColor *LGLayerColor::create(const ccColor4B &color)
{
    LGLayerColor * pLayer = new LGLayerColor();
    if(pLayer && pLayer->initWithColor(color))
    {
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return NULL;
}

LGLayerColor::LGLayerColor()
    : m_TouchDelegate(NULL)
{

}

LGLayerColor::~LGLayerColor()
{
    m_TouchDelegate = NULL;
};

bool LGLayerColor::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_TouchDelegate)
    {
        return m_TouchDelegate->ccTouchBegan(pTouch, pEvent);
    }
    else
    {
        return true;
    }
}

void LGLayerColor::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_TouchDelegate)
    {
        m_TouchDelegate->ccTouchMoved(pTouch, pEvent);
    }
}

void LGLayerColor::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_TouchDelegate)
    {
        m_TouchDelegate->ccTouchEnded(pTouch, pEvent);
    }
}

void LGLayerColor::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    if (m_TouchDelegate)
    {
        m_TouchDelegate->ccTouchCancelled(pTouch, pEvent);
    }
}

