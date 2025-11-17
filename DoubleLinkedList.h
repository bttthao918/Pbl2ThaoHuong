#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include <iostream>
#include <functional>
#include "Exception.h"

template <typename T>
class Node {
public:
    T data;
    Node* next;
    Node* prev;
    
    Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename T>
class DoubleLinkedList {
private:
    Node<T>* head;
    Node<T>* tail;
    int size;

public:
    DoubleLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    
    ~DoubleLinkedList() {
        clear();
    }

    // Thêm phần tử vào cuối
    void push_back(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }

    // Thêm phần tử vào đầu
    void push_front(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        size++;
    }

    // Xóa phần tử theo điều kiện
    bool remove_if(std::function<bool(const T&)> predicate) {
        Node<T>* current = head;
        while (current) {
            if (predicate(current->data)) {
                Node<T>* toDelete = current;
                if (current == head) {
                    head = current->next;
                    if (head) head->prev = nullptr;
                    else tail = nullptr;
                } else if (current == tail) {
                    tail = current->prev;
                    tail->next = nullptr;
                } else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                }
                current = current->next;
                delete toDelete;
                size--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Tìm phần tử theo điều kiện
    T* find_if(std::function<bool(const T&)> predicate) {
        Node<T>* current = head;
        while (current) {
            if (predicate(current->data)) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    // Cập nhật phần tử
    bool update_if(std::function<bool(const T&)> predicate, const T& newValue) {
        T* found = find_if(predicate);
        if (found) {
            *found = newValue;
            return true;
        }
        return false;
    }

    // Duyệt qua tất cả phần tử
    void for_each(std::function<void(T&)> action) {
        Node<T>* current = head;
        while (current) {
            action(current->data);
            current = current->next;
        }
    }

    // Lấy kích thước
    int get_size() const {
        return size;
    }

    // Kiểm tra rỗng
    bool is_empty() const {
        return size == 0;
    }

    // Xóa tất cả
    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        size = 0;
    }

    // Lọc và trả về danh sách mới
    DoubleLinkedList<T> filter(std::function<bool(const T&)> predicate) {
        DoubleLinkedList<T> result;
        Node<T>* current = head;
        while (current) {
            if (predicate(current->data)) {
                result.push_back(current->data);
            }
            current = current->next;
        }
        return result;
    }

    // Iterator để duyệt danh sách
    class Iterator {
    private:
        Node<T>* current;
    public:
        Iterator(Node<T>* node) : current(node) {}
        
        T& operator*() { return current->data; }
        Iterator& operator++() { current = current->next; return *this; }
        bool operator!=(const Iterator& other) { return current != other.current; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

#endif