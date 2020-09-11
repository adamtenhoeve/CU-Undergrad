#include "square.h"
#include <QtWidgets>

//Name: Adam Ten Hoeve

// Constructor assigns all values to defaults.
Square::Square()
{
    is_alive_ = false;
    this -> color_ = QColor(255, 255, 255, 255);
    x_ = 0;
    y_ = 0;
    neighbors_ = 0;
}

// Functions to make the squares appear in the view.
QRectF Square::boundingRect() const
{
    return QRectF(x_, y_, size_, size_);
}

QPainterPath Square::shape() const
{
    QPainterPath path;
    // Set the shape of the objects as squares with size 20.
    path.addRect(x_, y_, size_, size_);
    return path;
}


void Square::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);


    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_.dark(option->state & QStyle::State_Sunken ? 100 : 100)));

    painter->drawRect(QRect(this->x_, this->y_, this->size_, this->size_));
    painter->setBrush(b);
}

// Event that triggers when the grid is clicked.
void Square::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // If right clicked, send the rightclicked signal
    if(event -> buttons() == Qt::RightButton)
    {
        emit SquareRightClicked(this);
    }
    // otherwise send the leftclick signal.
    else
    {
        emit SquareLeftClicked(this);
    }
}
