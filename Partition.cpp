#include "Partition.h"

//using iterator = MyIterator<Node>;

Info::Info(int id,int size,int address,int state){
	this->id = id;
	this->size = size;
	this->address = address;
	this->state = state;
}

Node::Node(Info&& data,Node* pre,Node* next){
	this->data = data;
	this->pre = pre;
	this->next = next;
}

Linklist::Linklist(){
	Linklist::init();
}

Linklist::~Linklist(){
    Linklist::del();
}

void Linklist::init(){
	this->free_head = new Node(Info(0,MAXSIZE,1,FREE));
	this->free_tail = free_head;
	this->free_size = 1;
	this->free_node.emplace_back(free_head);
	this->busy_head = nullptr;
	this->busy_tail = nullptr;
	this->busy_size = 0;
}


int Linklist::free_one(int id){
//	auto result = find_if(this->busy_begin(),this->busy_end(),[&](Node& node){
//		return node.data.id==id;
//	});
//	if(result==this->busy_end())
//		return -1;
	Node* p = busy_head;
	while(p){
		if(p->data.id==id){
			//
		}
		p = p->next;
	}
	return -1;
}

void Linklist::free_all(){
	
}

void Linklist::del(){
	Node* p = free_head;
    if(p) qDebug()<<"p==free_head"<<endl;
	Node* temp = nullptr;
	while(p){
		temp = p->next;
		delete p;
		p = temp;
	}
	
	p = busy_head;
	while(p){
		temp = p->next;
		delete p;
		p = temp;
	}
    if(p==nullptr) qDebug()<<"del p"<<endl;
//    if(free_head==nullptr) qDebug()<<"del free_head"<<endl;
}

MyIterator<Node> Linklist::free_begin(){
	return iterator(free_head);
}

MyIterator<Node> Linklist::free_end(){
	return iterator(free_head+free_size+1);
}

MyIterator<Node> Linklist::busy_begin(){
	return iterator(busy_head);
}

MyIterator<Node> Linklist::busy_end(){
	return iterator(busy_head+busy_size+1);
}


