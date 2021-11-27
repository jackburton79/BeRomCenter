#ifndef __COLORSTRINGITEM_H
#define __COLORSTRINGITEM_H

#include <ListItem.h>
#include <GraphicsDefs.h>

class TColorStringItem : public BStringItem
{
public:
	TColorStringItem(const char *, uint32 level = 0, bool expanded = true);
	virtual void DrawItem(BView *, BRect, bool complete = false);
	void SetColor(rgb_color);
private:	
	rgb_color fTextColor;
};

#endif // __COLORSTRINGITEM_H
