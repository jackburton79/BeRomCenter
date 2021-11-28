#ifndef __DATPOPUPMENU_H
#define __DATPOPUPMENU_H

//#include "FilePopUpMenu.h"

#include <String.h>

class TDatPopUpMenu //: public TFilePopUpMenu
{
public:
	TDatPopUpMenu(const char *name, bool radioMode = true,
					bool labelFromMarked = true, menu_layout layout = B_ITEMS_IN_COLUMN)
		//: TFilePopUpMenu(name, radioMode, labelFromMarked, layout)
	{
		
	};
protected:
	virtual bool ShouldAdd(const char *fileName) const
	{
		BString string(fileName);
		string.ToLower();
		if (string.FindLast(".dat") != B_ERROR)
			return true;
		return false;
	};
};

#endif
