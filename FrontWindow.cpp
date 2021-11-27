#include <Alert.h>
#include <Application.h>
#include <Box.h>
#include <Button.h>
#include <Directory.h>
#include <Entry.h>
#include <Menu.h>
#include <MenuBar.h>
#include <MenuField.h>
#include <MenuItem.h>
#include <OutlineListView.h>
#include <StatusBar.h>
#include <ScrollView.h>
#include <TextView.h>

#include <cstdio>
#include <cstring>
#include <unistd.h>

#include "ColorStringItem.h"
#include "DataFile.h"
#include "DatPopUpMenu.h"
#include "FrontWindow.h"
#include "PrefsWindow.h"
#include "RomCenterApp.h"
#include "Settings.h"

#include "unzip/unzip.h"

const char *kWindowCaption = "BeRomCenter";
const int32 kLVSelectionChanged = 'LVsc';
const int32 kCheckRoms = 'Ckrm';

FrontWindow::FrontWindow()
	:BWindow(BRect(100, 50, 760, 510), kWindowCaption,
							B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS),
	fLastSelected(-1),
	fCheckThread(B_BAD_THREAD_ID)
{	
	Settings::Load();
	
	AddMenu();
				
	fView = new BBox(Bounds().InsetByCopy(-1, -1), "Box", B_FOLLOW_ALL_SIDES);
	
	float menuHeight = fMenuBar->Bounds().Height();		
	fView->MoveBy(0, menuHeight);
					
	AddListView();
	
	fPopUpMenu = new TDatPopUpMenu("DataFiles");
	
	BRect fieldRect(300, 50, 600, 90);
	fDataFilesMenuField = new BMenuField(fieldRect, "datafiles",
			"DataFile: ", fPopUpMenu, B_FOLLOW_RIGHT|B_FOLLOW_TOP);
	fDataFilesMenuField->SetDivider(50);
	
	fView->AddChild(fDataFilesMenuField);
	
	fCheckButton = new BButton(BRect(360, 20, 450, 40), "Check Roms", "Check Roms",
				new BMessage(kCheckRoms));
	fView->AddChild(fCheckButton);
	
	BRect tvRect(300, 100, fView->Bounds().right - 30, fView->Bounds().bottom - 80);
	fRomInfo = new BTextView(tvRect, "Rom Info", BRect(0, 0, 300, 400), B_FOLLOW_ALL, B_WILL_DRAW);
	
	BScrollView *tvScrollView = new BScrollView("TV Scroll View", fRomInfo, B_FOLLOW_ALL,
				0, false, false);
	
	fView->AddChild(tvScrollView);
	
	BRect statusRect(15, Bounds().bottom - 70, 80, Bounds().bottom - 15);
	fStatusBar = new BStatusBar(statusRect, "Status Bar");
	fView->AddChild(fStatusBar);
	
	AddChild(fView);
	
	BPath path = static_cast<RomCenterApp *>(be_app)->Path();
	path.Append("datafiles");	
	
	fPopUpMenu->SetTargetForItems(this);
	fPopUpMenu->Rebuild(path.Path());
}


void
FrontWindow::MessageReceived(BMessage *message)
{
	switch (message->what) {
		case B_ABOUT_REQUESTED: 
		{
			(new BAlert("Info", "BeRomCenter\n\nby Jack Burton", "Ok"))->Go(NULL);
			break;
		}
		case kFPopUpSelChanged:
		{
			BMenuItem *item = NULL;
			if (message->FindPointer("source", (void**)&item) == B_OK) {
				if (item->Menu()->IndexOf(item) != fLastSelected) {
					fLastSelected = item->Menu()->IndexOf(item);
										
					BPath path = static_cast<RomCenterApp *>(be_app)->Path();
					path.Append("datafiles");
					path.Append(item->Label());
					
					Params params;
					params.dataFile = path.Path();
					params.list = &fList;
					
					fListView->MakeEmpty();
					ParseDataFile(&params);
					fListView->AddList(&fList);
					//if (ParseDataFile(&params) == B_OK) {
					//	fTVBuilderThread = spawn_thread(RebuildTreeView, "Builder Thread",
					//				B_NORMAL_PRIORITY, this);
					//	resume_thread(fTVBuilderThread);					
					//}
				}
			}
			break;
		}
		case kLVSelectionChanged:
		{
			fRomInfo->SelectAll();
			fRomInfo->Clear();
			BListItem *selected = fListView->ItemAt(fListView->CurrentSelection());
			if (selected && selected->OutlineLevel() == 1) {
				RomSetItem *rsItem = static_cast<RomSetItem *>(selected);
				int32 itemCount = rsItem->roms.CountItems();
				for (int c = 0; c < itemCount; c++) {
					Rom *rom = static_cast<Rom*>(rsItem->roms.ItemAtFast(c));
					fRomInfo->Insert(rom->name);
					fRomInfo->Insert("\t");
					char crcString[32];
					sprintf(crcString, "%lu", rom->crc);
					fRomInfo->Insert(crcString);
					fRomInfo->Insert("\n");
				}
			}
			break;
		}
		case kCheckRoms:
			fCheckThread = spawn_thread(CheckRoms, "Rom Checker Thread",
							B_NORMAL_PRIORITY, this);
			resume_thread(fCheckThread);
			break;
		default:
			BWindow::MessageReceived(message);
			break;
	}
}


