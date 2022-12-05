#include "Partition.h"


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


Tree::Tree(Info&& data,Tree* father,Tree* left,Tree* right,Tree* pre,Tree* next)
{
    this->data = data;
    this->father = father;
    this->left = left;
    this->right = right;
    this->pre = pre;
    this->next = next;
}

Tree::~Tree()
{
    qDebug()<<"Tree->data.id:"<<this->data.id<<" delete"<<endl;
}

Linklist::Linklist()
{

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

}


/*
 * 首次适应
 * seq:作业序号
 * job_size:作业大小
 * */
bool Linklist::First_Fit(int seq,int job_size)
{
    //根据链表节点的起始地址来排序空闲数组，起始地址从小到大
    sort(free_array.begin(),free_array.end(),[&](Node* a,Node* b){
        return a->data.address < b->data.address;
    });

    return alloc(seq,job_size);
}

/*
 * 最优适应
 * seq:作业序号
 * job_size:作业大小
 * */
bool Linklist::Best_Fit(int seq, int job_size)
{
    //根据链表节点的内存大小来排序空闲数组，内存大小从小到大
    sort(free_array.begin(),free_array.end(),[&](Node* a,Node* b){
        return a->data.size < b->data.size;
    });

    return alloc(seq,job_size);
}

/*
 * 最坏适应
 * seq:作业序号
 * job_size:作业大小
 * */
bool Linklist::Worst_Fit(int seq, int job_size)
{
    //根据链表节点的内存大小来排序空闲数组，内存大小从大到小
    sort(free_array.begin(),free_array.end(),[&](Node* a,Node* b){
        return a->data.size > b->data.size;
    });

    return alloc(seq,job_size);
}

/*
 * 下次适应
 * seq:作业序号
 * job_size:作业大小
 * */
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

        int i=last_flag;

        while(1){
            if(free_array[i]->data.size == job_size){//找到第一个大小相等的
                free_array[i]->data.state = BUSY;
                free_array[i]->data.id = seq;

                //记录是哪个作业
                map[free_array[i]] = seq;
                last_flag = i;

                update_array();
                return true;
            }
            else if(free_array[i]->data.size > job_size){//找到第一个比作业大的
                //切块 新建一个空闲节点

                int data_size = free_array[i]->data.size;

                //直接用原来节点
                free_array[i]->data.size = job_size;//更改大小
                free_array[i]->data.state = BUSY;
                free_array[i]->data.id = seq;

                //记录是哪个作业
                map[free_array[i]] = seq;

                int address = free_array[i]->data.address + job_size;

                //free_array[i]    <-node->    free_array[i]->next
                Node* node = new Node(Info(seq,data_size-job_size,address,FREE),free_array[i],free_array[i]->next);

                //node    <-free_array[i]->next
                if(free_array[i]->next != nullptr) free_array[i]->next->pre = node;

                //free_array[i]->    node
                free_array[i]->next = node;

                free_array[i] = node;//把原来节点换成新的空闲节点

                last_flag = i;

                update_array();
                return true;
            }

            i++;
            if(i==last_flag) return false;
            if(i==size) i = 0;

        }

        return false;
    }
}


/*
 * 快速适应
 * seq:作业序号
 * job_size:作业大小
 * */
bool Linklist::Quick_Fit(int seq, int job_size)
{

    int i = 0;
    while(pow(2,i)<job_size) i++;

    if(quick_array[i] == nullptr){
        int j = i+1;
        while(j<=10){
            if(quick_array[j] != nullptr){
                quick_array[j]->data.state = BUSY;
                Tree* tree = quick_array[j];
                quick_array[j] = tree->next;
                for( ;j>i; ){
                    Tree* tree_left = new Tree(Info(1,tree->data.size/2,tree->data.address,BUSY),tree);
                    Tree* tree_right = new Tree(Info(1,tree->data.size/2,tree->data.address+tree->data.size/2,FREE),tree);
                    tree->left = tree_left;
                    tree->right = tree_right;
                    quick_array[--j] = tree_right;
                    tree = tree_left;
                    if(j == i){
                        tree_left->data.id = seq;
                        busy_tree.emplace_back(tree_left);
                    }
                }
                update_list();
                return true;
            }
            j++;
            if(j==11) return false;
        }
    }
    else{
        quick_array[i]->data.state = BUSY;
        quick_array[i]->data.id = seq;
        busy_tree.emplace_back(quick_array[i]);

        if(quick_array[i]->next) quick_array[i]->next->pre = nullptr;
        quick_array[i] = quick_array[i]->next;

        update_list();
        return true;
    }

    return false;
}


/*
 * 分配
 * seq:作业序号
 * job_size:作业大小
 * */
