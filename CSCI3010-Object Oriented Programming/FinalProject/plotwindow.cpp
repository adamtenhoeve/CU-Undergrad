#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QDebug>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

#include "plotwindow.h"
#include "ui_plotwindow.h"
#include "square.h"
#include "piece.h"
#include "base.h"

using namespace std;

// Name:
// Adam Ten Hoeve

PlotWindow::PlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlotWindow)
{
    // we need to set up the ui before we draw on our scene
    ui->setupUi(this);

    // scene is a pointer field of plot window
    scene = new QGraphicsScene;
    // graph is a pointer field to the graph
    graph = new QGraphicsScene;

    QGraphicsView * view = ui->plotGraphicsView;
    QGraphicsView * graphView = ui->GraphView;

    // Set the scene for the grid and the graph
    view->setScene(scene);
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());
    view->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    graphView->setScene(graph);
//    graphView->setSceneRect(0,0,graphView->frameSize().width(), graphView->frameSize().height());
//    // Get rid of the scroll bars for the graph
    graphView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    graphView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    // so that you have random available and properly seeded
    srand(time(0));
    // Sets the number of games participated in to 0.
    games = 0;
    // Starts the game
    NewGame();

    // End Turn Button
    connect(ui->EndTurnButton, &QAbstractButton::clicked,
            this, &PlotWindow::On_EndTurnButton_Clicked);
    // New Game Button
    connect(ui->NewGameButton, &QAbstractButton::clicked,
            this, &PlotWindow::On_NewGameButton_Clicked);
    // Level Up Button
    connect(ui->LevelUpButton, &QAbstractButton::clicked,
            this, &PlotWindow::On_LevelUpButton_Clicked);
    // Create Piece Button
    connect(ui->CreatePieceButton, &QAbstractButton::clicked,
            this, &PlotWindow::On_CreatePieceButton_Clicked);
    // Create Base Button
    connect(ui->CreateBaseButton, &QAbstractButton::clicked,
            this, &PlotWindow::On_CreateBaseButton_Clicked);
    // Simulate Games Button
    connect(ui->SimulateGamesButton, &QAbstractButton::clicked,
            this, &PlotWindow::SimulateGames);

    for(int i = 0; i < 25; i++)
    {
        RedOutcomes[i] = 0;
        BlueOutcomes[i] = 0;
    }
}

// Deconstructor.
PlotWindow::~PlotWindow()
{
    delete ui;
}

