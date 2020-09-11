#ifndef PIECE_H
#define PIECE_H

#include<QColor>
#include<QGraphicsItem>

using namespace std;

// Name: Adam Ten Hoeve

class Piece : public QObject, public QGraphicsItem {
    Q_OBJECT

public:
    // Constructors
    // Constructor with team, x, and y
    Piece(int, int, int, int);
    // Constructor with level included
    Piece(int, int, int, int, int);
    // Functions to make it appear in the view
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    // Setters
    void set_x(int x) {x_ = x; }
    void set_y(int y) {y_ = y; }
    void set_level(int l) {level_ = l; update();}
    void set_moved(bool m) {has_moved_ = m; }
    void set_color(QColor q) {color_ = q; update();}
    void set_is_green(bool g) {is_green_ = g; }

    // Getters
    int get_id() {return id_; }
    int get_x() {return x_; }
    int get_y() {return y_; }
    int get_team() {return team_; }
    int get_level() {return level_; }
    int get_moved() {return has_moved_; }
    bool get_is_green() {return is_green_; }

signals:
    // select the piece by left clicking it
    void PieceLeftClicked(Piece*);
    // Deselect the piece by right clicking it
    void PieceRightClicked();

protected:
    // Function when square is clicked. Decided which signal to send.
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    // Each piece has a unique id (per color)
    int id_;
    // x and y location of the piece
    int x_;
    int y_;
    // Which team and color it is
    int team_;
    QColor color_;
    // Whether the piece has moved this round
    bool has_moved_;
    // The level of the piece, controls how far it can move
    int level_;
    // Whether the piece is about to be eaten
    bool is_green_;

    // All pieces are circles with 25 radius
    static const int size_ = 25;
};

#endif // PIECE_H
