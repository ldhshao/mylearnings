#include "DynamicLib.h"


/* CLASSINIT Implement*/
MetaDataLinker::MetaDataLinker(MetaData* pNewClass)
{
	pNewClass->m_pNextClass = MetaData::m_pFirstClass;
	MetaData::m_pFirstClass = pNewClass;
}
static MetaDataLinker _init_IObject(&BaseObject::classBaseObject);

/* IRuntimeClass Implement*/
MetaData* MetaData::m_pFirstClass = NULL;
BaseObject* MetaData::CreateBaseObject()
{
	if (m_pfnCreateObject == NULL)
	{
		return NULL;
	}

	BaseObject* pObject = NULL;
	pObject = (*m_pfnCreateObject)();

	return pObject;
}

MetaData* HNDZ_PASCAL MetaData::Load(const char* pClassName)
{
	if (pClassName == NULL)
	{
		return NULL;
	}

	MetaData* pClass;

	for (pClass = m_pFirstClass;pClass !=NULL;pClass=pClass->m_pNextClass)
	{
		if (strcmp(pClassName,pClass->m_lpszClassName) == 0)
		{
			return pClass;
		}
	}

	return NULL;
}

/* IObject Implement*/
struct MetaData BaseObject::classBaseObject = {"BaseObject",sizeof(BaseObject),0xFFFF,NULL,NULL,NULL};

MetaData* BaseObject::GetMetaData() const
{
	return &BaseObject::classBaseObject;
}

bool  BaseObject::IsKindOf(const MetaData* pRuntimeClass) const
{
	MetaData* pClassThis = GetMetaData();
	while (pClassThis != NULL)
	{
		if (pClassThis == pRuntimeClass)
		{
			return true;
		}
		pClassThis = pClassThis->m_pBaseClass;
	}

	return false;
}

bool  BaseObject::IsKindOf(const type_info& pClassType) const
{
	char *pClassName = strtok((char*)pClassType.name()," ");
	pClassName = strtok(NULL," ");
	if (pClassName == NULL)
	{
		return false;
	}

	MetaData* pClassThis = this->GetMetaData();

	while (pClassThis != NULL)
	{
		if (strcmp(pClassName,pClassThis->m_lpszClassName) == 0)
		{
			return true;
		}
		pClassThis = pClassThis->m_pBaseClass;
	}

	return false;
}

bool  BaseObject::IsInstanceOf(const type_info& pClassType) const
{
	char *pClassName = strtok((char*)pClassType.name()," ");
	pClassName = strtok(NULL," ");
	if (pClassName == NULL)
	{
		return false;
	}

	MetaData* pClassThis = this->GetMetaData();

	if (strcmp(pClassName,pClassThis->m_lpszClassName) == 0)
	{
		return true;
	}

	return false;
}

/*ReflectFactory Implement*/
BaseObject* ReflectFactory::CreateObject(const char *pClassName)
{
	if (pClassName == NULL)
	{
		return NULL;
	}

	MetaData* pClassRef = NULL;
	pClassRef = MetaData::Load(pClassName);
	if (pClassRef != NULL)
	{
		return pClassRef->CreateBaseObject();
	}

	return NULL;
}


