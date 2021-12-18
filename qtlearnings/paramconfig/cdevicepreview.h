#ifndef CDEVICEPREVIEW_H
#define CDEVICEPREVIEW_H

#include <QWidget>
class GroupCfgItem;
class UiPage;

namespace Ui {
class CDevicePreview;
}

class CDevicePreview : public QWidget
{
    Q_OBJECT

public:
    explicit CDevicePreview(QWidget *parent = nullptr);
    ~CDevicePreview();

    void updateByDevice(GroupCfgItem* dev);
    void showConfigPage(GroupCfgItem* dev);
    void clearConfigPage();
    bool isConfiging() { return (nullptr != pageCfg); }

protected:
    void onResize(int width, int height);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

protected slots:
    void slot_configFinished();

private:
    Ui::CDevicePreview *ui;
    GroupCfgItem       *devUiCfg;
    UiPage             *pageCfg;
};

#endif // CDEVICEPREVIEW_H
