#include "mazeFunctions.h"


int main(int argc, char* argv[]) {
    srand((unsigned) time(0));  //gör att rand hela tiden är olika och vi kan få mer slumpmässiga labyrinter
    
    if(argv[1] == nullptr){ //felhanterande check ifall inget argument ges
        std::cout<<"Error: arguments needed in form ./maze [-size || -s] int\nor the argument ./maze [-input || -i] filename.txt"<<std::endl;
        return EXIT_FAILURE;
    }

    if(strcmp(argv[1], "-size")==0 || strcmp(argv[1], "-s")==0){
        if(argv[2] != nullptr){     //så att argv[2] inte krashar programmet eftersom att vi förväntar oss en till inmatning
            try{
                size_t mazeSize = std::stoul(argv[2]);  //lägger detta till en try catchblock som felhanterande

                //sätter en min size och en max size för att inte överbelasta ett system krashar samt en koll så att talet är udda
                if((mazeSize>2) && (mazeSize<1000) && (mazeSize%2 == 1)){   
                    labyrinth tmp(mazeSize);
                    tmp.printMaze();
                    tmp.save("maze" + std::to_string(mazeSize) + ".txt");
                }else{
                    std::cout<<"wrong size. size need to be an odd number and bigger than 2"<<std::endl;
                    return EXIT_FAILURE;
                }
            }catch(std::invalid_argument& ia){      //catch för felinmatningar (ex ./maze -s t)
                std::cout<<"invalid character, only use numbers"<<std::endl;
                return EXIT_FAILURE;
            }
        }else{
            std::cout<<"no argument given, please give a size"<<std::endl;
            return EXIT_FAILURE;
        }
        
    }else if(strcmp(argv[1], "-input")==0 || strcmp(argv[1], "-i")==0 ){
        if(argv[2] != nullptr){     //så att argv[2] inte krashar programmet eftersom att vi förväntar oss en till inmatning
            labyrinth tmp;
            if(tmp.addMazeSquare(argv[2])){   //addmaze funktionen är en bool, om filen finns och går att öppna så körs detta
                //kan vi öppna programmet så körs solve funktionen 
                tmp.solve();            
            }else{
                std::cout<<"Error with File: "<<argv[2]<<"\nPlease check that the file contains a valid maze"<<std::endl;
                return EXIT_FAILURE;
            }
        }else{
            std::cout<<"no argument for input given, please specify file"<<std::endl;
            return EXIT_FAILURE;
        }

    }else{      //felhantering för argument som inte finns
        std::cout<<"wrong arguments type ./maze [-size || -s] int\nor the argument ./maze [-input || -i] filename.txt"<<std::endl;  //snabb hjälptext
        return EXIT_FAILURE;
    }



    return EXIT_SUCCESS;
}
