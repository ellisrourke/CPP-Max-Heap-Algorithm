#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
int sumOfDigits(int n){
            int sum = 0,m = 0;
            while(n>0){
                m = n%10;
                sum+=m;
                n = n/10;
            }
        return sum;
    }

class priorityQueue{
public:
    explicit priorityQueue(std::vector<int> *usedBalls){
        this->usedBalls = usedBalls;
    }
    bool isEmpty(){ return capacity==0; }
    virtual int getMax(){ return queue[1].second; }
    virtual int getMaxIndex(){ return queue[1].first; }
    virtual void insertElement(int value){
        if(capacity + 1 <= queue.size()){
            queue.emplace_back(std::make_pair(queue.size(),0));
        }
        queue[++capacity] = std::make_pair(queue.size()-1,value);
        shiftUp(capacity);
    }
    virtual void shiftUp(int index){
        if(index > capacity){ return; } //recursive base case
        if(index == 1){ return; } //recursive base case
        if(queue[index].second > queue[parent(index)].second){ //if element is greater value than its parent, swap
            std::swap(queue[parent(index)],queue[index]); //swap positions to arrange
        }
        shiftUp(parent(index)); //recurse until correct position is found
    }
    virtual void shiftDown(int index){
        if(index > capacity){ return; }
        int swapId = index;

        if(leftChild(index) <= capacity && queue[index].second < queue[leftChild(index)].second){
            swapId = leftChild(index); //left child is greater than parent, so swap
        }
        if(rightChild(index) <= capacity && queue[swapId].second < queue[rightChild(index)].second){
            swapId = rightChild(index); //right child grater than parent and left child
        }
        if(swapId != index){ //recursive base case
            std::swap(queue[index],queue[swapId]);
            shiftDown(swapId);
        }

    }
    virtual void removeMax(){
        updateUsedBalls(queue[1].first);
        std::swap(queue[1],queue[capacity--]);
        shiftDown(1);
    };
    virtual void dispQueue(){
        for(int i=0;i<capacity+1;i++){
            std::cout<< "(" <<this->queue[i].first << " , " << this->queue[i].second << ")";
        }std::cout << std::endl;
    }
    std::vector<int> *usedBalls;
    void updateUsedBalls(int x){ //ball ID
        if(!(std::find(usedBalls->begin(), usedBalls->end(), x) != usedBalls->end()))
            usedBalls->push_back(x);
    }
    void displayUsedBalls(){
        int* index;
        for(int i=0;i<this->usedBalls->size();i++){
            std::cout << (*usedBalls)[i] << " ";
        } std::cout << std::endl;
    }
protected:
    std::vector<std::pair<int,int>> queue{std::make_pair(-1,-1)};
    int capacity{};
    static int parent(int const& index){ return (index/2); }
    static int leftChild(int const& index){ return (index*2); }
    static int rightChild(int const& index){ return (index*2)+1 ; }
};

class rustyQueue : public priorityQueue{ //<ball no,value, sum of digits>
public:
    using priorityQueue::priorityQueue;
    int getMax() override{ return std::get<1>(queue[1]); }
    int getMaxIndex() override { return std::get<0>(queue[1]);}
    void insertElement(int value) override {
        if(capacity + 1 <= queue.size()){
            queue.emplace_back(std::make_tuple(queue.size(),0,0));
        }
        queue[++capacity] = std::make_tuple(queue.size()-1,value,sumOfDigits(value));
        shiftUp(capacity);
    }
    void shiftUp(int index) override{
        if(index > capacity){ return; } //recursive base case
        if(index == 1){ return; } //recursive base case
        if(std::get<2>(queue[index]) > std::get<2>(queue[parent(index)])){ //if element is greater value than its parent, swap
            std::swap(queue[parent(index)],queue[index]); //swap positions to arrange
        }
        shiftUp(parent(index)); //recurse until correct position is found
    }
    void shiftDown(int index) override{
        if(index > capacity){ return; }
        int swapId = index;

        if(leftChild(index) <= capacity && std::get<2>(queue[index]) < std::get<2>(queue[leftChild(index)])){
            swapId = leftChild(index); //left child is greater than parent, so swap
        }
        if(rightChild(index) <= capacity && std::get<2>(queue[swapId]) < std::get<2>(queue[rightChild(index)])){
            swapId = rightChild(index); //right child grater than parent and left child
        }
        if(swapId != index){ //recursive base case
            std::swap(queue[index],queue[swapId]);
            shiftDown(swapId);
        }
    }
    void removeMax() override {
        updateUsedBalls(std::get<0>(queue[1]));
        std::swap(queue[1],queue[capacity--]);
        shiftDown(1);

    };
    void dispQueue() override{
        for(int i=0;i<capacity+1;i++){
            std::cout<< "(" << std::get<0>(this->queue[i]) << " , " <<std::get<1>(this->queue[i]) << " , " << std::get<2>(this->queue[i]) << ")";
        }std::cout << std::endl;
    }
    std::vector<int> usedBalls;
private:
    std::vector<std::tuple<int,int,int>> queue {std::make_tuple(-1,-1,-1)};
};

