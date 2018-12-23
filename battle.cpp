#include <bits/stdc++.h>
#include "Student.h"

namespace playerOne{
    class Student;
}

namespace playerTwo{
    class Student;
}

bool renew(int Record[5][6], int Max[5][6], Color color[5][6], Color playerColor, int x, int y){
    Color enemyColor = (playerColor == Blue)? Red : Blue;
    ///error detect.
    if(color[x][y]==enemyColor||color[x][y]==Black||Record[x][y]>=Max[x][y]) return false;
    if(x<0||y<0||x>4||y>5) return false;

    ///placement
    std::queue<std::pair<int, int>> chain_reaction;
    Record[x][y]++;
    color[x][y]=playerColor;
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

        if(color[i][j]!=Black)color[i][j] = playerColor;
        if(color[i][j]!=Black)Record[i][j]++;

        if(Record[i][j]==Max[i][j]&&color[i][j]!=Black){
            color[i][j]=Black;
            chain_reaction.push({i-1, j});//UP
            chain_reaction.push({i+1, j});//DOWN
            chain_reaction.push({i, j-1});//LEFT
            chain_reaction.push({i, j+1});//RIGHT
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

void print(int Record[5][6], Color color[5][6]){
    ///Record
    std::cout<<"\n--------------------------\n";
    std::cout<<"The Record board is below:\n";
    for(int i=0;i<5;i++){
        for(int j=0;j<6;j++){
            std::cout<<Record[i][j]<<" ";
        }
        std::cout<<"\n";
    }
    std::cout<<"\n--------------------------\n";
    std::cout<<"The Color board is below:\n";
    ///Color
    for(int i=0;i<5;i++){
        for(int j=0;j<6;j++){
            if(color[i][j]==White) std::cout<<"  ";
            else if(color[i][j]==Blue) std::cout<<"B ";
            else if(color[i][j]==Red) std::cout<<"R ";
            else if(color[i][j]==Black) std::cout<<"# ";
        }
        std::cout<<"\n";
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
    int x, y;
    //int times=100;
    while(1){
        //s1 input
        s1.makeMove(Record, Max, color, color1);
        x = s1.getX();
        y = s1.getY();
        ex = renew(Record, Max, color, color1, x, y);
        std::cout<<"player1 placement: x = "<<x<<"   y = "<<y<<"\n";
        if(!ex){std::cout<<"Player1 Wrong Placement!\n";break;}

        //final
        flag1 = final_step(Record, Max, color, color1);
        if(flag1) break;

        //s2 input
        s2.makeMove(Record, Max, color, color2);
        x = s2.getX();
        y = s2.getY();
        ex = renew(Record, Max, color, color2, x, y);
        std::cout<<"player2 placement: x = "<<x<<"   y = "<<y<<"\n";
        if(!ex){std::cout<<"Player2 Wrong Placement!\n"; break;}

        //final
        flag2 = final_step(Record, Max, color, color2);
        if(flag2) break;
        print(Record, color);
    }

    if(flag1) std::cout<<"player1 wins.";
    if(flag2) std::cout<<"player2 wins.";
    print(Record, color);

    return 0;
}
