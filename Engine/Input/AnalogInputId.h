/**
 * Enumeration of all analog input identifiers
 *
 * G.J.P. de Carpentier, 2007 
 */


#ifndef __ANALOGINPUTID_H__
#define __ANALOGINPUTID_H__


namespace ScapeEngine
{
	namespace AnalogInputId
	{
		// Enumeration of all analog input identifiers
		enum EAnalogInputId 
		{
			// Absolute pointer position in pixels, clamped to screen resolution
			POINTER_X,
			POINTER_Y,
			POINTER_Z,

			// Difference between absolute pointer position of current and 
			// last frame (bound and possibly accelerated)
			POINTER_DELTAX,
			POINTER_DELTAY,
			POINTER_DELTAZ,

			// Raw pointer units moved since last frame (unbounded, unaccelerated and input-device-resolution-dependent)
			POINTER_MOVEX,
			POINTER_MOVEY,
			POINTER_MOVEZ,

			// If present and pressed, a value between 0.0 and 1000 (inclusive) indicating the pointer pressure.
			// If not present of not pressed, this input will return -1
			POINTER_PRESSURE

		};
	}
}

#endif //__ANALOGINPUTID_H__