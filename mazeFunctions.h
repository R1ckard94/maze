#ifndef mazeFunctions_h
#define mazeFunctions_h

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <thread> //för att kunna använda en strypning som gör att genereringen kan visualiseras om man vill

class labyrinth{
private:
    const char WALL = '#';      
    const char PATH = ' ';
    const char WALK = '+';
    size_t SIZEW;
    size_t SIZEH;
    std::vector<std::vector<char>> mazeVec;     //en visuell grid av labyrinten
    std::vector<std::vector<int>> visited;      //stack för labyrinten med en liknande layout som mazeVec där 1:or är visited och 0:or obesökt
    const int startPos = 1;     //fixad start position
    int walk;       //en int som används i recmaze för att välja vilken väg vi ska gå 
    bool foundX = false; //bool som används när X är hittat så att solvern kan backtracka och visa vägen utan att leta andra vägar
    
public:
    labyrinth();
    labyrinth(size_t& value);
    labyrinth(size_t& value1, size_t& value2);
    void newSquare();
    void printMaze();
    void addWalls();
    bool notAllVisited(int, int);
    void startStop();
    void save(const std::string& name);
    bool addMazeSquare(const std::string& name);
    bool isInBounds(int, int);
    bool recMaze(int, int);     
    void solve();
    
};


#endif /* mazeFunctions_h */
