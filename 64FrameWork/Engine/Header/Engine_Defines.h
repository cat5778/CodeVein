#ifndef Engine_Defines_h__
#define Engine_Defines_h__

#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <ctime>
#include <process.h>
#pragma warning(disable : 4251)

#define	DIRECTINPUT_VERSION		0x0800
#include <dinput.h>

#include "Engine_Typedef.h"
#include "Engine_Macro.h"

#include "Engine_Function.h"
#include "Engine_Struct.h"
#include "Engine_Enum.h"
#include <stdlib.h>
#include <cassert>
//#ifdef _DEBUG
#include <iostream>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

//#endif // _DEBUG


using namespace std;
#endif // Engine_Defines_h__
