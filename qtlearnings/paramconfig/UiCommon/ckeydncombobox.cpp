#include "ckeydncombobox.h"
#include <QKeyEvent>
#include <QDebug>
//#include <QMessageBox>


CKeyDnComboBox::CKeyDnComboBox(QWidget *parent) :
    CStateComboBox(parent), pVal(nullptr)
{
    updating = false;
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_currentIndexChanged(int)));
}

void CKeyDnComboBox::updateText()
{
    updating = true;
    if (nullptr != pVal){
        setCurrentIndex(static_cast<int>(*pVal));
    }
    updating = false;
}

bool CKeyDnComboBox::setEditText(const QString &strText)
{
    bool bRet = true;
    int iFind = findText(strText);
    if (-1 < iFind){
        setCurrentIndex(iFind);
    }

    return bRet;
}

void CKeyDnComboBox::focusInEvent(QFocusEvent *ev)
{
    //setStyleSheet("background-color:rgba(200,60,60,100%)");
    QComboBox::focusInEvent(ev);
}
void CKeyDnComboBox::focusOutEvent(QFocusEvent *ev)
{
    //setStyleSheet("");
    QComboBox::focusOutEvent(ev);
}

void CKeyDnComboBox::keyPressEvent(QKeyEvent *ev)
{
    // 未绑定数据直接返回按键事件
    qDebug()<<"combobox key press";
    int iCurr = currentIndex();
    int iCnt = count();
    int keyVal = ev->key();
    switch (keyVal) {
    case Qt::Key_Left:
        iCurr = (iCurr + iCnt - 1) % iCnt;
        break;
    case Qt::Key_Right:
        iCurr = (iCurr + iCnt + 1) % iCnt;
        break;
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Return:
    case Qt::Key_Escape:
        ev->setAccepted(false);
        break;
    default:
        break;
    }

    setCurrentIndex(iCurr);
}

void CKeyDnComboBox::slot_currentIndexChanged(int index)
{
    if (nullptr != pVal && -1 < index){
        uint16_t val = static_cast<uint16_t>(itemData(index).toInt());
        if (val != *pVal){
            *pVal = val;
            if (!updating)
                emit sig_valueChanged(pVal, *pVal);
        }
    }
}

/////CKeyDnComboBoxSet//////////////////////////////
CKeyDnComboBoxSet::CKeyDnComboBoxSet(QWidget *parent) :
    CKeyDnComboBox(parent)
{
    dataCnt = 0;
    dataIdx = 0;
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_currentIndexChanged(int)));
}

CKeyDnComboBoxSet::~CKeyDnComboBoxSet()
{
}

void CKeyDnComboBoxSet::initData(uint16_t* pAddr, uint16_t cnt)
{
    if (nullptr != pAddr && 0 < cnt){
        pVal = pAddr;
        dataCnt = cnt;
        dataIdx = 0;
        setDataIndex(dataIdx);
        emit sig_dataIndexChanged(dataIdx);
    }
}

void CKeyDnComboBoxSet::initUi()
{
    initData(pVal, dataCnt);
}

void CKeyDnComboBoxSet::setDataIndex(int idx)
{
    if (-1 < idx && idx < dataCnt){
        setCurrentIndex(pVal[idx]);
        dataIdx = idx;
    }
}

void CKeyDnComboBoxSet::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_T:
        onPrevData();
        break;
    case Qt::Key_K:
        onNextData();
        break;
    default:
        CKeyDnComboBox::keyPressEvent(event);
        break;
    }
}

void CKeyDnComboBoxSet::focusInEvent(QFocusEvent *event)
{
    CKeyDnComboBox::focusInEvent(event);
}

void CKeyDnComboBoxSet::slot_dataSetChanged(uint16_t* pAddr, uint16_t setSize)
{
    initData(pAddr, setSize);
}

void CKeyDnComboBoxSet::slot_currentIndexChanged(int index)
{
    if (nullptr != pVal && dataIdx < dataCnt){
        if (pVal[dataIdx] != index){
            pVal[dataIdx] = index;
            if (!updating)
                emit sig_valueChanged(pVal, *pVal);
        }
    }
}

void CKeyDnComboBoxSet::onPrevData()
{
    if (0 < dataCnt){
        dataIdx = (dataIdx - 1 + dataCnt) % dataCnt;
        setDataIndex(dataIdx);
        emit sig_dataIndexChanged(dataIdx);
    }
}

void CKeyDnComboBoxSet::onNextData()
{
    if (0 < dataCnt){
        dataIdx = (dataIdx + 1) % dataCnt;
        setDataIndex(dataIdx);
        emit sig_dataIndexChanged(dataIdx);
    }
}
void CKeyDnComboBoxSet::onDataCountChanged(uint16_t dataCount)
{
    dataCnt = dataCount;
    if (dataIdx >= dataCount){
        dataIdx = 0;
        setDataIndex(dataIdx);
        emit sig_dataIndexChanged(dataIdx);
    }
}

////CIndexLabel
CIndexLabel::CIndexLabel(QWidget *parent): QLabel(parent)
{

}
void CIndexLabel::slot_dataIndexChanged(int dataIdx)
{
    QString name(strDataName);
    setText(name.append(QString::number(dataIdx+1)));
    //int width = QFontMetrics(font()).width(text()); //has set a fit size
    //resize(width, height());
}
