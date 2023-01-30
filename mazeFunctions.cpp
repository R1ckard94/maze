#include "mazeFunctions.h"
//default constructor används för att skapa labrint via fil
labyrinth::labyrinth(){
}

labyrinth::labyrinth(size_t& value){ //generera maze med ett värde
    SIZEH = value;
    SIZEW = value;
    newSquare();        //skapar en fyllt sqare med väggar
    recMaze(startPos, startPos);    //genererar en maze ifrån startpunkten
    startStop();    //lägger till S och X
}

//funktion som tar in en maze ifrån fil och lägger till alla '#' som walls och sätter dom till 1:or i visited
bool labyrinth::addMazeSquare(const std::string& filename){
    std::ifstream file(filename);
    std::vector<char> tmpVec;   
    if(file.is_open()){
        std::string tmpstr;
        while(std::getline(file, tmpstr)){  //inläsning via getline till en sträng
            if(!tmpstr.empty()){
                for(int i = 0; i<tmpstr.size(); i++){   //strängen loopas till chars som läggs till vectorn som läggs i mazeVec
                    char tmpchar = tmpstr[i];
                    if(tmpchar == 'S' || tmpchar == 'X' || tmpchar == PATH || tmpchar == WALL){ //felhanterande som kollar att det inte förekommer oönskade rader i textfilerna så som karaktärer som inte känns igen
                        tmpVec.push_back(tmpchar);
                    }else{ 
                        return false;
                    }
                    
                }
                mazeVec.push_back(tmpVec);
                tmpVec.clear();
            }else{
                continue;
            }
        }
    }else{
        return false;
    }
    file.close();

    //lägger till i visited så att dom delar med WALLS är satt till visited
    addWalls();
    for(int i = 0; i<mazeVec.size(); i++){
        for(int j = 0; j<mazeVec[i].size(); j++){
            if(mazeVec[i][j] == PATH){      //eftersom att vi har några väggar mitt i mazen så sätter vi allt som inte är en PATH(' ') till visited = 1(redan besökt)
                visited[i][j] = 0;
            }else{
                visited[i][j] = 1;
            }
        }
    }
    return true;
}


//skapar en fylld kvadrat(maze) med väggar
void labyrinth::newSquare(){
    std::vector<char> tmpVec;
    for(int i = 0; i<this->SIZEH; i++){
        tmpVec.clear();
        for(int j = 0; j<this->SIZEW; j++){
            tmpVec.push_back(WALL);
        }
        mazeVec.push_back(tmpVec);
    }
    addWalls();

}

//rekursiv funktion som används vid genereringar och lösningar av labyrinter
bool labyrinth::recMaze(int x, int y){
    if(mazeVec[x][y+1] == 'X'){
        foundX = true;
    }
    
    mazeVec[x][y] = WALK;
    visited[x][y] = 1;
    //std::this_thread::sleep_for (std::chrono::milliseconds(30));  //ifall man vill få en visualisering hur mazen löses/genereras
    //printMaze();
    while(notAllVisited(x, y) && foundX == false){ //kollar ifall vi har noder runt om som går att gå till och inte är besökt
                                                    //foundX funktionen är en bool som solve använder sig av i detta fall för att programmet inte ska fortsätta söka i onödan
        walk = (rand()%4)+1;
        switch (walk) {
            case 1: //case 1 går ett steg till höger
                if(isInBounds(x, y+2)){     //kollar inboundsfunktion om det går att gå 2 steg till höger
                    if(visited[x][y+2] != 1 && visited[x][y+1] != 1){       //kollar så att det inte är en vägg därimellan och att dit vi vill gå inte redan är besökt
                        mazeVec[x][y+1] = WALK;     //lägger till walk på mitten steget
                        visited[x][y+1] = 1;        //samt att vi besökt den positionen
                        recMaze(x, (y+2));        //återkallar funktionen med 2 steg till höger värdet
                        if(foundX){                 //när funktion jobbar "bakåt" igen så kollar vi ifall denna väg hittade X (målet)
                            return true;            //om så returnerar vi true och skickar tillbaka till tidigare steg
                        }else{
                            mazeVec[x][y+1] = PATH;     //om inte så betyder detta att denna väg är en återvändsgränd så vi lägger positionerna till
                            mazeVec[x][y+2] = PATH;     //två blanka steg eftersom vi vill visa en väg till målet
                        }
                    }
                }
                break;
                
            case 2://case 2 går ett steg till vänster
                if(isInBounds(x, y-2)){
                    if(visited[x][y-2] != 1 && visited[x][y-1] != 1){
                        mazeVec[x][y-1] = WALK;
                        visited[x][y-1] = 1;
                        recMaze(x, (y-2));
                        if(foundX){
                            return true;
                        }else{
                            mazeVec[x][y-1] = PATH;
                            mazeVec[x][y-2] = PATH;
                        }
                    }
                }
                break;
                
            case 3://case 3 går ett steg till neråt
                if(isInBounds(x+2, y)){
                    if(visited[x+2][y] != 1 && visited[x+1][y] != 1){
                        mazeVec[x+1][y] = WALK;
                        visited[x+1][y] = 1;
                        recMaze((x+2), y);
                        if(foundX){
                            return true;
                        }else{
                            mazeVec[x+1][y] = PATH;
                            mazeVec[x+2][y] = PATH;
                        }
                    }
                }
                break;
                
            case 4://case 4 går ett steg till uppåt
                if(isInBounds(x-2, y)){
                    if(visited[x-2][y] != 1 && visited[x-1][y] != 1){
                        mazeVec[x-1][y] = WALK;
                        visited[x-1][y] = 1;
                        recMaze((x-2), y);
                        if(foundX){
                            return true;
                        }else{
                            mazeVec[x-1][y] = PATH;
                            mazeVec[x-2][y] = PATH;
                        }
                    }
                }
                break;
                
        }
    }
    //när vi gått igenom hela mazen och kommer tillbaka till start så kallar vi denna för att kolla om vi hittade X alltså målet. 
    //Generationen kommer alltid att returnera false här och därför sätter vi positionen till PATH och solve maze kommer returnera beroende på om vi hittar x eller inte.
    if(mazeVec[startPos][startPos]==mazeVec[x][y] && notAllVisited(x, y) == false){    //om vi är tillbaka på startpositionen och det inte finns några närliggande 
            if(foundX){
                mazeVec[x][y] = WALK;                       //ifall vi hittade slutpunkten så ska vi ha behålla detta som walk
                return true;
            }else{
                mazeVec[x][y] = PATH;                                   //grannar att besöka så har hela labyrinten besökts så att det inte finns något sätt att hitta X
                return false;                                           //och det betyder att labyrinten inte går att lösa
            }
    }                                                                   
    return true;
}

