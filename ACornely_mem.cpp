#include <iostream>
#include <string>
using namespace std;

//pass in memory size (1048576) via command arguments

struct Process{
  string pid;
  int size;
};

struct MemoryBlock{
  Process process;
  int start, end, index;
};

struct Memory{
  int freeBlocks, usedBlocks, memoryLimit, memoryUsed, length, index;
  MemoryBlock* memory;
  

  Memory(int memLimit, int size) {
    (*this).memory = new MemoryBlock[size];
    (*this).length = size;
    (*this).memoryLimit = memLimit;
    (*this).freeBlocks = (*this).usedBlocks = (*this).memoryUsed = (*this).index = 0;
  }

  void add(MemoryBlock newMemBlock){
    if(memoryUsed >= memoryLimit){
      cout << "Memory is Full!" << endl;
      return;
    }
    if(length == usedBlocks){
      cout << "Memory Array is Full!" << endl;
      return;
    }
    if((memory[index - 1].end + 1) + newMemBlock.process.size > memoryLimit){
      cout << "Process is Too Big!!" << endl;
      return;
    }
    else{
      if(freeBlocks == 0){
        memory[index] = newMemBlock;
        memory[index].index = (*this).index;
        memory[index].start = memoryUsed;
        memory[index].end = memory[index].start + memory[index].process.size;
        memoryUsed = memory[index].end;
        index++;
        usedBlocks++;
        memoryUsed++;
        cout << "Process Successfully Added!" << endl;
      }
      else{ //if freeblocks exist
        MemoryBlock excessMB;
        int temp, lowest = lowestSizeIndex();
        temp = memory[lowest].end; //new end
        memory[lowest].end = memory[index].start + newMemBlock.process.size; // new start
        memory[lowest].process.pid = newMemBlock.process.pid;
        memory[lowest].process.size = newMemBlock.process.size;

        //if the free space isn't completely filled
        if(memory[lowest].end != memory[lowest + 1].start - 1){
          excessMB.start = memory[lowest].end + 1;
          excessMB.end = temp;
          excessMB.process.pid = "FREE";
          excessMB.process.size = excessMB.end - excessMB.start;
          excessMB.index = lowest + 1;
          addAt(lowest + 1, excessMB); 
        }
        else{
          freeBlocks--;
        } 
      }
    }
    return;
  }

  void addAt(int index, MemoryBlock block){
    for(int i = usedBlocks - 1; i >= index; i--){
      memory[i + 1] = memory[i];
      memory[i + 1].index++;
    }
    memory[index] = block;
  }

  int lowestSizeIndex(){
    int lowestSizeIndex;
    lowestSizeIndex = memoryLimit;
    for(int i = 0; i < usedBlocks; i++){
      if(memory[index].process.pid == "FREE" && memory[index].process.size < lowestSizeIndex){
        lowestSizeIndex = i;
      }
    }
    return lowestSizeIndex;
  }

  void remove(string pid){
    bool flag = false;
    for(int i = 0; i < usedBlocks; i++){
      if(memory[i].process.pid == pid){
        memory[i].process.pid = "FREE";
        flag = true;
        break;
      }
    }
    if(flag == true){
      freeBlocks++;
    }
  }
  // when users inputs command "C"
  void removeFreeBlocks(){
    while(freeBlocks != 0){
      for(int i = 0; i < usedBlocks; i++){
        if(memory[i].process.pid == "FREE"){
            memory[i].start = memory[i+1].start ;
            memory[i].end = memory[i+1].start + memory[i+1].process.size;
          for(int j = i + 1; j < usedBlocks; j++){
            memory[j].start = memory[j-1].end + 1;
            memory[j].end = memory[j].start + memory[j].process.size;
          }
          freeBlocks--;
        }
      }
    }
  }

  void displayMemory(){
    for(int i = 0; i < usedBlocks; i++){
      cout << "Addresses [" << memory[i].start << " : " << memory[i].end << "] Process: " << memory[i].process.pid << endl;
    }
    if(!(memory[usedBlocks-1].end >= memoryUsed)){
      cout << "Addresses [" << memory[usedBlocks-1].end + 1 << " :  END] Process: FREE" << endl;
    }
  }
};



//handles and executes all the possible commands that a user may input
static void commandHandler(string commandLine, Memory* memory, MemoryBlock newMB){
  string pid;
 
  if(commandLine == "STAT"){
    memory->displayMemory();
  }

  else if(commandLine == "C"){
    memory->removeFreeBlocks();
  }

  else if(commandLine == "quit" || commandLine == "QUIT"){
    cout << "Goodbye" << endl;
    return;
  }

  else if(commandLine.find("RQ") != std::string::npos){
    string str;
    newMB.process.pid = commandLine.substr(3,2);
    str = commandLine.substr(6, 7);
    newMB.process.size = stoi(str);
    memory->add(newMB);  
  }

  else if(commandLine.find("RL") != std::string::npos){
    pid = commandLine.substr(3,2);
    memory->remove(pid);
    cout << pid + " has been removed! \n";
  }
  else{
    cout << "Command Invalid \n";
  }
}

int main(int argc, char* argv[]){
  string cmd = argv[1];
  // const int memoryLimit = 1048576;
  const int memoryLimit = stoi(cmd);
  string command;
  MemoryBlock newMB;
  Memory* memory = new Memory(memoryLimit, 50);
  while(1){
    cout << "allocator> ";
    getline(cin, command);
    cout << endl << endl;
    commandHandler(command, memory, newMB);
    if(command == "quit" || command == "QUIT"){
      break;
    }
  }
  return 0;
}