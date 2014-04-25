#include "LGMenuItemImage.h"

const unsigned int kZoomActionTag = 0xc0c05030;

LGMenuItemImage* LGMenuItemImage::create()
{
    LGMenuItemImage *pRet = new LGMenuItemImage();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

LGMenuItemImage *LGMenuItemImage::create(const char *normalImage, const char *selectedImage)
{
    return LGMenuItemImage::create(normalImage, selectedImage, NULL, NULL, NULL);
}

LGMenuItemImage *LGMenuItemImage::create(const char *normalImage, const char *selectedImage, const char *disabledImage)
{
    LGMenuItemImage *pRet = new LGMenuItemImage();
    if (pRet && pRet->initWithNormalImage(normalImage, selectedImage, disabledImage, NULL, NULL))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

LGMenuItemImage *LGMenuItemImage::create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector)
{
    return LGMenuItemImage::create(normalImage, selectedImage, NULL, target, selector);
}

LGMenuItemImage *LGMenuItemImage::create(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector)
{
    LGMenuItemImage *pRet = new LGMenuItemImage();
    if (pRet && pRet->initWithNormalImage(normalImage, selectedImage, disabledImage, target, selector))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void LGMenuItemImage::setScaleOnSelected(bool scale)
{
    m_ScaleOnSelected = scale;
}

void LGMenuItemImage::selected()
{
    CCMenuItemImage::selected();
    if (m_ScaleOnSelected)
    {
        m_OriginalScaleX = getScaleX();
        m_OriginalScaleY = getScaleY();

        setScaleX(m_OriginalScaleX * 1.1f);
        setScaleY(m_OriginalScaleY * 1.1f);
        //CCAction *zoomAction = CCScaleTo::create(0.1f, m_OriginalScale * 1.1f);
        //zoomAction->setTag(kZoomActionTag);
        //this->runAction(zoomAction);
    }
}

void LGMenuItemImage::unselected()
{
    if(m_bEnabled)
    {
        CCMenuItemImage::unselected();

        if (m_ScaleOnSelected)
        {
            setScaleX(m_OriginalScaleX);
            setScaleY(m_OriginalScaleY);
            //this->stopActionByTag(kZoomActionTag);
            //CCAction *zoomAction = CCScaleTo::create(0.1f, m_OriginalScale);
            //zoomAction->setTag(kZoomActionTag);
            //this->runAction(zoomAction);
        }
    }
}
