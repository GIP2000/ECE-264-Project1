#include <iostream> 
#include <fstream> 
#include <regex> 
#include <list>
#include <string> 
#include <functional>
#include <optional>
using namespace std; 

template<class T> 
class SimpleList{
    class Node {
        private: 
            T value;
            Node *nextNode;
            Node *previousNode;
            bool nonDeletable; 
        public: 
            Node();  
            Node(T value, Node *nextNode, Node *previousNode);
            void setNextNode(Node *nextNode);
            void setPreviousNode(Node *previousNode);
            Node* getNextNode();
            Node* getPreviousNode();
            T getValue(); 
            bool getNonDeletable(); 
             
    };
    protected: 
        SimpleList(string name);
        void insertAtTop(T value);
        void insertAtBot(T value); 
        optional<T> removeAtTop(); 
    public: 
        Node *topNode; 
        Node *botNode;
        string name;
        T getTop();
        virtual void push(T value){}
        virtual optional<T> pop(){} 
};

template<class T> 
class Stack : public SimpleList<T> {
    public:
        Stack(string name) : SimpleList<T>(name){

        }
        optional<T> pop(); 
        void push(T value);
};
template<class T> 
class Queue : public SimpleList<T> {
    public:
        Queue(string name) : SimpleList<T>(name){

        }
        optional<T> pop(); 
        void push(T value);
};
template<class T> 
void Stack<T>::push(T value){
    SimpleList<T>::insertAtTop(value); 
}
template<class T> 
void Queue<T>::push(T value){
    SimpleList<T>::insertAtBot(value); 
}
template<class T> 
optional<T> Queue<T>::pop(){
    return SimpleList<T>::removeAtTop(); 
}
template<class T> 
optional<T> Stack<T>::pop(){
    return SimpleList<T>::removeAtTop(); 
}

template<class T> 
SimpleList<T>::SimpleList(string name){
    this-> name = name;
    this->topNode = new Node(); 
    this->botNode = new Node(); 
    (*(this->topNode)).setPreviousNode(botNode);
    (*(this->botNode)).setNextNode(topNode); 

}
template<class T> 
SimpleList<T>::Node::Node(T value, Node *nextNode, Node *previousNode){ 
    this->nextNode = nextNode; 
    this->previousNode = previousNode; 
    this->value = value; 
    this->nonDeletable = false; 
}
template<class T> 
SimpleList<T>::Node::Node(){
    this->nonDeletable = true; 
}

template<class T> 
void SimpleList<T>::Node::setNextNode(SimpleList<T>::Node *nextNode){
    this->nextNode = nextNode; 
}

template<class T> 
void SimpleList<T>::Node::setPreviousNode(Node *previousNode){
    this->previousNode = previousNode; 
}
template<class T> 
typename SimpleList<T>::Node* SimpleList<T>::Node::getPreviousNode(){
    return this->previousNode;
}

template<class T> 
typename SimpleList<T>::Node* SimpleList<T>::Node::getNextNode(){
    return this->nextNode;
}
template<class T> 
T SimpleList<T>::Node::getValue(){
    return this->value; 
}
template<class T> 
bool SimpleList<T>::Node::getNonDeletable(){
    return this->nonDeletable; 
}

template<class T> 
void SimpleList<T>::insertAtTop(T value){
    typename SimpleList<T>::Node * newNode =  new typename SimpleList<T>::Node(value,this->topNode,(*(this->topNode)).getPreviousNode()); 
    (*((*(this->topNode)).getPreviousNode())).setNextNode(newNode);
    (*(this->topNode)).setPreviousNode(newNode); 
}

template<class T> 
void SimpleList<T>::insertAtBot(T value){
    typename SimpleList<T>::Node * newNode =  new typename SimpleList<T>::Node(value,(*(this->botNode)).getNextNode(),this->botNode); 
    (*((*(this->botNode)).getNextNode())).setPreviousNode(newNode); 
    (*(this->botNode)).setNextNode(newNode); 
}

template<class T> 
optional<T> SimpleList<T>::removeAtTop(){
    if((*(*(this->topNode)).getPreviousNode()).getNonDeletable()) return {}; 
    typename SimpleList<T>::Node * nodeToDelete = (*(this->topNode)).getPreviousNode();
    T value = (*(nodeToDelete)).getValue(); 
    (*((*(nodeToDelete)).getPreviousNode())).setNextNode(this->topNode); 
    (*(this->topNode)).setPreviousNode((*(nodeToDelete)).getPreviousNode()); 
    delete nodeToDelete; 
    return value; 
}

template<class T> 
T SimpleList<T>::getTop(){
    if((*(this->topNode)).getPreviousNode() == this->botNode) return NULL; 
    return (*((*(this->topNode)).getPreviousNode())).getValue(); 

}

template<class T> 
optional<SimpleList<T>*> searchAList(string name, list<SimpleList<T>*> lst){
    for(auto i: lst){
        if((*i).name == name)
            return i;
    }
    return {}; 
}

void addLineToFile(string line,ofstream* fileStream){
    *fileStream << line << "\n"; 
}

