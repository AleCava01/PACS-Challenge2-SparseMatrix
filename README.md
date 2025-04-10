[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/HlQKP7Zu)
# Challenge 2: A Sparse Matrix
<p align="center">
  <a href="https://img.shields.io/badge/C%2B%2B-20-blue"><img src="https://img.shields.io/badge/C%2B%2B-20-blue" alt="C++"></a>
  <a href="https://img.shields.io/badge/GNU Make-3.27.6-blue"><img src="https://img.shields.io/badge/GNU Make-4.3-blue" alt="make"></a>
  <a href="https://img.shields.io/badge/clang-18.1.3-blue"><img src="https://img.shields.io/badge/clang-18.1.3-blue" alt="clang"></a>
</p>

## Project Structure and Implementation Choices
```
challenge2-gasati/
├── src/
│   ├── main.cpp
├── include/
│   ├── Matrix.hpp
│   ├── Matrix.tpp
|   ├── StorageOrder.hpp
├── Challenge24-25-2.pdf
├── Makefile
├── run
├── LICENSE
└── README.md
```
# Setup (Linux)
### Prerequisites
Ensure you have the following dependencies installed on your Linux environment:
- C++ Compiler (Clang, GCC, MSVC)
- Make

To install all the prerequisites, run the following command:
 ```
 sudo apt update && sudo apt install g++ cmake make
```
(the project was tested using C++ 20, GNU Make 3.27.6, clang 18.1.3)
### Building the Project
Clone the repository:
``` 
git clone https://github.com/PACS-24-25/challenge2-gasati.git
cd challenge2-gasati
```
Build with the Makefile:
```
make
```
Run the program:
```
./main
```
