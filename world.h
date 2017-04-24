#ifndef WORLD_H
#define WORLD_H

#include <QtWidgets>
#include "logical.h"

// ============================================================================

class World : public QLabel
{
    Q_OBJECT

private:
    const QSize WORLD_SIZE;
    const int LENGTH_CELL;

    QImage background;

    int tool;

    bool gameStarted;

    QList<QPoint> startCells;
    QList<QPoint> livingCells;
    QList<QPoint> lastGeneration;

    QList< QList<QPoint> > previousGenerations;

    Logical* logical;

    QTimer* delay;

    QPoint _rounding(QPoint point);

protected:
    void paintEvent(QPaintEvent*);

    void mousePressEvent(QMouseEvent* me);
    void mouseMoveEvent(QMouseEvent* me);

public:
    World(QSize sz, QWidget* parent = 0);

    void setLivingCells(QList <QPoint> cells);

    QList<QPoint> getLivingCells() const;

    enum tools {PEN, ERASER};

signals:
    void coordinatesChanged(QPoint);
    void changedSomething(bool);

    void runAvailable(bool);
    void pauseAvailable(bool);
    void stopAvailable(bool);
    void nextAvailable(bool);
    void previousAvailable(bool);

    void penAvailable(bool);
    void eraserAvailable(bool);
    void clearAvailable(bool);

private slots:
    void slotSetGeneration(QList<QPoint> generation);

public slots:
    void setTool(int tl);
    void setInterval(QString value);
    void clearAll();

    void start();
    void pause();
    void stop();
    void next();
    void previous();
};

#endif // WORLD_H

// ============================================================================
