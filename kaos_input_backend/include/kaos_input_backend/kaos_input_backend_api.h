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

#ifndef __KAOS_INPUT_USES_POISED_INTERFACES__ //DO NOT DEFINE THIS
//#error "Poisoned interfaces has been used in a context that it is not suposed too. If you are seeing this message there's something gwrong with your configuration!"
#endif // !__KAOS_INPUT_USES_POISED_INTERFACES__
