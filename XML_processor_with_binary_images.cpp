//! XML_processor_with_binary_images.cpp
/*!
* \author Alan Djon Lüdke & Matheus Schaly
* \since 04/10/2018
* \version 1.0
* \copyright 2018
* \warning dataset06.xml with error
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

using namespace std;

/*! Coordinates class */
class Coordinates {
 public:
    //! Empty constructor method
    Coordinates() {}

    //! Constructor method
    Coordinates(const int i, const int j) {
        this -> i = i;
        this -> j = j;
    }

    //! Coordinate I getter
    int getI() {
        return i;
    }

    //! Coordinate J getter
    int getJ() {
        return j;
    }

    //! Coordinate I setter
    void setI(const int i) {
        this -> i = i;
    }

    //! Coordinate J setter
    void setJ(const int j) {
        this -> j = j;
    }

    //! Overloads the [] operator
    const int operator[](std::size_t index) const {
        if (index == 0) {
            return i;
        } else {
            return j;
        }
    }

 private:
    int i,j;
};

namespace structures {

/*! Linked Stack class */
template<typename T>
class LinkedStack {
 public:
    //! Constructor method;
    LinkedStack();
    //! Destructor method;
    ~LinkedStack();
    //! Wipes the list
    void clear();
    //! Inserts an element in the stack
    void push(const T& data);
    //! Removes an element
    T pop();
    //! Returns the first element
    T& top() const;
    //! Returns true if empty and false otherwise
    bool empty() const;
    //! Returns the size of the stack
    std::size_t size() const;

 private:
    //! Node class
    class Node {
     public:
        //! Constructor with 1 parameter
        /*!
          @param data node's data
         */
        explicit Node(const T& data):
            data_{data}
        {}


        //! Constructor with 2 parameters
        /*!
          @param data node's data
          @param next node's next node
         */
        Node(const T& data, Node* next):
            data_{data},
            next_{next}
        {}

        //! Node's data getter
        /*!
          @return node's data
         */
        T& data() {
            return data_;
        }

        //! Node's data getter constant
        /*!
          @return node's data constant
         */
        const T& data() const {
            return data_;
        }

        //! Node's next node getter
        /*!
          @return node's next node
         */
        Node* next() {
            return next_;
        }

        //! Node's next node getter constant
        /*!
          @return node's next node constant
         */
        const Node* next() const {
            return next_;
        }

        //! Node's next node setter
        /*!
          @param node node's next node
         */
        void next(Node* node) {
            next_ = node;
        }

     private:
        //! Node's data
        T data_;
        //! Node's next node
        Node* next_;
    };

    //! Top node
    Node* top_;
    //! Stack's current size
    std::size_t size_;
};

}  // namespace structures

//! Constructor method
template<typename T>
structures::LinkedStack<T>::LinkedStack() {
    top_ = nullptr;
    size_ = 0;
}

//! Destructor method
template<typename T>
structures::LinkedStack<T>::~LinkedStack() {
  clear();
}

//! Wipe the list
template<typename T>
void structures::LinkedStack<T>::clear() {
    while (!empty()) {
        pop();
    }
}

//! Inserts an element in the stack
/*!
  \param data data to be inserted
 */
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

//! Returns the first element
/*!
  \return the data inside the removed element
 */
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

//! Removes an element
/*!
  \return the data of the node at the stack's top
 */
template<typename T>
T& structures::LinkedStack<T>::top() const {
    if (empty()) {
        throw std::out_of_range("Lista vazia, metodo top()");
    }
    return top_->data();
}

//! Returns true if empty and false otherwise
/*!
  \return true if empty
 */
template<typename T>
bool structures::LinkedStack<T>::empty() const {
  if (size_ == 0) {
    return true;
  } else {
    return false;
  }
}

//! Returns the size of the stack
/*!
  \return stack's current size
 */
template<typename T>
std::size_t structures::LinkedStack<T>::size() const {
    return size_;
}

//! Gets a file by name
/*!
  \param filename the filename to return
  \return filename as a string
 */
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

//! Gets a complete tag
/*!
  \param line the tag's line
  \param pos the tag's index
  \return the tag found
 */
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

//! Counts how many "imgs" tags there are
/*!
  \param xmlfilename the document's filename
  \return quantity of tags in the file
 */
