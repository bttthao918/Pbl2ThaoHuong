#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include <iostream>
#include <functional>
#include <stdexcept>

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
    // Constructor
    DoubleLinkedList() : head(nullptr), tail(nullptr), count(0) {}

    // Destructor
    ~DoubleLinkedList()
    {
        clear();
    }

    // Copy constructor
    DoubleLinkedList(const DoubleLinkedList &other) : head(nullptr), tail(nullptr), count(0)
    {
        Node *current = other.head;
        while (current)
        {
            pushBack(current->data);
            current = current->next;
        }
    }

    // Assignment operator
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

    // Add element to front
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

    // Add element to back
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

    // Remove element from front
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

    // Remove element from back
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

    // Get size
    int size() const
    {
        return count;
    }

    // Check if empty
    bool empty() const
    {
        return count == 0;
    }

    // Clear all elements
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

    // Find element by condition
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

    // Remove element by condition
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

    // Filter elements
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

    // Sort using merge sort (stable, O(n log n))
    void sort(std::function<bool(const T &, const T &)> compare)
    {
        if (count <= 1)
            return;
        head = mergeSort(head, compare);

        // Update tail
        tail = head;
        while (tail->next)
        {
            tail = tail->next;
        }
    }

    // Iterator support
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

    // Const iterator
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
    // Merge sort helper functions
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