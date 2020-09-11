#include <iostream>
#include "MovieTree.h"
using namespace std;

MovieTree::MovieTree()
{
  root = NULL;
  nil = new MovieNode (0, "nil", 0, 0);
  nil -> isRed = false;
  nil -> leftChild = nil;
  nil -> rightChild = nil;
  nil -> parent = nil;
}

MovieTree::~MovieTree()
{
  MovieNode *temp = root;
  if (temp -> leftChild != nil)
  {
    DeleteAll(temp -> leftChild);
  }
  if (temp -> rightChild != nil)
  {
    DeleteAll(temp -> rightChild);
  }
  cout << "Deleting: " << temp -> title << endl;
}

void MovieTree::DeleteAll(MovieNode *temp)
{
  if (temp -> leftChild != nil)
  {
    DeleteAll(temp -> leftChild);
  }
  if (temp -> rightChild != nil)
  {
    DeleteAll(temp -> rightChild);
  }
  cout << "Deleting: " << temp -> title << endl;
  delete temp;
}

void MovieTree::printMovieInventory(MovieNode *node)
{
  MovieNode *temp = node;
  if (temp -> leftChild != nil)
  {
    printMovieInventory(temp -> leftChild);
  }
  cout << "Movie: " << temp -> title << " " << temp -> quantity << endl;
  if (temp -> rightChild != nil)
  {
    printMovieInventory(temp -> rightChild);
  }
}

void MovieTree::printMovieInventory()
{
  MovieNode *temp = root;
  if (temp -> leftChild != nil)
  {
    printMovieInventory(temp -> leftChild);
  }
  else
  {
    cout << "left child is NULL" << endl;
  }
  cout << "Movie: " << temp -> title << " " << temp -> quantity << endl;
  if (temp -> rightChild != nil)
  {
    printMovieInventory(temp -> rightChild);
  }
}

void MovieTree::deleteMovieNode(string movie)
{
  MovieNode *temp = search(movie);
  MovieNode *x = NULL;
  if (temp == NULL)
  {
    cout << "Movie not found" << endl;
    return;
  }
  rbValid(temp);
  bool nodeColor = temp -> isRed;
  MovieNode *parent = temp -> parent;
  if (temp != root)
  {
    if (temp -> leftChild == nil && temp -> rightChild == nil)
    {
      if (temp -> title < temp -> parent -> title)
      {
        temp -> parent -> leftChild = nil;
        x = temp -> parent -> leftChild;
        x -> parent = temp -> parent;
      }
      else if (temp -> title > temp -> parent -> title)
      {
        temp -> parent -> rightChild = nil;
        x = temp -> parent -> rightChild;
        x -> parent = temp -> parent;
      }
    }
    else if (temp -> leftChild != nil && temp -> rightChild != nil)
    {
      MovieNode *min = temp -> rightChild;
      while (min -> leftChild != nil)
      {
        min = min -> leftChild;
      }
      nodeColor = min -> isRed;
      x = min -> rightChild;
      x -> parent = min;
      if (min == temp -> rightChild)
      {
        if (temp == temp -> parent -> leftChild)
        {
          temp -> parent -> leftChild = min;
        }
        else
        {
          temp -> parent -> rightChild = min;
        }
        min -> parent = temp -> parent;
        min -> leftChild = temp -> leftChild;
        min -> leftChild -> parent = min;
      }
      else
      {
        min -> parent -> leftChild = min -> rightChild;
        if (min -> rightChild != nil)
        {
          min -> rightChild -> parent = min -> parent;
        }
        if (temp == temp -> parent -> leftChild)
        {
          temp -> parent -> leftChild = min;
        }
        else
        {
          temp -> parent -> rightChild = min;
        }
        min -> parent = temp -> parent;
        min -> leftChild = temp -> leftChild;
        min -> rightChild = temp -> rightChild;
        temp -> rightChild -> parent = min;
        temp -> leftChild -> parent = min;
      }
      min -> isRed = temp -> isRed;
    }
    else if (temp -> rightChild == nil && temp -> leftChild != nil)
    {
      x = temp -> leftChild;
      if (temp == temp -> parent -> leftChild)
      {
        temp -> parent -> leftChild = x;
      }
      else
      {
        temp -> parent -> rightChild = x;
      }
      x -> parent = temp -> parent;
    }
    else if (temp -> rightChild != nil && temp -> leftChild == nil)
    {
      x = temp -> rightChild;
      if (temp == temp -> parent -> leftChild)
      {
        temp -> parent -> leftChild = x;
      }
      else
      {
        temp -> parent -> rightChild = x;
      }
      x -> parent = temp -> parent;
    }
  }
  else
  {
    if (temp -> leftChild != nil && temp -> rightChild != nil)
    {
      MovieNode *min = temp -> rightChild;
      while (min -> leftChild != nil)
      {
        min = min -> leftChild;
      }
      nodeColor = min -> isRed;
      x = min -> rightChild;
      if (min == temp -> rightChild)
      {
        min -> parent = nil;
        min -> leftChild = temp -> leftChild;
        min -> leftChild -> parent = min;
      }
      else
      {
        if (min -> rightChild != nil)
        {
          min -> rightChild -> parent = min -> parent;
        }
        min -> parent -> leftChild = min -> rightChild;
        min -> parent = nil;
        min -> leftChild = root -> leftChild;
        min -> rightChild = root -> rightChild;
        min -> rightChild -> parent = min;
        min -> leftChild -> parent = min;
        root = min;
      }
    }
    else if (temp -> rightChild == nil && temp -> leftChild != nil)
    {
      x = temp -> leftChild;
      root = x;
      x -> parent = nil;
    }
    else if (temp -> rightChild != nil && temp -> leftChild == nil)
    {
      x = temp -> rightChild;
      root = x;
      x -> parent = nil;
    }
  }
  if (nodeColor == false)
  {
    rbDeleteFixup(x);
  }
  delete temp;
}

