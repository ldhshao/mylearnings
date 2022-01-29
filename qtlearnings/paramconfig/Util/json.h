#ifndef JSON_H
#define JSON_H

#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
#include <jsk_json.h>
#include <QString>
#define QJsonObject QT_JSON::JSONObject

#else
#include <QJsonObject>
#include <QJsonValue>
#endif

static bool getJsonObjectIntVal(QJsonObject* obj, const QString& key, int* pVal){
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
    if (nullptr != obj && obj->exists(key)){
        bool bOK = false;
        int val = obj->value(key).toInt(&bOK);
        if (bOK) {
            *pVal = val;
            return true;
        }
    }
#else
    QJsonValue valTmp = obj->value(key);
    if (!valTmp.isUndefined() && valTmp.isDouble()){
        *pVal = valTmp.toInt();
        return true;
    }
#endif
    return false;
}

static void setJsonObjectIntVal(QJsonObject* obj, const QString& key, int val){
    if (nullptr != obj && !key.isEmpty()){
#if (QT_VERSION < QT_VERSION_CHECK(5,0,0))
        obj->add(key, val);
#else
        obj->insert(key, val);
#endif
    }
}

#endif
