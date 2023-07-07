
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <queue>
#include <algorithm>
#include <climits>
#include <time.h>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>

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



void show(vector<vector<int>> state){
    cout<<"-------------------"<<endl;
    for (int i=0;i<state.size();i++){
        for(int j=0;j<state[i].size();j++){
            cout<<state[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}
// void static show_score(){
//     cout<<"-------------------"<<endl;
//     cout<< "g_hat: " << g_hat <<endl;
//     cout<< "h_hat: " << h_hat <<endl;
//     cout<< "f_hat: " << f_hat <<endl;
// }
// void show_path(
//     tuple<int,int,vector<vector<int>>> node,
//     unordered_map<size_t,tuple<int,int,vector<vector<int>>>> hash_map
//     ){
//     vector<vector<int>> state = get<2>(node);
//     show(state);

//     size_t hashValue = vector_hash(state);

//     tuple<int,int,vector<vector<int>>> pa = hash_map[hashValue];
 
//     while(pa){
//         pa->show();
//         pa = pa->parent;
//     }
// }

inline void expand(const shared_ptr<vector<vector<int>>> parent_state_ptr,
    shared_ptr<vector<shared_ptr<vector<vector<int>>>>>children_state_ptr
    ){
    vector<vector<int>> child_state;
    shared_ptr<vector<vector<int>>> child_state_ptr;
    vector<pair<int,int>> directions ={
        {-1,0}, {1,0},{0,-1},{0,1}
    };
    int empty_location_x;
    int empty_location_y;
    for (int i = 0; i < (*parent_state_ptr).size(); i++) {
        for (int j = 0; j < (*parent_state_ptr)[i].size(); j++) {
            if ((*parent_state_ptr)[i][j] == 0) {
                empty_location_x = j;
                empty_location_y = i;
            }
        }
    }
    for (auto direction :directions){
        int destination_x = empty_location_x + direction.first;
        int destination_y = empty_location_y + direction.second;
        if ((0<=destination_x && destination_x<(*parent_state_ptr)[0].size()) && ((0<=destination_y && destination_y<(*parent_state_ptr).size())) ){
            child_state = (*parent_state_ptr);
            swap(child_state[empty_location_y][empty_location_x], child_state[destination_y][destination_x]);
            child_state_ptr = make_shared<vector<vector<int>> >(child_state);
            (*children_state_ptr).push_back(child_state_ptr);
        }
    }


}   
    // static bool Compare(const Node& person1, const Node& person2) {
        
    // }



std::size_t vector_hash(const shared_ptr<vector<vector<int>>>vec){
  std::size_t seed = (*vec).size();
  for( auto row:(*vec)){
      for(auto& i : row) {
        seed ^= static_cast<uint32_t>(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
  }

  return seed;
}

inline int get_manhattan_distance(const shared_ptr<vector<vector<int>>> state_ptr) {
    int manhattan_distance = 0;
    for (int y = 0; y < (*state_ptr).size(); y++) {
        for (int x = 0; x < (*state_ptr)[y].size(); x++) {
            int item = (*state_ptr)[y][x];
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
struct Node{
    int f_hat;
    int g_hat;
    shared_ptr<vector<vector<int>>> state_ptr;
    shared_ptr<Node> parent;
};
struct CompareNodes {
    bool operator()(const shared_ptr<Node>& a, const shared_ptr<Node>& b) {
        return a->f_hat > b->f_hat;
    }
};



void a_star_search(vector<vector<int>> puzzle){
    clock_t start = clock();

    
    int loop_count = 0;
    int node_num = 0;
    int min;
    int index;
    int i;

    
    shared_ptr<Node>head_ptr;
    shared_ptr<Node>child_ptr;

    priority_queue<shared_ptr<Node> , vector<shared_ptr<Node>>,CompareNodes> pq;
    unordered_map<size_t,shared_ptr<Node>> hash_map;
    size_t hashValue;

    Node root;
    
    root.state_ptr = make_shared<vector<vector<int>>>(puzzle);
    root.g_hat = 0;
    root.f_hat = root.g_hat + get_manhattan_distance(root.state_ptr);
    root.parent = nullptr;

    // hashValue = vector_hash(root.state);
    auto node_ptr = make_shared<Node>(root);
    // hash_map[hashValue] = node_ptr;

    pq.push(node_ptr);

    while(1){
        
        head_ptr = pq.top();
        pq.pop();

        hashValue = vector_hash(head_ptr->state_ptr);
        if (hash_map.find(hashValue) != hash_map.end()) {
            if (hash_map[hashValue]->f_hat <= head_ptr->f_hat){
                continue;
            }else{
                hash_map[hashValue] = head_ptr;
            }
        } else {
            hash_map[hashValue] = head_ptr;
        }
        loop_count = loop_count + 1;


        if (*(head_ptr->state_ptr)==GOALSTATE){
            clock_t end = clock();
            const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC ;

            cout<<"-------------------"<<endl;
            cout<<"g_hat:               "<<head_ptr->g_hat<<endl;
            cout<<"loop_count:          "<<loop_count<<endl;
            cout<<"node_num:            "<<node_num<<endl;
            cout<<"time[s]:             "<<time<<endl;
            cout<<"node_num/time[s]:    "<<std::fixed << std::setprecision(0) <<node_num/time<<endl;

            return ;
        }

        vector<shared_ptr<vector<vector<int>>>> children_state;
        auto children_state_ptr = make_shared<vector<shared_ptr<vector<vector<int>>>>>(children_state);
        expand(head_ptr->state_ptr,children_state_ptr);

        node_num = node_num + (*children_state_ptr).size();

        for (const shared_ptr<vector<vector<int>>> child_state_ptr: (*children_state_ptr)){
            Node child;
            child.g_hat = head_ptr->g_hat + 1;
            child.f_hat = child.g_hat + get_manhattan_distance(child_state_ptr);
            child.state_ptr = child_state_ptr;
            child.parent = head_ptr;

            child_ptr = make_shared<Node>(child);
            pq.push(child_ptr);

        }

    }
    return;
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


    a_star_search(puzzle);

    


    // answer->show_path();


    return 0;
}
