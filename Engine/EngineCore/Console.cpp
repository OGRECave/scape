/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */

#include "PCH/stdafx.h"

#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <string>

#include "Console.h"

using namespace ScapeEngine;

#ifdef _DEBUG
#define CREATE_OWN_CONSOLE
#endif

#ifdef CREATE_OWN_CONSOLE

// ----------------------------------------------------------------------------
void Console::openConsole()
{
	static const WORD MAX_CONSOLE_LINES = 1000;
	int hConHandle;
	UINT_PTR lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;

	// allocate a console for this app
	AllocConsole();

	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),	coninfo.dwSize);

	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_MAXIMIZE);

	// redirect unbuffered STDOUT to the console
	lStdHandle = (UINT_PTR)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );

	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	lStdHandle = (UINT_PTR)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	lStdHandle = (UINT_PTR)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well
	std::ios::sync_with_stdio();
}

#else // CREATE_OWN_CONSOLE

typedef std::basic_ostream<TCHAR> ostream_t; 

/// contains globals data and related functions
class DebugStream
{
	// ----------------------------------------------------------------------------
	/// stream buffer
	class streambuff: 
		public std::basic_streambuf<TCHAR>
	{
		static const size_t BUFFSZ = 128;
		TCHAR buffer[BUFFSZ];
	public:

		// ----------------------------------------------------------------------------
		// imitialize the debug library
		streambuff()
		{
			/*
			_CrtSetDbgFlag(
				_CRTDBG_LEAK_CHECK_DF|
				_CRTDBG_ALLOC_MEM_DF|
				_CRTDBG_CHECK_ALWAYS_DF);
			*/

			OutputDebugString(_T("Opening debug stream\n"));
			pubsetbuf(buffer,BUFFSZ-1);
		}

		// ----------------------------------------------------------------------------
		// finalize the debug
		~streambuff()
		{
			OutputDebugString(_T("deleting debug stream buffer\n"));
		}

		// ----------------------------------------------------------------------------
		// makes the streambuff working on \c buffer
		virtual basic_streambuf* setbuf(TCHAR* _Buffer, std::streamsize _Count)
		{
			setp(buffer,buffer,buffer+BUFFSZ-1);
			return this;
		}


		// ----------------------------------------------------------------------------
		// sync and rewind when full
		virtual int_type overflow(int_type i = traits_type::eof()) 
		{
			sync();
			*pptr() = traits_type::to_char_type(i);
			pbump(1);
			return traits_type::not_eof(i);
		}


		// ----------------------------------------------------------------------------
		// spit the buffer to the debugger
		virtual int sync()
		{
			char_type* p = pbase();
			int sz = (int)(pptr()-p);
			p[sz] = 0;
			OutputDebugString(p);
			pbump(-sz);
			return 0;
		}
	};

	// ----------------------------------------------------------------------------
	streambuff buff;
	ostream_t stream; 

public:
	// ----------------------------------------------------------------------------
	// initialize the stream
	DebugStream() : stream(&buff)
	{}

	ostream_t& operator()()
	{
		return stream;
	}
};

static DebugStream& debugStream() {static DebugStream g; return g;}


// ----------------------------------------------------------------------------
void Console::openConsole()
{
	ostream_t& stream = debugStream()();

	std::cout.rdbuf(stream.rdbuf());
	std::cerr.rdbuf(stream.rdbuf());
}

#endif // CREATE_OWN_CONSOLE