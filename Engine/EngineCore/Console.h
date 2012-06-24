/**
 * ScapeEngine::Console class
 *
 * Opens simple input/output text console and redirects all standard outputs to it.
 *
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details. 
 */


#ifndef __CONSOLE_H__
#define __CONSOLE_H__


namespace ScapeEngine 
{

	class Console
	{

	public:
		// Open text console, redirecting all standard outputs to it
		static void openConsole();
	};
}

#endif // __CONSOLE_H__