bool Linklist::alloc(int seq,int job_size)
{
    int size = free_array.size();

    for(int i=0;i<size;i++){//遍历数组
        if(free_array[i]->data.size == job_size){//找到第一个大小相等的
            free_array[i]->data.state = BUSY;
            free_array[i]->data.id = seq;

            //记录是哪个作业
            map[free_array[i]] = seq;

            last_flag = i;//下次适应算法中用到，用来标记下次从哪里开始扫描，其值为空闲数组的下标

            update_array();//更新空闲数组
            return true;
        }
        else if(free_array[i]->data.size > job_size){//找到第一个比作业大的
            //切块 新建一个空闲节点

            int data_size = free_array[i]->data.size;

            //直接用原来节点
            free_array[i]->data.size = job_size;//更改大小
            free_array[i]->data.state = BUSY;
            free_array[i]->data.id = seq;

            //记录是哪个作业
            map[free_array[i]] = seq;

            int address = free_array[i]->data.address + job_size;

            //free_array[i]    <-node->    free_array[i]->next
            Node* node = new Node(Info(seq,data_size-job_size,address,FREE),free_array[i],free_array[i]->next);

            //node    <-free_array[i]->next
            if(free_array[i]->next != nullptr) free_array[i]->next->pre = node;

            //free_array[i]->    node
            free_array[i]->next = node;

            free_array[i] = node;//把原来节点换成新的空闲节点

            last_flag = i;//下次适应算法中用到，用来标记下次从哪里开始扫描，其值为空闲数组的下标

            update_array();//更新空闲数组
            return true;
        }
    }
    return false;
}


/*
 * 回收
 * seq:作业序号
 * */
bool Linklist::free_one(int seq)
{
    Node* node = nullptr;
    for(const auto& item : map){
        if(item.second == seq){
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


/*
 * 快速适应算法的回收
 * seq:作业序号
 * layer:层数
 * */
void Linklist::Quick_Fit_free(int seq, int layer)
{
    Tree* tree = nullptr;
    for(auto it=busy_tree.begin();it!=busy_tree.end();it++){
        if((*it)->data.id == seq){
            tree = *it;
            busy_tree.erase(it);
            break;
        }
    }
    tree->data.state = FREE;

    merge_tree(tree->father,layer);//递归合并树

    update_list();//更新链表
}


/*
 * 前序遍历树
 * root:树根
 * */
void Linklist::traverse_tree(Tree* root)
{
    if(root == nullptr) return ;
    tree_array.emplace_back(root);
    if(root->left==nullptr && root->right==nullptr) show_tree.emplace_back(root);
    if(root->left) traverse_tree(root->left);
    if(root->right) traverse_tree(root->right);
}


/*
 * 合并树
 * father:子节点的父亲
 * layer:层数
 * */
void Linklist::merge_tree(Tree *father,int layer)
{
    if(father == nullptr) return ;
    if(father->left==nullptr && father->right==nullptr) return ;
    if(father->left->data.state==FREE && father->right->data.state==FREE){
        Tree* tree = quick_array[layer];
         if(tree){
//             if(tree->next==nullptr)
             while(tree){
                 if(tree==father->left || tree==father->right){
                     if(tree->pre == nullptr){
                         quick_array[layer] = tree->next;
                         if(tree->next) tree->next->pre = nullptr;
                     }
                     else{
                         if(tree->next == nullptr){
                            tree->pre->next = nullptr;
                         }
                         else{
                             tree->pre->next = tree->next;
                             tree->next->pre = tree->pre;
                         }
                     }
                 }
                 tree = tree->next;
             }
         }
         father->data.state = FREE;

         delete father->left;
         delete father->right;
         father->left = nullptr;
         father->right = nullptr;
         if(father->father == nullptr) quick_array[layer+1] = father;
         merge_tree(father->father,layer+1);
    }
    else{
        Tree* tree = quick_array[layer];
        Tree* free_tree = father->left->data.state==FREE ? father->left : father->right;
        if(tree){
            quick_array[layer] = free_tree;
            free_tree->pre = nullptr;
            free_tree->next = tree;
            tree->pre = free_tree;
        }
        else{
            quick_array[layer] = free_tree;
            free_tree->pre = nullptr;
            free_tree->next = nullptr;
        }
    }
}


/*
 * 销毁链表和树
 * */
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

    tree_array.clear();
    traverse_tree(root);
    for(auto& i : tree_array){
        delete i;
    }
}


/*
 * 更新链表
 * */
void Linklist::update_list()
{
    tree_array.clear();
    show_tree.clear();
    traverse_tree(root);
    sort(show_tree.begin(),show_tree.end(),[&](Tree* a,Tree* b){
        return a->data.address < b->data.address;
    });

    Node* p = head;
    Node* temp = nullptr;
    while(p){

        qDebug()<<"id:"<<p->data.id<<"\tsize:"<<p->data.size<<"\taddress:"
               <<p->data.address<<"\tstate:"<<p->data.state<<endl;

        temp = p->next;
        delete p;
        p = temp;
    }

    head = new Node(Info(show_tree[0]->data.id,show_tree[0]->data.size,show_tree[0]->data.address,show_tree[0]->data.state));

    Node* t = head;

    for(int i=1;i<show_tree.size();i++){
        Node* node = new Node(Info(show_tree[i]->data.id,show_tree[i]->data.size,show_tree[i]->data.address,show_tree[i]->data.state),t);
        t->next = node;
        t = node;
    }

}


/*
 * 更新空闲数组
 * */
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


void Linklist::tree_init()
{
    root = new Tree(Info(0,1024,0,FREE));
}

void Linklist::quick_array_init()
{
    tree_init();
    for(int i=0;i<10;i++){
        quick_array.push_back(nullptr);
    }
    quick_array.push_back(root);
}





