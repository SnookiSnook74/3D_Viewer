#ifndef BACKEND_H
#define BACKEND_H

#include <locale.h>
#include <math.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// Коды статуса операции
#define OK 0
#define ERROR 1

// Размер буфера для чтения строк из файла
#define BUFFER_SIZE 1000

// Структура для представления вершины
typedef struct Vertex {
  float x;
  float y;
  float z;
} Vertex;

// Структура для представления грани
typedef struct Face {
  int *vertexes;  // Массив индексов вершин, образующих грань
  int num_of_vertexes;  // Количество вершин в грани
} Face;

// Структура для представления модели
typedef struct Model {
  Vertex *vertexes;  // Массив вершин модели
  Face *faces;       // Массив граней модели
  int num_of_faces;  // Количество граней в модели
  int total_vertexes;  // Общее количество вершин в модели
} Model;

// Функция для проверки начала строки
bool _starts_with(const char *pre, const char *str);

// Функции для разбора данных граней и вершин
int _parse_faces(Model **model, char *buffer);
int _parse_vertexes(Model **model, char *buffer);

// Функция для загрузки модели из файла
int _get_model(const char *filename, Model **model);

// Функция для сброса данных модели
void _reset_model(Model **model);

// Функции для преобразований вершин
Vertex rotateX(Vertex point, double angle);
Vertex rotateY(Vertex point, double angle);
Vertex rotateZ(Vertex point, double angle);

Vertex translateX(Vertex point, double translate);
Vertex translateY(Vertex point, double translate);
Vertex translateZ(Vertex point, double translate);

Vertex scale_vertex(Vertex point, double scale);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif
