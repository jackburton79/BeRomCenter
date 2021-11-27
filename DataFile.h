#ifndef __IOSTUFF_H
#define __IOSTUFF_H

#include "ColorStringItem.h"
#include <List.h>

class BMessenger;

class ParentSetItem : public TColorStringItem
{
public:
	ParentSetItem();
	virtual void DrawItem(BView *owner, BRect frame, bool complete = false);
	
	bool Completed() const;
	
	char name[9];
	char desc[100];
	
	BList romSets;
};


class RomSetItem : public TColorStringItem 
{
public:
	RomSetItem();
	virtual void DrawItem(BView *owner, BRect frame, bool complete = false);
	
	bool Completed() const;
	
	ParentSetItem *parent;
	char name[9];
	char desc[100];
		
	BList roms;
};


struct Rom {
	Rom();
	
	RomSetItem *romSet;	
	char name[13];
	uint32 crc;
	uint32 size;
	char parentRomName[13];
	char parentRomSize[9];
	bool ok;
};


struct Params {
	const char *dataFile;
	BList *list;
	BMessenger *messenger;
};


extern int ParseDataFile(void *castToParams);
extern int CleanupData(void *castToParams);

#endif
