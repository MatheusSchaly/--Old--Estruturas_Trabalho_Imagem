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
#include <string.h>
#include <sstream>
#include <list>

using namespace std;

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
    if (empty()) {
        throw std::out_of_range("Lista vazia");
    }
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

string getImage(string xmlfilename, string &image_name, size_t &image_width, size_t &image_height) {
    ifstream inFile;
    inFile.open(xmlfilename);

    string tag = "";
    string image = "";
    string line = "";
    string string_height = "";
    string string_width = "";
    bool found = false;

    while (!inFile.eof()) { // While file still have lines to read
        inFile >> line;
        for (size_t caracter = 0; caracter < line.length(); caracter++) {
            if (found) {
                image += line[caracter];
            }
            if (line[caracter] == '<') {
                tag = getTag(line, caracter);
                if (tag.compare("<name>") == 0) {
                    caracter += 6;  // To skip the caracters name>
                    while (line[caracter] != '<') {
                        image_name += line[caracter];
                        caracter ++;
                    }
                } else if (tag.compare("<height>") == 0) {
                    caracter += 8;  // To skip the caracters height>
                    while (line[caracter] != '<') {
                        string_height += line[caracter];
                        caracter ++;
                    }
                    stringstream sstream(string_height); // Converts string to size_t
                    sstream >> image_height;
                } else if (tag.compare("<width>") == 0) {
                    caracter += 7;  // To skip the caracters width>
                    while (line[caracter] != '<') {
                        string_width += line[caracter];
                        caracter ++;
                    }
                    stringstream sstream(string_width); // Converts string to size_t
                    sstream >> image_width;
                } else if (tag.compare("<data>") == 0) {
                    caracter += 6;  // To skip the caracters data>
                    found = true;
                } else if (tag.compare("</data>") == 0) {
                    return image.erase(image.length() - 1, image.length()); // Removes a <
                }
            }
        }
    }
    return image;
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
    bool finished_part_1 = false;

    while (inFile && finished_part_1 == false){
        inFile >> line;
        for (size_t caracter = 0; caracter < line.length(); caracter++) {
            if (line[caracter] == '<') {
                tag = getTag(line, caracter);
                if (tag[1] == '/') {
                  if((tag.erase(1,1)).compare(tag_list.top()) == 0) {
                    if (tag.compare("<dataset>") != 0) {
                      if(tag_list.empty()){
                        cout << "error" << endl;
                      } else{
                        tag_list.pop();
                      }
                    } else if(tag.compare("<dataset>") == 0) {
                      if(tag_list.size() > 0) {
                        tag_list.pop();
                        finished_part_1 = true;
                      }
                    }
                  } else { //top != popped
                    cout << "error" << endl;
                    finished_part_1 = true;
                  }
                } else {
                    tag_list.push(tag);
                }
            }
        }
    }
    inFile.close();
    inFile.clear();
    

    // Second Part:
    string image_name = "";
    int coord[2];
    size_t image_width = 0;
    size_t image_height = 0;
    structures::LinkedStack<int*> coord_stack;

    string image = getImage(xmlfilename, image_name, image_width, image_height);

    int dataset[image_height][image_width];
    int dataset_temp[image_height][image_width];
    int index = 0;
    int label = 1;

    for (size_t i = 0; i < image_height; i++) {
        for (size_t j = 0; j < image_width; j++) {
            dataset[i][j] = (int)(image[index]) - 48;
            dataset_temp[i][j] = 0;
            index++;
        }
    }
    
    for (size_t i = 0; i < image_height; i++) {
        for (size_t j = 0; j < image_width; j++) {
            cout << dataset[i][j];
        }
        cout << endl;
    }

    for (size_t i = 0; i < image_height; i++) {
        for (size_t j = 0; j < image_width; j++) {
            if (dataset[i][j] == 1) {
                coord[0] = i;
                coord[1] = j;
                coord_stack.push(coord);
            }
            while (!coord_stack.empty()) {
                coord_stack.pop();
                dataset_temp[i][j] = label;
                if (i-1 >= 0 && dataset[i-1][j] == 1 && dataset_temp != 0) {
                    dataset_temp[i-1][j] = label;
                }
                if (i+1 <= image_height && dataset[i+1][j] == 1 && dataset_temp != 0) {
                    dataset_temp[i+1][j] = label;
                }
                if (j-1 >= 0 && dataset[i][j-1] == 1 && dataset_temp != 0) {
                    dataset_temp[i][j-1] = label;
                }
                if (j+1 <= image_width && dataset[i][j+1] == 1 && dataset_temp != 0) {
                    dataset_temp[i][j+1] = label;
                }
            }
            //if (dataset_temp[i][j] == 0 && )
            //label ++;
        }
    }
    
    cout << endl;
    for (size_t i = 0; i < image_height; i++) {
        for (size_t j = 0; j < image_width; j++) {
            cout << dataset_temp[i][j];
        }
        cout << endl;
    }
}
