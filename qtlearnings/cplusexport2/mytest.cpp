#include "mytest.h"

MyTest::MyTest(QObject* parent)
    : QObject(parent),m_msg(NULL),m_model(NULL),m_timer(NULL)
{
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
}
MyTest::~MyTest()
{
    delete m_timer;
    m_timer = NULL;
}
