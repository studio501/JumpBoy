#ifndef __LG_MENU_ITEM_IMAGE__
#define __LG_MENU_ITEM_IMAGE__

#include "cocos2d.h"

USING_NS_CC;

class LGMenuItemImage : public CCMenuItemImage
{
protected:
    bool m_ScaleOnSelected;
    float m_OriginalScaleX;
    float m_OriginalScaleY;

public:
    LGMenuItemImage():m_ScaleOnSelected(true), m_OriginalScaleX(1.0f), m_OriginalScaleY(1.0f){}

    virtual ~LGMenuItemImage(){}

    static LGMenuItemImage* create();
    static LGMenuItemImage* create(const char *normalImage, const char *selectedImage);
    static LGMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage);
    static LGMenuItemImage* create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector);
    static LGMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector);

    void setScaleOnSelected(bool scale);

    virtual void selected();
    virtual void unselected();
};

#endif /* defined(__LG_MENU_ITEM_IMAGE__) */
