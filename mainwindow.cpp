#include "mainwindow.h"


// ============================================================================

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    world = new World(QSize(4000, 4000));

    screenshotPath = qApp->applicationDirPath();

    setIcons();
    createActions();
    createToolBar();

    addToolBar(toolBar);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(world);

    QScrollBar* scb = scrollArea->verticalScrollBar();
    scb->setValue(scb->maximum() / 2);

    scb = scrollArea->horizontalScrollBar();
    scb->setValue(scb->maximum() / 2);

    setCentralWidget(scrollArea);

    connect(world, SIGNAL(coordinatesChanged(QPoint)),
            SLOT(slotToStatusBar(QPoint)));

    slotToStatusBar(QPoint(0, 0));

    setWindowTitle("Game of life.");
}

// ----------------------------------------------------------------------------

void MainWindow::setIcons()
{
    runIcon = style()->standardIcon(QStyle::SP_MediaPlay);
    pauseIcon = style()->standardIcon(QStyle::SP_MediaPause);
    stopIcon = style()->standardIcon(QStyle::SP_MediaStop);
    nextIcon = style()->standardIcon(QStyle::SP_MediaSeekForward);
    previousIcon = style()->standardIcon(QStyle::SP_MediaSeekBackward);
}

// ----------------------------------------------------------------------------

void MainWindow::createActions()
{
    screenshotAct = new QAction("&Screenshot", this);

    penAct = new QAction("&Pen", this);
    eraserAct = new QAction("&Eraser", this);
    clearAct = new QAction("&Clear", this);

    runAct = new QAction(runIcon, "&Run", this);
    pauseAct = new QAction(pauseIcon, "&Pause", this);
    stopAct = new QAction(stopIcon, "&Stop", this);
    nextAct = new QAction(nextIcon, "&Next", this);
    previousAct = new QAction(previousIcon, "&Previous", this);

    QStringList speeds;
    speeds << "40 ms" << "100 ms" << "500 ms";
    speedCmbBox = new QComboBox;
    speedCmbBox->setToolTip("Interval");
    speedCmbBox->addItems(speeds);
    world->setInterval(speedCmbBox->currentText());

    penAct->setDisabled(true);
    eraserAct->setEnabled(true);

    pauseAct->setDisabled(true);
    stopAct->setDisabled(true);

    connect(screenshotAct, SIGNAL(triggered()), SLOT(slotScreenshot()));

    connect(penAct, SIGNAL(triggered()), SLOT(slotPen()));
    connect(eraserAct, SIGNAL(triggered()), SLOT(slotEraser()));
    connect(clearAct, SIGNAL(triggered()), world, SLOT(clearAll()));

    connect(runAct, SIGNAL(triggered()), world, SLOT(start()));
    connect(pauseAct, SIGNAL(triggered()), world, SLOT(pause()));
    connect(stopAct, SIGNAL(triggered()), world, SLOT(stop()));
    connect(nextAct, SIGNAL(triggered()), world, SLOT(next()));
    connect(previousAct, SIGNAL(triggered()), world, SLOT(previous()));

    connect(world, SIGNAL(runAvailable(bool)), runAct, SLOT(setEnabled(bool)));

    connect(world, SIGNAL(pauseAvailable(bool)),
            pauseAct, SLOT(setEnabled(bool)));

    connect(world, SIGNAL(stopAvailable(bool)),
            stopAct, SLOT(setEnabled(bool)));

    connect(world, SIGNAL(nextAvailable(bool)),
            nextAct, SLOT(setEnabled(bool)));

    connect(world, SIGNAL(previousAvailable(bool)),
            previousAct, SLOT(setEnabled(bool)));


    connect(world, SIGNAL(penAvailable(bool)), penAct, SLOT(setEnabled(bool)));

    connect(world, SIGNAL(eraserAvailable(bool)),
            eraserAct, SLOT(setEnabled(bool)));

    connect(world, SIGNAL(clearAvailable(bool)),
            clearAct, SLOT(setEnabled(bool)));


    connect(speedCmbBox, SIGNAL(activated(QString)),
            world, SLOT(setInterval(QString)));
}

// ----------------------------------------------------------------------------

void MainWindow::createToolBar()
{
    toolBar = new QToolBar("Tool bar", this);
    toolBar->addAction(previousAct);
    toolBar->addAction(runAct);
    toolBar->addAction(pauseAct);
    toolBar->addAction(stopAct);
    toolBar->addAction(nextAct);

    toolBar->addSeparator();
    toolBar->addAction(penAct);
    toolBar->addAction(eraserAct);
    toolBar->addAction(clearAct);

    toolBar->addSeparator();
    toolBar->addAction(screenshotAct);

    toolBar->addSeparator();
    toolBar->addWidget(speedCmbBox);

    toolBar->setMovable(false);
}

// ----------------------------------------------------------------------------

void MainWindow::slotScreenshot()
{
    if(stopAct->isEnabled())
        world->pause();

    const QWidget* viewport = scrollArea->viewport();
    QPixmap screen = scrollArea->grab(viewport->rect());

    QString name =
            QDateTime::currentDateTime().toString("yyyy.MMM.ddd_hh.mm.ss.z");

    if(!screen.save(name + ".png"))
    {
        QMessageBox::critical(0, "Failure...",
                              "Error saving! "
               "Maybe your antivirus does not allow to save a screenshot.",
                              QMessageBox::Close);
    }
}

// ----------------------------------------------------------------------------

void MainWindow::slotPen()
{
    world->setTool(World::PEN);
}

// ----------------------------------------------------------------------------

void MainWindow::slotEraser()
{
    world->setTool(World::ERASER);
}

// ----------------------------------------------------------------------------

void MainWindow::slotToStatusBar(QPoint point)
{
    QString status = "X = " + QString::number(point.x()) + " | Y = ";
    status += QString::number(point.y());

    statusBar()->showMessage(status);
}

// ============================================================================
