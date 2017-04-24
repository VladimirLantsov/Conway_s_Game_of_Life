#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "world.h"

// ============================================================================

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QScrollArea* scrollArea;
    World* world;
    QToolBar* toolBar;

    QString screenshotPath;

    QMenu* runMenu;

    QIcon runIcon;
    QIcon pauseIcon;
    QIcon stopIcon;
    QIcon nextIcon;
    QIcon previousIcon;

    QAction* screenshotAct;

    QAction* penAct;
    QAction* eraserAct;
    QAction* clearAct;

    QAction* runAct;
    QAction* pauseAct;
    QAction* stopAct;
    QAction* nextAct;
    QAction* previousAct;

    QComboBox* speedCmbBox;

    void setIcons();
    void createActions();
    void createToolBar();

public:
    MainWindow(QWidget* parent = 0);

private slots:
    void slotScreenshot();

    void slotPen();
    void slotEraser();

public slots:
    void slotToStatusBar(QPoint point);
};

#endif // MAINWINDOW_H

// ============================================================================
