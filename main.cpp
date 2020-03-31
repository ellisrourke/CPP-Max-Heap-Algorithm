#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

void dispVect(std::vector<int> & arr){
    for(int i=0;i<arr.size();i++){
        std::cout<<arr[i] << " ";
    }std::cout << std::endl;
}
class table{
public:
    table(){

    };
    static int coinFlip(){
        return rand() % 2;
    }
    static int sumOfDigits(std::vector<int> const & nums){
        int n,m, sum = 0;
        for(int i=0;i<nums.size();i++){
            n = nums[i];
            while(n>0){
                m = n%10;
                sum+=m;
                n = n/10;
            }
            std::cout << sum << std::endl;
            sum=0;
        }
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
    int getMax(){ return queue[1]; }
    void insertElement(int value){
        if(capacity + 1 <= queue.size()){
            queue.push_back(0);
        }
        queue[++capacity] = value;
        shiftUp(capacity);
        dispVect(queue);
    }
    void shiftUp(int index){
        if(index > capacity){ return; } //recursive base case
        if(index == 1){ return; } //recursive base case
        if(queue[index] > queue[parent(index)]){ //if element is greater value than its parent, swap
            std::swap(queue[parent(index)],queue[index]); //swap positions to arrange
        }
        shiftUp(parent(index)); //recurse until correct position is found
    }
    void shiftDown(int index){
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
    void removeMax(){
        int value = queue[1];
        std::swap(queue[1],queue[capacity]);
        capacity--;
        shiftDown(1);

    };
private:
    std::vector<int> queue{-1};
    int capacity{};
    static int parent(int const& index){ return (index/2); }
    static int leftChild(int const& index){ return (index*2); }
    static int rightChild(int const& index){ return (index*2)+1 ; }
};


int main(int argc, char *argv[]) {
    srand(time(nullptr));
    int k = 0; //number of turns per round
    int n = 0; //number of balls on table
    std::vector<int> myNumbers = {22, 97, 123}; // 4 16 6

    priorityQueue maxHeap = *new priorityQueue;
    maxHeap.insertElement(1);
    maxHeap.insertElement(3);
    maxHeap.insertElement(4);
    maxHeap.insertElement(6);
    maxHeap.insertElement(13);
    maxHeap.insertElement(10);
    maxHeap.insertElement(9);
    maxHeap.insertElement(8);
    maxHeap.insertElement(15);
    maxHeap.insertElement(17);
}
//1 3 4 6 13 10 9 8 15 17
//1 ,3 ,4 ,6 ,13 ,10 ,9 ,8 ,15 ,17
//8 3 1 6 13 10 9 4 15 17