#include <Roster.h>

//#include <unistd.h>

#include "RomCenterApp.h"
#include "FrontWindow.h"
//#include "SplashWindow.h"

const char *kAppSignature = "application/x-vnd.BeRomCenter";

int main()
{
	RomCenterApp app;
	app.Run();
	
	return 0;
}


RomCenterApp::RomCenterApp()
	:BApplication(kAppSignature)
{
	app_info info;
	if (be_app->GetAppInfo(&info) == B_OK) {
		entry_ref ref = info.ref;
		BEntry entry;
		if (entry.SetTo(&ref) == B_OK) {
			if (fPath.SetTo(&entry) == B_OK) {
				if (fPath.GetParent(&fPath) == B_OK) {
					//chdir(fPath.Path());
				}
			}
		}
	}
	
	fMainWindow = new FrontWindow;
	//fSplash = new TSplashWindow;
}


void
RomCenterApp::ReadyToRun()
{
	fMainWindow->Show();
	BApplication::ReadyToRun();
}


BPath
RomCenterApp::Path() const
{
	return fPath;
}