void PlotWindow::NewGame()
{
    // Seeing is believing
    QGraphicsView * view = ui->plotGraphicsView;
    // records the number of games
    games += 1;

    // Initialize the variables.
    // Starts with the red player
    is_game_over = false;
    player_ = 0;
    moves_left = 3;

    // Initalize stuff for tracking player resources
    blue_id = 0;
    red_id = 0;
    blue_colored_ = 0;
    red_colored_ = 0;
    blue_gold_ = 1;
    red_gold_ = 1;

    // Set states to false
    isPieceClicked = false;
    isLevelUpState = false;
    isPieceCreatedState = false;
    isBuildingBaseState = false;

    // Initialize a temporary piece to record when pieces are selected
    Piece *temp = new Piece(0, 1, 0, 0);
    tempPiece = temp;

    // Set the center position of each Square in the grid
    for(int i = 0; i < 15; i++)
    {
        for(int j = 0; j < 15; j++)
        {
            // Add LevelUp square to locations on the grid
            if(((i == 9) && (j == 9)) || ((i == 5) && (j == 5)))
            {
                LevelUpSquare *s = new LevelUpSquare();
                grid[i][j] = s;
                grid[i][j] ->set_x(i*30);
                grid[i][j] ->set_y(j*30);
            }
            // Add ColorAround sqaures to specified locations as well.
            else if((i==4 && j==1) || (i==1 && j==4) || (i==7 && j==7) || (i==10 && j==13) || (i==13 && j==10))
            {
                ColorAroundSquare *s = new ColorAroundSquare(i, j);
                grid[i][j] = s;
                grid[i][j]->set_x(i*30);
                grid[i][j]->set_y(j*30);
            }
            // Otherwise just make normal squares
            else
            {
                Square *s = new Square();
                grid[i][j] = s;
                grid[i][j]->set_x(i*30);
                grid[i][j]->set_y(j*30);
            }

            // Connect the signals to each square
            connect(grid[i][j], &Square::GreenSquareLeftClicked,
                    this, &PlotWindow::GreenSquareLeftClickedSlot);

            connect(grid[i][j], &Square::SquareRightClicked,
                    this, &PlotWindow::PieceRightClickedSlot);

            connect(grid[i][j], &Square::BuildingBaseSquareClicked,
                    this, &PlotWindow::CreateBase);

            scene->addItem(grid[i][j]);
        }
    }

    // Set the neighbors of the ColorAround Pieces
    for(int i = 0; i < 15; i++)
    {
        for(int j = 0; j < 15; j++)
        {
            if(grid[i][j]->get_is_special() == 2)
            {
                grid[i][j]->set_neighbors(grid[i-1][j-1], grid[i][j-1], grid[i-1][j+1], grid[i-1][j], grid[i+1][j], grid[i-1][j+1], grid[i][j+1], grid[i+1][j+1]);
            }
        }
    }

    // Set starting pieces for each color, one of each level, and connect all their slots.
    Piece *blueP1 = new Piece(blue_id, 1, 3*30, 12*30);
    blue_id += 1;
    scene -> addItem(blueP1);
    Piece *blueP2 = new Piece(blue_id, 1, 2*30, 11*30, 2);
    blue_id += 1;
    scene -> addItem(blueP2);
    Piece *blueP3 = new Piece(blue_id, 1, 3*30, 11*30, 3);
    blue_id += 1;
    scene -> addItem(blueP3);

    connect(blueP1, &Piece::PieceLeftClicked,
            this, &PlotWindow::PieceLeftClickedSlot);
    connect(blueP1, &Piece::PieceRightClicked,
            this, &PlotWindow::PieceRightClickedSlot);
    connect(blueP2, &Piece::PieceLeftClicked,
            this, &PlotWindow::PieceLeftClickedSlot);
    connect(blueP2, &Piece::PieceRightClicked,
            this, &PlotWindow::PieceRightClickedSlot);
    connect(blueP3, &Piece::PieceLeftClicked,
            this, &PlotWindow::PieceLeftClickedSlot);
    connect(blueP3, &Piece::PieceRightClicked,
            this, &PlotWindow::PieceRightClickedSlot);
    // Color the squares these pieces are on blue
//    grid[2][11]->set_color(QColor(0, 0, 205, 255));
//    grid[2][11]->set_team(1);
//    grid[2][11]->set_hasPiece(1);
    grid[2][11]->Landed(blueP2);
//    grid[3][12]->set_color(QColor(0, 0, 205, 255));
//    grid[3][12]->set_team(1);
//    grid[3][12]->set_hasPiece(1);
    grid[3][12]->Landed(blueP1);
//    grid[3][11]->set_color(QColor(0, 0, 205, 255));
//    grid[3][11]->set_team(1);
//    grid[3][11]->set_hasPiece(1);
    grid[3][11]->Landed(blueP3);

    // Do the same for the red pieces
    Piece *redP1 = new Piece(red_id, 0, 11*30, 2*30);
    red_id += 1;
    scene -> addItem(redP1);
    Piece *redP2 = new Piece(red_id, 0, 12*30, 3*30, 2);
    red_id += 1;
    scene -> addItem(redP2);
    Piece *redP3 = new Piece(red_id, 0, 11*30, 3*30, 3);
    red_id += 1;
    scene -> addItem(redP3);

    connect(redP1, &Piece::PieceLeftClicked,
            this, &PlotWindow::PieceLeftClickedSlot);
    connect(redP1, &Piece::PieceRightClicked,
            this, &PlotWindow::PieceRightClickedSlot);
    connect(redP2, &Piece::PieceLeftClicked,
            this, &PlotWindow::PieceLeftClickedSlot);
    connect(redP2, &Piece::PieceRightClicked,
            this, &PlotWindow::PieceRightClickedSlot);
    connect(redP3, &Piece::PieceLeftClicked,
            this, &PlotWindow::PieceLeftClickedSlot);
    connect(redP3, &Piece::PieceRightClicked,
            this, &PlotWindow::PieceRightClickedSlot);
    // Color squares these pieces are on as red
//    grid[11][2]->set_color(QColor(139, 0, 0, 255));
//    grid[11][2]->set_team(0);
//    grid[11][2]->set_hasPiece(0);
    grid[11][2]->Landed(redP1);
//    grid[12][3]->set_color(QColor(139, 0, 0, 255));
//    grid[12][3]->set_team(0);
//    grid[12][3]->set_hasPiece(0);
    grid[12][3]->Landed(redP2);
//    grid[11][3]->set_color(QColor(139, 0, 0, 255));
//    grid[11][3]->set_team(0);
//    grid[11][3]->set_hasPiece(0);
    grid[11][3]->Landed(redP3);

    // Make sure vectors of pieces are empty
    bluePieces.clear();
    redPieces.clear();
    // Add the new pieces to the vectors of pieces.
    bluePieces.push_back(blueP1);
    bluePieces.push_back(blueP2);
    bluePieces.push_back(blueP3);

    redPieces.push_back(redP1);
    redPieces.push_back(redP2);
    redPieces.push_back(redP3);

    // Create base for blue player, add to the vector
    Base *blueBase = new Base(1, 2*30, 12*30);
    scene -> addItem(blueBase);
    grid[2][12] -> set_team(1);
    grid[2][12] -> set_hasPiece(1);
    grid[2][12] -> set_color(QColor(0, 0, 205, 255));
    connect(blueBase, &Base::BaseLeftClicked,
            this, &PlotWindow::BaseLeftClickedSlot);
    blueBases.push_back(blueBase);


    // Creates bases for red player
    Base *redBase = new Base(0, 12*30, 2*30);
    scene -> addItem(redBase);
    grid[12][2] -> set_team(0);
    grid[12][2] -> set_hasPiece(0);
    grid[12][2] -> set_color(QColor(139, 0, 0, 255));
    connect(redBase, &Base::BaseLeftClicked,
            this, &PlotWindow::BaseLeftClickedSlot);
    redBases.push_back(redBase);

    // Determines how many squares of each color there are
    Find_Colors();

    // Update the label for which player is the active player
    Update_Player_Label();

    // Update tiles labels
    Update_Tiles();

    // Update gold labels
    Update_Gold();

    // Update the moves label
    Update_Moves_Label();

}

// Updates the labels for the number and proportion of tiles that have been colored by each color.
void PlotWindow::Update_Tiles()
{
    // Update Tiles label for blue
    string tilesB = to_string(blue_colored_);
    string propB = to_string((float)blue_colored_ / 225.0);
    string textB = string("Tiles Colored: ") + tilesB + string(" ") + propB;
    ui -> BlueTiles -> setText(QString(textB.c_str()));

    // Update tiles label for red
    string tilesR = to_string(red_colored_);
    string propR = to_string((float)red_colored_ / 225.0);
    string textR = string("Tiles Colored: ") + tilesR + string(" ") + propR;
    ui -> RedTiles -> setText(QString(textR.c_str()));
}

// Updates the gold labels to properly display the amount of gold each player has.
void PlotWindow::Update_Gold()
{
    // Update label for gold for blue player
    string goldB = to_string(blue_gold_);
    string textB = string("Gold: ") + goldB;
    ui -> BlueGold -> setText(QString(textB.c_str()));

    // Update label for gold for red player
    string goldR = to_string(red_gold_);
    string textR = string("Gold: ") + goldR;
    ui -> RedGold -> setText(QString(textR.c_str()));
}

// Updates the current player to the playing color.
void PlotWindow::Update_Player_Label()
{
    string text;
    // 0 is red player
    if (player_ == 0)
    {
        text = string("Current Player: Red");
    }
    // 1 is blue player
    else if (player_ == 1)
    {
        text = string("Current Player: Blue");
    }

    ui -> CurrentPlayer -> setText(QString(text.c_str()));
}

// Updates the label for the number of moves left in the turn.s
void PlotWindow::Update_Moves_Label()
{
    string text;
    text = string("Moves Left: ") + to_string(moves_left);
    ui -> MovesLabel -> setText(QString(text.c_str()));
}

