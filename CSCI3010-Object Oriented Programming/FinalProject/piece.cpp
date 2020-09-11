#include "piece.h"
#include <QtWidgets>

using namespace std;

// Name: Adam Ten Hoeve

// Constructor
Piece::Piece(int id, int team, int x, int y)
{
    // If blue team, set team and color to blue
    if (team == 1)
    {
        color_ = QColor(0, 191, 255, 255);
//        color_ = QColor(0, 0, 205, 255);
    }
    // Otherwise is red team, set team and color to red
    else
    {
        color_ = QColor(220, 20, 60, 255);
//        color_ = QColor(139, 0, 0, 255);
    }

    id_ = id;
    team_ = team;
    x_ = x;
    y_ = y;
    level_ = 1;
    has_moved_ = false;
    is_green_ = false;
}
// Second constructor for when a piece is being made of a specific level
Piece::Piece(int id, int team, int x, int y, int level)
{
    // If blue team, set team and color to blue
    if (team == 1)
    {
        color_ = QColor(0, 191, 255, 255);
//        color_ = QColor(0, 0, 205, 255);
    }
    // Otherwise is red team, set team and color to red
    else
    {
        color_ = QColor(220, 20, 60, 255);
//        color_ = QColor(139, 0, 0, 255);
    }

    id_ = id;
    team_ = team;
    x_ = x;
    y_ = y;
    level_ = level;
    has_moved_ = false;
    is_green_ = false;
}

// Things to make the piece appear like circles
QRectF Piece::boundingRect() const
{
    return QRectF(x_+2, y_+2, size_, size_);
}

QPainterPath Piece::shape() const
{
    QPainterPath path;
    path.addEllipse(x_+2, y_+2, size_, size_);
    return path;
}

void Piece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_.dark(option->state & QStyle::State_Sunken ? 100 : 100)));

    painter->drawEllipse(QRect(this->x_+2, this->y_+2, this->size_, this->size_));
    painter->setBrush(b);

    // Putting the level of the piece in the view
    string text = "P" + to_string(level_);
    painter -> drawText(this->x_+8, this->y_+19, QString(text.c_str()));
}

// When a piece is clicked
void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // If the piece is right clicked, deselect the piece
    if (event -> buttons() == Qt::RightButton)
    {
        emit PieceRightClicked();
    }
    // Otherwise send leftclick signal
    else
    {
        emit PieceLeftClicked(this);
    }
}
