/*/////////////////////////////////////////////////////////////////////////////////
/// This file is part of
///    ___                   _ _ _             
///   /___\__ _ _ __ ___  __| (_) |_ ___  _ __ 
///  //  // _` | '__/ _ \/ _` | | __/ _ \| '__|
/// / \_// (_| | | |  __/ (_| | | || (_) | |   
/// \___/ \__, |_|  \___|\__,_|_|\__\___/|_|   
///       |___/                                
///             Copyright (c) 2010 Jacob 'jacmoe' Moen
/// The MIT License
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
/// THE SOFTWARE. 
///////////////////////////////////////////////////////////////////////////////////*/
#include "paths.h"
#include <OgrePlatform.h>

std::string bundlePath() 
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	
	char path[1024];
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	assert( mainBundle );
	
	CFURLRef mainBundleURL = CFBundleCopyBundleURL( mainBundle);
	assert( mainBundleURL);
	
	CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
	assert( cfStringRef);
	
	CFStringGetCString( cfStringRef, path, 1024, kCFStringEncodingASCII);
	
	CFRelease( mainBundleURL);
	CFRelease( cfStringRef);
	
	return std::string( path);
	
#else
	return "" ;
#endif
}

std::string logsPath() 
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	return bundlePath() + "/Contents/Resources/Logs/" ;
#else
	return bundlePath() + "/Logs/" ;
#endif
}

std::string configPath() 
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	return bundlePath() + "/Contents/Resources/" ;
#else
	return bundlePath() ;
#endif
}

std::string getPluginPath() 
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	return bundlePath() + "/Contents/Resources/Plugins/";
#else
	return bundlePath() + "../Plugins/";
#endif	
}

std::string getDataPath() 
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	return bundlePath() + "/Contents/Resources/Data/";
#else
	return bundlePath() + "../Data/";
#endif	
}
