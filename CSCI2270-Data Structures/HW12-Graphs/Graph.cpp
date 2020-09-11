#include "Graph.h"
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

template<class T>
Graph<T>::Graph()
{

}

template<class T>
Graph<T>::~Graph()
{
    //dtor

}

template<class T>
void Graph<T>::addEdge(T v1, T v2, int weight){

    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i].name == v1){
            for(int j = 0; j < vertices.size(); j++){
                if(vertices[j].name == v2 && i != j){
                    adjVertex<T> av;
                    av.v = &vertices[j];
                    av.weight = weight;
                    vertices[i].adj.push_back(av);

                    //add edge in the other direction
                    //vertices[i].adj.push_back(av);
                    //another vertex for edge in other direction
                    //adjVertex<T> av2;
                    //av2.v = &vertices[i];
                    //av2.weight = weight;
                    //vertices[j].adj.push_back(av2);
                }
            }
        }
    }
}
template<class T>
void Graph<T>::addVertex(T n){
    bool found = false;
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i].name == n){
            found = true;
            cout<<vertices[i].name<<" found."<<endl;
        }
    }
    if(found == false){
        vertex<T> v;
        v.name = n;
        vertices.push_back(v);

    }
}

template<class T>
void Graph<T>::displayEdges(){
    //loop through all vertices and adjacent vertices
    for(int i = 0; i < vertices.size(); i++){
        cout<<vertices[i].name<<"-->";
        for(int j = 0; j < vertices[i].adj.size(); j++){
            cout<<vertices[i].adj[j].v->name<<"***";
        }
        cout<<endl;
    }

}

template<class T>
string Graph<T>::getVertex(int i)
{
  return vertices[i].name;
}

template<class T>
void Graph<T>::printVerticies()
{
  for(int i = 0; i < vertices.size(); i++)
  {
    cout << vertices[i].district << ":" << vertices[i].name << "-->";
    for (int j = 0; j < vertices[i].adj.size(); j++)
    {
      cout<<vertices[i].adj[j].v->name;
      if (j != vertices[i].adj.size() - 1)
      {
        cout << "***";
      }
    }
    cout << endl;
  }
}


template<class T>
void Graph<T>::BFS2(vertex<string>* v, vertex<string>* endValue)
{
  v -> visited = true;
  v -> distance = 0;
  v -> parent = NULL;
  queue<vertex<string>*> myQueue;
  myQueue.push(v);
  while (myQueue.size() != 0)
  {
    vertex<string> *n = myQueue.front();
    myQueue.pop();
    for (int x = 0; x < n -> adj.size(); x++)
    {
      if (n -> adj[x].v -> visited == false)
      {
        n -> adj[x].v -> distance = n -> distance + 1;
        n -> adj[x].v -> parent = n;
        n -> adj[x].v -> visited = true;
        myQueue.push(n -> adj[x].v);
      }
    }
  }
}

template<class T>
void Graph<T>::BFS(string startValue, int i)
{
  vertex<string> *v;
  for(int i = 0; i < vertices.size(); i++)
  {
    if (startValue == vertices[i].name)
    {
      v = &vertices[i];
      break;
    }
  }
  v -> visited = true;
  v -> distance = 0;
  v -> parent = NULL;
  queue<vertex<string>*> myQueue;
  myQueue.push(v);
  while (myQueue.size() != 0)
  {
    vertex<string> *n = myQueue.front();
    myQueue.pop();
    n -> district = i;
    for (int x = 0; x < n -> adj.size(); x++)
    {
      if (n -> adj[x].v -> visited == false)
      {
        n -> adj[x].v -> distance = n -> distance + 1;
        n -> adj[x].v -> parent = n;
        n -> adj[x].v -> visited = true;
        myQueue.push(n -> adj[x].v);
      }
    }
  }
}

template <class T>
void Graph<T>::findDistricts()
{
  int n = 1;
  for (int i = 0; i < vertices.size(); i++)
  {
    if (vertices[i].visited == false)
    {
      BFS(vertices[i].name, n);
      n++;
    }
  }
  for (int j = 0; j < vertices.size(); j++)
  {
    vertices[j].visited = false;
  }
}

template<class T>
void Graph<T>::shortestPath(string start, string end)
{
  vertex<string> *begin = NULL;
  vertex<string> *ending = NULL;
  for(int i = 0; i < vertices.size(); i++)
  {
    if (vertices[i].name == start)
    {
      begin = &vertices[i];
    }
    if (vertices[i].name == end)
    {
      ending = &vertices[i];
    }
  }
  if (begin == NULL || ending == NULL)
  {
    cout << "One or more cities doesn't exist" << endl;
    return;
  }
  else if (begin -> district == -1)
  {
    cout << "Please identify the districts before checking distances" << endl;
    return;
  }
  else if (begin -> district != ending -> district)
  {
    cout << "No safe path between cities" << endl;
    return;
  }
  else
  {
    BFS2(begin, ending);
    vertex<string>* array[10];
    vertex<string>* temp = ending;
    cout << temp -> distance;
    int i = 0;
    while (temp != NULL)
    {
      array[i] = temp;
      i++;
      temp = temp -> parent;
    }
    for (int j = i -1; j >= 0; j--)
    {
      cout << "," << array[j] -> name;
    }
    cout << endl;
  }
}
