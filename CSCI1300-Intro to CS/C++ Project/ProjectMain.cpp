# include <iostream>
# include <iomanip>
# include <fstream>

# include "Card.h"
# include "DeckofCards.h"
# include "Dealer.h"
# include "player.h"

using namespace std;

void BlackJack (Player &Me, Player &You)
{
        //Betting
        int bet;
        cout << "You have $" << You.getChange() << ". How much would you like to bet?" << endl; // Says how much money the player has.
        cin >> bet;
        if (bet > You.getChange())
        {
            cout << "You can't bet more money than you have. How much do you want to bet?" << endl; // You can't bet more money than you have, that would be stupid.
            cin >> bet;
        }

        // Let's play the game
        int n = 0;
        int i = 0;
        Deck deck;
        ifstream ReadMe;
        ReadMe.open("Deck.txt"); // Reads in all of the data for each card from a text file.
        if (!ReadMe.is_open())
        {
            cout << "Did not open."<<endl;
        }
        string face, suit, amount, sortAmount;
        while (!ReadMe.eof())
        {
            getline(ReadMe, face);
            getline(ReadMe, suit);
            getline(ReadMe, amount);
            getline(ReadMe, sortAmount);
            face.erase(face.find_last_not_of(" \n\r\t")+1);
            suit.erase(suit.find_last_not_of(" \n\r\t")+1);
            int Amount = stoi (amount);
            int Sort = stoi (sortAmount);
            deck.cards[i].setFace(face);
            deck.cards[i].setSuit(suit);
            deck.cards[i].setAmount(Amount);
            deck.cards[i].setSortAmount(Sort); // Assigning all of the variables.
            i++;
        }
        ReadMe.close();
        deck.Shuffle(); // Mix those cards so everything is all nice and random.

        You.Hand[0] = deck.cards[n];
        n++;
        You.sethandSize();

        Me.Hand[0] = deck.cards[n]; // Copies a card from the deck to the hand, then incremements the deck so the next card is taken.
        n++;
        Me.sethandSize();

        You.Hand[1] = deck.cards[n]; // Gives 2 cards to the player and 1 card to the dealer. They don't know if that's not really how the game is played.
        n++;
        You.sethandSize();

        int PCards = 2;
        int Pvalue = You.calcValue();
        cout << endl << "You have the:" << endl;

        cout << You.Hand[0].getFace() << " of " << You.Hand[0].getSuit() << endl;
        cout << You.Hand[1].getFace() << " of " << You.Hand[1].getSuit() << endl;
        cout << "You total is " << Pvalue << endl; // Tells the user all of the info they need to know.

        if (Pvalue == 21)
        {
            cout << endl << "Blackjack!" << endl; // If they have a blackjack, they don't get to play. So sad for them.
        }

        cout << endl << "The Dealer has the:" << endl; // Tells the player one of the cards that the dealer has, which is actually his only card.
        cout << Me.Hand[0].getFace() << " of " << Me.Hand[0].getSuit() << endl;
        cout << "And an unknown card." << endl;


        // Player's turn
        int answer = 0;
        if (Pvalue != 21)
        {
            cout << endl << "Would you like to hit or stay? Enter 1 to hit or 2 to stay." << endl; // First time asking the user if they want to hit or stay.
            cin >> answer;
            while ((answer < 1) || (answer > 2))
            {
                cout << "Invalid response. Please enter 1 or 2." << endl;
                cin >> answer;
            }
        }
        while (answer == 1) // This is the whole loop that gives the user cards if they keep hitting.
        {
            You.Hand[PCards] = deck.cards[n++]; // Gives them another card.
            You.sethandSize();
            cout << endl << "You got the " << You.Hand[PCards].getFace() << " of " << You.Hand[PCards].getSuit() << endl;
            PCards++;
            Pvalue = You.calcValue();
            cout << "Your total value is " << Pvalue << endl;
            if (Pvalue > 21) // Checks to see if they busted.
            {
                for (int i = 0; i < 10; i++)
                {
                    string ace = You.Hand[i].getFace();
                    if (ace == "Ace")
                    {
                        cout << "You went over 21, but you had an ace so your new value is "; // If they did bust, but they have an ace, then their scores are recalculated.
                        You.Hand[i].setAmount(1);
                        Pvalue = You.calcValue();
                        cout << Pvalue << "." << endl;
                        cout << endl << "Would you like to hit or stay? Enter 1 to hit or 2 to stay." << endl;
                        cin >> answer;
                    }
                }
                if (Pvalue > 21)
                {
                    cout << "You busted." << endl; // but they still bust if they don't. that means they can't play anymore.
                    Pvalue = 0;
                    answer = 2;
                }
            }
            else if (Pvalue == 21)
            {
               cout << "You got 21!" << endl;
               answer = 2;
            }
            else
            {
                cout << "Would you like to hit or stay? Enter 1 or 2." << endl;
                cin >> answer;
                cout << endl;
                while ((answer < 1) || (answer > 2))
                {
                    cout << "Invalid entry. Please enter 1 or 2." << endl;
                    cin >> answer;
                }
            }
        }
        cout << endl << "Your total score is " << Pvalue << "." << endl;


        cout << "Now it's the dealer's turn. Enter a value to continue." << endl; // Just to separate out the code a bit.
        string none;
        cin >> none;


        // Dealer's turn
        int Dcards = 1, Dvalue = 0;
        cout << endl << "The Dealer's cards are:" << endl;
        cout << Me.Hand[0].getFace() << " of " << Me.Hand[0].getSuit() << endl;
        bool stand = false;
        while (stand == false)
        {
            Me.Hand[Dcards] = deck.cards[n++];
            Me.sethandSize();
            cout << Me.Hand[Dcards].getFace() << " of " << Me.Hand[Dcards].getSuit() << endl; // The player doesn't know that you only now gave the dealer the second card.
            Dcards++;
            Me.sethandSize();
            Dvalue = Me.calcValue();
            cout << "The Dealer's current score is " << Dvalue << "." << endl << endl;
            if (Dvalue > 21)
            {
                for (int i = 0; i < 10; i++)
                {
                    string ace = Me.Hand[i].getFace();
                    if (ace == "Ace")
                    {
                        Me.Hand[i].setAmount(1);
                        Dvalue = Me.calcValue();
                        cout << "The Dealer went over 21, but he had an ace so his new score is " << Dvalue << endl; // Same thing with aces. Checks if over 21 and reassigns values if yes.
                    }
                    if (Dvalue > 21)
                    {
                        cout << "The dealers busts." << endl;
                        Dvalue = 0;
                        stand = true;
                    }
                }
            }
            if ((Dvalue > 16) && (Dvalue <= 21)) // Dealer has to stand if the value is 17 or greater.
            {
                stand = true;
            }
        }

        cout << endl << "The Dealer's final value is " << Dvalue << endl;


        // Who wins?

        if( Dvalue > Pvalue) // Comparison of player's score to dealer's score to see who wins.
        {
            cout << "The Dealer wins." << endl;
            You.setChangeLoss(bet);
            cout << endl << "Your now have $" << You.getChange() << "." << endl;
        }
        if (Dvalue < Pvalue)
        {
            cout << "You win! We should record this in the hall of champions! What is your name?" << endl; // If they win, they get to carve their name into the hall of champions of blackjack.
            string input;
            cin >> input;
            ofstream Editme;
            Editme.open("LookAtAllTheWinners.txt");
            Editme << input << " \n";
            Editme.close();
            You.setChangeWin(bet); // And the bets increase or decrease the money that the player has depending on if they won or lost.
            cout << endl << "Your now have $" << You.getChange() << "." << endl;
        }
        if (Dvalue == Pvalue)
        {
            cout << "It is a push. Nobody loses." << endl;
        }
        Me.Clear();
        You.Clear(); // Clears the hands of the player and dealer so it doesn't add extra cards if they want to play again.
}

int main ()
{
    Player dealer; // Declaration of dealer and player class. Because they're declared there, the money gets saved between plays.
    Player player;
    cout << "Preparing to play Blackjack." << endl;
    int Playing = 1;
    while (Playing == 1)
    {
        BlackJack(dealer, player);
        if (player.getChange() <= 0)
        {
            Playing = 0;
            cout << "You are out of money. Thank you for playing. Please come again soon." << endl;
            return 0;
        }
        cout << "Would you like to play again? Press 1 for yes, 2 for no." << endl;
        cin >> Playing;
        if ((Playing < 1) || (Playing > 2)) // Asks the user if they want to play again.
        {
            cout << "I don't know what that input means, so lets play again!" << endl;
        }
        if (Playing == 2)
        {
            Playing = false;
        }
    }
    cout << "Thank you for playing. Please come again." << endl;
    return 0;
}
