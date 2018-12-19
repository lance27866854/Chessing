#ifndef _STUDENT_
#define _STUDENT_

enum PlayStatus{};
enum Color{
    White, // Initital 0
    Blue, // Player One
    Red, // Player Two
    Black //Explosion
};

namespace playerOne{
    typedef std::pair<int, int> Pair;
    typedef std::pair<int, Pair> t_Pair;
    class Student{
        public:
            Student(){}
            ~Student(){}
            void makeMove(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor){
                ///declaration
                bool threat[5][6];
                std::vector<t_Pair> diff_domains;
                std::vector<Pair> can_kill;

                analyst(Record, Max, color, inputColor, threat, attack_area, diff_domains, can_kill);
                decision_maker();
            }
            int getX(){return x;}
            int getY(){return y;}

        private:
            int x;
            int y;
            void analyst(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor, bool threat[5][6],
                        const std::vector<t_Pair>& diff_domains, const std::vector<Pair>& can_kill){
                ///declaration
                bool attack_area[5][6];
                Color enemyColor = (inputColor == Blue)? Red : Blue;
                ///initialize
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        threat[i][j]=0;
                        attack_area[i][j]=0;
                    }
                }
                ///count treat, attack_area
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        if(Record[i][j]==Max[i][j]-1){
                            if(i!=0){threat[i-1][j]=1;if(color[i][j]==inputColor){attack_area[i-1][j]=1;}}
                            if(i!=4){threat[i+1][j]=1;if(color[i][j]==inputColor){attack_area[i+1][j]=1;}}
                            if(j!=0){threat[i][j-1]=1;if(color[i][j]==inputColor){attack_area[i][j-1]=1;}}
                            if(j!=5){threat[i][j+1]=1;if(color[i][j]==inputColor){attack_area[i][j+1]=1;}}
                        }
                    }
                }

                ///count diff_domain
                for(int i=0;i<5;i++){
                    if(i==1||i==3) continue;
                    for(int j=0;j<6;j++){
                        if(i==1||i==4) continue;
                        bool ex_u = (i!=0)? (Record[i-1][j]==0) : 1;
                        bool ex_d = (i!=4)? (Record[i+1][j]==0) : 1;
                        bool ex_l = (j!=0)? (Record[i][j-1]==0) : 1;
                        bool ex_r = (j!=5)? (Record[i][j+1]==0) : 1;
                        if(ex_u&&ex_d&&ex_r&&ex_l){
                            diff_domain.push_back({Max[i][j], {i, j}});
                        }
                    }
                }
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        if(attack_area[i][j]==1){
                            if(i!=0&&j!=0) diff_domain.push_back({Max[i-1][j-1], {i-1, j-1}});
                            if(i!=0&&j!=5) diff_domain.push_back({Max[i-1][j+1], {i-1, j+1}});
                            if(i!=4&&j!=0) diff_domain.push_back({Max[i+1][j-1], {i+1, j-1}});
                            if(i!=4&&j!=5) diff_domain.push_back({Max[i+1][j+1], {i+1, j+1}});
                        }
                    }
                }

                ///count can_kill
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        if(color[i][j]==enemyColor){
                            int ex = Max[i][j]-Record[i][j];
                            int ex_u = (i!=0&&color[i-1][j]==inputColor)? Max[i-1][j]-Record[i-1][j] : 4;
                            int ex_d = (i!=4&&color[i+1][j]==inputColor)? Max[i+1][j]-Record[i+1][j] : 4;
                            int ex_l = (j!=0&&color[i][j-1]==inputColor)? Max[i][j-1]-Record[i][j-1] : 4;
                            int ex_r = (j!=5&&color[i][j+1]==inputColor)? Max[i][j+1]-Record[i][j+1] : 4;
                            if(ex>=ex_u) can_kill.push_back({i-1, j});
                            if(ex>=ex_d) can_kill.push_back({i+1, j});
                            if(ex>=ex_l) can_kill.push_back({i, j-1});
                            if(ex>=ex_r) can_kill.push_back({i, j+1});
                        }
                    }
                }
            }
            void decision_maker(){
            }
    };
};
#endif //_STUDENT_