void createList(string name,string modifier,string cmd, list<SimpleList<int>*>* intList, list<SimpleList<double>*>* doubleList, list<SimpleList<string>*>* stringList, ofstream* outputFilePtr ){
    addLineToFile("PROCESSING COMMAND: " + cmd,outputFilePtr);
    char type = name[0]; 
    
    if(type == 'i') {
        if(searchAList<int>(name,*intList).has_value()) return addLineToFile("ERROR: This name already exists!",outputFilePtr); 
        (*intList).push_back(modifier == "stack" ? (SimpleList<int>*)(new Stack<int>(name)): (SimpleList<int>*)(new Queue<int>(name)));
    } else if(type == 'd'){
        if(searchAList<double>(name,*doubleList).has_value()) return addLineToFile("ERROR: This name already exists!",outputFilePtr); 
        (*doubleList).push_back(modifier == "stack" ? (SimpleList<double>*)(new Stack<double>(name)):(SimpleList<double>*)(new Queue<double>(name)));
    }else if(type == 's'){
        if(searchAList<string>(name,*stringList).has_value()) return addLineToFile("ERROR: This name already exists!",outputFilePtr); 
        (*stringList).push_back(modifier == "stack" ? (SimpleList<string>*)(new Stack<string>(name)):(SimpleList<string>*)(new Queue<string>(name)));
    }else
        return addLineToFile("Invalid command",outputFilePtr);
}
void push(string name, string value, string cmd, list<SimpleList<int>*>* intList, list<SimpleList<double>*>* doubleList,list<SimpleList<string>*>* stringList, ofstream* outputFilePtr  ){
    addLineToFile("PROCESSING COMMAND: " + cmd,outputFilePtr);
    char type = name[0]; 
    if(type == 'i'){
        optional<SimpleList<int>*> dataStructureOp = searchAList<int>(name,*intList);
        if(!dataStructureOp.has_value()) return addLineToFile("ERROR: This name does not exist!",outputFilePtr);
        (*(dataStructureOp.value())).push(stoi(value));
    } else if(type == 'd'){
        optional<SimpleList<double>*> dataStructureOp = searchAList<double>(name,*doubleList);
        if(!dataStructureOp.has_value()) return addLineToFile("ERROR: This name does not exist!",outputFilePtr);
        (*(dataStructureOp.value())).push(stod(value));
    } else if(type == 's'){
        optional<SimpleList<string>*> dataStructureOp = searchAList<string>(name,*stringList);
        if(!dataStructureOp.has_value()) return addLineToFile("ERROR: This name does not exist!",outputFilePtr);
        (*(dataStructureOp.value())).push(value);
    } else 
        return addLineToFile("Invalid command",outputFilePtr);
    
}

void pop(string name, string cmd, list<SimpleList<int>*>* intList, list<SimpleList<double>*>* doubleList,list<SimpleList<string>*>* stringList, ofstream* outputFilePtr ){
    addLineToFile("PROCESSING COMMAND: " + cmd,outputFilePtr);
    char type = name[0];
    if(type == 'i'){
        optional<SimpleList<int>*> dataStructureOp = (searchAList<int>(name,*intList));
        if(!dataStructureOp.has_value()) return addLineToFile("ERROR: This name does not exist!",outputFilePtr);
        optional<int> val = (*(dataStructureOp.value())).pop();
        if (!val.has_value()) return addLineToFile("ERROR: This list is empty!",outputFilePtr); 
            return addLineToFile("Value popped: " + to_string(val.value()),outputFilePtr);
    } else if(type == 'd'){
        optional<SimpleList<double>*> dataStructureOp = searchAList<double>(name,*doubleList);
        if(!dataStructureOp.has_value()) return addLineToFile("ERROR: This name does not exist!",outputFilePtr);
        optional<double> val = (*(dataStructureOp.value())).pop();
        if(!val.has_value()) return addLineToFile("ERROR: This list is empty!",outputFilePtr);  
        string doubleAsString = to_string(val.value()).erase(to_string(val.value()).find_last_not_of('0') + 1, string::npos);
        doubleAsString = doubleAsString.erase(doubleAsString.find_last_not_of('.') + 1, string::npos); 
        return addLineToFile("Value popped: " + doubleAsString,outputFilePtr);
    } else if(type == 's'){
        optional<SimpleList<string>*> dataStructureOp = searchAList<string>(name,*stringList);
        if(!dataStructureOp.has_value()) return addLineToFile("ERROR: This name does not exist!",outputFilePtr);
        optional<string> val = (*(dataStructureOp.value())).pop();
        if(!val.has_value()) return addLineToFile("ERROR: This list is empty!",outputFilePtr);
        return addLineToFile("Value popped: " + val.value(),outputFilePtr); 
    } else 
        return addLineToFile("Invalid Command",outputFilePtr); 
}

void processCommand(string cmd, list<SimpleList<int>*>* intList,list<SimpleList<double>*>* doubleList, list<SimpleList<string>*>* stringList,ofstream* outputFilePtr ){
      regex r("(\\w+) (\\w+) ?([\\w\\-\\.]+)?");
      smatch match;
      if(regex_match(cmd,match,r)){
          string cmdType = match[1] ;
          string name  = match[2]; 
          string modifier = match[3];
          if(cmdType == "create") return createList(name,modifier,cmd,intList,doubleList,stringList,outputFilePtr);
          else if(cmdType == "push") return push(name,modifier,cmd,intList,doubleList,stringList,outputFilePtr);
          else if(cmdType == "pop") return pop(name,cmd,intList,doubleList,stringList,outputFilePtr);
          else return addLineToFile("Invalid Command",outputFilePtr);
      }
      return addLineToFile("Invalid Command",outputFilePtr);
}



int main (){

    ifstream inputFile("input.txt"); 
    ofstream outputFile ("output.txt");
    list<SimpleList<int> *> intList; 
    list<SimpleList<double> *> doubleList;  
    list<SimpleList<string> *> stringList; 
    
    if (inputFile.is_open()){
        string cmd; 
        while(getline(inputFile,cmd))
            processCommand(cmd,&intList,&doubleList,&stringList,&outputFile);
    }


    return 0; 
}