void MovieTree::rbDeleteFixup(MovieNode *x)
{
  while ((x != root) && (x -> isRed == false))
  {
    if (x == x -> parent -> leftChild)
    {
      MovieNode *sibling = x -> parent -> rightChild;
      if (sibling -> isRed == true) // Case 1
      {
        sibling -> isRed = false;
        x -> parent -> isRed = true;
        leftRotate(x -> parent);
        sibling = x -> parent -> rightChild;
      }
      if ((sibling -> leftChild -> isRed == false) && (sibling -> rightChild -> isRed == false)) // Case 2
      {
        sibling -> isRed = true;
        x = x -> parent;
      }
      else if ((sibling -> leftChild -> isRed == true) && (sibling -> rightChild -> isRed == false))
      {
        sibling -> leftChild -> isRed = false;
        sibling -> isRed = true;
        rightRotate (sibling);
        sibling = x -> parent -> rightChild;
      }
      else
      {
        sibling -> isRed = x -> parent -> isRed;
        x -> parent -> isRed = false;
        sibling -> rightChild -> isRed = false;
        leftRotate(x -> parent);
        x = root;
      }
    }
    else // x is a right Child
    {
      MovieNode *sibling = x -> parent -> leftChild;
      if (sibling -> isRed == true)
      {
        sibling -> isRed = false;
        x -> parent -> isRed = true;
        rightRotate (x -> parent);
        sibling = x -> parent -> leftChild;
      }
      if ((sibling -> leftChild -> isRed == false) && (sibling -> rightChild -> isRed == false))
      {
        sibling -> isRed = true;
        x = x -> parent;
      }
      else if ((sibling -> leftChild -> isRed == false) && (sibling -> rightChild -> isRed == true))
      {
        sibling -> rightChild -> isRed = false;
        sibling -> isRed = true;
        leftRotate (sibling);
        sibling = x -> parent -> leftChild;
      }
      else
      {
        sibling -> isRed = x -> parent -> isRed;
        x -> parent -> isRed = false;
        sibling -> rightChild -> isRed = false;
        rightRotate(x -> parent);
        x = root;
      }
    }
  }
  x -> isRed = false;
}

