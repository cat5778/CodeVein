#ifndef Macro_h__
#define Macro_h__

#define  WINCX 1280
#define  WINCY 800

#define MAX_STR 256
#define MIN_STR 128
#define NO_EVENT		0
#define CHANGE_SCENE	1
#define VTXCNTX 129
#define VTXCNTZ 129
#define VTXITV  1
#define DEFAULT_MESH_SCALE 0.01f,0.01f,0.01f
#define INIT_VEC3 0.f,0.f,0.f

#ifndef MSG_BOX
#define	MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#ifdef _DEBUG
#define DE_COUT(_message) wcout<<_message<<endl;
#endif // !_DEBUG
#ifndef _DEBUG
#define DE_COUT(_message) wstring wstrOut = _message;
#endif

#define MYNULL (_hr)							\
{if( ((HRESULT)_hr)<0)assert(_hr==E_FAIL);}		

//#define NULL_CHECK64( _ptr)	\
//{if( _ptr == 0){return;}}

#define NULL_CHECK( _ptr)	\
{if( _ptr == 0){/*_asm { int 3 };*/return;}}

#define NULL_CHECK_RETURN( _ptr, _return)	\
{if( _ptr == 0){/*_asm { int 3 };*/return E_FAIL;}}

#define NULL_CHECK_MSG( _ptr, _message )		\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);/*_asm { int 3 };*/}}

#define NULL_CHECK_RETURN_MSG( _ptr, _return, _message )	\
{if( _ptr == 0){MessageBox(NULL, _message, L"System Message",MB_OK);/*_asm { int 3 };*/return E_FAIL;}}


#define FAILED_CHECK(_hr)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); /*_asm { int 3 };*/return E_FAIL;}

#define FAILED_CHECK_RETURN(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )		\
{ MessageBoxW(NULL, L"Failed", L"System Error",MB_OK); /*_asm { int 3 };*/return E_FAIL;}

#define FAILED_CHECK_MSG( _hr, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, _message, L"System Message",MB_OK); /*_asm { int 3 };*/return E_FAIL;}

#define FAILED_CHECK_RETURN_MSG( _hr, _return, _message)	if( ((HRESULT)(_hr)) < 0 )	\
{ MessageBoxW(NULL, _message, L"System Message",MB_OK); /*_asm { int 3 };*/return E_FAIL;}



#define NO_COPY(CLASSNAME)							\
	private:										\
	CLASSNAME(const CLASSNAME&);					\
	CLASSNAME& operator = (const CLASSNAME&);		

#define DECLARE_SINGLETON(CLASSNAME)				\
	NO_COPY(CLASSNAME)								\
	private:										\
	static CLASSNAME*	m_pInstance;				\
	public:											\
	static CLASSNAME*	GetInstance( void );		\
	static void DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)				\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;		\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{	\
		if(NULL == m_pInstance) {					\
			m_pInstance = new CLASSNAME;			\
		}											\
		return m_pInstance;							\
	}												\
	void CLASSNAME::DestroyInstance( void ) {		\
		if(NULL != m_pInstance)	{					\
			delete m_pInstance;						\
			m_pInstance = NULL;						\
		}											\
	}


#endif // Macro_h__
