#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <iterator>

using namespace std;

struct element{
    int a;
    element *next;
    element(int a_temp){
        a = a_temp;
        next = 0;
    }
};

struct list{
    element *first;
    void add(int x){
        element *new_ = new element(x);
        if(first == 0){
            first = new_;
        }
        else{
            element *prev = first;
            if(new_->a < first->a){
                prev = new_;
                new_->next = first;
                first = prev;
            }
            else {
                if(prev->next == 0){
                    element *temp = new_;
                    temp->next = prev->next;
                    prev->next = new_;
                }
                else{
                    while(prev->next->a < new_->a){
                        prev = prev->next;
                        if(prev->next == 0){
                            break;
                        }
                    }
                    element *temp = new_;
                    temp->next = prev->next;
                    prev->next = new_;
                }
            }
        }
    }
    void display(){
        element *temp = first;
        while(temp){
            cout<<temp->a<<", ";
            temp = temp->next;
        }
    }
    void search(int n,int x[],clock_t tv[],clock_t t){
        t = clock();
        element *temp = first;
        for(int i = 0;i < n;i++){
            while(temp->a != x[i]){
                temp = temp->next;
            }
            //cout<<temp->a<<", ";
            temp = first;
            tv[i] = clock() - t;
        }   
        ofstream f("search_lista.txt");
        for(int i = 0;i < n; i++){
            f<<(float)tv[i]/CLOCKS_PER_SEC<<endl;
        }
        f.close();
    }
    void del(clock_t tv[], clock_t t){
        t = clock();
        int i = 0;
        while(first != NULL){
            element *temp = first;
            first = temp->next;
            delete temp;
            tv[i] = clock() - t;
            i++;
        }
        ofstream f("del_lista.txt");
        for(int j = 0;j < i; j++){
            f<<(float)tv[j]/CLOCKS_PER_SEC<<endl;
        }
        f.close();
    }
    list(){
        first = 0;
    }
};

struct leaf{
  int a;
  struct leaf *left;
  struct leaf *right;
  leaf(int a_temp){
      a = a_temp;
      left = right = NULL;
  }

};

struct leaf *insert(struct leaf *root, int x){
  if (root == NULL) return new leaf(x);
  if (x < root->a)
    root->left = insert(root->left, x);
  else
    root->right = insert(root->right, x);
  return root;
}

void postorder_del(struct leaf *&root, clock_t tv[],clock_t &t,int &k){
    if (root == NULL) return;
    postorder_del(root->left,tv,t,k);
    postorder_del(root->right,tv,t,k);
    delete root;
    tv[k] = clock() - t;
    k++;
}

void inorder_to_vecotr(struct leaf *root,vector<int> &x){
    if(root != NULL) {
        inorder_to_vecotr(root->left,x);
        x.push_back(root->a);
        inorder_to_vecotr(root->right,x);
    }
}

void inorder(struct leaf *root){
    if(root != NULL) {
        inorder(root->left);
        cout<<root->a<<", ";
        inorder(root->right);
    }
}

void preorder(struct leaf *root){
    if(root != NULL) {
        cout<<root->a<<", ";
        preorder(root->left);
        preorder(root->right);
    }
    return;
}

void postorder(struct leaf *root){
    if(root != NULL) {
        postorder(root->left);
        postorder(root->right);
        cout<<root->a<<", ";
    }
    return;
}

void binarySearch(vector<int> x,int l,int r,vector<int> &result){
    if(l == 0 && r == 1){
        int mid =((r - l)/2)+l;
        result.push_back(x[mid]);
    }
    if(r - l == 1){
        return;
    }
    else{
        int mid =((r - l)/2)+l;
        result.push_back(x[mid]);
        binarySearch(x,l,mid,result);
        binarySearch(x,mid,r,result);
    }
}

int tree_H(struct leaf *root){
    if(root == NULL){
        return 0;
    }
    int leftTHeight = tree_H(root->left);
    int righTHeight = tree_H(root->right);
    return max(leftTHeight,righTHeight) + 1;
}

leaf *AVL(struct leaf *root){
    vector<int> x;
    vector<int> result;
    inorder_to_vecotr(root,x);
    leaf *rootAVL = NULL;
    binarySearch(x,0,x.size(),result);
    ofstream f("AVL_wysokosc.txt");
    for(int i = 0; i < result.size(); i++){
        rootAVL = insert(rootAVL,result[i]);
        f<<tree_H(rootAVL)<<endl;
        //cout<<result[i]<<", ";
    }
    f.close();
    return rootAVL;
}

