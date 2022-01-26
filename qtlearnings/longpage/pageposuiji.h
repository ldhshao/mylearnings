#ifndef PAGEPOSUIJI_H
#define PAGEPOSUIJI_H

#include <QWidget>

namespace Ui {
class PagePosuiji;
}

class PagePosuiji : public QWidget
{
    Q_OBJECT

public:
    explicit PagePosuiji(QWidget *parent = nullptr);
    ~PagePosuiji();

private:
    Ui::PagePosuiji *ui;
};

#endif // PAGEPOSUIJI_H
