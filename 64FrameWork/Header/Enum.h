#ifndef Enum_h__
#define Enum_h__

enum RESOURCEID { 
				RESOURCE_STATIC,
				RESOURCE_LOGO, 
				RESOURCE_STAGE, 
				RESOURCE_END 
				};

enum LOADINGID {
				LOADING_STAGE,
				LOADING_BOSS, 
				LOADING_END 
				};

enum DIRECTIONFLAG{
					DIR_F = 1,
					DIR_R = 2,
					DIR_FR= 3,
					DIR_B = 4,
					DIR_BR= 6,
					DIR_L = 8,
					DIR_FL= 9,
					DIR_BL=12,
					};


#endif // Enum_h__
