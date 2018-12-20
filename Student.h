#ifndef _STUDENT_
#define _STUDENT_

enum Color{
    White, // Initital 0
    Blue, // Player One
    Red, // Player Two
    Black //Explosion
};

namespace playerOne{
    #define FAIL 444
    #define DEFAULT_X 0
    #define DEFAULT_Y 0

    typedef std::pair<int, int> Pair;
    typedef std::pair<int, Pair> t_Pair;
    class Student{
        public:
            Student(){}
            ~Student(){}
            void makeMove(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor){
                ///declaration
                std::vector<t_Pair> can_domain;
                std::vector<t_Pair> can_kill;
                std::vector<t_Pair> can_place;

                analyst(Record, Max, color, inputColor, can_domain, can_kill, can_place);
                decision_maker(can_domain, can_kill, can_place);
            }
            int getX(){return x;}
            int getY(){return y;}

        private:
            int x;
            int y;
            void analyst(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor,
                        std::vector<t_Pair>& can_domain, std::vector<t_Pair>& can_kill, std::vector<t_Pair>& can_place){
                ///declaration
                bool attack_area[5][6];
                bool threat_area[5][6];
                Color enemyColor = (inputColor == Blue)? Red : Blue;

                ///initialize
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        threat_area[i][j]=0;
                        attack_area[i][j]=0;
                    }
                }

                ///count treat, attack_area
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        if(Record[i][j]==Max[i][j]-1){
                            if(i!=0){
                                if(color[i][j]==enemyColor) threat_area[i-1][j]=1;
                                if(color[i][j]==inputColor) attack_area[i-1][j]=1;
                            }
                            if(i!=4){
                                if(color[i][j]==enemyColor) threat_area[i+1][j]=1;
                                if(color[i][j]==inputColor) attack_area[i+1][j]=1;
                            }
                            if(j!=0){
                                if(color[i][j]==enemyColor) threat_area[i][j-1]=1;
                                if(color[i][j]==inputColor) attack_area[i][j-1]=1;
                            }
                            if(j!=5){
                                if(color[i][j]==enemyColor) threat_area[i][j+1]=1;
                                if(color[i][j]==inputColor) attack_area[i][j+1]=1;
                            }
                        }
                    }
                }

                ///count can_domain
                for(int i=0;i<5;i++){
                    if(i==1||i==3) continue;
                    for(int j=0;j<6;j++){
                        if(i==1||i==4) continue;
                        bool color_flag = (color[i][j]==White)? 1 : 0;
                        bool ex_u = (i!=0)? (Record[i-1][j]==0) : 1;
                        bool ex_d = (i!=4)? (Record[i+1][j]==0) : 1;
                        bool ex_l = (j!=0)? (Record[i][j-1]==0) : 1;
                        bool ex_r = (j!=5)? (Record[i][j+1]==0) : 1;
                        if(ex_u&&ex_d&&ex_r&&ex_l&&color_flag){
                            can_domain.push_back({Max[i][j], {i, j}});
                        }
                    }
                }
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        if(attack_area[i][j]==1){
                            bool color_flag = (color[i][j]==inputColor||color[i][j]==White)?1:0;
                            if(i!=0&&j!=0&&color_flag) can_domain.push_back({Max[i-1][j-1], {i-1, j-1}});
                            if(i!=0&&j!=5&&color_flag) can_domain.push_back({Max[i-1][j+1], {i-1, j+1}});
                            if(i!=4&&j!=0&&color_flag) can_domain.push_back({Max[i+1][j-1], {i+1, j-1}});
                            if(i!=4&&j!=5&&color_flag) can_domain.push_back({Max[i+1][j+1], {i+1, j+1}});
                        }
                    }
                }

                ///count can_kill
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        if(color[i][j]==enemyColor){
                            int ex = Max[i][j]-Record[i][j];
                            int ex_u = (i!=0&&threat_area[i][j]==0&&(color[i-1][j]==inputColor||color[i-1][j]==White))? Max[i-1][j]-Record[i-1][j] : FAIL;
                            int ex_d = (i!=4&&threat_area[i][j]==0&&(color[i+1][j]==inputColor||color[i+1][j]==White))? Max[i+1][j]-Record[i+1][j] : FAIL;
                            int ex_l = (j!=0&&threat_area[i][j]==0&&(color[i][j-1]==inputColor||color[i][j-1]==White))? Max[i][j-1]-Record[i][j-1] : FAIL;
                            int ex_r = (j!=5&&threat_area[i][j]==0&&(color[i][j+1]==inputColor||color[i][j+1]==White))? Max[i][j+1]-Record[i][j+1] : FAIL;
                            if(ex>=ex_u) can_kill.push_back({ex_u, {i-1, j}});
                            if(ex>=ex_d) can_kill.push_back({ex_d, {i+1, j}});
                            if(ex>=ex_l) can_kill.push_back({ex_l, {i, j-1}});
                            if(ex>=ex_r) can_kill.push_back({ex_r, {i, j+1}});
                        }
                    }
                }

                ///count can_place
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        if(color[i][j]==White||color[i][j]==inputColor){
                            can_place.push_back({threat_area[i][j], {i, j}});
                        }
                    }
                }
            }
            void decision_maker(std::vector<t_Pair>& can_domain, std::vector<t_Pair>& can_kill, std::vector<t_Pair>& can_place){
                if(!can_kill.empty()){
                    std::sort(can_kill.begin(), can_kill.end());
                    x = can_kill.front().second.first;
                    y = can_kill.front().second.second;
                    return;
                }
                if(!can_domain.empty()){
                    std::sort(can_domain.begin(), can_domain.end());
                    x = can_domain.front().second.first;
                    y = can_domain.front().second.second;
                    return;
                }
                if(!can_place.empty()){
                    std::sort(can_place.begin(), can_place.end());
                    x = can_place.front().second.first;
                    y = can_place.front().second.second;
                    return;
                }
                x = DEFAULT_X;
                y = DEFAULT_Y;
                return;
            }
    };
};