bool
FrontWindow::QuitRequested()
{
	BAlert *alert = new BAlert("YO!", "Are you sure you want to quit?", "Yes", "No");
	int32 button = alert->Go();
	
	if (button == 0) {
		be_app->PostMessage(B_QUIT_REQUESTED);
		status_t retVal;
		wait_for_thread(fCheckThread, &retVal);
		Settings::Save();
		return BWindow::QuitRequested();
	}
	else
		return false;
}


void
FrontWindow::AddMenu()
{
	// fMenuBar
	fMenuBar = new BMenuBar(BRect(0, 0, 0, 0), "fMenuBar", 
				B_FOLLOW_LEFT_RIGHT|B_FOLLOW_TOP, B_ITEMS_IN_ROW, true);
	
	// Create menu
	BMenu *menu = new BMenu("File");
	menu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'));
	fMenuBar->AddItem(menu);
	
	menu = new BMenu("Edit");
	BMenuItem *item = new BMenuItem("Preferences", new BMessage(kEditPrefs), 'P');
	item->SetTarget(be_app);
	menu->AddItem(item);
	fMenuBar->AddItem(menu);
	
	menu = new BMenu("Help");
	menu->AddItem(new BMenuItem("About", new BMessage(B_ABOUT_REQUESTED)));
	fMenuBar->AddItem(menu);
	
	AddChild(fMenuBar); // Add the full menuBar
}


void
FrontWindow::AddListView()
{
	BRect listRect(fView->Bounds());
	listRect.top += 20;
	listRect.left += 10;
	listRect.right = listRect.left + 265;
	listRect.bottom -= 80;
	
	fListView = new BOutlineListView(listRect, "list view",
				B_SINGLE_SELECTION_LIST, B_FOLLOW_ALL);
	BScrollView *scrollView = new BScrollView("Scroll View", fListView, B_FOLLOW_ALL,
				0, false, true);
		
	fView->AddChild(scrollView);
	
	fListView->SetTarget(this);
	fListView->SetSelectionMessage(new BMessage(kLVSelectionChanged));
}


int32
FrontWindow::CheckRoms(void *arg)
{
	/*int32 posted = 0;
	FrontWindow *object = static_cast<FrontWindow *>(arg);
	int32 itemCount = object->fList.CountItems();
	if (object->LockLooper()) {
		object->fStatusBar->Reset();
		object->fStatusBar->SetMaxValue(itemCount);
		object->UnlockLooper();
	};
	
	BMessage message(B_UPDATE_STATUS_BAR);
	message.AddFloat("delta", 1.0);
	
	BPath path = static_cast<RomCenterApp *>(be_app)->Path();
	path.Append(Settings().RomsFolder());
	BDirectory dir(path.Path());
	chdir(path.Path());
	BEntry entry;
	for (int32 c = 0; c < itemCount; c++) {
		ParentSet *parent = static_cast<ParentSet *>(object->fList.ItemAtFast(c));
		BString parentName(parent->name);
		parentName << ".zip";
		if (dir.FindEntry(parentName.String(), &entry, true) == B_OK) {
			unz_global_info info;
			unzFile zipFile = unzOpen(parentName.String());
			if (zipFile != NULL) {
				list<RomSet *>::const_iterator romSet;
				for (romSet = parent->romSets.begin();
							romSet != parent->romSets.end(); romSet++) {
						list<Rom *>::const_iterator rom;
						for (rom = (*romSet)->roms.begin();
								rom != (*romSet)->roms.end(); rom++) {
							unzGetGlobalInfo(zipFile, &info);
							unzGoToFirstFile(zipFile);
							int32 count = info.number_entry;
							while (count-- > 0) {
								unz_file_info fileInfo;
								char name[B_FILE_NAME_LENGTH];
								unzGetCurrentFileInfo(zipFile, &fileInfo, name,
										B_FILE_NAME_LENGTH, NULL, 0, NULL, 0);
								
								if (!strcmp(name, (*rom)->name)) {
									if ((*rom)->size == fileInfo.uncompressed_size
											&& (*rom)->crc == fileInfo.crc) {
										(*rom)->ok = true;
										printf("%s is ok\n", name);
									}
									break;
								}							
								unzGoToNextFile(zipFile);							
							}
						}
				}
				unzClose(zipFile);
				printf("\n");
			}
		}
		posted++;
		object->PostMessage(&message, object->fStatusBar);				
		snooze(100);
	}
	
	object->LockLooper();
	object->fListView->Invalidate();
	object->UnlockLooper();
*/		
	return 0;
}
