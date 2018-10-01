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
#include <vector>

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
    void print_stack();

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
        throw std::out_of_range("Lista vazia, metodo top()");
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

template<typename T>
void structures::LinkedStack<T>::print_stack() {
    Node* node = top_;
    for (int i = 0; i < size_; i++) {
        cout << node->data() << " ";
        node = node->next();
    }
    cout << endl;
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

size_t getImgTagCount(string xmlfilename, size_t lines) {
  // First Part:
  ifstream inFile;
  inFile.open(xmlfilename);

  string tag = "";
  string line = "";
  size_t img_tag_counter = 0;

  while (inFile) { // While file still have lines to read
    inFile >> line;
    for (size_t caracter = 0; caracter < line.length(); caracter++) {
      if (line[caracter] == '<') {
        tag = getTag(line, caracter);
        if (tag.compare("<img>") == 0) {
          img_tag_counter++;
        }
      }
    }
  }

  inFile.close();
  inFile.clear();
  return img_tag_counter;
}

string getImage(string xmlfilename, size_t totalImgTag, size_t index) {
    ifstream inFile;
    inFile.open(xmlfilename);

    string tag = "";
    string image = "<";
    string line = "";
    size_t imgsFounded = 0;

    while (inFile) {
      inFile >> line;
      for (size_t caracter = 0; caracter < line.length(); caracter++) {
        if (imgsFounded==index && tag.compare("</img>") != 0) {
            image += line[caracter];
        }
        if (line[caracter] == '<') {
          tag = getTag(line, caracter);;
          if (tag.compare("<img>") == 0) {
            imgsFounded++;
          }
        }
      }
    }
    inFile.close();
    inFile.clear();
    return image+"/img>";
}

string getName(string image) {

    string image_name = "";

    for (int caracter = 0; caracter < image.length(); caracter++) {
      if (image[caracter] == '<'){
        if (getTag(image, caracter).compare("<name>") == 0) {
          caracter += 6;
          while (image[caracter] != '<') {
            image_name += image[caracter];
            caracter++;
          }
        }
      }
    }
    return image_name;
}

size_t getHeight(string image) {

  string string_height = "";
  size_t image_height;

  for (int caracter = 0; caracter < image.length(); caracter++) {
    if (image[caracter] == '<'){
      if (getTag(image, caracter).compare("<height>") == 0) {
        caracter += 8;
        while (image[caracter] != '<') {
          string_height += image[caracter];
          caracter ++;
        }
        stringstream sstream(string_height); // Converts string to size_t
        sstream >> image_height;
      }
    }
  }
  return image_height;
}

size_t getWidth(string image) {

  string string_width = "";
  size_t image_width;

  for (int caracter = 0; caracter < image.length(); caracter++) {
    if (image[caracter] == '<'){
      if (getTag(image, caracter).compare("<width>") == 0) {
        caracter += 7;  // To skip the caracters height>
        while (image[caracter] != '<') {
          string_width += image[caracter];
          caracter ++;
        }
        stringstream sstream(string_width); // Converts string to size_t
        sstream >> image_width;
      }
    }
  }
  return image_width;
}

string getData(string image) {

    string image_data = "";
    string data[getHeight(image)][getWidth(image)];

    for (int caracter = 0; caracter < image.length(); caracter++) {
      if (image[caracter] == '<' && getTag(image, caracter).compare("<data>") == 0){
        caracter += 6;  // To skip the caracters name>
        while (image[caracter] != '<') {
          image_data += image[caracter];
          caracter++;
        }
      }
    }
    return image_data;
}

bool doFirstPart (string xmlfilename, size_t lines) {
    // First Part:
    ifstream inFile;
    inFile.open(xmlfilename);

    structures::LinkedStack<string> tag_list;
    size_t line_counter = lines;
    string tag = "";
    string line = "";

    while (inFile && line_counter > 1) {
      //cout<<endl<<"linhas: "<<line_counter<<"/"<<lines<<endl;
      inFile >> line;
      line_counter--;
      for (size_t caracter = 0; caracter < line.length(); caracter++) {
        if (line[caracter] == '<') {
          tag = getTag(line, caracter);;
          if (tag[1] == '/') {
            if (tag_list.empty()) {
              cout << "error" << endl;
              inFile.close();
              inFile.clear();
              return true;
            } else {
              string temp = tag.erase(1,1);
              //cout << tag_list.top() << " == " << tag<< endl;
              if (temp.compare(tag_list.top()) == 0) {
                  tag_list.pop();
                  //cout << tag << " popped" << endl;
              } else {
                  cout << "error" << endl;
                  inFile.close();
                  inFile.clear();
                  return true;
              }
            }
          } else {
            tag_list.push(tag);
            //cout << tag << " pushed" << endl;
          }
        }
      }
    }

    inFile.close();
    inFile.clear();
    return false;
}

void print_array(string image) {
  int caracter = 0;

  string temp[getHeight(image)][getWidth(image)];

  for (size_t i = 0; i < getHeight(image); i++) {
    cout<<endl;
    for (size_t j = 0; j < getWidth(image); j++) {
      cout<<getData(image)[caracter];
      caracter++;
    }
  }
  cout<<endl<<endl;
}

size_t doSecondPart (string xmlfilename, string image) {

    // Second Part:
    size_t image_height = getWidth(image);
    size_t image_width =  getHeight(image);
    string image_data = getData(image);

    structures::LinkedStack<int*> coord_stack;

    int dataset[image_height][image_width];
    int dataset_temp[image_height][image_width];
    int dataset_visited[image_height][image_width];
    int index = 0;
    int label = 1;
    int i_aux = 0;
    int j_aux = 0;

    for (size_t i = 0; i < image_height; i++) {
        for (size_t j = 0; j < image_width; j++) {
            dataset[i][j] = (int)(image[index]) - 48;
            dataset_temp[i][j] = 0;
            dataset_temp[i][j] = 0;
            index++;
        }
    }

   for (size_t i = 0; i < image_height; i++) {
        for (size_t j = 0; j < image_width; j++) {
            cout << dataset[i][j];
        }
    }

    for (size_t i = 0; i < image_height; i++) {
        for (size_t j = 0; j < image_width; j++) {
            if (dataset[i][j] == 1 && dataset_visited[i][j] != 1) {
                int *coord;
                coord = new int[2];
                coord[0] = i;
                coord[1] = j;
                coord_stack.push(coord);
                dataset_visited[i][j] = 1;
                dataset_temp[i][j] = label;
                i_aux = i;
                j_aux = j;
            }
            while (!coord_stack.empty()) {
                i_aux = coord_stack.top()[0];
                j_aux = coord_stack.top()[1];
                coord_stack.pop();
                if (i_aux-1 >= 0 && dataset[i_aux-1][j_aux] == 1 && dataset_temp[i_aux-1][j_aux] == 0) {
                    int *coord;
                    coord = new int[2];
                    coord[0] = i_aux-1;
                    coord[1] = j_aux;
                    coord_stack.push(coord);
                    dataset_temp[coord_stack.top()[0]][coord_stack.top()[1]] = label;
                    dataset_visited[coord_stack.top()[0]][coord_stack.top()[1]] = 1;
                }
                if (i_aux+1 <= image_height && dataset[i_aux+1][j_aux] == 1 && dataset_temp[i_aux+1][j_aux] == 0) {
                    int *coord;
                    coord = new int[2];
                    coord[0] = i_aux+1;
                    coord[1] = j_aux;
                    coord_stack.push(coord);
                    dataset_temp[coord_stack.top()[0]][coord_stack.top()[1]] = label;
                    dataset_visited[coord_stack.top()[0]][coord_stack.top()[1]] = 1;
                }
                if (j_aux-1 >= 0 && dataset[i_aux][j_aux-1] == 1 && dataset_temp[i_aux][j_aux-1] == 0) {
                    int *coord;
                    coord = new int[2];
                    coord[0] = i_aux;
                    coord[1] = j_aux-1;
                    coord_stack.push(coord);
                    dataset_temp[coord_stack.top()[0]][coord_stack.top()[1]] = label;
                    dataset_visited[coord_stack.top()[0]][coord_stack.top()[1]] = 1;
                }
                if (j_aux+1 <= image_width && dataset[i_aux][j_aux+1] == 1 && dataset_temp[i_aux][j_aux+1] == 0) {
                    int *coord;
                    coord = new int[2];
                    coord[0] = i_aux;
                    coord[1] = j_aux+1;
                    coord_stack.push(coord);
                    dataset_temp[coord_stack.top()[0]][coord_stack.top()[1]] = label;
                    dataset_visited[coord_stack.top()[0]][coord_stack.top()[1]] = 1;
                }
                if (coord_stack.empty()) {
                    label++;
                }
            }
        }
    }
    //cout << image_name << " " <<  label - 1 << endl;

    return label;
}

int main() {

    //char xmlfilename[100];
    //std::cin >> xmlfilename;
    string xmlfilename;
    xmlfilename="dataset01.xml";
    ifstream inFile;
    inFile.open(xmlfilename);

    size_t line_counter = 0;
    string tag = "";
    string line = "";
    string image_name = "";
    size_t image_width = 0;
    size_t image_height = 0;
    string image_data = "";

    while (inFile) {
        inFile >> line;
        line_counter++;
    }
    inFile.close();
    inFile.clear();
    size_t imgTagCount = getImgTagCount(xmlfilename, line_counter);
    cout << line_counter << " linhas"<< endl;
    cout << imgTagCount << " imagens"<< endl;

    for (size_t i = 1; i <= imgTagCount; i++) {
      bool error = doFirstPart(xmlfilename, line_counter);
      if (!error) {
        string image = getImage(xmlfilename, imgTagCount, i);

        image_name = getName(image);
        image_height = getHeight(image);
        image_width =  getWidth(image);
        image_data = getData(image);

        cout << "Name: " << image_name << endl;
        cout << "Height: " << image_height << endl;
        cout << "Width: " << image_width << endl;
        cout << "Data: " << image_data << endl;
        print_array(image);

        //cout<<doSecondPart(xmlfilename, image)<<endl;
      }
    }
}
