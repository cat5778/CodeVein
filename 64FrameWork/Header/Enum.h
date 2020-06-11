#ifndef Enum_h__
#define Enum_h__
enum LOADMODE
{
	LOAD_NOMAL,
	LOAD_PLAYER,
	LOAD_MONSTER,
	LOAD_END
};
enum RESOURCEID		{ 
					RESOURCE_STATIC,
					RESOURCE_LOGO, 
					RESOURCE_STAGE, 
					RESOURCE_END 
					};

enum LOADINGID		{	
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

enum OBJECTSTATE{
					OBJ_IDLE,
					OBJ_WALK,
					OBJ_RUN,
					OBJ_DODGE,
					OBJ_JUMP,
					OBJ_FALL,
					OBJ_ATTACK,
					OBJ_STRONG_ATTACK,
					OBJ_CHARGE_ATTACK,
					OBJ_GUARD,
					OBJ_HURT,
					OBJ_DEAD,
					OBJ_START,
					OBJ_END
};
enum COLLIDEROPTION
{
	COLL_OPT_OBJECT,
	COLL_OPT_ATTACK,
	COLL_OPT_HURT,
	COLL_OPT_END
};



#endif // Enum_h__
