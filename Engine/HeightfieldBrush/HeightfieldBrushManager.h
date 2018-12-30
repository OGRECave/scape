/**
 * ScapeEngine::Brush class
 *
 * Manages all HeightfieldBrushes
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#ifndef __HEIGHTFIELDBRUSHMANAGER_H__
#define __HEIGHTFIELDBRUSHMANAGER_H__

#include "ScapeEnginePrerequisites.h"

namespace ScapeEngine
{

class HeightfieldBrushManager
{
public:
    HeightfieldBrushManager();
    ~HeightfieldBrushManager();

    HeightfieldBrush* getBrush(const string& brushName);

    HeightfieldBrush* getCurrentBrush() const;

private:
    typedef std::map<string, HeightfieldBrush*> Brushes;
    Brushes mBrushes;
};
}

#endif // __HEIGHTFIELDBRUSHMANAGER_H__