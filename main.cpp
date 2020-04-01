#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <tuple>

class table{
public:
    table()= default;;
    static int coinFlip(){
        return rand() % 2;
    }
    static int sumOfDigits(int n){
            int sum = 0,m = 0;
            while(n>0){
                m = n%10;
                sum+=m;
                n = n/10;
            }
        return sum;
    }
    /*int rustyMove(){
        //chose ball with highest sum of digits
    }
    int scottMove(){
        //chose highest value ball
    }
    */
};

class priorityQueue{
public:
    explicit priorityQueue(std::vector<int> *usedBalls){
        this->usedBalls = usedBalls;
    }
    bool isEmpty(){ return capacity==0; }

    virtual int getMax(){ return queue[1].second; }

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
        std::cout << std::endl << value << std::endl;
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
    int getMax() override{ return std::get<2>(queue[1]); }
    void insertElement(int value) override {
        if(capacity + 1 <= queue.size()){
            //queue.emplace_back(queue.size(),0);
            queue.emplace_back(std::make_tuple(queue.size(),0,0));
        }
        queue[++capacity] = std::make_tuple(queue.size()-1,value,table::sumOfDigits(value));
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
    int k = 0; //number of turns per round
    int n = 0; //number of balls on table
    std::vector<int> usedBalls = {};
    priorityQueue scottValues = *new priorityQueue(&usedBalls);
    rustyQueue rustyValues = *new rustyQueue(&usedBalls);
    scottValues.insertElement(12);
    scottValues.insertElement(54);
    scottValues.insertElement(11);
    scottValues.dispQueue();
    scottValues.removeMax();
    scottValues.dispQueue();
    scottValues.removeMax();
    scottValues.dispQueue();
    //scottValues.update(12);

    rustyValues.displayUsedBalls();
    //sample input
    //2 - number of test cases
    //3 2 - n & k where n =n number of balls, k = number of turns
    //1000 99 98 - the balls
    //TAILS - result of coin flip(T = Rusty, H = Scott)

}
//1 3 4 6 13 10 9 8 15 17
//1 ,3 ,4 ,6 ,13 ,10 ,9 ,8 ,15 ,17
//8 3 1 6 13 10 9 4 15 17