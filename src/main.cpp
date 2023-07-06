
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <queue>
#include <algorithm>
#include <climits>
#include <time.h>
using namespace std;

// const vector<vector<int>> GOALSTATE = {
//     {0, 1, 2},
//     {3, 4, 5},
//     {6, 7, 8}
// };

const vector<vector<int>> GOALSTATE = {
    { 0, 1, 2, 3},
    { 4, 5, 6, 7},
    { 8, 9,10,11},
    {12,13,14,15}
};

class Node{
    public:
    vector<vector<int>> state;
    shared_ptr<Node> parent;
    int empty_location_x;
    int empty_location_y;
    int g_hat;
    int h_hat;
    int f_hat;


    inline Node(vector<vector<int>> state, shared_ptr<Node>parent = nullptr){
        this->parent = parent;
        this->state = state;
        this->h_hat = get_manhattan_distance(state);
        this->g_hat = parent ? parent->g_hat+1:0;
        this->f_hat = this->g_hat + this->h_hat;

        for (int i = 0; i < state.size(); i++) {
            for (int j = 0; j < state[i].size(); j++) {
                if (state[i][j] == 0) {
                    this->empty_location_x = j;
                    this->empty_location_y = i;
                    return;
                }
            }
        }
    }

    inline static int get_manhattan_distance(vector<vector<int>> state) {
        int manhattan_distance = 0;
        for (int y = 0; y < state.size(); y++) {
            for (int x = 0; x < state[y].size(); x++) {
                int item = state[y][x];
                for (int g_y = 0; g_y < GOALSTATE.size(); g_y++) {
                    for (int g_x = 0; g_x < GOALSTATE[g_y].size(); g_x++) {
                        int g_item = GOALSTATE[g_y][g_x];
                        if (item != 0 && item == g_item) {
                            manhattan_distance += abs(x - g_x) + abs(y - g_y);
                        }
                    }
                }
            }
        }
        return manhattan_distance;
    }

    void show(){
        cout<<"-------------------"<<endl;
        for (int i=0;i<state.size();i++){
            for(int j=0;j<state[i].size();j++){
                cout<<state[i][j]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }

    void show_score(){
        cout<<"-------------------"<<endl;
        cout<< "g_hat: " << g_hat <<endl;
        cout<< "h_hat: " << h_hat <<endl;
        cout<< "f_hat: " << f_hat <<endl;
    }
    void show_path(){

        this->show();

        shared_ptr<Node> pa = this->parent;
        while(pa){
            pa->show();
            pa = pa->parent;
        }
    }

    inline vector<shared_ptr<Node>> expand(shared_ptr<Node> me){
        vector<shared_ptr<Node>>children;
        vector<pair<int,int>> directions ={
            {-1,0}, {1,0},{0,-1},{0,1}
        };

        for (auto direction :directions){
            int destination_x = empty_location_x + direction.first;
            int destination_y = empty_location_y + direction.second;
            if ((0<=destination_x && destination_x<state[0].size()) && ((0<=destination_y && destination_y<state.size())) ){
                vector<vector<int>> child_state = state;
                swap(child_state[empty_location_y][empty_location_x], child_state[destination_y][destination_x]);
                shared_ptr<Node> child = make_shared<Node>(child_state,me);
                children.push_back(child);
            }

        }
        return children;

    }   
    // static bool Compare(const Node& person1, const Node& person2) {
        
    // }

};



struct CompareNodes {
    bool operator()(shared_ptr<Node> a, shared_ptr<Node> b) {
        return a->f_hat > b->f_hat;
    }
};

shared_ptr<Node>  a_star_search(vector<vector<int>> puzzle){

    int node_num = 0;
    int min;
    int index;
    int i;
    vector<shared_ptr<Node>> children;
    shared_ptr<Node> parent = make_shared<Node>(puzzle);
    // vector<shared_ptr<Node>> open_list;
    // vector<shared_ptr<Node>> closed_list;
    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>,CompareNodes> pq;
    shared_ptr<Node> head;
    clock_t start = clock();

    pq.push(parent);

    while(1){



        head = pq.top();
        pq.pop();

        if (head->state==GOALSTATE){
            clock_t end = clock();
            const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC ;

            cout<<node_num/time<<endl;
            return head;
        }

        children = head->expand(head);

        node_num = node_num + children.size();

        for (auto child: children){
            pq.push(child);
            // auto in_open_list = find_if(open_list.begin(),open_list.end(),[child](shared_ptr<Node> n){
            //     return child->state==n->state;
            // });

            // auto in_closed_list = find_if(closed_list.begin(),closed_list.end(),[child](shared_ptr<Node> n){
            //     return child->state==n->state;
            // });

            // if (in_open_list==open_list.end() && in_closed_list==closed_list.end()){
            //     open_list.push_back(child);
            // }else if (in_open_list!=open_list.end() && (*in_open_list)->f_hat>child->f_hat){

            //     open_list.erase(in_open_list);
            //     open_list.push_back(child);
            // }
            // else if (in_closed_list!=closed_list.end() && (*in_closed_list)->f_hat>child->f_hat){
            //     std::cout <<"wawawa"<<std::endl;
            //     // throw std::runtime_error("ヒューリスティック関数に矛盾性があるかもしれません");
            // }

        }

    }
    return nullptr;
}

std::vector<std::vector<int>> generateRandomPuzzle(int moveCount) {

    std::vector<std::vector<int>> puzzle = GOALSTATE;

    int emptyRow, emptyCol;
    srand(0);

    std::vector<std::pair<int, int>> moves = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // 右、左、下、上に移動するための座標の変化量

    for (int i = 0; i < moveCount; i++) {
        std::vector<std::pair<int, int>> possibleMoves;

        // 空タイルの位置を検索
        for (int row = 0; row < puzzle.size(); row++) {
            for (int col = 0; col < puzzle[0].size(); col++) {
                if (puzzle[row][col] == 0) {
                    emptyRow = row;
                    emptyCol = col;
                    break;
                }
            }
        }

        // 空タイルの周囲に移動可能なタイルの位置を取得
        for (const auto& move : moves) {
            int newRow = emptyRow + move.first;
            int newCol = emptyCol + move.second;
            if (newRow >= 0 && newRow < puzzle.size() && newCol >= 0 && newCol < puzzle[0].size()) {
                possibleMoves.push_back({newRow, newCol});
            }
        }

        // ランダムに移動先を選択して空タイルを移動させる
        std::random_shuffle(possibleMoves.begin(), possibleMoves.end());
        int newRow = possibleMoves[0].first;
        int newCol = possibleMoves[0].second;
        std::swap(puzzle[emptyRow][emptyCol], puzzle[newRow][newCol]);
    }

    return puzzle;
}


int main() {


    // std::vector<std::vector<int>> puzzle = {
    //     {7, 2, 4}, 
    //     {5, 0, 6}, 
    //     {8, 3, 1}
    //     };
    
    std::vector<std::vector<int>> puzzle = generateRandomPuzzle(130);

    std::cout << "------problem------" << endl;
    for (int row = 0; row < puzzle.size(); row++) {
        for (int col = 0; col < puzzle[row].size(); col++) {
            std::cout << puzzle[row][col] << " ";
        }
        std::cout << std::endl;
    }  
    std::cout << std::endl;

    shared_ptr<Node> answer = a_star_search(puzzle);

    
    answer->show_score();

    // answer->show_path();


    return 0;
}
