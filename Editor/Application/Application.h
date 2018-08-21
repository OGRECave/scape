/**
 * ScapeEditor::Application class
 *
 * Initializes wxWidgets and creates and updates the main window. 
 * The .cpp contains the application entry point
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

namespace ScapeEditor
{
	// ----------------------------------------------------------------------------
	class MainFrame;


	// ----------------------------------------------------------------------------
	class Application : public wxApp
	{
	protected:
		MainFrame* mMainFrame;

		virtual bool OnInit();
		virtual bool ProcessIdle(); 
		virtual int OnRun();

		virtual bool OnExceptionInMainLoop();
		virtual void OnFatalException();
		virtual void OnUnhandledException();
	};
}

#endif //__APPLICATION_H__
