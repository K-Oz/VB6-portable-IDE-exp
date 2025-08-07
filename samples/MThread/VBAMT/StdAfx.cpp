/*	Copyright (C) Microsoft Corporation, 1999.  All rights reserved.


	This source code is intended only as a supplement to Microsoft
	Development Tools and/or on-line documentation.  See these other
	materials for detailed information regarding Microsoft code samples.
*/

// stdafx.cpp : source file that includes just the standard includes
//	VBAMT.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

#include <apcguids.h>
#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif
#include <atlimpl.cpp>
#include <apcimpl.cpp>

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()