void tree_search(struct leaf *root,int n, int x[],clock_t tv[],clock_t t){
    t = clock();
    leaf *temp = root;
    int h = 1;
    for(int i = 0;i<n;i++){
        while(temp->a != x[i]){
            if(temp->a > x[i]){ temp = temp->left; h++;}
            else{ temp = temp->right; h++;}
        }
        //cout<<temp->a<<" h:"<<h<<", ";
        h = 1;
        temp = root;
        tv[i] = clock() - t;
    }
    ofstream f("search_drzewo.txt");
    for(int i = 0;i < n; i++){
        f<<(float)tv[i]/CLOCKS_PER_SEC<<endl;
    }
    f.close();
}

void elements_in_lvl(struct leaf *root,int n, int x[],int lvl){
    leaf *temp = root;
    int h = 1;
    for(int i = 0;i<n;i++){
        while(temp->a != x[i]){
            if(temp->a > x[i]){ temp = temp->left; h++;}
            else{ temp = temp->right; h++;}
        }
        if(h == lvl) cout<<temp->a<<", ";
        h = 1;
        temp = root;
    }
}

void skrajny_prawy(struct leaf *root){
    leaf *temp = root;
    while(temp->right !=NULL)
        temp = temp->right;
    cout<<"najbardziej prawy potomek : "<<temp->a<<endl;
}

void skrajny_lewy(struct leaf *root){
    leaf *temp = root;
    while(temp->right !=NULL)
        temp = temp->left;
    cout<<"najbardziej lewy potomek : "<<temp->a<<endl;
}


int main()
{
    //wczytywanie z txt
    vector<string> v;
    fstream file;
    file.open("bst.txt");
    string line;
    while(!file.eof()){
        getline(file,line);
        v.push_back(line);
    }
    file.close();
    int n = v.size();
    int x[n];
    for(int i = 0; i < n; i++){
        x[i] = stoi(v[i]);
    }

    /*//generacja tablicy liczb
    int n = 50000;
    vector<int> v;
    int x[n];
    srand((unsigned)time(0));
    for(int i = 0; i < n; i++){
        v.push_back(i);
    }
    random_shuffle(v.begin(), v.end());
    for(int i = 0; i < n; i++){
        x[i] = v[i];
        //cout<<x[i]<<endl;
    }
    //cout<<endl;*/

    //czas
    clock_t t;

    /*//lista
    list *l = new list;
    clock_t tv1[n];
    t = clock();
    for(int i = 0; i < n; i++)
    {
        l->add(x[i]);
        tv1[i] = clock() - t;
        //cout<<tv[i]<<endl;
    }
    ofstream f("tworzenie_lista.txt");
    for(int i = 0;i < n; i++){
        f<<(float)tv1[i]/CLOCKS_PER_SEC<<endl;
    }
    f.close();
    //cout<<endl;
    //l->display();
    //cout<<endl;
    clock_t tv2[n];
    t = clock();
    l->search(n,x,tv2,t);
    //cout<<endl;
    clock_t tv3[n];
    t = clock();
    l->del(tv3,t);*/

    //drzewo
    leaf *root = NULL;
    clock_t tv4[n];
    t = clock();
    ofstream f10("BTS_wysokosc.txt");
    for(int i = 0; i < n; i++){
        root = insert(root,x[i]);
        f10<<tree_H(root)<<endl;
        tv4[i] = clock() - t;
    }
    f10.close();
    ofstream f2("tworzenie_drzewo.txt");
    for(int i = 0;i < n; i++){
        f2<<(float)tv4[i]/CLOCKS_PER_SEC<<endl;
    }
    f2.close();
    leaf *rootAVL = AVL(root);
    //elements_in_lvl(rootAVL,n,x,4);
    //cout<<endl;
    clock_t tv5[n];
    t = clock();
    //tree_search(root,n,x,tv5,t);
    //cout<<endl;
    //tree_search(rootAVL,n,x);
    //inorder(root);
    //cout<<endl;
    //preorder(root);
    //cout<<endl;
    cout<<"Porzadek wsteczny :";
    postorder(root);
    cout<<endl;
    cout<<"BTS_h : "<<tree_H(root)<<"     AVL_h : "<<tree_H(rootAVL)<<endl;
    clock_t tv6[n];
    t = clock();
    int k = 0;
    //cout<<"Porzadek wsteczny :";
    //postorder_del(root,tv6,t,k);
    skrajny_prawy(root);
    //skrajny_lewy(root);
    ofstream f3("del_drzewo.txt");
    for(int i = 0;i < n; i++){
        f3<<(float)tv6[i]/CLOCKS_PER_SEC<<endl;
    }
    f3.close();
    //postorder_del(rootAVL);
}
