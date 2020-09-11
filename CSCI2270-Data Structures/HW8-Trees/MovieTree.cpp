#include <iostream>
#include "MovieTree.h"
using namespace std;

MovieTree::MovieTree()
{
  root = NULL;
}

MovieTree::~MovieTree()
{
  MovieNode *temp = root;
  if (temp -> leftChild != NULL)
  {
    DeleteAll(temp -> leftChild);
  }
  if (temp -> rightChild != NULL)
  {
    DeleteAll(temp -> rightChild);
  }
  cout << "Deleting: " << temp -> title << endl;
}

void MovieTree::DeleteAll(MovieNode *temp)
{
  if (temp -> leftChild != NULL)
  {
    DeleteAll(temp -> leftChild);
  }
  if (temp -> rightChild != NULL)
  {
    DeleteAll(temp -> rightChild);
  }
  cout << "Deleting: " << temp -> title << endl;
  delete temp;
}

void MovieTree::printMovieInventory(MovieNode *node)
{
  MovieNode *temp = node;
  if (temp -> leftChild != NULL)
  {
    printMovieInventory(temp -> leftChild);
  }
  cout << "Movie: " << temp -> title << " " << temp -> quantity << endl;
  if (temp -> rightChild != NULL)
  {
    printMovieInventory(temp -> rightChild);
  }
}

void MovieTree::printMovieInventory()
{
  MovieNode *temp = root;
  if (temp -> leftChild != NULL)
  {
    printMovieInventory(temp -> leftChild);
  }
  else
  {
    cout << "left child is NULL" << endl;
  }
  cout << "Movie: " << temp -> title << " " << temp -> quantity << endl;
  if (temp -> rightChild != NULL)
  {
    printMovieInventory(temp -> rightChild);
  }
}

void MovieTree::deleteMovieNode(string movie)
{
  MovieNode *temp = search(movie);
  if (temp == NULL)
  {
    cout << "Movie not found" << endl;
    return;
  }
  MovieNode *parent = temp -> parent;
  if (temp != root)
  {
    if (temp -> leftChild == NULL && temp -> rightChild == NULL)
    {
      if (temp -> title < temp -> parent -> title)
      {
        temp -> parent -> leftChild = NULL;
      }
      else if (temp -> title > temp -> parent -> title)
      {
        temp -> parent -> rightChild = NULL;
      }
    }
    else if (temp -> leftChild != NULL && temp -> rightChild != NULL)
    {
      MovieNode *min = temp -> rightChild;
      while (min -> leftChild != NULL)
      {
        min = min -> leftChild;
      }
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
        if (min -> rightChild != NULL)
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
    }
    else if (temp -> rightChild == NULL && temp -> leftChild != NULL)
    {
      MovieNode *x = temp -> leftChild;
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
    else if (temp -> rightChild != NULL && temp -> leftChild == NULL)
    {
      MovieNode *x = temp -> rightChild;
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
    if (temp -> leftChild != NULL && temp -> rightChild != NULL)
    {
      MovieNode *min = temp -> rightChild;
      while (min -> leftChild != NULL)
      {
        min = min -> leftChild;
      }
      if (min == temp -> rightChild)
      {
        min -> parent = NULL;
        min -> leftChild = temp -> leftChild;
        min -> leftChild -> parent = min;
      }
      else
      {
        if (min -> rightChild != NULL)
        {
          min -> rightChild -> parent = min -> parent;
        }
        min -> parent -> leftChild = min -> rightChild;
        min -> parent = NULL;
        min -> leftChild = root -> leftChild;
        min -> rightChild = root -> rightChild;
        min -> rightChild -> parent = min;
        min -> leftChild -> parent = min;
        root = min;
      }
    }
    else if (temp -> rightChild == NULL && temp -> leftChild != NULL)
    {
      MovieNode *x = temp -> leftChild;
      root = x;
      x -> parent = NULL;
    }
    else if (temp -> rightChild != NULL && temp -> leftChild == NULL)
    {
      MovieNode *x = temp -> rightChild;
      root = x;
      x -> parent = NULL;
    }
  }
  delete temp;
}

void MovieTree::addMovieNode(int ranking, string title, int year, int quantity)
{
  MovieNode *n = new MovieNode (ranking, title, year, quantity);
  MovieNode *temp = root;
  MovieNode *parent = NULL;
  if (temp == NULL)
  {
    root = n;
    n -> parent = NULL;
    n -> rightChild = NULL;
    n -> leftChild = NULL;
    return;
  }
  while (temp != NULL)
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
  }
  else if (title > parent -> title)
  {
    n -> parent = parent;
    parent -> rightChild = n;
  }
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
  if (temp -> leftChild != NULL)
  {
    countMovieNodes(temp -> leftChild, &count);
  }
  if (temp -> rightChild != NULL)
  {
    countMovieNodes(temp -> rightChild, &count);
  }
  count++;
  return count;
}

void MovieTree::countMovieNodes(MovieNode *node, int *count)
{
  MovieNode *temp = node;
  if (temp -> leftChild != NULL)
  {
    countMovieNodes(temp -> leftChild, count);
  }
  if (temp -> rightChild != NULL)
  {
    countMovieNodes(temp -> rightChild, count);
  }
  (*count)++;
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