///------------------------------------------------------------------------

namespace playerTwo{
    #define FAIL 444
    #define DEFAULT_X 0
    #define DEFAULT_Y 0

    typedef std::pair<int, int> Pair;
    typedef std::pair<int, Pair> t_Pair;
    class Student{
        public:
            Student(){}
            ~Student(){}
            void makeMove(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor){
                ///declaration
                std::vector<t_Pair> can_domain;
                std::vector<t_Pair> can_kill;
                std::vector<t_Pair> can_place;

                analyst(Record, Max, color, inputColor, can_domain, can_kill, can_place);
                decision_maker(can_domain, can_kill, can_place);
            }
            int getX(){return x;}
            int getY(){return y;}

        private:
            int x;
            int y;
            void analyst(int Record[5][6], int Max[5][6], Color color[5][6], Color inputColor,
                        std::vector<t_Pair>& can_domain, std::vector<t_Pair>& can_kill, std::vector<t_Pair>& can_place){
                ///declaration
                bool attack_area[5][6];
                bool threat_area[5][6];
                Color enemyColor = (inputColor == Blue)? Red : Blue;

                ///initialize
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        threat_area[i][j]=0;
                        attack_area[i][j]=0;
                    }
                }

                ///count treat, attack_area
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        if(Record[i][j]==Max[i][j]-1){
                            if(i!=0){
                                if(color[i][j]==enemyColor) threat_area[i-1][j]=1;
                                if(color[i][j]==inputColor) attack_area[i-1][j]=1;
                            }
                            if(i!=4){
                                if(color[i][j]==enemyColor) threat_area[i+1][j]=1;
                                if(color[i][j]==inputColor) attack_area[i+1][j]=1;
                            }
                            if(j!=0){
                                if(color[i][j]==enemyColor) threat_area[i][j-1]=1;
                                if(color[i][j]==inputColor) attack_area[i][j-1]=1;
                            }
                            if(j!=5){
                                if(color[i][j]==enemyColor) threat_area[i][j+1]=1;
                                if(color[i][j]==inputColor) attack_area[i][j+1]=1;
                            }
                        }
                    }
                }

                ///count can_domain
                for(int i=0;i<5;i++){
                    if(i==1||i==3) continue;
                    for(int j=0;j<6;j++){
                        if(i==1||i==4) continue;
                        bool color_flag = (color[i][j]==White)? 1 : 0;
                        bool ex_u = (i!=0)? (Record[i-1][j]==0) : 1;
                        bool ex_d = (i!=4)? (Record[i+1][j]==0) : 1;
                        bool ex_l = (j!=0)? (Record[i][j-1]==0) : 1;
                        bool ex_r = (j!=5)? (Record[i][j+1]==0) : 1;
                        if(ex_u&&ex_d&&ex_r&&ex_l&&color_flag){
                            can_domain.push_back({Max[i][j], {i, j}});
                        }
                    }
                }
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        if(attack_area[i][j]==1){
                            bool color_flag = (color[i][j]==inputColor||color[i][j]==White)?1:0;
                            if(i!=0&&j!=0&&color_flag) can_domain.push_back({Max[i-1][j-1], {i-1, j-1}});
                            if(i!=0&&j!=5&&color_flag) can_domain.push_back({Max[i-1][j+1], {i-1, j+1}});
                            if(i!=4&&j!=0&&color_flag) can_domain.push_back({Max[i+1][j-1], {i+1, j-1}});
                            if(i!=4&&j!=5&&color_flag) can_domain.push_back({Max[i+1][j+1], {i+1, j+1}});
                        }
                    }
                }

                ///count can_kill
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        if(color[i][j]==enemyColor){
                            int ex = Max[i][j]-Record[i][j];
                            int ex_u = (i!=0&&threat_area[i][j]==0&&(color[i-1][j]==inputColor||color[i-1][j]==White))? Max[i-1][j]-Record[i-1][j] : FAIL;
                            int ex_d = (i!=4&&threat_area[i][j]==0&&(color[i+1][j]==inputColor||color[i+1][j]==White))? Max[i+1][j]-Record[i+1][j] : FAIL;
                            int ex_l = (j!=0&&threat_area[i][j]==0&&(color[i][j-1]==inputColor||color[i][j-1]==White))? Max[i][j-1]-Record[i][j-1] : FAIL;
                            int ex_r = (j!=5&&threat_area[i][j]==0&&(color[i][j+1]==inputColor||color[i][j+1]==White))? Max[i][j+1]-Record[i][j+1] : FAIL;
                            if(ex>=ex_u) can_kill.push_back({ex_u, {i-1, j}});
                            if(ex>=ex_d) can_kill.push_back({ex_d, {i+1, j}});
                            if(ex>=ex_l) can_kill.push_back({ex_l, {i, j-1}});
                            if(ex>=ex_r) can_kill.push_back({ex_r, {i, j+1}});
                        }
                    }
                }

                ///count can_place
                for(int i=0;i<5;i++){
                    for(int j=0;j<6;j++){
                        if(color[i][j]==White||color[i][j]==inputColor){
                            can_place.push_back({threat_area[i][j], {i, j}});
                        }
                    }
                }
            }
            void decision_maker(std::vector<t_Pair>& can_domain, std::vector<t_Pair>& can_kill, std::vector<t_Pair>& can_place){
                if(!can_kill.empty()){
                    std::sort(can_kill.begin(), can_kill.end());
                    x = can_kill.front().second.first;
                    y = can_kill.front().second.second;
                    return;
                }
                if(!can_domain.empty()){
                    std::sort(can_domain.begin(), can_domain.end());
                    x = can_domain.front().second.first;
                    y = can_domain.front().second.second;
                    return;
                }
                if(!can_place.empty()){
                    std::sort(can_place.begin(), can_place.end());
                    x = can_place.front().second.first;
                    y = can_place.front().second.second;
                    return;
                }
                x = DEFAULT_X;
                y = DEFAULT_Y;
                return;
            }
    };
};
#endif //
