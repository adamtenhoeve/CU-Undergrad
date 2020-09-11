#ifndef GRAPH_H
#define GRAPH_H
#include<vector>
#include<iostream>

using namespace std;

template<class T>
struct vertex;

template<class T>
struct adjVertex{
    vertex<T> *v;
    int weight;
};

template<class T>
struct vertex{
    T name;
    int district;
    bool visited;
    int distance;
    vertex *parent;
    std::vector<adjVertex<T> > adj;

    vertex()
    {
      district = -1;
      visited = false;
      distance = 0;
      parent = NULL;
    };
};

template<class T>
class Graph
{
    public:
        Graph();
        ~Graph();
        void addEdge(T v1, T v2, int weight);
        void addVertex(T name);
        void displayEdges();
        string getVertex(int);
        void printVerticies();
        void BFS(string, int);
        void BFS2(vertex<string>*, vertex<string>*);
        void findDistricts();
        void shortestPath(string, string);
    protected:
    private:
        std::vector<vertex<T> > vertices;
        vertex<T> *search(string);

};
#include "Graph.cpp"
#endif // GRAPH_H
