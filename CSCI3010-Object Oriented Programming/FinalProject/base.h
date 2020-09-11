#ifndef BASE_H
#define BASE_H

#include<QColor>
#include<QGraphicsItem>
#include<piece.h>

using namespace std;

// Name: Adam Ten Hoeve

class Base : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    //Constructors
    Base(int, int, int);

    // Things Qt needs
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    // Setters
    void set_x(int x) {x_ = x; }
    void set_y(int y) {y_ = y; }
    void set_level(int l) {level_ = l; update(); }
    void set_color(QColor q) {color_ = q; update(); }
    // Getters
    int get_x() { return(x_); }
    int get_y() { return(y_); }
    bool get_team() { return(team_); }
    int get_level() { return(level_); }

    // Base is a factory
    // Functions to create pieces at specified grid location, id, team and level
    static Piece* GetPiece(int, int, int, int, int);

signals:
    // When a base is left clicked. Hopefully for a reason
    void BaseLeftClicked(Base*);

protected:
    // When a base is clicked at all
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    // x and y of where it is in the view
    int x_;
    int y_;
    // What team/color it is
    bool team_;
    // What level the base is. Controls what level the pieces that come out of it are.
    int level_;
    QColor color_;

    // All bases are circles with 25 radius
    static const int size_ = 25;
};

#endif // BASE_H
