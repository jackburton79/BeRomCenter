#include "Settings.h"

#include <Entry.h>
#include <File.h>
#include <FindDirectory.h>
#include <Message.h>

const char *fileName = "BeRomCenter";
const int32 kSettingsMessage = 'StMs';

BPath Settings::sPath;
BMessage *Settings::sSettings = NULL;

Settings::Settings()
{
}


status_t
Settings::Load()
{
	if (find_directory(B_USER_SETTINGS_DIRECTORY, &sPath) == B_OK) {
		sPath.Append(fileName);
		//BFile settingsFile(sPath.Path(), B_READ_ONLY);
		//if (settingsFile.InitCheck() == B_OK) {
			sSettings = new BMessage(kSettingsMessage);
			//sSettings->Unflatten(&settingsFile);
			return B_OK;
		//}
	}
	
	return B_ERROR;
}


status_t
Settings::Save()
{
/*
	BEntry entry(sPath.Path());
	if (!entry.Exists()) {
		BFile settingsFile(sPath.Path(), B_READ_WRITE|B_CREATE_FILE);
		sSettings->AddString("roms_folder", "roms");
		sSettings->Flatten(&settingsFile);
		delete sSettings;
	}
*/	
	return B_OK;
}


const char *
Settings::RomsFolder() const
{
	//const char *folder = NULL;
	const char *folder = "roms";
	//sSettings->FindString("roms_folder", &folder);
	
	return folder;	
}
