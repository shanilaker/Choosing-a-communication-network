#include "headers.h"

int main() {
	int n;
	EdgeList* Net;
	int* Prim;
	EdgeList* PrimPath;
	int first, last;

	scanf("%d", &n);

	if (n < 0)
	{
		printf("Invalid input. ");
		return 0;
	}

	if (n != 0)
	{
		Net = build_net(n);

		if (Net == NULL)
		{
			return 0;
		}

		Prim = build_prim_tree(Net, n);
		if (Prim == NULL)
		{
			free_edge_list_array(Net, n);
			return 0;
		}

		PrimPath = build_paths(Prim, n);

		int count = scanf("%d%d", &first, &last);

		if (count == 2)
		{
			find_and_print_path(PrimPath, n, first, last);
		}

		free_edge_list_array(Net, n);
		free_edge_list_array(PrimPath, n);
		free(Prim);
	}
	return 0;
}

//printing the path as wanted
void PrintPath(int last, int first, int* Parents)
{
	if (last == first)
	{
		printf("%d", last);
	}

	else
	{
		PrintPath(Parents[last], first, Parents);
		printf(" %d", last);
	}
}

//the envelope function of FindPath
void find_and_print_path(EdgeList primpaths[], int n, int first, int last)
{
	if (first < 0 || n <= first || last < 0 || n <= last)
	{
		printf("Invalid input. ");
		return;
	}

	char* Colors;
	int* Parents;
	int i;

	//memory allocation and testing
	Colors = (char*)malloc((n + 1) * sizeof(char));
	check_memory_allocation(Colors);

	Parents = (int*)malloc(n * sizeof(int));
	check_memory_allocation(Parents);

	//initializing the arrays
	for (i = 0; i < n; i++)
	{
		Colors[i] = WHITE;
		Parents[i] = -1;
	}
	Colors[i] = '\0';

	FindPath(first, last, primpaths, Colors, Parents);
	PrintPath(last, first, Parents);

	free(Colors);
	free(Parents);
}

//A function that find the between first and last
void FindPath(int first, int last, EdgeList primpaths[], char* Colors, int* Parents) {
	EdgeNode* curr = primpaths[first].head->next;
	int comp;

	//initializing first 
	Colors[first] = GRAY;

	while (curr->next != NULL)
	{

		//get the computers num
		comp = curr->e.neighbor;

		//check if this computer hasent been touched
		if (Colors[comp] == WHITE)
		{

			//set computer parent
			Parents[comp] = first;
			if (comp == last)
			{
				return;
			}
			else
			{
				//recursion call
				FindPath(comp, last, primpaths, Colors, Parents);
			}
		}

		curr = curr->next;
	}

	//done, all branches were searched
	Colors[first] = BLACK;
}

//A function that builds the computer network
EdgeList* build_net(int n)
{
	int lines_number;
	EdgeList* Net;

	scanf("%d", &lines_number);

	if (lines_number < 0)
	{
		return NULL;
	}

	//Memory allocation and testing
	Net = (EdgeList*)malloc(sizeof(EdgeList) * n);
	check_memory_allocation(Net);

	// bulid empty Net
	for (int j = 0; j < n;j++)
	{
		Net[j] = build_empty_edge_list();
	}

	//fill them with the communications links
	for (int i = 0; i < lines_number; i++)
	{
		int computer1, computer2, price;
		int count = scanf("%d %d %d", &computer1, &computer2, &price);
		if (count < 3)
		{
			printf("Invalid input. ");
			free_edge_list_array(Net, n);
			return NULL;
		}
		if (computer1 < 0 || computer2 < 0 || price < 0 || computer1 >= n || computer2 >= n)
		{
			printf("Invalid input. ");
			free_edge_list_array(Net, n);
			return NULL;
		}
		add_to_sorted_list(&Net[computer1], computer2, price);
		add_to_sorted_list(&Net[computer2], computer1, price);
	}

	return Net;
}