// Finds how many squares of the grid are colored to each team.
void PlotWindow::Find_Colors()
{
    blue_colored_ = 0;
    red_colored_ = 0;

    // Loop over entire grid
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            // If team is 1, is blue team
            if (grid[i][j]->get_team() == 1)
            {
                blue_colored_ += 1;
            }
            // If team is 0, is red team
            else if (grid[i][j]-> get_team() == 0)
            {
                red_colored_ += 1;
            }
        }
    }
}

// Checks which squares can be moved to and assigns them as ready.
void PlotWindow::MovePiece(Piece* p)
{
    // Set the isPieceClicked state to true so can not be double clicked.
    isPieceClicked = true;

    // Set the clicked piece as the temp stored piece
    tempPiece = p;

    int i = p->get_x() / 30;
    int j = p->get_y() / 30;
    // Want to paint all surrounding squares in green
    // Get 4 cardinal directions
    int left = i - 1;
    int right = i + 1;
    int up = j - 1;
    int down = j + 1;

    int level = p -> get_level();

    // Check if at the boundary. Don't want to go out of bounds.
    // Also check if a piece is already on the square. Prevent players from moving two pieces ontop of each other.
    // Level 1 moves in 4 cardinal directions
    if(level >= 1)
    {
        // top
        if(j != 0) { ColorSquare(i, up); }
        // right
        if(i != 14) { ColorSquare(right, j); }
        // Bottom
        if(j != 14) { ColorSquare(i, down); }
        // Left
        if(i != 0) { ColorSquare(left, j); }
    }
    // If level 2, can move diagonally 1 square
    if(level >= 2)
    {
        // top right
        if(j != 0 && i != 14) { ColorSquare(right, up); }
        // Bottom right
        if(j != 14 && i != 14) { ColorSquare(right, down); }
        // Bottom left
        if(j != 14 && i != 0) { ColorSquare(left, down); }
        // Top Left
        if(j != 0 && i != 0) { ColorSquare(left, up); }
    }
    // If level 3, can move straight and diagonally 2 squares
    if(level == 3)
    {
        // top
        if(j > 1) { ColorSquare(i, j - 2); }
        // bottom
        if(j < 13) { ColorSquare(i, j + 2); }
        // left
        if(i > 1) { ColorSquare(i - 2, j); }
        // right
        if(i < 13) { ColorSquare(i + 2, j); }
        // top right
        if(j > 1 && i < 13) { ColorSquare(i + 2, j - 2); }
        // top left
        if(j > 1 && i > 1) { ColorSquare(i - 2, j - 2); }
        // bot right
        if(j < 13 && i < 13) { ColorSquare(i + 2, j + 2); }
        // bot left
        if(j < 13 && i > 1) { ColorSquare(i - 2, j + 2); }
    }
}

// Colors the square green based on the team
// 3 different colors depending on whether the square was white, blue or red.
void PlotWindow::ColorSquare(int i, int j)
{
    int team = grid[i][j]->get_team();

    // Check if an enemy piece is on the square
    if ((team != -1) && (grid[i][j]->get_hasPiece() != player_))
    {
        // If one is, find which piece it is and color them green
    }

    // If is already blue, color blue-green
    if (team == 1)
    {
        // Check if a piece is on the square of the opposite color. In this case, other piece is blue and player is red
        if((grid[i][j]->get_hasPiece() == 1) && (player_ == 0))
        {
            // Find that piece and color it green
            for(unsigned k = 0; k < bluePieces.size(); k++)
            {
                if((bluePieces[k]->get_x() / 30 == i) && (bluePieces[k]->get_y() / 30 == j))
                {
                    bluePieces[k] -> set_color(QColor(57, 255, 20, 255));
                    bluePieces[k] -> set_is_green(true);
                }
            }
        }
        // Check if an allied piece is on the square. Don't let move ontop of other pieces of the same color.
        if(grid[i][j]->get_hasPiece() == 1) { return; }
        grid[i][j]->set_color(QColor(32, 178, 170, 255));
    }
    // If is red, color green-red.
    else if(team == 0)
    {
        // Check if a piece is on the square of the opposite color. In this case, other piece is red and player is blue
        if((grid[i][j]->get_hasPiece() == 0) && (player_ == 1))
        {
            // Find that piece and color it green
            for(unsigned k = 0; k < redPieces.size(); k++)
            {
                if((redPieces[k]->get_x() / 30 == i) && (redPieces[k]->get_y() / 30 == j))
                {
                    redPieces[k] -> set_color(QColor(57, 255, 20, 255));
                    redPieces[k] -> set_is_green(true);
                }
            }
        }
        // Same as above, don't let move ontop of allied pieces.
        if(grid[i][j]->get_hasPiece() == 0) { return; }
        // If enemy piece, then want to select the piece, not the square
        grid[i][j]->set_color(QColor(85, 107, 47, 255));
    }
    else
    {
        grid[i][j]->set_color(QColor(57, 255, 20, 255));
    }

    grid[i][j]->set_team(team + 3);
}

// Function to select a piece to move or level up. Happens when it is left clicked.
void PlotWindow::PieceLeftClickedSlot(Piece* p)
{
    // Check if piece is getting moved onto (deleted)
    if(p->get_is_green() == true)
    {
//        qDebug() << "clicked piece was green";
        // Move the previous selected piece onto that square
        Square* s = grid[p->get_x()/30][p->get_y()/30];
        GreenSquareLeftClickedSlot(s);
        // Move the moved to piece back to starting base
        SendPieceHome(p);
        return;
    }

    // Make sure the clicked piece is of the current player's team.
    if(p->get_team() != player_)
    {
        qDebug() << "Wrong Team";
        return;
    }

    // Don't do anything is if isBuildingBaseState
    if(isBuildingBaseState)
    {
        return;
    }

    // Check if in level up state. Decides if piece should move or level up.
    if(isLevelUpState)
    {
        // Update the pieces level by 1;
        LevelUpPiece(p);
    }
    else
    {
        // Check if piece has already been clicked. Prevents double clicks.
        // Check if piece is of the correct team. Don't let them move pieces of the wrong team.
        if ((isPieceClicked) || (p->get_moved()))
        {
            qDebug() << "can't move";
            return;
        }

        MovePiece(p);
    }
}

