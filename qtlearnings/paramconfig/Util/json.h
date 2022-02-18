#ifndef JSON_H
#define JSON_H

#ifdef USE_JSON_SRC
#include <jsk_json.h>
#include <QString>
#define QJsonObject QT_JSON::JSONObject
#else
#include <QJsonObject>
#include <QJsonValue>
#endif

static bool getJsonObjectIntVal(QJsonObject* obj, const QString& key, int* pVal){
#ifdef USE_JSON_SRC
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
#ifdef USE_JSON_SRC
        obj->add(key, val);
#else
        obj->insert(key, val);
#endif
    }
}
static void setJsonObjectStrVal(QJsonObject* obj, const QString& key, const QString& val){
    if (nullptr != obj && !key.isEmpty()){
#ifdef USE_JSON_SRC
        obj->add(key, val);
#else
        obj->insert(key, val);
#endif
    }
}

#endif
