#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#define GRAY 'g'
#define WHITE 'w'
#define BLACK 'b'

//Edge
typedef struct {
	int neighbor;
	int cost;
} Edge;

typedef struct edge_node {
	Edge e;
	struct edge_node* next;
} EdgeNode;

typedef struct {
	EdgeNode* head;
	EdgeNode* tail;
} EdgeList;

//Candidate
typedef struct {
	int computer;
	int min;
} Candidate;

typedef struct candidate_node {
	Candidate c;
	struct candidate_node* next;
	struct candidate_node* prev;
} CandidateNode;

typedef struct {
	CandidateNode* head;
	CandidateNode* tail;
} CandidateList;


EdgeList* build_net(int n);
int* build_prim_tree(EdgeList net[], int n);
EdgeList* build_paths(int* tree, int n);
void find_and_print_path(EdgeList primpaths[], int n, int first, int last);
//Others
void FindPath(int first, int last, EdgeList* primpaths, char* Colors, int* Parents);
void PrintPath(int last, int first, int* Parents);
//Edge
EdgeList build_empty_edge_list();
EdgeNode* create_edge_node(int neighbor, int cost);
void insert_to_list(EdgeNode* previous, EdgeNode* new_node);
void add_to_sorted_list(EdgeList* lst, int computer, int price);
//Candidate
CandidateList build_empty_candidate_list();
CandidateNode* create_candidate_node(int comp, int min, CandidateNode* prev, CandidateNode* next);
void insert_to_dlist(CandidateNode* previous, CandidateNode* new_node);
int DeleteMin(CandidateList priority, CandidateNode** loc);
void DecreaseKey(CandidateNode** loc, int comp, int new_min);
//Free
void free_edge_list_array(EdgeList* lst, int size);
void free_edge_list(EdgeList* lst);
void free_Cand_list(CandidateList* lst);
void check_memory_allocation(void* ptr);
