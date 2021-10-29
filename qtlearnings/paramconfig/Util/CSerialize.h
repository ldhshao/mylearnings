///////////////////////////////////////////////////////////
//  CSerialize.h
//  Implementation of the Class CSerialize
//  Created on:      4-3-2015 
//  Original author: DLL
///////////////////////////////////////////////////////////

#ifndef _CSERIALIZE_H_
#define _CSERIALIZE_H_
#include<QString>

class CSerialize
{
public: 
    CSerialize(){}
    ~CSerialize(){}

public:

	// Use class or struct name
    static bool Serialize(void* pAddr, unsigned int size, const char *blockname);
    static bool DeSerialize(void* pAddr,unsigned int size,const char* blockname);

	// Use path string
    //static void SerializeEx(void* pAddr,unsigned int size,char* filePath);
    static bool SerializeEx(void* pAddr,unsigned int size,QString filePath);
    //static void DeSerializeEx(void* pAddr,unsigned int size,char* filePath);
    static bool DeSerializeEx(void* pAddr,unsigned int size,QString filePath);

};
#endif
