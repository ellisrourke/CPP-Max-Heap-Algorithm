#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

class table{
public:
    table(){

    };
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
    int rustyMove(){
        //chose ball with highest sum of digits
    }
    int scottMove(){
        //chose highest value ball
    }

private:

};

class priorityQueue{
public:
    priorityQueue()= default;;
    bool isEmpty(){ return capacity==0; }

    virtual int getMax(){ return queue[1]; }
    virtual void insertElement(int value){
        if(capacity + 1 <= queue.size()){
            queue.push_back(0);
        }
        queue[++capacity] = value;
        shiftUp(capacity);
        dispQueue(queue);
    }

    virtual void shiftUp(int index){
        if(index > capacity){ return; } //recursive base case
        if(index == 1){ return; } //recursive base case
        if(queue[index] > queue[parent(index)]){ //if element is greater value than its parent, swap
            std::swap(queue[parent(index)],queue[index]); //swap positions to arrange
        }
        shiftUp(parent(index)); //recurse until correct position is found
    }

    virtual void shiftDown(int index){
        if(index > capacity){ return; }
        int swapId = index;

        if(leftChild(index) <= capacity && queue[index] < queue[leftChild(index)]){
            swapId = leftChild(index); //left child is greater than parent, so swap
        }
        if(rightChild(index) <= capacity && queue[swapId] < queue[rightChild(index)]){
            swapId = rightChild(index); //right child grater than parent and left child
        }
        if(swapId != index){ //recursive base case
            std::swap(queue[index],queue[swapId]);
            shiftDown(swapId);
        }
    }

    virtual void removeMax(){
        int value = queue[1];
        std::swap(queue[1],queue[capacity]);
        capacity--;
        shiftDown(1);

    };
    static void dispQueue(std::vector<int> & arr){
        for(int i=0;i<arr.size();i++){
            std::cout<<arr[i] << " ";
        }std::cout << std::endl;
    }
protected:
    std::vector<int> queue{-1};
    int capacity{};
    static int parent(int const& index){ return (index/2); }
    static int leftChild(int const& index){ return (index*2); }
    static int rightChild(int const& index){ return (index*2)+1 ; }
};

class rustyQueue : public priorityQueue{ //<ball no, sum of digits
public:
    int getMax(){ return queue[1].second; }
    void insertElement(int value) override {
        if(capacity + 1 <= queue.size()){
            queue.emplace_back(queue.size(),0);
        }
        queue[++capacity] = std::make_pair(queue.size()-1,value);
        shiftUp(capacity);
        dispQueue(queue);
    }
    void shiftUp(int index) override{
        if(index > capacity){ return; } //recursive base case
        if(index == 1){ return; } //recursive base case
        if(queue[index] > queue[parent(index)]){ //if element is greater value than its parent, swap
            std::swap(queue[parent(index)],queue[index]); //swap positions to arrange
        }
        shiftUp(parent(index)); //recurse until correct position is found
    }
    void shiftDown(int index) override{
        if(index > capacity){ return; }
        int swapId = index;

        if(leftChild(index) <= capacity && queue[index] < queue[leftChild(index)]){
            swapId = leftChild(index); //left child is greater than parent, so swap
        }
        if(rightChild(index) <= capacity && queue[swapId] < queue[rightChild(index)]){
            swapId = rightChild(index); //right child grater than parent and left child
        }
        if(swapId != index){ //recursive base case
            std::swap(queue[index],queue[swapId]);
            shiftDown(swapId);
        }
    }
    void removeMax() override {
        int value = queue[1].second;
        std::swap(queue[1],queue[capacity]);
        capacity--;
        shiftDown(1);

    };
    static void dispQueue(std::vector<std::pair<int,int>>& arr){
        for(int i=0;i<arr.size();i++){
            std::cout<< "(" << arr[i].first << " , " <<arr[i].second << ")";
        }std::cout << std::endl;
    }
private:
    std::vector<std::pair<int,int>> queue {std::make_pair(-1,-1)};
};

int main(int argc, char *argv[]) {
    srand(time(nullptr));
    int k = 0; //number of turns per round
    int n = 0; //number of balls on table
    std::vector<int> myNumbers = {22, 97, 123}; // 4 16 6

    priorityQueue maxHeap = *new priorityQueue;
    priorityQueue scottValues = *new priorityQueue;
    rustyQueue rustyValues = *new rustyQueue;
    rustyValues.insertElement(table::sumOfDigits(11));
    rustyValues.insertElement(table::sumOfDigits(1122));
    rustyValues.insertElement(table::sumOfDigits(123));    ;
    rustyValues.insertElement(table::sumOfDigits(1234));
    //std::cout << rustyValues.getMax();

}
//1 3 4 6 13 10 9 8 15 17
//1 ,3 ,4 ,6 ,13 ,10 ,9 ,8 ,15 ,17
//8 3 1 6 13 10 9 4 15 17