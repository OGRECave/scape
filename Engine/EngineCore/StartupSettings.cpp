#include "ScapeEngineStableHeaders.h"

#include "StartupSettings.h"

namespace ScapeEngine
{

StartupSettings::StartupSettings() : mHeightfieldRows(0), mHeightfieldColumns(0), mHeightfieldHeight(0) {}

StartupSettings::~StartupSettings() {}

int StartupSettings::getHeightfieldRows() const { return mHeightfieldRows; }

void StartupSettings::setHeightfieldRows(int heightfieldRows) { mHeightfieldRows = heightfieldRows; }

int StartupSettings::getHeightfieldColumns() const { return mHeightfieldColumns; }

void StartupSettings::setHeightfieldColumns(int heightfieldColumns)
{
    mHeightfieldColumns = heightfieldColumns;
}

Ogre::Real StartupSettings::getHeightfieldHeight() const { return mHeightfieldHeight; }

void StartupSettings::setHeightfieldHeight(Ogre::Real heightfieldHeight)
{
    mHeightfieldHeight = heightfieldHeight;
}
}