int main(int argc, char *argv[]) {
    srand(time(nullptr));
    int n = 8; //number of balls on table
    int k = 2; //number of turns per round
    bool flip = true; //were 1 = heads and 0 = tails // tails = rusty , heads = scott
    std::vector<int> usedBalls = {};
    std::unordered_map<int, bool> used;
    //std::vector<int> initialBalls = {76 ,89,49 ,2 ,31, 311 ,56 ,445, 343 ,100 ,900 ,111 ,323 ,232, 32 ,35 ,6456, 565 ,760, 878};
    std::vector<int> initialBalls = {50,60,20,10,90,30,60,70};
    //std::vector<int> initialBalls = {65,8,74,97,61,19,75,52,85,80,96,82,26,52,50,67,99,27,42,61,45,67,61,52,21,13,31,16,5,50,65,53,24,65,62,51,34,43,82,68,40,1,73,63,88,44,47,6,34,57,11,51,86,55,38,25,72,94,18,92,98,65,1,16,75,61,2,92,64,63,27,17,28,75,33};
    priorityQueue scottValues = *new priorityQueue(&usedBalls);
    rustyQueue rustyValues = *new rustyQueue(&usedBalls);
    int scottScore = 0;
    int rustyScore = 0;
    int totalTurns =0;
    int rustyTurns =0;
    int scottTurns = 0;
    for (int i = 0; i < n; i++) {
        rustyValues.insertElement(initialBalls[i]);
        scottValues.insertElement(initialBalls[i]);
        used[i] = false;
    }

    while(totalTurns < n)  {

        if (flip) { //scott turn
            int turn = 0;
            while(turn < k){
                if(scottValues.getMax() == -1){break;}

                int value = scottValues.getMax();

                int index = scottValues.getMaxIndex();
                //std::cout << value << " " << index << std::endl;
                scottValues.removeMax();
                //if(!(std::find(usedBalls.begin(), usedBalls.end(), index) != usedBalls.end())){
                if(!used[index]){
                    scottScore += value;
                    std::cout << "turn " << turn << " scott choses " << value <<std::endl;
                    turn++;
                    totalTurns++;
                    used[index] = true;
                }
            }
        }
        else if(!flip){ //rusty turn
            int turn = 0;
            while (turn < k) {
                if(rustyValues.getMax() == -1){break;}
                int value = rustyValues.getMax();
                int index = rustyValues.getMaxIndex();
                //std::cout << value << " " << index << std::endl;
                rustyValues.removeMax();
                //if(!(std::find(usedBalls.begin(), usedBalls.end(), index) != usedBalls.end())){
                if(!used[index]){
                    rustyScore += value;
                    std::cout << "turn " << turn << "rusty choses " << value <<std::endl;

                    turn++;
                    totalTurns++;
                    used[index] = true;

                }

            }
        }
        flip = !flip;
        //std::cout << flip << std::endl;
    }

//1000 197
//240 150
//2100000000 98888899
//9538 2256
//30031 17796
//4726793900 3941702128
//13793 12543
//2173 1665
//3923529875 3049188235
//0 284401
//std::cout << "RustyTurns: " << rustyTurns << " " << "ScottTurns: " << scottTurns << std::endl;
    std::cout << "scores " << scottScore << " " << rustyScore << " "; //<< totalTurns;
}
