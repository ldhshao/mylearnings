#ifndef SPLITERTEST_H
#define SPLITERTEST_H

#include <QWidget>

namespace Ui {
class SpliterTest;
}

class SpliterTest : public QWidget
{
    Q_OBJECT

public:
    explicit SpliterTest(QWidget *parent = nullptr);
    ~SpliterTest();

private:
    Ui::SpliterTest *ui;
};

#endif // SPLITERTEST_H
