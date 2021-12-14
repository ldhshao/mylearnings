#ifndef IBINDOBJ_H
#define IBINDOBJ_H

#include "QKeyEvent"

enum{
    kd_null = -1,
    kd_check,
    kd_emit,
    kd_check_emit,
    kd_show,
    kd_show_emit
};

class IBindObj
{
public:
    IBindObj():_state(BS_INIT){}
    virtual ~IBindObj(){} // 使用父类指针析构子类对象

    enum {
        BS_INIT = 0,
        BS_MODIFIED = 1,
        BS_SOURCEKEY = 0x10
    };

    virtual int keyEventFilter(QKeyEvent* ev) = 0;

    virtual QString checkSet() = 0;
    virtual QString showSet() = 0;
    virtual void clear() = 0;
    virtual bool setValue(const QString& strVal) { return false; }

    uint8_t state() { return _state; }
    bool    isModified() { return (_state & BS_MODIFIED); }
    bool    isFromKeyEvent() { return (_state & BS_SOURCEKEY); }
    uint8_t setState(uint8_t s) { _state = (s | _state); return _state; }
    uint8_t clearState() { return _state = BS_INIT; }
protected:
    uint8_t _state;
};

#endif // IBINDOBJ_H