//Turn Prime tree to edgelist
EdgeList* build_paths(int* tree, int n) {
	EdgeList* new_net;

	//If tree is empty return null
	if (n == 0)
	{
		return NULL;
	}

	//Create a new PC net 
	new_net = (EdgeList*)malloc(n * sizeof(EdgeList));
	check_memory_allocation(new_net);

	//Initialise pc's in the new net
	for (int i = 0; i < n; i++)
	{
		new_net[i] = build_empty_edge_list();
	}

	//Add PC's connections to the new net
	for (int i = 0; i < n; i++)
	{
		if (tree[i] != -1)
		{
			add_to_sorted_list(&new_net[i], tree[i], 0);
			add_to_sorted_list(&new_net[tree[i]], i, 0);
		}
	}

	return new_net;
}

//Build the Prim tree
int* build_prim_tree(EdgeList Net[], int n) {
	int* min, * prim;
	CandidateList priority;
	CandidateNode** location;
	CandidateNode* curr;
	int root = 0, u;
	unsigned char* my_inT;

	my_inT = (unsigned char*)calloc((n / 8) + 1, sizeof(unsigned char));
	check_memory_allocation(my_inT);

	//Memory allocation and testing
	min = (int*)malloc(sizeof(int) * n);
	check_memory_allocation(min);

	//Memory allocation and testing
	prim = (int*)malloc(sizeof(int) * n);
	check_memory_allocation(prim);

	//Building the Priority list
	priority = build_empty_candidate_list();

	//Memory allocation and testing
	location = (CandidateNode**)malloc(n * sizeof(CandidateNode*));
	check_memory_allocation(location);

	//initializing the Min, Prim, Priority, Location arrays
	for (int i = 0; i < n; i++) {
		min[i] = INT_MAX;
		prim[i] = -1;

		//creating a candidate for every computer and insert them to the lhe Priority list
		curr = create_candidate_node(i, INT_MAX, priority.tail->prev, priority.tail);
		insert_to_dlist(priority.tail->prev, curr);
		location[i] = curr;
	}

	min[root] = 0;
	prim[root] = -1;

	//Creating the minimal span tree
	while (priority.head->next != priority.tail)
	{
		//Getting the minimal commection cost in Priority 
		u = DeleteMin(priority, location);

		//Edage case: if the father doesnt exist in the Prim
		if (min[u] == INT_MAX)
		{
			printf("No spanning tree available.");
			free(min);
			free(my_inT);
			free(location);
			free_Cand_list(&priority);
			return NULL;
		}
		else
		{
			//getting the bit and bit of the current computer
			int byte_num = u / 8;
			int bit_num = u % 8;

			//creating the mask
			int mask = 1 << bit_num;

			//turning on the wanted bit without affecting the others
			my_inT[byte_num] = my_inT[byte_num] | mask;

			EdgeList u_list = Net[u];
			EdgeNode* node = u_list.head->next;
			int curr_node;

			//Opptimaizing u's connections and potentially adding the to the tree
			while (node != u_list.tail)
			{
				curr_node = node->e.neighbor;

				//checking if the current node isn't in the tree and his connection is the minimal one - connect them 
				if (!(my_inT[curr_node / 8] & (1 << (curr_node % 8))) && node->e.cost < min[curr_node])
				{
					min[curr_node] = node->e.cost;
					prim[curr_node] = u;
					DecreaseKey(location, curr_node, node->e.cost);
				}
				node = node->next;
			}
		}
	}

	//rleasing the meory allocations
	free(min);
	free(my_inT);
	free(location);
	free_Cand_list(&priority);
	return prim;
}

/////////////////////////Help Functions////////////////////////////////////////////////////////////////////////////////

//Build an empty candidate list
CandidateList build_empty_candidate_list() {
	CandidateList newList;

	newList.head = create_candidate_node(-1, INT_MAX, NULL, NULL);
	newList.tail = create_candidate_node(-1, INT_MAX, newList.head, NULL);
	newList.head->next = newList.tail;

	return newList;
}

//Remove the candidate node from list
void delete_cand_node(CandidateNode* del_node)
{
	del_node->next->prev = del_node->prev;
	del_node->prev->next = del_node->next;

	free(del_node);
}

