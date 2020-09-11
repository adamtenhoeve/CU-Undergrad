#ifndef SQUARE_H
#define SQUARE_H

#include<QColor>
#include<QGraphicsItem>
#include"piece.h"

using namespace std;

// Name: Adam Ten Hoeve

class Square : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    // Constructor
    Square();
    // Setters
    void set_team(int t) {team_ = t; }
    void set_x(int x) {x_ = x;}
    void set_y(int y) {y_ = y;}
    void set_color(QColor color) {color_ = color; update();}
    void set_hasPiece(int b) {has_piece_ = b; }
    void set_canBuild(bool b) {can_build_ = b; }
    void set_is_special(int s) {is_special = s; }
    // getters
    int get_x() { return x_; }
    int get_y() { return y_; }
    int get_team() {return team_; }
    int get_hasPiece() {return has_piece_; }
    bool get_canBuild() {return can_build_; }
    QColor get_color() {return color_; }
    int get_is_special() {return is_special; }
    // Things to make the object look like a rectangle.
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    // When a piece lands on the square, what happens?
    virtual void Landed(Piece*);
    // Doesn't actually do anything, just for the child classes
    virtual void set_neighbors(Square* s1, Square* s2, Square* s3, Square *s4, Square* s5, Square *s6, Square *s7, Square *s8);

signals:
    // Signal that is emitted when the square is clicked.
    // When a squares is about to be moved onto.
    void GreenSquareLeftClicked(Square*);
    // To deselect a piece or stop a state
    void SquareRightClicked();
    // What square a base is going to be built on.
    void BuildingBaseSquareClicked(Square*);


protected:
    // Function when square is clicked. Decides which signal to send.
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    // -1 is no team, 0 is red, 1 is blue
    // 2-4 is green, could be selected to be moved to.
    int team_;
    int x_;
    int y_;
    QColor color_;
    // Int for if a piece is ontop of this square. -1 is no piece, 0 is red, 1 is blue
    int has_piece_;
    // State for if a base can be built on the square
    bool can_build_;
    // If the square has something special in it, meaning an inherited class
    int is_special;


    // All rectangles are 30*30. 225 in total on the grid.
    static const int size_ = 30;

};

// First inherited class. Levels up the first piece that steps on it.
class LevelUpSquare : public Square
{
public:
    LevelUpSquare();
    // Overloads this virtual function of the parent
    void Landed(Piece*);
    // Setters and getters
    void set_level_up_used(bool l) {level_up_used = l; }
    bool get_level_up_used() {return level_up_used; }
private:
    // Can only be used once
    bool level_up_used;
};

// Second Inherited class. Colors all the squares around it when the first piece steps on it.
class ColorAroundSquare : public Square
{
public:
    // Constructor
    ColorAroundSquare(int i, int j);
    // When a piece lands on it, colors all the squares surrounding it.
    void Landed(Piece*);
    // Getters and setters
    void set_color_used(bool u) {color_used = u; }
    bool get_color_used() {return color_used; }
    // Gets pointers to all the surrounding squares. I promise this was the easiest way for me to do this in an inherited class.
    void set_neighbors(Square* s1, Square* s2, Square* s3, Square *s4, Square* s5, Square *s6, Square *s7, Square *s8);

private:
    // Can only be used once
    bool color_used;
    // Store pointers to the squares around this square
    Square* neighbors[8];
};

#endif // SQUARE_H

