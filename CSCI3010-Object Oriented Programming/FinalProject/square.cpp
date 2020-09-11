#include "square.h"
#include <QtWidgets>
#include "plotwindow.h"

//Name: Adam Ten Hoeve

// Constructor assigns all values to defaults.
Square::Square()
{
    team_ = -1;
    // Default color is white
    this -> color_ = QColor(255, 255, 255, 255);
    x_ = 0;
    y_ = 0;
    has_piece_ = -1;
    can_build_ = false;
    is_special = 0;
}

// Functions to make the squares appear in the view.
QRectF Square::boundingRect() const
{
    return QRectF(x_, y_, size_, size_);
}
// Squares are actually rectangles
QPainterPath Square::shape() const
{
    QPainterPath path;
    // Set the shape of the objects as squares with size 25.
    path.addRect(x_, y_, size_, size_);
    return path;
}

// Makes them appear, again
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
    // If right clicked
    if (event -> buttons() == Qt::RightButton)
    {
        emit SquareRightClicked();
    }
    // If in the base_building state and was clicked
    else if (can_build_)
    {
        emit BuildingBaseSquareClicked(this);
    }
    // Otherwise, only do anything if a piece is about to move onto it.
    else if (team_ >= 2)
    {
        emit GreenSquareLeftClicked(this);
    }
}

// Dummy function. Doesn't do anything. Just here so the inherited classes can use it in the grid
void Square::set_neighbors(Square* s1, Square* s2, Square* s3, Square *s4, Square* s5, Square *s6, Square *s7, Square *s8)
{
    return;
}

// Method for when a piece lands on this square
void Square::Landed(Piece* p)
{
    // Sets the team to the same as the piece that landed on it.
    team_ = p->get_team();
    // Square now has a piece on it.
    has_piece_ = p->get_team();
    //Change the color based on the team
    if(team_)
    {
        color_ = QColor(0, 0, 205, 255);
    }
    else
    {
        color_ = QColor(139, 0, 0, 255);
    }
    // update the visuals
    update();
}

//Methods for inherited class LevelUpSquare
// Constructor
LevelUpSquare::LevelUpSquare()
{
    // Sets special status and level_up to unused.
    Square();
    level_up_used = false;
    set_is_special(1);
    set_color(QColor(254, 127, 156, 255));
}

// Overload the virtual function Landed for when a piece lands on the sqaure
void LevelUpSquare::Landed(Piece* p)
{
    // Normal operation, set the squares stuff to the pieces
    Square::Landed(p);
    // If the level hasn't been used yet, then level up the piece
    if(!level_up_used)
    {
        // Sadly, if the piece is already level 3, then it is wasted.
        int level = p -> get_level();
        if(level != 3)
        {
            qDebug() << "leveling up piece" << endl;
            p -> set_level(level + 1);
        }
        level_up_used = true;
        // Square is no longer special
        set_is_special(0);
    }
}

// Methods for inherited class ColorAroundSquare
// Constructor. It is also special.
ColorAroundSquare::ColorAroundSquare(int i, int j)
{
    Square();
    color_used = false;
    set_is_special(2);
    set_color(QColor(255, 165, 0, 255));
}

// Set each of 8 elements in an array to a square that surround it.
void ColorAroundSquare::set_neighbors(Square* s1, Square* s2, Square* s3, Square *s4, Square* s5, Square *s6, Square *s7, Square *s8)
{
    neighbors[0] = s1;
    neighbors[1] = s2;
    neighbors[2] = s3;
    neighbors[3] = s4;
    neighbors[4] = s5;
    neighbors[5] = s6;
    neighbors[6] = s7;
    neighbors[7] = s8;
}

// Overload the virtual funtion landed for when piece lands on the square
void ColorAroundSquare::Landed(Piece* p)
{
    // Does the normal setting of color of a normal square
    Square::Landed(p);

    if(!color_used)
    {
        // Set all surrounding squares to the color of the piece that stepped on it.
        QColor color;
        if(p->get_team() == 1)
        {
            color = QColor(0, 0, 205, 255);
        }
        else
        {
            color = QColor(139, 0, 0, 255);
        }

        for(int i = 0; i < 8; i++)
        {
            // Check that a piece isn't on the neighbor
            if(neighbors[i]->get_hasPiece() == -1)
            {
                neighbors[i] -> set_color(color);
                neighbors[i] -> set_team(p->get_team());
            }
        }

        // Set color_used to true so can't be used again
        color_used = true;
        // No longer special
        set_is_special(0);
    }
}