void MovieTree::addMovieNode(int ranking, string title, int year, int quantity)
{
  MovieNode *n = new MovieNode (ranking, title, year, quantity);
  MovieNode *temp = root;
  MovieNode *parent = NULL;
  if (temp == NULL)
  {
    root = n;
    n -> parent = nil;
    n -> rightChild = nil;
    n -> leftChild = nil;
    rbAddFixup(n);
    return;
  }
  while (temp != nil)
  {
    parent = temp;
    if (title < temp -> title)
    {
      temp = temp -> leftChild;
    }
    else if (title > temp -> title)
    {
      temp = temp -> rightChild;
    }
  }
  if (title < parent -> title)
  {
    n -> parent = parent;
    parent -> leftChild = n;
    n -> leftChild = nil;
    n -> rightChild = nil;
  }
  else if (title > parent -> title)
  {
    n -> parent = parent;
    parent -> rightChild = n;
    n -> leftChild = nil;
    n -> rightChild = nil;
  }
  rbAddFixup(n);
  //cout << rbValid(n) << endl;
  rbValid(n);
}

void MovieTree::rbAddFixup(MovieNode *n)
{
  while ((n != root) && (n -> parent -> isRed == true))
  {
    if (n -> parent == n -> parent -> parent -> leftChild)
    {
      MovieNode *uncle = n -> parent -> parent -> rightChild;
      if (uncle -> isRed == true)
      {
        //Case 1 on left
        n -> parent -> isRed = false;
        uncle -> isRed = false;
        n -> parent -> parent -> isRed = true;
        n = n -> parent -> parent;
      }
      else // Uncle color is black
      {
        if (n == n -> parent -> rightChild) // Case 2, right child on left
        {
          n = n -> parent;
          leftRotate(n);
        }
          n -> parent -> isRed = false;
          n -> parent -> parent -> isRed = true;
          rightRotate (n -> parent -> parent);
      }
    }
    else // n's parent is a right child
    {
      MovieNode *uncle = n -> parent -> parent -> leftChild;
      if (uncle -> isRed == true)
      {
        // Case 1 for right
        n -> parent -> isRed = false;
        uncle -> isRed = false;
        n -> parent -> parent -> isRed = true;
        n = n -> parent -> parent;
      }
      else // Uncle is black
      {
        if (n == n -> parent -> leftChild)
        {
          n = n -> parent;
          rightRotate(n);
        }
        n -> parent -> isRed = false;
        n -> parent -> parent -> isRed = true;
        leftRotate (n -> parent -> parent);
      }
    }
  }
  root -> isRed = false;
}

void MovieTree::rightRotate (MovieNode *y)
{
  MovieNode *x = y -> leftChild;
  y -> leftChild = x -> rightChild;
  if (x -> rightChild != nil)
  {
    x -> rightChild -> parent = y;
  }
  x -> parent = y -> parent;
  if (y -> parent == nil)
  {
    root = x;
  }
  else if (y == (y -> parent -> leftChild))
  {
    y -> parent -> leftChild = x;
  }
  else
  {
    y -> parent -> rightChild = x;
  }
  x -> rightChild = y;
  y -> parent = x;
}

void MovieTree::leftRotate (MovieNode *x)
{
  MovieNode *y = x -> rightChild;
  x -> rightChild = y -> leftChild;
  if (y -> leftChild != nil)
  {
    y -> leftChild -> parent = x;
  }
  y -> parent = x -> parent;
  if (x -> parent == nil)
  {
    root = y;
  }
  else
  {
    if (x == x -> parent -> leftChild)
    {
      x -> parent -> leftChild = y;
    }
    else
    {
      x -> parent -> rightChild = y;
    }
  }
  y -> leftChild = x;
  x -> parent = y;
}

void MovieTree::findMovie(string movie)
{
  MovieNode *temp = search(movie);
  if (temp == NULL)
  {
    cout << "Movie not found." << endl;
  }
  else
  {
    cout << "Movie Info:" << endl;
    cout << "===========" << endl;
    cout << "Ranking:" << temp -> ranking << endl;
    cout << "Title:" << temp -> title << endl;
    cout << "Year:" << temp -> year << endl;
    cout << "Quantity:" << temp -> quantity << endl;
  }
}

