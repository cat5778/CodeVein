#ifndef Struch_h__
#define Struch_h__


typedef struct Mesh_Path
{
	wstring wstrRelative;	//
	wstring wstrMeshType;   //Static or Dynamic
	wstring wstrGroup;		//Mesh Group
	wstring wstrMap;		//Mesh Map Folder
	wstring wstrObjectType;	//Mesh Type Ex: Pillar, Grass, Tree
	wstring wstrName;		//Mesh File Name
}MESH_PATH;

typedef struct Effect_Path
{
	wstring wstrType;
	wstring wstrName;
}EFFECT_PATH;

typedef struct Transform_Info
{
	_vec3	vScale;
	_vec3	vRotation;
	_vec3	vPosition;
}TRANSFORM_INFO;
typedef struct NavMesh
{
	_vec3	vPoint1;
	_vec3	vPoint2;
	_vec3	vPoint3;
	_uint	uiNavMeshIdx;
	_float	fRadius;

}NAV_MESH;

typedef struct RAY
{
	_vec3	vDir;
	_vec3	vPos;
}RAY;

typedef struct Collider_data
{
	wstring wstrObject;
	wstring wstrBone;
	_vec3	vPos;
	_float  fRadius;
}COLL_DATA;

#endif // Struch_h__