// Doesn't do anything unless in the CreatePiece or LevelUp states.
// Performs those functions on the base.
void PlotWindow::BaseLeftClickedSlot(Base* b)
{
    if(b->get_team() != player_)
    {
        qDebug() << "Wrong Team";
        return;
    }

    // Check if in level up state.
    if(isLevelUpState)
    {
        // Increase the base's level by 1 if enough gold
        LevelUpBase(b);
    }

    // Check if in the CreatingPieces state. Want to create a piece next to the base if true
    if(isPieceCreatedState)
    {
        // Create a new piece near the base
        FindNewPieceLocation(b);

        // Then recolors all the bases to their normal colors
        if(player_)
        {
            for(int i = 0; i < blueBases.size(); i++)
            {
                blueBases[i]->set_color(QColor(0, 191, 255, 255));
            }
        }
        else
        {
            for(int i = 0; i < redBases.size(); i++)
            {
                redBases[i]->set_color(QColor(220, 20, 60, 255));
            }
        }

        // Set state to false so not creating any more pieces
        On_CreatePieceButton_Clicked();
    }

    // Don't do anything if not in either state.
}

// Function to move a piece to a possible square
// Deletes the piece and makes an copy at the new location
// Colors grid based on how the piece moved.
void PlotWindow::GreenSquareLeftClickedSlot(Square* s)
{
    // Check if LevelUpState is true. Return if true.
    if(isLevelUpState)
    {
        return;
    }

    // Set the team value, has_piece and color of the square to the current team
    s->Landed(tempPiece);

    // Set the previous square's hasPiece to false
    int oldX = tempPiece -> get_x() / 30;
    int oldY = tempPiece -> get_y() / 30;

    grid[oldX][oldY]->set_hasPiece(-1);

    // Update center block if the piece moved 2 squares. Only possible if piece was level 3.
    if(tempPiece -> get_level() == 3)
    {
        // Check if the piece moved 2 squares
        int changeX = abs(s->get_x() - tempPiece->get_x()) / 30;
        int changeY = abs(s->get_y() - tempPiece->get_y()) / 30;
        if ((changeX == 2) || (changeY == 2))
        {
            // Then also want to paint the middle piece in the colors.
            // Get the position of the mid square based on half the change
            int midX = (tempPiece->get_x() / 30) + ((s->get_x() - tempPiece->get_x()) / 60);
            int midY = (tempPiece->get_y() / 30) + ((s->get_y() - tempPiece->get_y()) / 60);

            // Make sure there isn't a piece on the middle square
            if(grid[midX][midY]->get_hasPiece() == -1)
            {
                grid[midX][midY]->set_team(player_);

                //Change the color based on the team
                if(player_)
                {
                    grid[midX][midY]->set_color(QColor(0, 0, 205, 255));
                }
                else
                {
                    grid[midX][midY]->set_color(QColor(139, 0, 0, 255));
                }
            }
        }
    }

    // Recolor the rest of the graph to be white
    for(int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            // If was a special square, then recolor it the specific color
            if(grid[i][j]->get_is_special() == 1)
            {
                grid[i][j] -> set_team(-1);
                grid[i][j] -> set_color(QColor(254, 127, 156, 255));
            }
            else if(grid[i][j]->get_is_special() == 2)
            {
                grid[i][j] -> set_team(-1);
                grid[i][j] -> set_color(QColor(255, 165, 0, 255));
            }
            // If team is now 2, then was a blank square
            if(grid[i][j]->get_team() == 2)
            {
                grid[i][j]->set_team(-1);
                grid[i][j]->set_color(QColor(255, 255, 255, 255));
            }
            // If team is now 3, then was a red square
            else if(grid[i][j]->get_team() == 3)
            {
                grid[i][j]->set_team(0);
                grid[i][j]->set_color(QColor(139, 0, 0, 255));
            }
            // If team is now 4, then was a blue square
            else if(grid[i][j]->get_team() == 4)
            {
                grid[i][j]->set_team(1);
                grid[i][j]->set_color(QColor(0, 0, 205, 255));
            }
        }
    }

    // Set is_green of all pieces to be false.
    // If blue team, set all red team's is green to false
    if(player_)
    {
        for(unsigned k = 0; k < redPieces.size(); k++)
        {
            redPieces[k] -> set_is_green(false);
            // Recolor to be red
            redPieces[k] -> set_color(QColor(220, 20, 60, 255));
        }
    }
    // If player is red, set all blue team's pieces is_green to false
    else
    {
        for(unsigned k = 0; k < bluePieces.size(); k++)
        {
            bluePieces[k] -> set_is_green(false);
            // Recolor to be blue
            bluePieces[k] -> set_color(QColor(0, 191, 255, 255));
        }
    }

    // Update all the labels.
    Find_Colors();
    Update_Tiles();

    // Then move the piece. Do so by deleting and create a new piece.
    int id = tempPiece -> get_id();
    Piece* newPiece = new Piece(id, player_, s->get_x(), s->get_y(), tempPiece->get_level());

    // Set has_moved to true so piece can't move again this turn
    newPiece->set_moved(true);
    scene -> addItem(newPiece);

    // Connect all the signals of the new piece
    connect(newPiece, &Piece::PieceLeftClicked,
            this, &PlotWindow::PieceLeftClickedSlot);
    connect(newPiece, &Piece::PieceRightClicked,
            this, &PlotWindow::PieceRightClickedSlot);

    // Remove the piece at the previous location from the view
    scene -> removeItem(tempPiece);

    // Change the vector of pieces to contain the new piece, rather than the old piece
    if(player_)
    {
        bluePieces[unsigned(id)] = newPiece;
    }
    else
    {
        redPieces[unsigned(id)] = newPiece;
    }
    // Set the isPieceClicked state to false so a new piece can be clicked.
    isPieceClicked = false;
    delete(tempPiece);
    tempPiece = newPiece;

    // Player just moved a piece, so number of pieces left that can move is one less
    moves_left -= 1;
    Update_Moves_Label();
    // If no moves left, switch to other player;
    if(moves_left == 0)
    {
        On_EndTurnButton_Clicked();
    }
}

