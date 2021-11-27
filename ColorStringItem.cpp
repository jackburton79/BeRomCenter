#include "ColorStringItem.h"

#include <View.h>
const rgb_color kBlack = { 0, 0, 0 };

TColorStringItem::TColorStringItem(const char *text, uint32 index, bool expanded)
	:BStringItem(text, index, expanded)
{
	fTextColor = kBlack; 
}


void
TColorStringItem::DrawItem(BView *owner, BRect bounds, bool complete)
{
	const rgb_color old = owner->HighColor();
	
	owner->SetHighColor(fTextColor);
	BStringItem::DrawItem(owner, bounds, complete);
	owner->SetHighColor(old);
}


void
TColorStringItem::SetColor(rgb_color color)
{
	fTextColor = color;
}

