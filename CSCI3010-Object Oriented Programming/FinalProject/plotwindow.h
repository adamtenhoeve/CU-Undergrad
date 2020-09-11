#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

#include "square.h"
#include "piece.h"
#include "base.h"

using namespace std;

// Name: Adam Ten Hoeve

namespace Ui {
class PlotWindow;
}

class PlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlotWindow(QWidget *parent = 0);
    ~PlotWindow();

private slots:
    // Select which square to move to after selecting a piece to move
    void GreenSquareLeftClickedSlot(Square*);
    // Select a piece to move.
    void PieceLeftClickedSlot(Piece*);
    // Deselect a piece after selecting it.
    void PieceRightClickedSlot();
    // When EndTurn is clicked
    void On_EndTurnButton_Clicked();
    // When NewGame is clicked
    void On_NewGameButton_Clicked();
    // When LevelUp is clicked
    void On_LevelUpButton_Clicked();
    // When a base is left clicked
    void BaseLeftClickedSlot(Base*);
    // When the CreatePiece Button is clicked
    void On_CreatePieceButton_Clicked();
    // When the CreateBase Button is clicked
    void On_CreateBaseButton_Clicked();

private:

    Ui::PlotWindow *ui;
    // The two scenes of the GUI
    QGraphicsScene *scene;
    QGraphicsScene *graph;

    // Boolean for if game is still going. If true, then someone has won the game.
    bool is_game_over;
    // Current player. 0 if Red player, 1 if blue player.
    bool player_;
    // Moves left of the current turn
    int moves_left;
    //Number of blue pieces. Keep track of which one is in vector
    int blue_id;
    // Number of red pieces.
    int red_id;
    // Number of cells colored blue
    int blue_colored_;
    // Number of cells colored red
    int red_colored_;
    // Amount of gold on blue team
    int blue_gold_;
    // Amount of gold on red team
    int red_gold_;

    // State check for if a piece has been clicked already.
    bool isPieceClicked;
    // State so when pieces are clicked, they are leveled up instead of moved
    bool isLevelUpState;
    // State for when pieces are being created. Only interacts with bases when active
    bool isPieceCreatedState;
    // State for when a base is about to be built
    bool isBuildingBaseState;

    // Grid of squares that can be clicked. Represent the cells of the game.
    Square *grid[15][15];
    // Vector of blue pieces created
    vector<Piece*> bluePieces;
    // Vector of blue bases created
    vector<Base*> blueBases;
    // Vector of red pieces created
    vector<Piece*> redPieces;
    // Vector of red bases created
    vector<Base*> redBases;
    // Temperary piece to be stored when clicked
    Piece *tempPiece;

    // Array to keep track of squares painted at end of 25 simulated rounds
    int RedOutcomes[25];
    int BlueOutcomes[25];
    int games;

    // Function to initialize all starting values and populate the board in the initial game state
    void NewGame();

    // These functions update all the labels of the view.
    void Update_Tiles();
    void Update_Gold();
    void Update_Player_Label();
    void Update_Moves_Label();
    // Find the number of tiles each player has
    void Find_Colors();
    // Move a piece from one square to another
    void MovePiece(Piece*);
    // When a piece is clicked, color the surrounding tiles green
    void ColorSquare(int, int);
    // Level up a piece and subtract gold
    void LevelUpPiece(Piece*);
    // Level up a base and subtract gold
    void LevelUpBase(Base*);


    // Functions for creating pieces
    // Checks which squares don't have other pieces on them so a new piece can be placed there
    void FindNewPieceLocation(Base*);
    // Checks if that square is valid
    void CheckSurroundings(int&, int&);
    // Creates a new piece and updates all tracking info.
    void CreatePiece(int, int, int);
    // When a piece is "captured" by an opponents piece, it is sent back to the main base.
    void SendPieceHome(Piece* p);

    // Creates a base on any square of that player's color.
    void CreateBase(Square*);

    // Functions to Automate games
    // Simulates a specified number of games. Takes a while.
    void SimulateGames();
    // Simulates a single game.
    void SimulateOutcome();
    // Takes a single turn of a game. Decides what each piece is going to do randomly.
    // Also upgrades pieces if it has enough gold
    void TakeTurn();
    // When a piece decides to move, it picks a random direction to move in.
    void RandomDirection(int&, int&, int);
    // Updates the graph at the bottom when a game is completed to show how many squares each player had.
    void UpdateGraph();
};

#endif // PLOTWINDOW_H