// Deselect a piece and exit all states
void PlotWindow::PieceRightClickedSlot()
{
    // Set the isPieceClicked state to false to deselect piece
    isPieceClicked = false;

    // Recolor the graph to be white, getting rid of green cells.
    for(int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            // If was a special square, then recolor it the specific color
            if(grid[i][j]->get_is_special() == 1)
            {
                grid[i][j] -> set_team(-1);
                grid[i][j] -> set_color(QColor(254, 127, 156, 255));
            }
            else if(grid[i][j]->get_is_special() == 2)
            {
                grid[i][j] -> set_team(-1);
                grid[i][j] -> set_color(QColor(255, 165, 0, 255));
            }
            // If team is now 2, then was a blank square
            if(grid[i][j]->get_team() == 2)
            {
                grid[i][j]->set_team(-1);
                grid[i][j]->set_color(QColor(255, 255, 255, 255));
            }
            // If team is now 3, then was a red square
            else if(grid[i][j]->get_team() == 3)
            {
                grid[i][j]->set_team(0);
                grid[i][j]->set_color(QColor(220, 20, 60, 255));
            }
            // If team is now 4, then was a blue square
            else if(grid[i][j]->get_team() == 4)
            {
                grid[i][j]->set_team(1);
                grid[i][j]->set_color(QColor(0, 191, 255, 255));
            }
        }
    }
}

// Changes turns from one color to the other
// Also generates gold based on how many pieces didn't move
// Updates the movement states of all the other team's pieces
void PlotWindow::On_EndTurnButton_Clicked()
{
    // Check if game is over
    if(blue_colored_ > 100)
    {
        is_game_over = true;
        qDebug() << "Blue Player won! Please click 'New Game'";
    }
    else if(red_colored_ > 100)
    {
        is_game_over = true;
        qDebug() << "Red Player won! Please click 'New Game'";
    }

    // Set all the has_moved_ of all pieces of other team to false.
    // 1 is blue player
    if(player_)
    {
        // Find how much gold the player generated from not moving. Generate 1 gold per unmoved piece
        for (unsigned i = 0; i < bluePieces.size(); i++)
        {
            if(!bluePieces[i]->get_moved())
            {
                blue_gold_ += 1;
            }
        }
        // set red team pieces to has_moved_ = false
        for(unsigned i = 0; i < redPieces.size(); i++)
        {
            redPieces[i]->set_moved(false);
        }
    }
    // 0 is red team
    else
    {
        // Generate red gold based on how many didn't move
        for (unsigned i = 0; i < redPieces.size(); i++)
        {
            if(!redPieces[i]->get_moved())
            {
                red_gold_ += 1;
            }
        }
        // Set all of the has_moved of blue pieces to false.
        for(unsigned i = 0; i < bluePieces.size(); i++)
        {
            bluePieces[i]->set_moved(false);
        }
    }

    // Update the gold labels
    Update_Gold();

    // At end of turn, switch player
    player_ = !player_;
    // If blue player is now active, new number of moves left is length of vector of blue pieces
    if (player_)
    {
        moves_left = bluePieces.size();
    }
    // otherwise is length of vector of red pieces
    else
    {
        moves_left = redPieces.size();
    }

    Update_Player_Label();
    Update_Moves_Label();

}

// Starts a new game
void PlotWindow::On_NewGameButton_Clicked()
{
    NewGame();
}

// Turns on or off the LevelUpState button
void PlotWindow::On_LevelUpButton_Clicked()
{
    //Turn state to opposite of what it was
    isLevelUpState = !isLevelUpState;
    // If turning on, turn cursor into a cross
    if(isLevelUpState)
    {
        ui->centralWidget->setCursor(Qt::CrossCursor);
    }
    // If turning off, turn cursor back into arrow
    else
    {
        ui->centralWidget->setCursor(Qt::ArrowCursor);
    }
}

// Sets the state to PieceCreationState so a piece will be created when a base is clicked
void PlotWindow::On_CreatePieceButton_Clicked()
{
    // Set the CreatingPiece state on or off. Prevents other things from being interacted with when active
    isPieceCreatedState = !isPieceCreatedState;
    if(isPieceCreatedState)
    {
        ui->centralWidget->setCursor(Qt::CrossCursor);
        // If blue team, go through the blue bases and color them green.
        if(player_)
        {
            for(int i = 0; i < blueBases.size(); i++)
            {
                blueBases[i]->set_color(QColor(57, 255, 20, 255));
            }
        }
        // If red team, go through red bases and color them green.
        else
        {
            for(int i = 0; i < redBases.size(); i++)
            {
                redBases[i]->set_color(QColor(57, 255, 20, 255));
            }
        }
    }
    // Turn off CreatingPiece state
    else
    {
        ui->centralWidget->setCursor(Qt::ArrowCursor);
        // If blue team, go through the blue bases and make them blue again.
        if(player_)
        {
            for(int i = 0; i < blueBases.size(); i++)
            {
                blueBases[i]->set_color(QColor(0, 191, 255, 255));
            }
        }
        // If red team, go through bases and color them red.
        else
        {
            for(int i = 0; i < redBases.size(); i++)
            {
                redBases[i]->set_color(QColor(220, 20, 60, 255));
            }
        }
    }
}

// Function to level up a piece when on is clicked with the LevelUpState on
void PlotWindow::LevelUpPiece(Piece* p)
{
    int level = p->get_level();
    // determine what team the piece is on. If on blue team:
    if(p->get_team())
    {
        // Check amount of gold and level of piece. If enough, then upgrade piece and subtract gold.
        if(level == 1)
        {
            if (blue_gold_ < 2)
            {
                qDebug() << "Not enough gold";
                On_LevelUpButton_Clicked();
                return;
            }
            p->set_level(2);
            blue_gold_ -= 2;
        }
        if(level == 2)
        {
            if (blue_gold_ < 3)
            {
                qDebug() << "Not enough gold";
                On_LevelUpButton_Clicked();
                return;
            }
            p->set_level(3);
            blue_gold_ -= 3;
        }
    }
    // If on red team
    else
    {
        // Check the amount of gold and level of piece. If enough, then upgrade piece and subtract gold.
        if(level == 1)
        {
            if(red_gold_ < 2)
            {
                qDebug() << "Not enough gold";
                On_LevelUpButton_Clicked();
                return;
            }
            p->set_level(2);
            red_gold_ -= 2;
        }
        if(level == 2)
        {
            if(red_gold_ < 3)
            {
                qDebug() << "Not enough gold";
                On_LevelUpButton_Clicked();
                return;
            }
            p->set_level(3);
            red_gold_ -= 3;
        }
    }

    // Update Gold Labels
    Update_Gold();

    // Turn off level up state
    On_LevelUpButton_Clicked();
}

