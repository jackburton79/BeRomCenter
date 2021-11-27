#ifndef __ROMCENTERAPP_H
#define __ROMCENTERAPP_H

#include <Application.h>
#include <Path.h>

class BWindow;
class RomCenterApp : public BApplication
{
public:
	RomCenterApp();
	virtual void ReadyToRun();
	BPath Path() const;
private:
	BWindow *fMainWindow;
	BPath fPath;
	//BWindow *fSplash;
};


#endif // __ROMCENTERAPP_H
