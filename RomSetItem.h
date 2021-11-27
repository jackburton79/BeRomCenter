#ifndef __ROMSET_ITEM_H
#define __ROMSET_ITEM_H

#include "ColorStringItem.h"

struct RomSet;
class TRomSetItem : public TColorStringItem
{
public:
	TRomSetItem(RomSet *);
	virtual void DrawItem(BView *owner, BRect frame, bool complete = false); 
	RomSet *fRomSet;
};

#endif // __ROMSET_ITEM_H