//Find connection off minimal value, remove him from list and return him
int DeleteMin(CandidateList priority, CandidateNode** loc)
{
	CandidateNode* curr = priority.head->next->next;
	CandidateNode* min_can = priority.head->next;
	int num_comp;

	//Find minimal connection
	while (curr != priority.tail)
	{
		//if current lower then min update min
		if (curr->c.min < min_can->c.min)
		{
			min_can = curr;
		}
		curr = curr->next;
	}

	num_comp = min_can->c.computer;

	//removing the computer from location array
	loc[num_comp] = NULL;
	delete_cand_node(min_can);

	return num_comp;
}

//Decrease CandidateNode's min value
void DecreaseKey(CandidateNode** loc, int comp, int new_min)
{
	loc[comp]->c.min = new_min;
}

//Create a new empty candidate node
CandidateNode* create_candidate_node(int comp, int min, CandidateNode* prev, CandidateNode* next) {
	CandidateNode* newNode;
	Candidate newCand;

	//Memory allocation and testing
	newNode = (CandidateNode*)malloc(sizeof(CandidateNode));
	check_memory_allocation(newNode);

	//Initialize candidate data
	newCand.computer = comp;
	newCand.min = min;

	//Set candidate's connections
	newNode->c = newCand;
	newNode->prev = prev;
	newNode->next = next;

	return newNode;
};

//A function that inserts new CandidateNode into a Candidatelist
void insert_to_dlist(CandidateNode* previous, CandidateNode* new_node)
{
	previous->next->prev = new_node;
	previous->next = new_node;

}

//A function that inserts new EdgeNode into a EdgeList
void insert_to_list(EdgeNode* previous, EdgeNode* new_node)
{
	new_node->next = previous->next;
	previous->next = new_node;
}

//A function that adds to the EdgeList in a sorted manner
void add_to_sorted_list(EdgeList* lst, int computer, int price) {
	EdgeNode* prev = lst->head;

	/*We would like to promote the previous one as long as it is smaller than it, the head is
	not coincidentally the tail, and as long as we are not in an empty list.*/
	while (prev->next != NULL && prev->next->e.neighbor != -1 && prev->next->e.neighbor < computer)
	{
		prev = prev->next;
	}

	// Avoid duplicates
	if (prev->next != NULL && prev->next->e.neighbor == computer)
	{
		return;
	}

	EdgeNode* new_node = create_edge_node(computer, price);
	insert_to_list(prev, new_node);
}

//Function that constructs an empty EdgeList
EdgeList build_empty_edge_list()
{
	EdgeList newList;

	//Create an empty edge list with dummy head and tail
	newList.head = create_edge_node(-1, -1);
	newList.tail = create_edge_node(-1, -1);
	newList.head->next = newList.tail;

	return newList;
}

//Function that creates a new EdgeNode
EdgeNode* create_edge_node(int neighbor, int cost)
{
	EdgeNode* newNode;
	Edge newEdge;

	//Memory allocation and testing
	newNode = (EdgeNode*)malloc(sizeof(EdgeNode));
	check_memory_allocation(newNode);

	//Initialize edge data
	newEdge.neighbor = neighbor;
	newEdge.cost = cost;

	newNode->e = newEdge;
	newNode->next = NULL;

	return newNode;
}

//A function that releases the EdgeListArray
void free_edge_list_array(EdgeList* lst, int size)
{
	if (lst != NULL) {
		for (int i = 0; i < size; i++)
			free_edge_list(&lst[i]);
		free(lst);
	}
}

//A function that releases the EdgeList
void free_edge_list(EdgeList* lst) {
	EdgeNode* tempNode = lst->head;

	while (tempNode != NULL) {
		lst->head = lst->head->next;
		free(tempNode);  // Free node
		tempNode = lst->head;
	}

}

//A function that releases the EdgeList
void free_Cand_list(CandidateList* lst) {
	CandidateNode* tempNode = lst->head;

	while (tempNode != NULL) {
		lst->head = lst->head->next;
		free(tempNode);  // Free node
		tempNode = lst->head;
	}

}

//A function that checks memory allocation
void check_memory_allocation(void* ptr)
{
	if (ptr == NULL) {
		printf("Memory allocation failed!\n");
		exit(1);
	}
}