// Function to level up a base when on is seleceted
void PlotWindow::LevelUpBase(Base* b)
{
    int level = b -> get_level();
    // If blue team
    if (b->get_team())
    {
        // If base is first level, need 5 gold to level up
        if(level == 1)
        {
            if(blue_gold_ < 4)
            {
                qDebug() << "Not enough gold";
                return;
            }
            blue_gold_ -= 4;
            b->set_level(2);
        }
        // If base is second level, need 7 gold to level up
        else if(level == 2)
        {
            if(blue_gold_ < 6)
            {
                qDebug() << "Not enough gold";
                return;
            }
            blue_gold_ -= 6;
            b->set_level(3);
        }
    }
    // Otherwise is red team;
    else
    {
        // If base is first level, need 5 gold to level up
        if(level == 1)
        {
            if(red_gold_ < 4)
            {
                qDebug() << "Not enough gold";
                return;
            }
            red_gold_ -= 4;
            b->set_level(2);
        }
        // If base is second level, need 7 gold to level up
        else if(level == 2)
        {
            if(red_gold_ < 6)
            {
                qDebug() << "Not enough gold";
                return;
            }
            red_gold_ -= 6;
            b->set_level(3);
        }
    }

    // Update Gold Labels
    Update_Gold();

    // Turn off level up state
    On_LevelUpButton_Clicked();
}

// Decides where to put piece when a new one is created.
void PlotWindow::FindNewPieceLocation(Base *b)
{
    int x = b -> get_x() / 30;
    int y = b -> get_y() / 30;
    // Make sure level input was there
    bool safe = false;
    int level = ui -> CreatePieceLevel -> text().toInt(&safe);
    if (!safe) {
        qDebug() << "unsafe input";
        return;
    }
    // Check to make sure input level is valid
    if((level < 1) || (level > 3) || (level > b->get_level()))
    {
        qDebug() << "Invalid Level";
        return;
    }

    // Check that player has enough gold
    // If blue team
    if(player_)
    {
        if(blue_gold_ < level + 2)
        {
            qDebug() << "Not enough gold";
            return;
        }
        // Update gold levels
        blue_gold_ -= level + 2;
    }
    // If red team
    else
    {
        if(red_gold_ < level + 2)
        {
            qDebug() << "Not enough gold";
            return;
        }
        // Update gold levels
        red_gold_ -= level + 2;
    }

    // Check surrounding cells to see if there is a piece there or if out of bounds
    CheckSurroundings(x, y);
    CreatePiece(x, y, level);
}

// Input is place on grid, not (x,y) locationon ui. Need to multiply be 30.
void PlotWindow::CreatePiece(int x, int y, int level)
{
    // Use base as factory.
    // If blue team
    if(player_)
    {
        // Create a new piece, update tracking, and add to scene
        Piece *p = Base::GetPiece(blue_id, 1, x*30, y*30, level);
        blue_id += 1;
        bluePieces.push_back(p);
        scene -> addItem(p);

        // Connect slots to piece
        connect(p, &Piece::PieceLeftClicked,
                this, &PlotWindow::PieceLeftClickedSlot);
        connect(p, &Piece::PieceRightClicked,
                this, &PlotWindow::PieceRightClickedSlot);

        // Color the square to the color
        grid[x][y]->set_color(QColor(0, 0, 205, 255));
        grid[x][y]->set_team(1);
        grid[x][y]->set_hasPiece(1);
    }
    // else red team
    else
    {
        Piece *p = Base::GetPiece(red_id, 0, x*30, y*30, level);
        red_id += 1;
        redPieces.push_back(p);
        scene -> addItem(p);

        // Connect slots to piece
        connect(p, &Piece::PieceLeftClicked,
                this, &PlotWindow::PieceLeftClickedSlot);
        connect(p, &Piece::PieceRightClicked,
                this, &PlotWindow::PieceRightClickedSlot);

        // Color the square to the color
        grid[x][y]->set_color(QColor(139, 0, 0, 255));
        grid[x][y]->set_team(0);
        grid[x][y]->set_hasPiece(0);
    }

    // Another piece so another move that can be made
    moves_left += 1;

    // Determines how many squares of each color there are
    Find_Colors();

    // Update labels for number of tiles colored
    Update_Tiles();

    // Update gold labels
    Update_Gold();

    // Update the number of moves left label
    Update_Moves_Label();
}

// Function gets called when a piece is landed on by a piece of the opposite color
// Sens piece back to the player's main base
void PlotWindow::SendPieceHome(Piece* p)
{
    // If current player is blue, then piece sent back is on red team
    if(player_)
    {
        Base* b = redBases[0];
        // Get the x,y locations of the new piece
        int x = b -> get_x()/30;
        int y = b -> get_y()/30;
        CheckSurroundings(x, y);
        // If x and y don't change, then there was no space to create a piece
        if((x == b->get_x()/30) && (y == b->get_y()/30))
        {
            return;
        }
        // Create a new piece with the same id as the previous piece
        int id = p->get_id();
        Piece* newPiece = new Piece(id, 0, x*30, y*30, p->get_level());
        scene -> addItem(newPiece);
        // Connect all the signals of the new piece
        connect(newPiece, &Piece::PieceLeftClicked,
                this, &PlotWindow::PieceLeftClickedSlot);
        connect(newPiece, &Piece::PieceRightClicked,
                this, &PlotWindow::PieceRightClickedSlot);

        // Color the square red
        grid[x][y]->set_color(QColor(139, 0, 0, 255));
        grid[x][y]->set_team(0);
        grid[x][y]->set_hasPiece(0);

        // Remove the piece at the previous location from the view
        scene -> removeItem(p);

        // Replace piece in red team's piece vector
        redPieces[unsigned(id)] = newPiece;

        // Then remove original piece from the view
        delete(p);
        tempPiece = newPiece;
    }
    else
    {
        Base* b = blueBases[0];
        // Get the x,y locations of the new piece
        int x = b -> get_x() / 30;
        int y = b -> get_y() / 30;
        CheckSurroundings(x, y);
        // Make sure the piece isn't spawning on the base
        if((x == b->get_x()/30) && (y == b->get_y()/30))
        {
            return;
        }
        // Create a new piece with the same id as the previous piece
        int id = p->get_id();
        Piece* newPiece = new Piece(id, 1, x*30, y*30, p->get_level());
        scene -> addItem(newPiece);
        // Connect all the signals of the new piece
        connect(newPiece, &Piece::PieceLeftClicked,
                this, &PlotWindow::PieceLeftClickedSlot);
        connect(newPiece, &Piece::PieceRightClicked,
                this, &PlotWindow::PieceRightClickedSlot);

        // Color the new square blue
        grid[x][y]->set_color(QColor(0, 0, 205, 255));
        grid[x][y]->set_team(1);
        grid[x][y]->set_hasPiece(1);

        qDebug() << "blue piece sent back";


        // Remove the piece at the previous location from the view
        scene -> removeItem(p);

        // Replace piece in red team's piece vector
        redPieces[unsigned(id)] = newPiece;

        // Then remove original piece from the view
        delete(p);
        tempPiece = newPiece;
    }
}