size_t getImgTagCount(string xmlfilename) {
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

//! Gets the complete image, containing tags and the binary data
/*!
  \param xmlfilename the file's name
  \param index the image's index
  \return the image as a string
 */
string getImage(string xmlfilename, size_t index) {
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

//! Gets the name of the image
/*!
  \param image the image that contains the name
  \return the image's name
 */
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

//! Gets the binary image height
/*!
  \param image the image to be measured
  \return the image's height
 */
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

//! Gets the binary image width
/*!
  \param image the image to be measured
  \return the image's width
 */
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

//! Gets the binary image data
/*!
  \param image the image where the data will be extracted
  \return the image's binary data
 */
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

//! Checks if all the tags are correctly formatted
/*!
  Returns false and prints error if:
  1 - A tag is openned but not closed
  2 - A tag is openned but the last tag in the stack is not the openned tag
  3 - The file ends and there is still some tag in the stack
  \param xmlfilename the name of the file
  \param lines the line where the the procedure should start
  \return true if there was an error, false otherwise
 */
bool doFirstPart (string xmlfilename, size_t lines) {
    // First Part:
    ifstream inFile;
    inFile.open(xmlfilename);

    structures::LinkedStack<string> tag_list;
    size_t line_counter = lines;
    string tag = "";
    string line = "";

    while (inFile && line_counter > 1) {
      inFile >> line;
      line_counter--;
      for (size_t caracter = 0; caracter < line.length(); caracter++) {
        if (line[caracter] == '<') {
          tag = getTag(line, caracter);;
          if (tag[1] == '/') {
            if (tag_list.empty()) {
              inFile.close();
              inFile.clear();
              return true;
            } else {
              string temp = tag.erase(1,1);
              if (temp.compare(tag_list.top()) == 0) {
                  tag_list.pop();
              } else {
                  inFile.close();
                  inFile.clear();
                  return true;
              }
            }
          } else {
            tag_list.push(tag);
          }
        }
      }
    }

    if(!tag_list.empty()){
      return true;
    }

    inFile.close();
    inFile.clear();
    return false;
}

//! Counts how many blocks of 1's there are
/*!
  Goes through the list, element by element, from left to
  right until a neighbour of such element is a 1.
  When the number 1 is found, it checks all the other neighbours
  and add them to the stack, giving them a label.
  It does it continously, until all the image is visited.
  \param xmlfilename the name of the file
  \param image the image containing the tags and the binary data
  \return the number of clusters of numbers 1
 */
size_t doSecondPart (string xmlfilename, string image) {

    // Second Part:
    string image_name = getName(image);
    size_t image_height = getHeight(image);
    size_t image_width =  getWidth(image);
    string image_data = getData(image);
    Coordinates coord;

    structures::LinkedStack<Coordinates> coord_stack;

    int dataset[image_height][image_width];
    int dataset_temp[image_height][image_width];
    int index = 0;
    int label = 1;
    int i_aux = 0;
    int j_aux = 0;

    for (size_t i = 0; i < image_height; i++) {
        for (size_t j = 0; j < image_width; j++) {
            dataset[i][j] = (int)(image_data[index]) - 48;
            dataset_temp[i][j] = 0;
            index++;
        }
    }

    for (size_t i = 0; i < image_height; i++) {
        for (size_t j = 0; j < image_width; j++) {
            if (dataset[i][j] == 1 && dataset_temp[i][j] < 1) {
                coord.setI(i);
                coord.setJ(j);
                coord_stack.push(coord);
                dataset_temp[i][j] = label;
                i_aux = i;
                j_aux = j;
            }
            while (!coord_stack.empty()) {
                i_aux = coord_stack.top()[0];
                j_aux = coord_stack.top()[1];
                coord_stack.pop();
                if (i_aux-1 >= 0 && dataset[i_aux-1][j_aux] == 1 && dataset_temp[i_aux-1][j_aux] == 0) {
                    coord.setI(i_aux-1);
                    coord.setJ(j_aux);
                    coord_stack.push(coord);
                    dataset_temp[coord_stack.top()[0]][coord_stack.top()[1]] = label;
                }
                if (i_aux+1 < image_height && dataset[i_aux+1][j_aux] == 1 && dataset_temp[i_aux+1][j_aux] == 0) {
                    coord.setI(i_aux+1);
                    coord.setJ(j_aux);
                    coord_stack.push(coord);
                    dataset_temp[coord_stack.top()[0]][coord_stack.top()[1]] = label;
                }
                if (j_aux-1 >= 0 && dataset[i_aux][j_aux-1] == 1 && dataset_temp[i_aux][j_aux-1] == 0) {
                    coord.setI(i_aux);
                    coord.setJ(j_aux-1);
                    coord_stack.push(coord);
                    dataset_temp[coord_stack.top()[0]][coord_stack.top()[1]] = label;
                }
                if (j_aux+1 < image_width && dataset[i_aux][j_aux+1] == 1 && dataset_temp[i_aux][j_aux+1] == 0) {
                    coord.setI(i_aux);
                    coord.setJ(j_aux+1);
                    coord_stack.push(coord);
                    dataset_temp[coord_stack.top()[0]][coord_stack.top()[1]] = label;
                }
                if (coord_stack.empty()) {
                    label++;
                }
            }
        }
    }
    return label-1;
}

int main() {

    char xmlfilename[100];
    std::cin >> xmlfilename;
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
    size_t imgTagCount = getImgTagCount(xmlfilename);

    for (size_t i = 1; i <= imgTagCount; i++) {
      bool error = false;
      error = doFirstPart(xmlfilename, line_counter);
      if (!error) {
        string image = getImage(xmlfilename, i);

        image_name = getName(image);
        image_height = getHeight(image);
        image_width =  getWidth(image);
        image_data = getData(image);

        cout << image_name << " " << doSecondPart(xmlfilename, image) << endl;
      } else{
        cout << "error" << endl;
        break;
      }
    }
}
