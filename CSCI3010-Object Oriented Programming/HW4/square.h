#ifndef SQUARE_H
#define SQUARE_H

#include<QColor>
#include<QGraphicsItem>

// Name: Adam Ten Hoeve

class Square : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    // Constructor
    Square();
    // Setters
    void set_alive(bool alive) {is_alive_ = alive;}
    void set_x(int x) {x_ = x;}
    void set_y(int y) {y_ = y;}
    void set_color(QColor color) {color_ = color; update();}
    void set_neighbors(int n) {neighbors_ = n;}
    // getters
    bool get_alive() {return is_alive_;}
    int get_neighbors() {return neighbors_;}
    // Things to make the object look like a rectangle.
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

signals:
    // Signal that is emitted when the square is clicked.
    // Either assigns as alive or dead.
    void SquareLeftClicked(Square*);
    void SquareRightClicked(Square*);

protected:
    // Function when square is clicked. Decides which signal to send.
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool is_alive_;
    int x_;
    int y_;
    int neighbors_;
    QColor color_;

    // All rectangles are 20*20
    static const int size_ = 20;
};

#endif // LIFEGRID_H
