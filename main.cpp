#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <string>
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
    explicit priorityQueue(){}
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
        std::swap(queue[1],queue[capacity--]);
        shiftDown(1);
    };
    virtual void dispQueue(){
        for(int i=0;i<capacity+1;i++){
            std::cout<< "(" <<this->queue[i].first << " , " << this->queue[i].second << ")";
        }std::cout << std::endl;
    }
    std::vector<int> *usedBalls;
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

void runTestCase(int n, int k, bool flip, std::vector<int> &initialBalls){

    std::vector<int> usedBalls = {};
    std::unordered_map<int, bool> used;

    priorityQueue scottValues = *new priorityQueue();
    rustyQueue rustyValues = *new rustyQueue();

    int scottScore = 0;
    int rustyScore = 0;
    int totalTurns =0;

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
                scottValues.removeMax();
                if(!used[index]){
                    scottScore += value;
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
                rustyValues.removeMax();

                if(!used[index]){
                    rustyScore += value;

                    turn++;
                    totalTurns++;
                    used[index] = true;

                }

            }
        }
        flip = !flip;
    }
    std::cout << "scores " << scottScore << " " << rustyScore << std::endl; //<< totalTurns;
}
int main(int argc, char *argv[]) {
    srand(time(nullptr));
    int testCases = 0;
    int n = 0;
    int k = 0;
    bool flip = false;
    std::vector<int> balls;

    std::ifstream inFile;
    inFile.open(argv[1]);

    std::string inLine;
    std::string buffer; // buffer string
    if(!inFile.is_open()){
        std::cerr << "file error" << std::endl;
    }
    //get first line and record the number of test cases to be read
    std::getline(inFile, inLine);
    testCases = std::stoi(inLine);

    int lineCount = 1;
    while(std::getline(inFile,inLine)){

        std::istringstream iss(inLine);

        if(lineCount == 1){
            iss >> buffer;
            n = std::stoi(buffer);
            iss >> buffer;
            k = std::stoi(buffer);
        } else if (lineCount == 2){
            int i = 0;
            while(iss >> buffer){
                balls.push_back(std::stoi(buffer));
                i++;
            }
        } else if (lineCount == 3){
            iss >> buffer;
            flip = buffer == "HEADS";
            lineCount = 0;
            runTestCase(n,k,flip,balls);
            balls = {};
        }
        lineCount++;
    }
}
//1000 197
//240 150
//2100000000 98888899
//9538 2256
//30031 17796
//4726793900 3941702128 xx
//13793 12543
//2173 1665 xx
//3923529875 3049188235 xx
//0 284401
