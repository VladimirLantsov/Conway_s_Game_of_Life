#include "world.h"

// ============================================================================

World::World(QSize sz, QWidget* parent) : QLabel(parent),
                                WORLD_SIZE(sz),
                                LENGTH_CELL(10)
{    
    setFixedSize(WORLD_SIZE);
    setMouseTracking(true);

    background = QImage(WORLD_SIZE, QImage::Format_ARGB32);
    background.fill(Qt::black);

    QPainter painter(&background);
    painter.setPen(Qt::darkGray);

    for(int y = LENGTH_CELL; y < WORLD_SIZE.height(); y += LENGTH_CELL)
    {
        painter.drawLine(QPoint(0, y), QPoint(WORLD_SIZE.width(), y));
    }

    for(int x = LENGTH_CELL; x < WORLD_SIZE.width(); x += LENGTH_CELL)
    {
        painter.drawLine(QPoint(x, 0), QPoint(x, WORLD_SIZE.height()));
    }

    painter.end();

    tool = PEN;

    gameStarted = false;

    logical = new Logical(WORLD_SIZE, LENGTH_CELL, this);

    connect(logical, SIGNAL(newGeneration(QList<QPoint>)),
                                     SLOT(slotSetGeneration(QList<QPoint>)));

    delay = new QTimer(this);
    setInterval("40");

    connect(delay, SIGNAL(timeout()), this, SLOT(next()));
}

// ----------------------------------------------------------------------------

void World::setLivingCells(QList<QPoint> cells)
{
    clearAll();
    livingCells = cells;
    startCells = livingCells;
    repaint();
}

// ----------------------------------------------------------------------------

QList<QPoint> World::getLivingCells() const
{
    return livingCells;
}

// ----------------------------------------------------------------------------

QPoint World::_rounding(QPoint point)
{
    int x = point.x(), y = point.y();

    return QPoint(x - (x % 10), y - (y % 10));
}

// ----------------------------------------------------------------------------

void World::slotSetGeneration(QList<QPoint> generation)
{
    livingCells = generation;

    if(livingCells.isEmpty() || (livingCells == lastGeneration))
    {
        QList< QList<QPoint> > generations = previousGenerations;
        stop();
        previousGenerations = generations;
        return;
    }

    repaint();
}

// ----------------------------------------------------------------------------

void World::setTool(int tl)
{
    tool = tl;

    if(tl == PEN)
    {
        emit penAvailable(false);
        emit eraserAvailable(true);
    }
    else if(tl == ERASER)
    {
        emit penAvailable(true);
        emit eraserAvailable(false);
    }
}

// ----------------------------------------------------------------------------

void World::setInterval(QString value)
{
    value.remove(" ms");
    int interval = value.toInt();
    delay->setInterval(interval);
}

// ----------------------------------------------------------------------------

void World::clearAll()
{
    startCells.clear();
    livingCells.clear();
    lastGeneration.clear();
    previousGenerations.clear();

    repaint();
}

// ----------------------------------------------------------------------------

void World::start()
{
    previousGenerations.clear();

    logical->setInitialGeneration(livingCells);
    delay->start();

    gameStarted = true;

    emit runAvailable(false);

    emit stopAvailable(true);
    emit pauseAvailable(true);

    emit nextAvailable(false);
    emit previousAvailable(false);
    emit clearAvailable(false);
}

// ----------------------------------------------------------------------------

void World::pause()
{
    delay->stop();

    emit pauseAvailable(false);

    emit runAvailable(true);
    emit stopAvailable(true);

    emit nextAvailable(true);
    emit previousAvailable(true);
}

// ----------------------------------------------------------------------------

void World::stop()
{
    delay->stop();
    livingCells = startCells;
    previousGenerations.clear();
    repaint();

    gameStarted = false;

    emit runAvailable(true);

    emit stopAvailable(false);
    emit pauseAvailable(false);

    emit nextAvailable(true);
    emit previousAvailable(true);
    emit clearAvailable(true);
}

// ----------------------------------------------------------------------------

void World::next()
{
    logical->setInitialGeneration(livingCells);
    previousGenerations << livingCells;
    lastGeneration = livingCells;
    logical->nextGeneration();
}

// ----------------------------------------------------------------------------

void World::previous()
{
    if(!previousGenerations.isEmpty())
    {
        livingCells = previousGenerations.last();
        previousGenerations.removeLast();
        repaint();
    }
}

// ----------------------------------------------------------------------------

void World::paintEvent(QPaintEvent*)
{   
    QPainter painter(this);

    QRect visibleRect = visibleRegion().boundingRect();
    QImage forDraw = background.copy(visibleRect);

    painter.drawImage(visibleRect.x(), visibleRect.y(), forDraw);

    painter.setPen(Qt::gray);
    painter.setBrush(Qt::green);

    foreach(QPoint cell, livingCells)
    {
        QRect cellRect(cell, QSize(LENGTH_CELL, LENGTH_CELL));

        if(!visibleRect.intersects(cellRect))
            continue;

        painter.drawRect(cellRect);
    }

    painter.end();
}

// ----------------------------------------------------------------------------

void World::mousePressEvent(QMouseEvent* me)
{
    if((me->buttons() != Qt::LeftButton) || gameStarted)
        return;

    QPoint cell = _rounding(me->pos());

    if(!livingCells.contains(cell))
    {
        if(tool == PEN)
        {
            livingCells << cell;

            emit changedSomething(true);
        }
    }
    else if(tool == ERASER)
    {
        livingCells.removeAll(cell);

        if(livingCells.isEmpty())
        {
            emit changedSomething(false);
        }
    }

    startCells = livingCells;
    repaint();
}

// ----------------------------------------------------------------------------

void World::mouseMoveEvent(QMouseEvent* me)
{
    if(!visibleRegion().contains(me->pos()))
        return;

    if(me->buttons() & Qt::LeftButton)
        mousePressEvent(me);

    emit coordinatesChanged(_rounding(me->pos()));
}

// ============================================================================