void MovieTree::rentMovie(string movie)
{
  MovieNode *temp = search(movie);
  if (temp == NULL)
  {
    cout << "Movie not found." << endl;
    return;
  }
  temp -> quantity = temp -> quantity - 1;
  cout << "Movie has been rented." << endl;
  cout << "Movie Info:" << endl;
  cout << "===========" << endl;
  cout << "Ranking:" << temp -> ranking << endl;
  cout << "Title:" << temp -> title << endl;
  cout << "Year:" << temp -> year << endl;
  cout << "Quantity:" << temp -> quantity << endl;
  if (temp -> quantity == 0)
  {
    deleteMovieNode(movie);
  }
}

int MovieTree::countMovieNodes()
{
  MovieNode *temp = root;
  int count = 0;
  if (temp -> leftChild != nil)
  {
    countMovieNodes(temp -> leftChild, &count);
  }
  if (temp -> rightChild != nil)
  {
    countMovieNodes(temp -> rightChild, &count);
  }
  count++;
  return count;
}

void MovieTree::countMovieNodes(MovieNode *node, int *count)
{
  MovieNode *temp = node;
  if (temp -> leftChild != nil)
  {
    countMovieNodes(temp -> leftChild, count);
  }
  if (temp -> rightChild != nil)
  {
    countMovieNodes(temp -> rightChild, count);
  }
  (*count)++;
}

int MovieTree::countLongestPath()
{
  int max = 1;
  MovieNode *temp = root;
  if (temp -> leftChild != nil)
  {
    countLongestPath (temp -> leftChild, &max);
  }
  if (temp -> rightChild != nil)
  {
    countLongestPath (temp -> rightChild, &max);
  }
  return max;
}

void MovieTree::countLongestPath(MovieNode *node, int *max)
{
  MovieNode *temp = node;
  if (temp -> leftChild != nil)
  {
    countLongestPath(temp -> leftChild, max);
  }
  if (temp -> rightChild != nil)
  {
    countLongestPath(temp -> rightChild, max);
  }
  if ((temp -> leftChild == nil) && (temp -> rightChild == nil))
  {
    int count = 1;
    MovieNode *reverse = temp;
    while (reverse -> parent != nil) // When this is set to reverse != nil, gives correct numbers but does not work in COG.
    {
      count++;
      reverse = reverse -> parent;
    }
    if (count > (*max))
    {
      (*max) = count;
    }
  }
}

MovieNode *MovieTree::search(string movie)
{
  MovieNode *temp = root;
  while (temp != NULL)
  {
    if (temp -> title == movie)
    {
      break;
    }
    if (movie < temp -> title)
    {
      temp = temp -> leftChild;
    }
    else if (movie > temp -> title)
    {
      temp = temp -> rightChild;
    }
  }
  return temp;
}

int MovieTree::rbValid(MovieNode * node)
{
    int lh = 0;
    int rh = 0;

    // If we are at a nil node just return 1
    if (node == nil)
        return 1;

    else
    {
        // First check for consecutive red links.
        if (node->isRed)
        {
            if(node->leftChild->isRed || node->rightChild->isRed)
            {
                cout << "This tree contains a red violation" << endl;
                return 0;
            }
        }

        // Check for valid binary search tree.
        if ((node->leftChild != nil && node->leftChild->title.compare(node->title) > 0) || (node->rightChild != nil && node->rightChild->title.compare(node->title) < 0))
        {
            cout << "This tree contains a binary tree violation" << endl;
            return 0;
        }

        // Deteremine the height of let and right children.
        lh = rbValid(node->leftChild);
        rh = rbValid(node->rightChild);

        // black height mismatch
        if (lh != 0 && rh != 0 && lh != rh)
        {
            cout << "This tree contains a black height violation" << endl;
            return 0;
        }

        // If neither height is zero, incrament if it if black.
        if (lh != 0 && rh != 0)
        {
                if (node->isRed)
                    return lh;
                else
                    return lh+1;
        }

        else
            return 0;

    }
}
