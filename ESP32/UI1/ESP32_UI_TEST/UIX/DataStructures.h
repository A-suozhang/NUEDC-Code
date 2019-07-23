#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

#include "esp_log.h"

template <typename T>
class LinkedListNode{
    public:
    T* obj;
    LinkedListNode* next=NULL;
    LinkedListNode(T* obj_,LinkedListNode* next_){
        obj=obj_;next=next_;
    }
    ~LinkedListNode(){
        delete obj;
    }
};

template <typename T>
class LinkedList{
    public:
    LinkedListNode<T>* head=NULL;
    void prepend(T* obj){
        LinkedListNode<T>* newhead=new LinkedListNode<T>(obj,head);
        head=newhead;
    }
    void removenext(LinkedListNode<T>* node){
        if(node->next!=NULL){
            node->next=(node->next)->next;
            delete node->next;
        }
    }
};

template <typename T>
class DoublyLinkedListNode{
    public:
    T* obj;
    DoublyLinkedListNode* next=NULL;
    DoublyLinkedListNode* prev=NULL;
    DoublyLinkedListNode(T* obj_,DoublyLinkedListNode* prev_,DoublyLinkedListNode* next_){
        obj=obj_;next=next_;prev=prev_;
    }
    DoublyLinkedListNode(T* obj_,DoublyLinkedListNode* next_){
        obj=obj_;next=next_;prev=NULL;
    }
    ~DoublyLinkedListNode(){
        delete obj;
    }
};

template <typename T>
class DoublyLinkedList{
    public:
    DoublyLinkedListNode<T>* head=NULL;
    void prepend(T* obj){
        DoublyLinkedListNode<T>* newhead=new DoublyLinkedListNode<T>(obj,head);
        if(head!=NULL)
            head->prev=newhead;
        head=newhead;
    }
    //slow implemention, todo: cache tail pointer
    void append(T* obj){
        DoublyLinkedListNode<T>* it=head;
        if(it!=NULL){
            while((it->next)!=NULL){
                it=it->next;
            }
            DoublyLinkedListNode<T>* newtail=new DoublyLinkedListNode<T>(obj,it,NULL);
            it->next=newtail;
        }else{
            prepend(obj);
        }
    }

    void iterate(T* obj, void (*itf)(T* obj)){
        DoublyLinkedListNode<T>* it=head;
        while(it!=NULL){
            (*itf)(it->obj);
            it=it->next;
        }
    }

    void remove(DoublyLinkedListNode<T>* node){
        if(node->prev!=NULL) (node->prev)->next=node->next;
        if(node->next!=NULL) (node->next)->prev=node->prev;
        if(head==node){
            head=node->next;
        }
        delete node;
    }

    DoublyLinkedList<T>& operator+=(T& rhs){
        this->append(&rhs);
        return *this;
    }
};

#endif
