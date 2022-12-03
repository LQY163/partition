#include "Partition.h"

//using iterator = MyIterator<Node>;

Info::Info(int id,int size,int address,int state)
{
	this->id = id;
	this->size = size;
	this->address = address;
	this->state = state;

    qDebug()<<"info.id:"<<id<<" create"<<endl;

}

Info::~Info()
{
    qDebug()<<"info.id:"<<id<<" delete"<<endl;
}

Node::Node(Info&& data,Node* pre,Node* next)
{
	this->data = data;
	this->pre = pre;
	this->next = next;
}

Node::~Node()
{
    qDebug()<<"Node->info.id:"<<this->data.id<<" delete"<<endl;
}

Linklist::Linklist()
{
//	Linklist::init();
}

Linklist::~Linklist()
{
    Linklist::del();
}

void Linklist::init(vector<vector<int>>& table_data)
{
    int n = table_data.size();
    if(n != 0){
        head = new Node(Info(0,table_data[0][0],table_data[0][1],FREE));
        free_array.push_back(head);
    }
    for(int i=1;i<n;i++){
        Node* node = new Node(Info(i,table_data[i][0],table_data[i][1],FREE),free_array[i-1]);
        free_array[i-1]->next = node;
        free_array.push_back(node);
    }

//	this->free_head = new Node(Info(0,MAXSIZE,1,FREE));
//	this->free_tail = free_head;
//	this->free_size = 1;
//	this->free_node.emplace_back(free_head);
//	this->busy_head = nullptr;
//	this->busy_tail = nullptr;
//	this->busy_size = 0;
}


bool Linklist::free_one(int seq)
{
    string job_seq = "J" + std::to_string(seq);
    Node* node = nullptr;
    for(const auto& item : map){
        if(item.second == job_seq){
            node = item.first;
            break;
        }
    }
    if(node){
        map.erase(node);
        Node* pre = node->pre;
        Node* next = node->next;
        if(pre!=nullptr && next!=nullptr){//节点    要回收的node    节点

            if(pre->data.state==FREE && next->data.state==FREE){ //空闲节点    要回收的node    空闲节点//i   i-1
                //三个合并成一个
                //pre    node    next    next->next
                //pre->    (next->next)
                pre->next = next->next;
                //pre->    <-(next->next)
                if(next->next != nullptr){
                    next->next->pre = pre;
                }
                pre->data.size = pre->data.size + node->data.size + next->data.size;

                qDebug()<<"空闲节点    要回收的node    空闲节点"<<endl;

                if(free_array[last_flag]->data.address > node->data.address) last_flag--;

                delete node;
                delete next;
            }
            else if(pre->data.state==FREE && next->data.state==BUSY){//空闲节点    要回收的node    忙碌节点//i i
                //前两个合并成一个
                //pre    node    next
                //pre->        next
                pre->next = next;
                //pre->      <-next
                next->pre = pre;

                pre->data.size = pre->data.size + node->data.size;

                qDebug()<<"空闲节点    要回收的node    忙碌节点"<<endl;
                delete node;
            }
            else if(pre->data.state==BUSY && next->data.state==FREE){//忙碌节点    要回收的node    空闲节点//i i
                //后两个合并成一个
                //pre    node    next    next->next
                //pre-><-node->          next->next
                node->next = next->next;
                //pre-><-node->        <-next->next
                if(next->next != nullptr) next->next->pre = node;

                node->data.size = node->data.size + next->data.size;
                node->data.state = FREE;

                qDebug()<<"忙碌节点    要回收的node    空闲节点"<<endl;
                delete next;
            }
            else{                                                   //忙碌节点    要回收的node    忙碌节点//i i+1
                qDebug()<<"忙碌节点    要回收的node    忙碌节点"<<endl;
                node->data.state = FREE;

                if(free_array[last_flag]->data.address > node->data.address) last_flag++;

            }
        }
        else if(pre!=nullptr && next==nullptr){//节点    要回收的node    NULL

            if(pre->data.state == FREE){//空闲节点    要回收的node    NULL//i
                pre->next = nullptr;
                pre->data.size = pre->data.size + node->data.size;

                qDebug()<<"空闲节点    要回收的node    NULL"<<endl;
                delete node;
            }
            else{//忙碌节点    要回收的node    NULL//i
                qDebug()<<"忙碌节点    要回收的node    NULL"<<endl;
                node->data.state = FREE;
            }
        }
        else if(pre==nullptr && next!=nullptr){//NULL    要回收的node    节点
            //此时要回收的node为头结点

            if(next->data.state == FREE){//NULL    要回收的node    空闲节点//i
                next->data.size = node->data.size + next->data.size;
                next->data.address = 0;
                //NULL    node    next
                //NULL          <-next(head)
                next->pre = nullptr;
                this->head = next;

                qDebug()<<"NULL    要回收的node    空闲节点"<<endl;
                delete node;
            }
            else{//NULL    要回收的node    忙碌节点//i+1
                qDebug()<<"NULL    要回收的node    忙碌节点"<<endl;
                node->data.state = FREE;

                if(free_array[last_flag]->data.address > node->data.address) last_flag++;

            }
        }
        else{//NULL    要回收的node    NULL//i=0
            qDebug()<<"NULL    要回收的node    NULL"<<endl;
             node->data.state = FREE;
        }
    }
    else return false;

    update_array();

    return true;
}


