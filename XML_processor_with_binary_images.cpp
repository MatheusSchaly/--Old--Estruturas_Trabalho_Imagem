/* Copyright [2018] <Alan Djon Lüdke & Matheus Schaly>
* XML_processor_with_binary_images.cpp
*/

#ifndef STRUCTURES_XML_PROCESSOR
#define STRUCTURES_XML_PROCESSOR

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
#include <string.h>

namespace structures {

//! class LinkedStack
template<typename T>
class LinkedStack {
 public:
    //! Constructor method of LinkedStack();
    LinkedStack();
    //! Destructor method
    ~LinkedStack();
    //! Wipe the list
    void clear();  // limpa pilha
    //! Puts an element in the stack
    void push(const T& data);  // empilha
    //! Takes an element off
    T pop();  // desempilha
    //! Return the first element
    T& top() const;  // dado no topo
    //! Return if empty
    bool empty() const;  // pilha vazia
    //! Returns the size of the stack
    std::size_t size() const;  // tamanho da pilha

 private:
    class Node {
     public:
        explicit Node(const T& data):
            data_{data}
        {}
        Node(const T& data, Node* next):
            data_{data},
            next_{next}
        {}

        T& data() {
            return data_;
        }  // getter: info

        const T& data() const {  // getter const: dado
            return data_;
        }

        Node* next() {  // getter: próximo
            return next_;
        }
        const Node* next() const {  // getter const: próximo
            return next_;
        }
        void next(Node* node) {  // setter: próximo
            next_ = node;
        }

     private:
        T data_;
        Node* next_;
    };

    Node* top_;  // nodo-topo
    std::size_t size_;  // tamanho
};

}  // namespace structures

template<typename T>
structures::LinkedStack<T>::LinkedStack() {
    top_ = nullptr;
    size_ = 0;
}

template<typename T>
structures::LinkedStack<T>::~LinkedStack() {
  clear();
}

template<typename T>
void structures::LinkedStack<T>::clear() {
    while (!empty()) {
        pop();
    }
}

template<typename T>
void structures::LinkedStack<T>::push(const T& data) {
  Node *novo = new Node(data);

  if (novo == nullptr) {
    throw std::out_of_range("Nodo não alocado!");
  } else {
    if (size_ == 0) {
      top_ = novo;
    } else {
      novo->next(top_);
      top_ = novo;
    }
    size_++;
  }
}

template<typename T>
T structures::LinkedStack<T>::pop() {
  Node *saiu;
  T volta;

  if (empty()) {
    throw std::out_of_range("Lista vazia, não é possivel retirar!");
  } else {
    saiu = top_;
    volta = saiu->data();
    top_ = saiu->next();
    size_ = size_ - 1;
    delete saiu;
    return volta;
  }
}

template<typename T>
T& structures::LinkedStack<T>::top() const {
  return top_->data();
}

template<typename T>
bool structures::LinkedStack<T>::empty() const {
  if (size_ == 0) {
    return true;
  } else {
    return false;
  }
}

template<typename T>
std::size_t structures::LinkedStack<T>::size() const {
    return size_;
}

string getFile(string filename) {
   string buffer;
   char c;

   ifstream in(filename);
   if (!in) {
     cout << filename << "not found";
     exit(1);
   }
   while (in.get(c)) buffer += c;
   in.close();

   return buffer;
}

#endif

string getTag(string line, int pos) {
    bool closed_bracket = false;  // >
    bool close_tag = false;  // /

    string tag = "";
    size_t caracter = pos;
    while (closed_bracket == false) {
        if(line[caracter] == '>') {
            closed_bracket = true;
        }
        tag += line[caracter];
        caracter++;
    }
    return tag;
}

int main() {

    char xmlfilename[100];
    std::cin >> xmlfilename;


    // First Part:
    ifstream inFile;
    inFile.open(xmlfilename);

    structures::LinkedStack<string> tag_list;
    string tag = "";
    string line = "";

    while (!inFile.eof()) { // While file still have lines to read
        inFile >> line;
        for (size_t caracter = 0; caracter < line.length(); caracter++) {
            if (line[caracter] == '<') {
                tag = getTag(line, caracter);
                if (tag[1] == '/') {
                    if ((tag.compare("</dataset>") != 0) && (tag_list.empty())) {
                        cout << "error" << endl;
                        return 0;
                    } else if((tag.compare("</dataset>") == 0) && (tag.erase(1,1)).compare(tag_list.top()) == 0){  // W
                      tag_list.pop();
                      return 0;
                    } else {
                      if((tag.erase(1,1)).compare(tag_list.top()) == 0) {
                        tag_list.pop();
                      } else {
                        cout << "error" << endl;
                        return 0;
                      }
                    }
                } else {
                    tag_list.push(tag);
                }
            }
        }
    }
    inFile.close();

/*
    // Second Part:
    inFile.clear();
    inFile.open(xmlfilename);

    tag = "";
    string name = "";
    int height = 0;
    int width= 0;
    //int data[][];


    while (!inFile.eof()) { // While file still have lines to read
        inFile >> line;
        for (size_t caracter = 0; caracter < line.length(); caracter++) {
            if (line[caracter] == '<') {
                tag = getTag(line, caracter);
                if (tag.compare("<img>") == 0) {  // Found the binary image
                  caracter = caracter + 6; // goes to tag <name>
                  if (line[caracter] == '<') {
                    tag = getTag(line, caracter);
                    if (tag.compare("<name>") == 0) {  // Found the binary image name
                      caracter++;
                      while(line[caracter] != '<') {
                        name = name + line[caracter];
                      }
                      cout << name << endl;
                    }
                  }
                }
            }
        }
    }*/
}
