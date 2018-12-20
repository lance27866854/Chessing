#include <bits/stdc++.h>
#include "Student.h"

namespace playerOne{
    class Student;
}

namespace playerTwo{
    class Student;
}

bool renew(int Record[5][6], int Max[5][6], Color color[5][6], Color playerColor, int x, int y){
    ///error detect.
    if(color[x][y]!=playerColor||color[x][y]!=White) return false;

    ///placement
    std::queue<std::pair<int, int>> chain_reaction;
    Record[x][y]++;
    if(Record[x][y]==Max[x][y]){
        color[x][y]=Black;
        chain_reaction.push({x-1, y});//UP
        chain_reaction.push({x+1, y});//DOWN
        chain_reaction.push({x, y-1});//LEFT
        chain_reaction.push({x, y+1});//RIGHT
    }

    while(!chain_reaction.empty()){
        //get.
        std::pair<int, int> position = chain_reaction.front();
        chain_reaction.pop();

        //chain reaction.
        int i = position.first;
        int j = position.second;
        if(i==-1||j==-1||i==5||j==6) continue;

        color[i][j] = playerColor;
        Record[x][y]++;
        if(Record[x][y]==Max[x][y]){
            color[x][y]=Black;
            chain_reaction.push({x-1, y});//UP
            chain_reaction.push({x+1, y});//DOWN
            chain_reaction.push({x, y-1});//LEFT
            chain_reaction.push({x, y+1});//RIGHT
        }
    }
    return true;
}
bool final_step(int Record[5][6], int Max[5][6], Color color[5][6], Color playerColor){
    Color enemyColor = (playerColor == Blue)? Red : Blue;
    for(int i=0;i<5;i++){
        for(int j=0;j<6;j++){
            if((color[i][j]==enemyColor && Record[i][j]<Max[i][j])||(color[i][j]==White)){return false;}
        }
    }
}

int main(void){
    ///declaration
    int Record[5][6];
    int Max[5][6];
    Color color[5][6];
    Color color1 = Blue;
    Color color2 = Red;
    playerOne::Student s1;
    playerTwo::Student s2;

    ///initialize the board
    for(int i=0;i<5;i++){
        for(int j=0;j<6;j++){
            int mi=(i==0||i==4)?1:0;
            int mj=(j==0||j==5)?1:0;
            Record[i][j]=0;
            Max[i][j]=4-mi-mj;
            color[i][j]=White;
        }
    }

    ///start the game
    bool flag1=0, flag2=0;
    bool ex;
    while(1){
        //s1 input
        s1.makeMove(Record, Max, color, color1);
        ex = renew(Record, Max, color, color1, s1.getX(), s1.getY());
        if(ex){std::cout<<"Player1 Wrong Placement!\n"; break;}

        //final
        flag1 = final_step(Record, Max, color, color1);
        if(flag1) break;

        //s2 input
        s2.makeMove(Record, Max, color, color2);
        ex = renew(Record, Max, color, color2, s2.getX(), s2.getY());
        if(ex){std::cout<<"Player2 Wrong Placement!\n"; break;}

        //final
        flag2 = final_step(Record, Max, color, color2);
        if(flag2) break;
    }

    if(flag1) std::cout<<"player1 wins.";
    if(flag2) std::cout<<"player2 wins.";

    return 0;
}
