#include "logical.h"

// ============================================================================

Logical::Logical(QSize sz, int cell, QObject* parent)
                                            : QObject(parent),
                                              WORLD_SIZE(sz), LENGTH_CELL(cell)

{
    countIterations = 0;
    countCalls = 0;
}

// ----------------------------------------------------------------------------

void Logical::_applyToroidicity(QPoint& point)
{
    if(point.x() < 0)
    {
        point.rx() = WORLD_SIZE.width() + point.x();
    }
    else if(point.x() > (WORLD_SIZE.width() - LENGTH_CELL))
    {
        point.rx() = point.x() - WORLD_SIZE.width();
    }

    if(point.y() < 0)
    {
        point.ry() = WORLD_SIZE.height() + point.y();
    }
    else if(point.y() > (WORLD_SIZE.height() - LENGTH_CELL))
    {
        point.ry() = point.y() - WORLD_SIZE.height();
    }
}

// ----------------------------------------------------------------------------

QList<QPoint> Logical::_groupedCells(QPoint cell)
{
    QList<QPoint> group;

    QPoint topLeft(cell.x() - LENGTH_CELL, cell.y() - LENGTH_CELL);

    int yLim = topLeft.y() + LENGTH_CELL * 2;
    int xLim = topLeft.x() + LENGTH_CELL * 2;

    for(int y = topLeft.y(); y <= yLim; y += LENGTH_CELL)
    {
        ++countIterations;
        for(int x = topLeft.x(); x <= xLim; x += LENGTH_CELL)
        {
            QPoint newCell(x, y);

            _applyToroidicity(newCell);

            group << newCell;

            ++countIterations;
        }
    }

    return group;
}

// ----------------------------------------------------------------------------

bool Logical::_checkCell(QPoint point)
{
    foreach(QPoint cell, currentLivingCells)
    {
        if(point == cell)
            return true;

        ++countIterations;
    }

    return false;
}

// ----------------------------------------------------------------------------

int Logical::_calculateNeighbors(QPoint point)
{
    QList<QPoint> groupThisPoint = _groupedCells(point);

    int countNeighbors = 0;

    foreach(QPoint currentPoint, groupThisPoint)
    {
        if((_checkCell(currentPoint)))
        {
            if(currentPoint != point)
                ++countNeighbors;
        }

        ++countIterations;
    }

    return countNeighbors;
}

// ----------------------------------------------------------------------------

void Logical::setInitialGeneration(QList<QPoint> gn)
{
    currentLivingCells = gn;
}

// ----------------------------------------------------------------------------

void Logical::nextGeneration()
{
    QList < QList<QPoint> > groupsCells;

    foreach(QPoint cell, currentLivingCells)
    {
        groupsCells << _groupedCells(cell);

        ++countIterations;
    }

    QList<QPoint> cellsForRemove;
    QList<QPoint> cellsForAdd;

    foreach(QList<QPoint> group, groupsCells)
    {
        ++countIterations;

        foreach(QPoint cell, group)
        {
            ++countIterations;

            int countNeighbors = _calculateNeighbors(cell);

            if(_checkCell(cell))
            {
                if((countNeighbors < 2) || (countNeighbors > 3))
                    cellsForRemove << cell;

            }
            else if(countNeighbors == 3)
                cellsForAdd << cell;
        }
    }

    foreach(QPoint add, cellsForAdd)
    {
        ++countIterations;
        currentLivingCells << add;
    }

    foreach(QPoint remove, cellsForRemove)
    {
        ++countIterations;
        currentLivingCells.removeAll(remove);
    }

    foreach(QPoint point, currentLivingCells)
    {
        ++countIterations;

        if(currentLivingCells.contains(point))
            currentLivingCells.removeAll(point);

        currentLivingCells << point;
    }

    emit newGeneration(currentLivingCells);

    qDebug() << "Iterations for " << ++countCalls
                           << "call: " << countIterations;

    countIterations = 0;
}

// ============================================================================
