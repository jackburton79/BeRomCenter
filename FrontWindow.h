#ifndef __FRONTWINDOW_H
#define __FRONTWINDOW_H

#include <List.h>
#include <Window.h>

class BBox;
class BButton;
class BMenuBar;
class BMenuField;
class BOutlineListView;
class BStatusBar;
class BTextView;
class TFilePopUpMenu;

class FrontWindow : public BWindow
{
public:	
	FrontWindow();
	virtual bool QuitRequested();
	virtual void MessageReceived(BMessage *);
private:
	BMenuBar *fMenuBar;
	
	BStatusBar *fStatusBar;
	
	BOutlineListView *fListView;
	BButton *fCheckButton;
	TFilePopUpMenu *fPopUpMenu;
	BMenuField *fDataFilesMenuField;
	int32 fLastSelected;
	
	BTextView *fRomInfo;
	BBox *fView;
	
	BList fList;
	thread_id fCheckThread;
		
	static int32 CheckRoms(void *arg);

	void AddMenu();
	void AddListView();
};


#endif