//recMaze använder denna för att kolla ifall så att nästa steg inte kliver utanför labyrintensstorlek
bool labyrinth::isInBounds(int x, int y){
    if (x < 0 || x >= mazeVec.size()){
        return false;
    }
    if (y < 0 || y >= mazeVec[0].size()) {
        return false;
    }
    return true;
}

//recMaze använder denna för att kolla om det finns nån väg som inte redan har blivit besökt
bool labyrinth::notAllVisited(int x, int y){
    if(isInBounds(x, y+2) && visited[x][y+2] == 0 && visited[x][y+1] == 0){     //kollar så att inte dom två närmaste positionerna åt varje håll inte är besökta och InBounds
        return true;
    }else if(isInBounds(x, y-2) && visited[x][y-2] == 0 && visited[x][y-1] == 0){
        return true;
    }else if(isInBounds(x-2, y) && visited[x-2][y] == 0 && visited[x-1][y] == 0){
        return true;
    }else if(isInBounds(x+2, y) && visited[x+2][y] == 0 && visited[x+1][y] == 0){
        return true;
    }else{
        return false;
    }
    
}

//printfunktion till labyrinten
void labyrinth::printMaze(){
    for (int i = 0; i < mazeVec.size(); i++)
    {
        for (int j = 0; j < mazeVec[i].size(); j++)
        {
            std::cout << mazeVec[i][j];

        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
}

//lägger till yttre väggarna till visited så att dom inte kan "tas bort" i genereringsfunktionen samt solvefunktioen
void labyrinth::addWalls(){
    std::vector<int> tmpvec;
    for(int i = 0; i < mazeVec.size(); i++){
        tmpvec.clear();
        for (int j = 0; j < mazeVec[i].size(); j++){
            if(i == 0){
                tmpvec.push_back(1);
            }
            else if (j == 0 || j == mazeVec[i].size()-1){
                tmpvec.push_back(1);
            }
            else if (i == mazeVec.size()-1){
                tmpvec.push_back(1);
            }else{
                tmpvec.push_back(0);
            }
        }
        visited.push_back(tmpvec);
    }
    
}

//lägger till startpunkten S och stopp-punkten X
void labyrinth::startStop(){
    mazeVec[1][0] = 'S';
    mazeVec[SIZEH-2][SIZEW-1] = 'X';
}

//funktion som sparar en skapad maze till fil
void labyrinth::save(const std::string& name){
    std::ofstream maze(name);
    if(maze.is_open()){
        for(int i = 0; i<mazeVec.size(); i++){
            for(int j = 0; j<mazeVec[i].size(); j++){
                maze<<mazeVec[i][j];
            }
            maze<<'\n';
        }
    }else{
        std::cout<<"ERROR: could not save to file"<<std::endl;
    }
    maze.close();
}


//solvefunktionen som blir kallad och som i sin tur kallar på den rekursiva funktionen, för att skriva ett meddelande om det går samt printa alternativ om det inte går
void labyrinth::solve(){                        
    if(recMaze(startPos, startPos)){      //om true betyder det att vi kunde lösa labyrinten
        printMaze();        
        std::cout<<"Maze solved!"<<std::endl;       //den lösta labyrinten blir printad samt ett meddelande om att labyrinten är löst
    }else{
        std::cout<<"This Maze can't be solved!"<<std::endl;   //om labyrinten inte kan lösas så printas inte labyrinten bara detta meddelande
    }
    

}