void Linklist::del_one(Node* node)
{

}

void Linklist::del()
{
    Node* p = head;
    Node* temp = nullptr;
    while(p){

        qDebug()<<"id:"<<p->data.id<<"\tsize:"<<p->data.size<<"\taddress:"
               <<p->data.address<<"\tstate:"<<p->data.state<<endl;

        temp = p->next;
        delete p;
        p = temp;
    }
}


bool Linklist::First_Fit(int seq,int job_size)
{
    //根据链表节点的起始地址来排序空闲数组，起始地址从小到大
    sort(free_array.begin(),free_array.end(),[&](Node* a,Node* b){
        return a->data.address < b->data.address;
    });

    return alloc(seq,job_size);
}

bool Linklist::Best_Fit(int seq, int job_size)
{
    sort(free_array.begin(),free_array.end(),[&](Node* a,Node* b){
        return a->data.size < b->data.size;
    });

    return alloc(seq,job_size);
}

bool Linklist::Worst_Fit(int seq, int job_size)
{
    sort(free_array.begin(),free_array.end(),[&](Node* a,Node* b){
        return a->data.size > b->data.size;
    });

    return alloc(seq,job_size);
}

bool Linklist::Nest_Fit(int seq, int job_size)
{
    if(last_flag==-1){
        sort(free_array.begin(),free_array.end(),[&](Node* a,Node* b){
            return a->data.address < b->data.address;
        });
        return alloc(seq,job_size);
    }
    else{
        int size = free_array.size();

//        auto it = free_array.begin();//空闲数组的起始迭代器，用来删除数组的内容

        int i=last_flag;

        while(1){
            if(free_array[i]->data.size == job_size){//找到第一个大小相等的
                free_array[i]->data.state = BUSY;
//                free_array.erase(it);//将当前节点从数组中删除
    //            job_index.push_back(seq);//记录是哪个作业
                map[free_array[i]] = "J" + std::to_string(seq);
                last_flag = i;
                return true;
            }
            else if(free_array[i]->data.size > job_size){//找到第一个比作业大的
                //切块 新建一个空闲节点

                int data_size = free_array[i]->data.size;

                //直接用原来节点
                free_array[i]->data.size = job_size;//更改大小
                free_array[i]->data.state = BUSY;

    //            job_index.push_back(seq);//记录是哪个作业
                map[free_array[i]] = "J" + std::to_string(seq);

                int address = free_array[i]->data.address + job_size;

                //free_array[i]    <-node->    free_array[i]->next
                Node* node = new Node(Info(9999,data_size-job_size,address,FREE),free_array[i],free_array[i]->next);

                //node    <-free_array[i]->next
                if(free_array[i]->next != nullptr) free_array[i]->next->pre = node;

                //free_array[i]->    node
                free_array[i]->next = node;

                free_array[i] = node;//把原来节点换成新的空闲节点

                last_flag = i;

                return true;
            }

            i++;
            if(i==last_flag) return false;
            if(i==size) i = 0;

        }

//        while()

//        for(int i=0;i<size;i++,it++){//遍历数组

//        }

        update_array();

        return false;
    }
}


bool Linklist::alloc(int seq,int job_size)
{
    int size = free_array.size();

    auto it = free_array.begin();//空闲数组的起始迭代器，用来删除数组的内容

    for(int i=0;i<size;i++,it++){//遍历数组
        if(free_array[i]->data.size == job_size){//找到第一个大小相等的
            free_array[i]->data.state = BUSY;
            free_array.erase(it);//将当前节点从数组中删除
//            job_index.push_back(seq);//记录是哪个作业
            map[free_array[i]] = "J" + std::to_string(seq);
            last_flag = i;
            return true;
        }
        else if(free_array[i]->data.size > job_size){//找到第一个比作业大的
            //切块 新建一个空闲节点

            int data_size = free_array[i]->data.size;

            //直接用原来节点
            free_array[i]->data.size = job_size;//更改大小
            free_array[i]->data.state = BUSY;

//            job_index.push_back(seq);//记录是哪个作业
            map[free_array[i]] = "J" + std::to_string(seq);

            int address = free_array[i]->data.address + job_size;

            //free_array[i]    <-node->    free_array[i]->next
            Node* node = new Node(Info(9999,data_size-job_size,address,FREE),free_array[i],free_array[i]->next);

            //node    <-free_array[i]->next
            if(free_array[i]->next != nullptr) free_array[i]->next->pre = node;

            //free_array[i]->    node
            free_array[i]->next = node;

            free_array[i] = node;//把原来节点换成新的空闲节点

            last_flag = i;

            return true;
        }
    }
    return false;
}


void Linklist::update_array()
{
    free_array.clear();
    Node* p = head;
    while(p){

        qDebug()<<"id:"<<p->data.id<<"\tsize:"<<p->data.size<<"\taddress:"
               <<p->data.address<<"\tstate:"<<p->data.state<<endl;

        if(p->data.state == FREE) free_array.push_back(p);
        p = p->next;
    }
}


MyIterator<Node> Linklist::free_begin()
{
	return iterator(free_head);
}

MyIterator<Node> Linklist::free_end()
{
	return iterator(free_head+free_size+1);
}

MyIterator<Node> Linklist::busy_begin()
{
	return iterator(busy_head);
}

MyIterator<Node> Linklist::busy_end()
{
	return iterator(busy_head+busy_size+1);
}


