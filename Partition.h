#ifndef PARTITION_H
#define PARTITION_H


#include<iostream>
#include<algorithm>
#include<iterator>
#include<vector>
#include<string>
#include<QObject>
#include<QtDebug>
#include<map>

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

struct Tree
{
    Info data;
    Tree* father;
    Tree* left;
    Tree* right;
    Tree* pre;
    Tree* next;
    Tree() = default;
    explicit Tree(Info&& data,Tree* father=nullptr,Tree* left=nullptr,Tree* right=nullptr,Tree* pre=nullptr,Tree* next=nullptr);
    ~Tree();
};


class Linklist
{

	public:
        Linklist();
		~Linklist();
		
        void init(vector<vector<int>>& table_data);

        bool free_one(int);
		void del();
        void traverse_tree(Tree* root);
		
        bool First_Fit(int seq,int job_size);
        bool Best_Fit(int seq,int job_size);
        bool Worst_Fit(int seq,int job_size);
        bool Nest_Fit(int seq,int job_size);
        bool Quick_Fit(int seq,int job_size);

        bool alloc(int seq,int job_size);
        void update_array();

        void tree_init();
        void quick_array_init();
        void Quick_Fit_free(int seq,int job_size);
        void merge_tree(Tree* father,int layer);

        void update_list();
		
	public:
        Tree* root = nullptr;
        Node* head = nullptr;

        int last_flag = -1;

        vector<Node*> free_array;
        vector<Node*> busy_array;
        vector<int>   job_index;
        std::map<Node*,int> map;
        vector<Tree*> quick_array;
        vector<Tree*> tree_array;
        vector<Tree*> busy_tree;
        vector<Tree*> show_tree;
};



#endif //PARTITION_H


