#ifndef UIITEM_H
#define UIITEM_H
#include "ItemBase.h"

class UiItem : public ItemBase
{
public:
    void setPos(int left, int top) { m_left = left; m_top = top;}
    void setSize(int width, int height) { m_width =width; m_height = height; }

protected:
    int m_left, m_top, m_width, m_height;
};
#endif

