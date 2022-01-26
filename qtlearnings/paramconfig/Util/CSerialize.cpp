///////////////////////////////////////////////////////////
//  CSerialize.h
//  Implementation of the Class CSerialize
//  Created on:      4-3-2015 
//  Original author: DLL
///////////////////////////////////////////////////////////

#include "CSerialize.h"
#include <stdio.h>
#include <sys/stat.h>
#include <QDir>
#include "Common.h"



/*
** Define path and extension string.
*/
#define SERIALIZEFILEPATH  "/Par/"
#define EXTENSION ".bin"


bool CSerialize::Serialize(void* pAddr,unsigned int size,const char* blockname)
{
//        QString  str = QDir::currentPath()+SERIALIZEFILEPATH;//2017-01-20 注释
        QString  str = PATH_PARAMETER;//+SERIALIZEFILEPATH;//2017-01-20 添加
        str+=SERIALIZEFILEPATH;

    return SerializeEx(pAddr,size,str+QString(blockname)+EXTENSION);
}

bool CSerialize::SerializeEx(void* pAddr,unsigned int size,QString filePath)
{
    FILE* fp = fopen(filePath.toStdString().c_str(),"wb+");
    if(fp != NULL)
    {
        fwrite(pAddr,1,size,fp);
        fclose(fp);
        return true;
    }
    return false;
}

bool CSerialize::DeSerialize(void* pAddr, unsigned int size, const char *blockname)
{
//    QString  str = QDir::currentPath()+SERIALIZEFILEPATH;//2017-01-20 注释
    QString  str = PATH_PARAMETER;//+SERIALIZEFILEPATH;//2017-01-20 添加
    str+=SERIALIZEFILEPATH;

    return DeSerializeEx(pAddr,size,str+QString(blockname)+EXTENSION);
}

bool CSerialize::DeSerializeEx(void* pAddr,unsigned int size,QString filePath)
{    
    unsigned long filesize = -1;
    struct stat statbuff;
    if(stat(filePath.toStdString().c_str(), &statbuff) < 0){
        qDebug("%s不存在...\n", filePath.toStdString().c_str());
        return false;
    }else{
        filesize = statbuff.st_size;
    }
    if(filesize != size){
        qDebug("参数文件错误, 使用默认值...\n");
        return false;
    }
    else
    {
        FILE* fp = fopen(filePath.toStdString().c_str(),"rb+");
        if(fp != NULL)
        {
//            size_t len = fread(pAddr,1,size,fp);//2017-03-07 注释
            fread(pAddr,1,size,fp);//2017-03-07 添加
            fclose(fp);
            return true;
        }
    }
    qDebug("读取参数文件%s失败 ...\n", filePath.toStdString().c_str());
    return false;
}

