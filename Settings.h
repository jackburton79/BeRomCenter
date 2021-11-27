#ifndef __SETTINGS_H
#define __SETTINGS_H

#include <Path.h>

class BMessage;
class Settings
{
public:
	Settings();
	static status_t Load();
	static status_t Save();
	
	const char *RomsFolder() const;
	
private:
	static BPath sPath;
	static BMessage *sSettings;
	static bool sLoaded;
};

#endif // __SETTINGS_H
