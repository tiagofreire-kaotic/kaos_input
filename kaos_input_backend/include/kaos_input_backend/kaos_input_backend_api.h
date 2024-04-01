//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///		Export definition for kaos_input_backend
///
///	\copyright
///		Copyright (c) Kaotic Software
/// 
//======== ======== ======== ======== ======== ======== ======== ========

#pragma once

#include <extension/dll_api_macros.h>

#ifdef _kaos_input_backend_EXPORTS_
#	define kaos_input_backend_API DLL_EXPORT
#else
#	define kaos_input_backend_API DLL_IMPORT
#endif // _kaos_input_backend_EXPORTS_
