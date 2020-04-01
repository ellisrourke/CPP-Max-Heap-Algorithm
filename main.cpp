#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <tuple>
#include <algorithm>
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
        int value = queue[1].second;
        //std::cout << std::endl << value << std::endl;
        updateUsedBalls(queue[1].first);
        std::swap(queue[1],queue[capacity--]);
        shiftDown(1);
        //capacity--;
    };

    virtual void dispQueue(){
        for(int i=0;i<capacity+1;i++){
            std::cout<< "(" <<this->queue[i].first << " , " << this->queue[i].second << ")";
        }std::cout << std::endl;
    }
    std::vector<int> *usedBalls;

    void updateUsedBalls(int x){ //ball ID
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
            //queue.emplace_back(queue.size(),0);
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
        int value = std::get<2>(queue[1]);
        updateUsedBalls(std::get<0>(queue[1]));
        std::swap(queue[1],queue[capacity]);
        capacity--;
        shiftDown(1);

    };
    void dispQueue() override{
        for(int i=0;i<capacity+1;i++){
            std::cout<< "(" << std::get<0>(this->queue[i]) << " , " <<std::get<1>(this->queue[i]) << " , " << std::get<2>(this->queue[i]) << ")";
        }std::cout << std::endl;
    }
    std::vector<int> usedBalls;
private:
    //std::vector<int> usedBalls;
    std::vector<std::tuple<int,int,int>> queue {std::make_tuple(-1,-1,-1)};
};


int main(int argc, char *argv[]) {
    srand(time(nullptr));
    int n = 8; //number of balls on table
    int k = 2; //number of turns per round
    bool flip = 1; //were 1 = heads and 0 = tails // tails = rusty , heads = scott
    std::vector<int> usedBalls = {};
    std::vector<int> initialBalls = {50, 60, 20, 10, 90, 30, 60, 70};
    priorityQueue scottValues = *new priorityQueue(&usedBalls);
    rustyQueue rustyValues = *new rustyQueue(&usedBalls);
    int scottScore = 0;
    int rustyScore = 0;

    for (int i = 0; i < initialBalls.size(); i++) {
        scottValues.insertElement(initialBalls[i]);
        rustyValues.insertElement(initialBalls[i]);
    }


    while(!rustyValues.isEmpty() || !scottValues.isEmpty())  {
        if (flip) { //scott turn
            int turn = 0;
            while (turn < k) {
                if (!(std::find(usedBalls.begin(), usedBalls.end(), scottValues.getMaxIndex()) != usedBalls.end())) {
                    scottScore += scottValues.getMax();
                    scottValues.removeMax();
                }
                turn++;
            }
            if (scottValues.isEmpty()) break;

        } else { //rusty turn
            int turn = 0;
            while (turn < k) {
                if (!(std::find(usedBalls.begin(), usedBalls.end(), rustyValues.getMaxIndex()) != usedBalls.end())) {
                    rustyScore += rustyValues.getMax();
                    rustyValues.removeMax();
                }
                turn++;
            }
            if (rustyValues.isEmpty()) break;
        }
        flip ^= true;
    }

//1000 197
//240 150
//2100000000 98888899
//9751 2043
//30105 17722
//4648186903 4020309125
//13823 12513
//2174 1664
//3923529875 3049188235
//0 284401


    std::cout << scottScore << " " << rustyScore;
//toggle x = !x



    //sample input
    //2 - number of test cases
    //3 2 - n & k where n =n number of balls, k = number of turns
    //1000 99 98 - the balls
    //TAILS - result of coin flip(T = Rusty, H = Scott)

}