// Checks whether the surrounding grid is out of bounds or has a piece on it
// Passes x and y has reference to update both to first location that is possible
void PlotWindow::CheckSurroundings(int& x, int& y)
{
    // Check surrounding cells to see if there is a piece there or if out of bounds
    // above
    if((y != 0) && (grid[x][y-1]->get_hasPiece() == -1))
    {
        y -= 1;
    }
    // down
    else if((y != 14) && (grid[x][y+1]->get_hasPiece() == -1))
    {
        y += 1;
    }
    // right
    else if((x != 14) && (grid[x+1][y]->get_hasPiece() == -1))
    {
        x += 1;
    }
    // left
    else if((x != 0) && (grid[x-1][y]->get_hasPiece() == -1))
    {
        x -= 1;
    }
    // top right
    else if((x != 14) && (y != 0) && (grid[x+1][y-1]->get_hasPiece() == -1))
    {
        x += 1;
        y -= 1;
    }
    // top left
    else if((x != 0) && (y != 0) && (grid[x-1][y-1]->get_hasPiece() == -1))
    {
        x -= 1;
        y -= 1;
    }
    // bot left
    else if((x != 0) && (y != 14) && (grid[x-1][y+1]->get_hasPiece() == -1))
    {
        x -= 1;
        y += 1;
    }
    // bot right
    else if((x != 14) && (y != 14) && (grid[x+1][y+1]->get_hasPiece() == -1))
    {
        x += 1;
        y += 1;
    }
    else
    {
        qDebug() << "No space to create piece";
    }
}

// Function for creating a base
void PlotWindow::On_CreateBaseButton_Clicked()
{
    // Assign isBuildingBaseState to opposite of what it was
    isBuildingBaseState = !isBuildingBaseState;

    // If state is turned on, then color grid.
    if(isBuildingBaseState)
    {
        // Loop through entire grid and determine which squares are of the proper color
        for(int i = 0; i < 15; i++)
        {
            for(int j = 0; j < 15; j++)
            {
                if((grid[i][j]->get_team() == player_) && (grid[i][j]->get_hasPiece() == -1))
                {
                    // If square is of the team's color, then color it green
                    grid[i][j] -> set_color(QColor(57, 255, 20, 255));
                    grid[i][j] -> set_canBuild(true);
                }
            }
        }
    }
    // Otherwise turning it off, so recolor grid back to white
    else
    {
        // Iterate over entire grid
        for(int i = 0; i < 15; i++)
        {
            for(int j = 0; j < 15; j++)
            {
                // Recolor and reassign each square that could've been built on to its original color
                if(grid[i][j] -> get_canBuild())
                {
                    if(player_)
                    {
                        grid[i][j] -> set_color(QColor(0, 0, 205, 255));
                    }
                    else
                    {
                        grid[i][j] -> set_color(QColor(139, 0, 0, 255));
                    }
                    grid[i][j] -> set_canBuild(false);
                }
            }
        }
    }
}

// Function to create a new base for the player
void PlotWindow::CreateBase(Square* s)
{
    // Create a new base at the selected location
    int x = s -> get_x() / 30;
    int y = s -> get_y() / 30;

    // Check that the player has enough gold (3).
    // If the do, subtract that much gold.
    if(player_)
    {
        if(blue_gold_ < 3)
        {
            qDebug() << "Not enough gold";
            return;
        }
        // subtract the gold
        blue_gold_ -= 3;
        // Create the base and add to vector of bases
        Base* b = new Base(0, x*30, y*30);
        blueBases.push_back(b);
        connect(b, &Base::BaseLeftClicked,
                this, &PlotWindow::BaseLeftClickedSlot);
        // Add to scene and make visual
        scene -> addItem(b);
        grid[x][y]->set_hasPiece(1);
    }
    else
    {
        if(red_gold_ < 3)
        {
            qDebug() << "Not enough gold";
            return;
        }
        // subtract the gold
        red_gold_ -= 3;
        // Create the base and add to vector of bases
        Base* b = new Base(0, x*30, y*30);
        redBases.push_back(b);
        connect(b, &Base::BaseLeftClicked,
                this, &PlotWindow::BaseLeftClickedSlot);
        // Add to scene and make visual
        scene -> addItem(b);
        grid[x][y]->set_hasPiece(0);
    }

    // Turn off the BaseBuilding State
    On_CreateBaseButton_Clicked();
}


// Function to simulate a certain number of games
void PlotWindow::SimulateGames()
{
    // Get the number of games to be simulated
    bool safe = false;
    int numGames = ui -> SimulateGamesEntry -> text().toInt(&safe);
    if (!safe) {
        qDebug() << "Enter an amount of simulations";
        return;
    }

    // Clear the graph and print "simulating"
    graph -> clear();
    graph -> addText(QString("Simulating"));

    // Perform that number of simulations
    for(int i = 0; i < numGames; i++)
    {
        SimulateOutcome();
        // If less than 25 games stored in array, then add to end
        if(i < 25)
        {
            RedOutcomes[i] = red_colored_;
            BlueOutcomes[i] = blue_colored_;
        }
        // Otherwise move each element 1 to the left and put new at the end.
        else
        {
            int tempR = 0;
            int tempB = 0;
            for(int j = 24; j > -1; j++)
            {
                tempR = RedOutcomes[j];
                RedOutcomes[j] = red_colored_;
                red_colored_ = tempR;

                tempB = BlueOutcomes[j];
                BlueOutcomes[j] = blue_colored_;
                blue_colored_ = tempB;
            }
        }
    }

    // Then Plot the graph after all the simulations
    UpdateGraph();
}

