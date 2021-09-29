#ifndef DYNAMICLIB_H
#define DYNAMICLIB_H

#include <typeinfo>
using namespace std;

#define _stdcall
#define HNDZ_PASCAL _stdcall

/*attach general class into the LinkedList of RuntimeClass*/
#define HNDZ_DECLARE_DYNAMIC(class_name) \
		public: \
			static  MetaData class##class_name; \
			virtual MetaData* GetMetaData() const; \

#define HNDZ_IMPLEMENT_DYNAMIC(class_name, base_class_name) \
		HNDZ_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, 0xFFFF, NULL)

/*attach general class into the LinkedList of RuntimeClass and support Create object of this class by class name*/
#define HNDZ_DECLARE_DYNCREATE(class_name)\
		HNDZ_DECLARE_DYNAMIC(class_name)\
		static BaseObject* HNDZ_PASCAL CreateBaseObject();

#define HNDZ_IMPLEMENT_DYNCREATE(class_name,base_class_name)\
		BaseObject* HNDZ_PASCAL class_name::CreateBaseObject()\
		{return new class_name;}\
		HNDZ_IMPLEMENT_RUNTIMECLASS(class_name,base_class_name,0xFFFF,class_name::CreateBaseObject)\

/*Provide technology service for above*/
#define HNDZ_RUNTIME_CLASS(class_name) (&class_name::class##class_name)

#define HNDZ_IMPLEMENT_RUNTIMECLASS(class_name, base_class_name, wSchema, pfnNew) \
		static char _lpsz##class_name[]=#class_name;\
		MetaData class_name::class##class_name={\
		_lpsz##class_name,sizeof(class_name),wSchema,pfnNew,\
		HNDZ_RUNTIME_CLASS(base_class_name),NULL};\
		static MetaDataLinker _init_##class_name(&class_name::class##class_name);\
		MetaData* class_name::GetMetaData() const\
		{return &class_name::class##class_name;}\

#include <stdio.h>
#include <string.h>

class BaseObject;
struct MetaData;

/*MetaDataLinker add metadata to linkedlist*/
struct MetaDataLinker
{
	MetaDataLinker(MetaData* pNewClass);
};

struct MetaData
{
	// Attributes
    const char* m_lpszClassName;
	int m_nObjectSize;
	// schema number of the loaded class
	unsigned int m_wSchema; 

	// NULL => abstract class
	BaseObject* (HNDZ_PASCAL *m_pfnCreateObject)(); 

	MetaData* m_pBaseClass;

	// Operations
	BaseObject*  CreateBaseObject();
	// Implementation
    static MetaData* HNDZ_PASCAL Load(const char *pClassName);

	// CRuntimeClass objects linked together in simple list
	static MetaData* m_pFirstClass;
	// linked list of registered classes
	MetaData* m_pNextClass;      
};


void* function(char* pbuf);

/*BaseObject is a root of all the class in the linkedlist ,just like the CObject in MFC*/
class  BaseObject
{
private:
	virtual MetaData* GetMetaData() const;
public:
	static MetaData classBaseObject;

	//judge this object is the subclass of pClass
	bool  IsKindOf(const MetaData* pClass) const;
	//judge this object is the subclass of pClassType
    bool  IsKindOf(const type_info& pClassType) const;

	//judge the type of this object.
	bool  IsInstanceOf(const type_info& pClassType) const;
};

/*CInheritIdentify just judge the relationship of two classes,that exist inherit */
template <class T,class BaseT>
class CInheritIdentify
{

private:
    static char Identify( void* vd )
    {
        return 0;
    }

    static int Identify( BaseT* bt )
    {
        return 1;
    }

public:
    enum
    {
        IdentifyResult = (sizeof(Identify((T*)NULL)) == sizeof(int)),
    };

};


/*ReflectFactory is a factory that produce object and recognise the relationship of two classes in the linkedlist*/
class ReflectFactory
{
public:
    BaseObject* CreateObject(const char *pClassName);
public:
	template<class T>
    T* CreateGivenObject(const char* pClassName);

	template<class T,class BaseT>
	bool IsDeriveOf();
};

template<class T>
T* ReflectFactory::CreateGivenObject(const char* pClassName)
{
	BaseObject* pObj = CreateObject(pClassName);
	if (pObj != NULL)
	{
		return dynamic_cast<T*>(pObj);
	}
	return NULL;
}

template<class T,class BaseT>
bool ReflectFactory::IsDeriveOf()
{
	return CInheritIdentify<T,BaseT>::IdentifyResult;
}


#endif




