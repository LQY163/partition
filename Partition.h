#ifndef PARTITION_H
#define PARTITION_H


#include<iostream>
#include<algorithm>
#include<iterator>
#include<vector>
#include<string>
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
    ~Info();
};

struct Node
{
	Info data;
	Node* pre;
	Node* next;
    Node() = default;
    explicit Node(Info&& data,Node* pre=nullptr,Node* next=nullptr);
    ~Node();
};


template<class T> class MyIterator;

class Linklist
{

	public:
		using iterator = MyIterator<Node>;
        Linklist();
		~Linklist();
		
        void init(vector<vector<int>>& table_data);

        bool free_one(int);
        void del_one(Node* node);
		void del();
		
        bool First_Fit(int seq,int job_size);
        bool Best_Fit(int seq,int job_size);
        bool Worst_Fit(int seq,int job_size);
        bool Nest_Fit(int seq,int job_size);
        bool Quick_Fit(int seq,int job_size);

        bool alloc(int seq,int job_size);
        void update_array();
		
		iterator free_begin();
		iterator free_end();
		iterator busy_begin();
		iterator busy_end();
		
	public:
        Node* head = nullptr;
		Node* free_head = nullptr;
		Node* free_tail = nullptr;
		Node* busy_head = nullptr;
		Node* busy_tail = nullptr;
        int last_flag = -1;
		int free_size;
		int busy_size;
        vector<Node*> free_array;
        vector<Node*> busy_array;
        vector<int> job_index;
        map<Node*,string> map;
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