// Simulates a single outcome of the game
void PlotWindow::SimulateOutcome()
{
    NewGame();
    while(!is_game_over)
    {
        // Performs the player's actions
        TakeTurn();
    }
}

// Takes a single turn of the game
void PlotWindow::TakeTurn()
{
    // Does 4 things
    // Step 1: Decides to upgrade pieces
    // Step 2: Randomly decides which pieces to move
    // Step 3: Randomly moves pieces to squares that are not of the same color.
    // Step 4: Buy more pieces

    // Have to give both teams specific instructions
    if(player_)
    {
        // Step 1 and step 2
        for(int i = 0; i < bluePieces.size(); i++)
        {
            // Level up the piece. Function takes care if level 3 or not enough gold.
            On_LevelUpButton_Clicked();
            LevelUpPiece(bluePieces[i]);
            // Decide if the piece will move
            // 50% chance to move, 50% chance to stay
            int r = rand() % 2;
            if(r == 1)
            {
                PieceLeftClickedSlot(bluePieces[i]);
                // Step 3: Randomly move squares to pieces of a different color
                int x = bluePieces[i] -> get_x() / 30;
                int y = bluePieces[i] -> get_y() / 30;
                RandomDirection(x, y, bluePieces[i]->get_level());
                GreenSquareLeftClickedSlot(grid[x][y]);
            }
        }
        // Step 4. Only buys pieces of first level because choices are difficult
        if(blue_gold_ >= 5)
        {
            On_CreatePieceButton_Clicked();
            BaseLeftClickedSlot(blueBases[0]);
        }
        // If not everything moved, end the turn
        if(moves_left > 0)
        {
            On_EndTurnButton_Clicked();
        }
    }
    // Repeat for red player's stuff
    else
    {
        // Step 1 and step 2
        for(int i = 0; i < redPieces.size(); i++)
        {
            // Level up the piece. Function takes care if level 3 or not enough gold.
            On_LevelUpButton_Clicked();
            LevelUpPiece(redPieces[i]);
            // Decide if the piece will move
            // 50% chance to move, 50% chance to stay
            int r = rand() % 2;
            if(r == 1)
            {
                PieceLeftClickedSlot(redPieces[i]);
                // Step 3: Randomly move squares to pieces of a different color
                int x = redPieces[i] -> get_x() / 30;
                int y = redPieces[i] -> get_y() / 30;
                RandomDirection(x, y, redPieces[i]->get_level());
                GreenSquareLeftClickedSlot(grid[x][y]);
            }
        }
        // Step 4. Only buys pieces of first level because choices are difficult
        if(red_gold_ >= 5)
        {
            On_CreatePieceButton_Clicked();
            BaseLeftClickedSlot(redBases[0]);
        }
        // If not everything moved, end the turn
        if(moves_left > 0)
        {
            On_EndTurnButton_Clicked();
        }
    }
}

// Funciton to return a random x,y that is a valid place to move to.
void PlotWindow::RandomDirection(int& x, int& y, int level)
{
    // Take a random walk 20 times. Exits on the first valid result
    // 20 iterations to prevent infinite loops
    for(int i = 0; i < 20; i++)
    {
        int dx = 0;
        int dy = 0;
        // Possible random moves depends on level of the piece. Level 1 can move up, down, left, right
        if(level == 1)
        {
            int r = rand() % 4;
            if(r == 0) {dx = x + 1; }
            else if(r == 1) {dx = x - 1; }
            else if(r == 2) {dy = y + 1; }
            else if(r == 3) {dy = y - 1; }
            else {qDebug() << "Rand is wrong"; }
        }
        // Level 2 can move in all 8 directions by 1
        else if(level == 2)
        {
            dx = x + (rand() % 3) - 1;
            dy = y + (rand() % 3) - 1;
        }
        // Level 3 can move 16 ways
        else if(level == 3)
        {
            dx = x + (rand() % 5) - 2;
            dy = y + (rand() % 5) - 2;
        }

        // Check that grid[dx][dy] is within bounds. Redo random # otherwise.
        if((dx < 0) || (dx > 14) || (dy < 0) || (dy > 14))
        {
            continue;
        }

        // Check that the square is "green," otherwise is not a valid choice.
        if(grid[dx][dy]->get_team() < 2)
        {
            continue;
        }

        // Check if a piece is already on that spot. Don't move ontop of another piece
        if(grid[dx][dy]->get_hasPiece() != -1)
        {
            continue;
        }

        // Check if the square is already colored the same color as the current player
        // If on the last iteration, let the piece move onto a square that is colored the same color
        // Prevents piece from getting stuck. Hopefully.
        if((grid[dx][dy]->get_team() == player_) && (i != 20))
        {
            continue;
        }
        // Update references to the new random locations.
        x = dx;
        y = dy;
        break;
    }
}

// Updates the graph at the bottom of the window.
// Creates two bars, one blue and one red, the represent the proportion of squares colored at the end of the game.
void PlotWindow::UpdateGraph()
{
    // Clear the previous graph
    graph -> clear();
    // Print to the graph of number of final cells colored.
    for(int i = 0; i < 25; i++)
    {
        // Calculate the proportion of total squares colored
        double propRed = double(RedOutcomes[i]) / 225.0 * 100.0;
        double propBlue = double(BlueOutcomes[i]) / 225.0 * 100.0;
        // Create a rectangle with height equal to the proportion
        QGraphicsRectItem* redRect = new QGraphicsRectItem(i*25, 100 - int(propRed), 25, propRed);
        QGraphicsRectItem* blueRect = new QGraphicsRectItem(i*25, 100 - int(propRed) - int(propBlue), 25, propBlue);
        // Set the color of each rectangle
        redRect -> setBrush(QBrush(Qt::red));
        blueRect -> setBrush(QBrush(Qt::blue));
        // Add the rectangles to the window
        graph -> addItem(redRect);
        graph -> addItem(blueRect);
    }
    return;
}








