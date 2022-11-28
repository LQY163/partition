#ifndef PARTITION_H
#define PARTITION_H


#include<iostream>
#include<algorithm>
#include<iterator>
#include<vector>
#include<QObject>
#include<QtDebug>
#include<memory>

using namespace std;

#define MAXSIZE  1024
#define FREE 0
#define BUSY 1

struct Info
{
    int id;
    int size;
    int address;
    int state;
    Info() = default;
    explicit Info(int id,int size,int address,int state);
};

struct Node
{
	Info data;
	Node* pre;
	Node* next;
    Node() = default;
    explicit Node(Info&& data,Node* pre=nullptr,Node* next=nullptr);
};


template<class T> class MyIterator;

class Linklist
{

	public:
		using iterator = MyIterator<Node>;
        Linklist();
		~Linklist();
		
		void init();

		int free_one(int);
		void free_all();
		void del();
		
		int First_Fit();
		int Best_Fit();
		int Worst_Fit();
		int Nest_Fit();
		int Quick_Fit();
		
		iterator free_begin();
		iterator free_end();
		iterator busy_begin();
		iterator busy_end();
		
	public:
		Node* free_head = nullptr;
		Node* free_tail = nullptr;
		Node* busy_head = nullptr;
		Node* busy_tail = nullptr;
		int free_size;
		int busy_size;
		vector<Node*> free_node;
		vector<Node*> busy_node;
};


template<class T>
class MyIterator : public iterator<input_iterator_tag,T>
{
	public:
		MyIterator(T* p){
			ptr = p;
		}
		MyIterator& operator=(const MyIterator& it){
			ptr = it.ptr;
		}
		bool operator!=(const MyIterator& it){
			return ptr!=it.ptr;
		}
		bool operator==(const MyIterator& it){
			return ptr==it.ptr;
		}
		MyIterator& operator++(){
			ptr++;
			return *this;
		}
		MyIterator operator++(int){
			MyIterator temp = *this;
			ptr++;
			return temp;
		}
		T& operator*(){
			return *ptr;
		}
		
	private:
		T* ptr;
};

#endif //PARTITION_H


