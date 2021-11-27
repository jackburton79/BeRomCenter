#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "DataFile.h"

#define kNumTokens 9

Rom::Rom()
{
	romSet = NULL;
	ok = false;
}


/* RomSetItem */
RomSetItem::RomSetItem()
	:TColorStringItem(desc)
{
	parent = NULL;
}


void
RomSetItem::DrawItem(BView *owner, BRect bounds, bool complete)
{
	rgb_color yellow = { 0, 150, 190 };
	rgb_color black = { 0, 0, 0 };
	
	if (Completed())
		SetColor(black);
	else
		SetColor(yellow);
	
	TColorStringItem::DrawItem(owner, bounds, complete);
}


bool
RomSetItem::Completed() const
{
	return true;
}


/* ParentSetItem */
ParentSetItem::ParentSetItem()
	:TColorStringItem(desc)
{
}


void
ParentSetItem::DrawItem(BView *owner, BRect bounds, bool complete)
{
	rgb_color yellow = { 0, 150, 190 };
	rgb_color black = { 0, 0, 0 };
	
	if (Completed())
		SetColor(black);
	else
		SetColor(yellow);
	
	TColorStringItem::DrawItem(owner, bounds, complete);
}


bool
ParentSetItem::Completed() const 
{
	return true;
};


static int
CompareParents(const void *first, const void *second)
{
	return strcmp((*(ParentSetItem **)first)->desc, (*(ParentSetItem **)second)->desc); 
}


int 
ParseDataFile(void *castToParams)
{
	Params *params = static_cast<Params *>(castToParams);
	CleanupData(castToParams);
	
	FILE *dataFile = fopen(params->dataFile, "r");
	if (dataFile == NULL) {
		printf("Can't open file\n");
		return B_ERROR;
	}
	
	BList *parentList = params->list;
	char buffer[500];
	char string[200];
	while (fgets(buffer, 500, dataFile)) {
		if (!strncmp(buffer, "[GAMES]", 7)) {
			ParentSetItem *newParent = NULL;
			RomSetItem *newSet = NULL;
			Rom *newRom = NULL;
				
			while (fgets(buffer, 500, dataFile)) {			
				if (!strncmp(buffer, "[RESOURCES]", strlen("[RESOURCES]")))
					break; 
		
				int32 numStrings = 0;
				int32 bufIndex = 0;
				do {			
					int32 strIndex = 0;	
					while ((string[strIndex++] = buffer[bufIndex++]) != (char)0xAC)
						;
					string[strIndex - 1] = '\0';
					
					switch(numStrings) {
						case 1:
							if (!newParent || (strcmp(newParent->name, string) != 0)) {												
								newParent = new ParentSetItem();
								strcpy(newParent->name, string);
								parentList->AddItem(newParent);
							}
							break;
						case 2:
							strcpy(newParent->desc, string);
							newParent->SetText(string);
							break;
						case 3:
							if (!newSet || (strcmp(newSet->name, string) != 0)) {
								newSet = new RomSetItem();
								strcpy(newSet->name, string);
								newSet->parent = newParent;
								newParent->romSets.AddItem(newSet);
							}
							break;
						case 4:
							strcpy(newSet->desc, string);
							newSet->SetText(string);
							break;
						case 5:
							newRom = new Rom;
							newRom->romSet = newSet;
							strcpy(newRom->name, string);
							newSet->roms.AddItem(newRom);
							break;
						case 6:
							newRom->crc = (uint32)strtol(string, NULL, 16);
							break;
						case 7:
							newRom->size = atoi(string);
							break;
						default:
							break;
					}
				} while (numStrings++ != kNumTokens);
			}
		}
	}
	fclose(dataFile);
	
	parentList->SortItems(CompareParents);
	
	return B_OK;
}


int
CleanupData(void *castToParams)
{
	Params *params = static_cast<Params *>(castToParams);	
	BList *parentList = params->list;
	
	for (int32 x = parentList->CountItems() - 1; x >= 0; x--) {
		ParentSetItem *parent = static_cast<ParentSetItem *>(parentList->ItemAtFast(x));
		for (int32 rs = 0; rs < parent->romSets.CountItems(); rs++) {
			RomSetItem *rsItem = static_cast<RomSetItem *>(parent->romSets.ItemAtFast(rs));
			for (int32 r = 0; r < rsItem->roms.CountItems(); r++) {
				Rom *rom = static_cast<Rom *>(rsItem->roms.ItemAtFast(r));
				delete rom;
			}
			delete rsItem;
		}
		delete parent;
	}
	
	parentList->MakeEmpty();
	
	return B_OK;
}
