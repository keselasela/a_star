
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <queue>
#include <algorithm>
using namespace std;

constexpr int STEATX = 3;
constexpr int STATEY = 3;
const vector<vector<int>> GOALSTATE = {
    {0, 1, 2},
    {3, 4, 5},
    {6, 7, 8}
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


    Node(vector<vector<int>> state, shared_ptr<Node>parent = nullptr){
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

    static int get_manhattan_distance(vector<vector<int>> state) {
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

    vector<shared_ptr<Node>> expand(shared_ptr<Node> me){
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

// struct CompareNodes {
//     bool operator()(Node* a, Node* b) {
//         return a->f_hat > b->f_hat;
//     }
// };

shared_ptr<Node>  a_star_search(vector<vector<int>> puzzle){

    
    shared_ptr<Node> parent = make_shared<Node>(puzzle);

    // cout<< "------問題------" <<endl;
    // parent->show();

    vector<shared_ptr<Node>> open_list;
    vector<shared_ptr<Node>> closed_list;

    open_list.push_back(parent);

    while(!open_list.empty()){

        sort(open_list.begin(),open_list.end(),[](const shared_ptr<Node>a,const shared_ptr<Node> b){
            return a->f_hat > b->f_hat;
        });
        shared_ptr<Node> head = open_list.back();
        open_list.pop_back();

        if (head->state==GOALSTATE){
            return head;
        }

        vector<shared_ptr<Node>> children = head->expand(head);

        for (auto child: children){
            auto in_open_list = find_if(open_list.begin(),open_list.end(),[child](shared_ptr<Node> n){
                return child->state==n->state;
            });

            auto in_closed_list = find_if(closed_list.begin(),closed_list.end(),[child](shared_ptr<Node> n){
                return child->state==n->state;
            });

            if (in_open_list==open_list.end() && in_closed_list==closed_list.end()){
                open_list.push_back(child);
            }else if (in_open_list!=open_list.end() && (*in_open_list)->f_hat>child->f_hat){

                open_list.erase(in_open_list);
                open_list.push_back(child);
            }else if (in_closed_list!=closed_list.end() && (*in_closed_list)->f_hat>child->f_hat){
                std::cout << std::endl;
                // throw std::runtime_error("ヒューリスティック関数に矛盾性があるかもしれません");
            }

        }

        closed_list.push_back(head);


    }
    return nullptr;
}


int main() {
    srand(time(0));

    std::vector<std::vector<int>> puzzle = {
        {7, 2, 4}, 
        {5, 0, 6}, 
        {8, 3, 1}
        };

    std::cout << "------問題------" << endl;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            std::cout << puzzle[row][col] << " ";
        }
        std::cout << std::endl;
    }  
    std::cout << std::endl;

    shared_ptr<Node> answer = a_star_search(puzzle);

    
    answer->show_score();


    return 0;
}
