#include "base.h"
#include <QtWidgets>

using namespace std;

// Name: Adam Ten Hoeve

// Constructor. Initialize all the things.
Base::Base(int team, int x, int y)
{
    x_ = x;
    y_ = y;
    team_ = team;
    level_ = 1;
    // If on blue team, set color to blue
    if (team == 1) {color_ = QColor(0, 191, 255, 255); }
    // Else on red team so color as red
    else {color_ = QColor(220, 20, 60, 255); }
}
// Things to make it appear like a ghost in the night
QRectF Base::boundingRect() const
{
    return QRectF(x_+2, y_+2, size_, size_);
}

QPainterPath Base::shape() const
{
    QPainterPath path;
    path.addEllipse(x_+2, y_+2, size_, size_);
    return path;
}

void Base::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_.dark(option->state & QStyle::State_Sunken ? 100 : 100)));

    painter->drawEllipse(QRect(this->x_+2, this->y_+2, this->size_, this->size_));
    painter->setBrush(b);

    // Putting the level of the piece on the piece
    string text = "B" + to_string(level_);
    painter -> drawText(this->x_+8, this->y_+19, QString(text.c_str()));
}

// The Base is a factory. This is the function that spits out new pieces when needed.
Piece* Base::GetPiece(int id, int team, int x, int y, int level)
{
    Piece* p = new Piece(id, team, x, y, level);
    return p;
}

// When clicked, the base sends a signal for help
void Base::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit BaseLeftClicked(this);
}
