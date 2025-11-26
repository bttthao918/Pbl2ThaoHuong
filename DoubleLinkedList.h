#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include <iostream>
#include <functional>
#include <stdexcept>
#include <string>
using std::function;

template <typename T>
class DoubleLinkedList
{
private:
    struct Node
    {
        T data;
        Node *prev;
        Node *next;

        Node(const T &value) : data(value), prev(nullptr), next(nullptr) {}
    };

    Node *head;
    Node *tail;
    int count;

public:
    DoubleLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~DoubleLinkedList()
    {
        clear();
    }

    DoubleLinkedList(const DoubleLinkedList &other) : head(nullptr), tail(nullptr), count(0)
    {
        Node *current = other.head;
        while (current)
        {
            pushBack(current->data);
            current = current->next;
        }
    }

    DoubleLinkedList &operator=(const DoubleLinkedList &other)
    {
        if (this != &other)
        {
            clear();
            Node *current = other.head;
            while (current)
            {
                pushBack(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    void pushFront(const T &value)
    {
        Node *newNode = new Node(value);
        if (!head)
        {
            head = tail = newNode;
        }
        else
        {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        count++;
    }

    void pushBack(const T &value)
    {
        Node *newNode = new Node(value);
        if (!tail)
        {
            head = tail = newNode;
        }
        else
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    void popFront()
    {
        if (!head)
            throw std::runtime_error("List is empty");
        Node *temp = head;
        head = head->next;
        if (head)
            head->prev = nullptr;
        else
            tail = nullptr;
        delete temp;
        count--;
    }

    void popBack()
    {
        if (!tail)
            throw std::runtime_error("List is empty");
        Node *temp = tail;
        tail = tail->prev;
        if (tail)
            tail->next = nullptr;
        else
            head = nullptr;
        delete temp;
        count--;
    }

    int size() const
    {
        return count;
    }

    bool empty() const
    {
        return count == 0;
    }

    void clear()
    {
        while (head)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        count = 0;
    }

    T *find(std::function<bool(const T &)> predicate)
    {
        Node *current = head;
        while (current)
        {
            if (predicate(current->data))
            {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }

    bool remove(std::function<bool(const T &)> predicate)
    {
        Node *current = head;
        while (current)
        {
            if (predicate(current->data))
            {
                if (current->prev)
                    current->prev->next = current->next;
                else
                    head = current->next;

                if (current->next)
                    current->next->prev = current->prev;
                else
                    tail = current->prev;

                delete current;
                count--;
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void forEach(std::function<void(const T &)> action) const
    {
        Node *current = head;
        while (current)
        {
            action(current->data);
            current = current->next;
        }
    }

    DoubleLinkedList<T> filter(std::function<bool(const T &)> predicate) const
    {
        DoubleLinkedList<T> result;
        Node *current = head;
        while (current)
        {
            if (predicate(current->data))
            {
                result.pushBack(current->data);
            }
            current = current->next;
        }
        return result;
    }

    void sort(std::function<bool(const T &, const T &)> compare)
    {
        if (count <= 1)
            return;
        head = mergeSort(head, compare);
        tail = head;
        while (tail->next)
        {
            tail = tail->next;
        }
    }

    class Iterator
    {
    private:
        Node *current;

    public:
        Iterator(Node *node) : current(node) {}

        T &operator*() { return current->data; }
        T *operator->() { return &(current->data); }

        Iterator &operator++()
        {
            if (current)
                current = current->next;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const Iterator &other) const
        {
            return current != other.current;
        }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    class ConstIterator
    {
    private:
        const Node *current;

    public:
        ConstIterator(const Node *node) : current(node) {}

        const T &operator*() const { return current->data; }
        const T *operator->() const { return &(current->data); }

        ConstIterator &operator++()
        {
            if (current)
                current = current->next;
            return *this;
        }

        bool operator==(const ConstIterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const ConstIterator &other) const
        {
            return current != other.current;
        }
    };

    ConstIterator begin() const { return ConstIterator(head); }
    ConstIterator end() const { return ConstIterator(nullptr); }

private:
    Node *mergeSort(Node *h, std::function<bool(const T &, const T &)> compare)
    {
        if (!h || !h->next)
            return h;

        Node *middle = getMiddle(h);
        Node *nextOfMiddle = middle->next;

        middle->next = nullptr;
        if (nextOfMiddle)
            nextOfMiddle->prev = nullptr;

        Node *left = mergeSort(h, compare);
        Node *right = mergeSort(nextOfMiddle, compare);

        return merge(left, right, compare);
    }

    Node *getMiddle(Node *h)
    {
        if (!h)
            return h;
        Node *slow = h;
        Node *fast = h->next;

        while (fast && fast->next)
        {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow;
    }

    Node *merge(Node *left, Node *right, std::function<bool(const T &, const T &)> compare)
    {
        if (!left)
            return right;
        if (!right)
            return left;

        Node *result = nullptr;

        if (compare(left->data, right->data))
        {
            result = left;
            result->next = merge(left->next, right, compare);
            if (result->next)
                result->next->prev = result;
        }
        else
        {
            result = right;
            result->next = merge(left, right->next, compare);
            if (result->next)
                result->next->prev = result;
        }

        return result;
    }
};

#endif