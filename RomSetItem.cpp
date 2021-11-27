#include "RomSetItem.h"
#include "DataFile.h"

TRomSetItem::TRomSetItem(RomSet *romSet)
	:TColorStringItem(romSet->desc),
	fRomSet(romSet)
{
}


void
TRomSetItem::DrawItem(BView *owner, BRect frame, bool complete)
{
	rgb_color yellow = { 0, 150, 190 };
	rgb_color black = { 0, 0, 0 };
	
	if (fRomSet->Completed())
		SetColor(black);
	else
		SetColor(yellow);
	
	TColorStringItem::DrawItem(owner, frame, complete);
}
