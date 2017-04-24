#ifndef LOGICAL_H
#define LOGICAL_H

#include <QtCore>

// ============================================================================

class Logical : public QObject
{
    Q_OBJECT

private:
    const QSize WORLD_SIZE;
    const int LENGTH_CELL;

    int countIterations;
    int countCalls;

    QList<QPoint> currentLivingCells;


    int _calculateNeighbors(QPoint point);

    QList<QPoint> _groupedCells(QPoint cell);

    bool _checkCell(QPoint point);

    void _applyToroidicity(QPoint& point);

public:
    Logical(QSize sz, int cell, QObject* parent = 0);

    void setInitialGeneration(QList<QPoint> gn);

public slots:
    void nextGeneration();

signals:
    void newGeneration(QList<QPoint> currentLivingCells);
};

// ============================================================================

#endif // LOGICAL_H
