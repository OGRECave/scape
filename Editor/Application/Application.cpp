/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "PCH/stdafx.h"

#include "wx/dcmirror.h"

#include "Application.h"
#include "EngineInterface.h"
#include "Windows/MainFrame.h"

using namespace ScapeEditor;

// ----------------------------------------------------------------------------
IMPLEMENT_APP(Application)

// ----------------------------------------------------------------------------
bool Application::OnInit()
{
    wxHandleFatalExceptions(true);

    //#ifdef _DEBUG
    ScapeEngine::EngineInterface::openConsole();
    //#endif

    mMainFrame = new MainFrame(this);

    mMainFrame->Show(true);

    return true;
}

// ----------------------------------------------------------------------------
int Application::OnRun()
{
    return wxApp::OnRun();
}

// ----------------------------------------------------------------------------
bool Application::ProcessIdle()
{
    if (mMainFrame)
        mMainFrame->onProcessIdle();
    wxApp::ProcessIdle();
    return true;
}

// ----------------------------------------------------------------------------
bool Application::OnExceptionInMainLoop()
{
    ScapeEngine::EngineInterface::onFatalException();
    wxApp::OnExceptionInMainLoop();
    throw;
}

// ----------------------------------------------------------------------------
void Application::OnFatalException()
{
    ScapeEngine::EngineInterface::onFatalException();
    wxApp::OnFatalException();
}

// ----------------------------------------------------------------------------
void Application::OnUnhandledException()
{
    ScapeEngine::EngineInterface::onFatalException();
    wxApp::OnUnhandledException();
    throw;
}