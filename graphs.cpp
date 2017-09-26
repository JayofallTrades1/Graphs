/**************************************************************
Suphasith Usdonvudhikai
109219981
ESE 344
Project 4
input file is named: input.txt
delete the last -1 input in the file. Not needed.
***************************************************************/

#include <iostream>
#include <fstream>

using namespace std;

struct Node
{
	int num;
	struct Node* next;
};

class Queue
{
private:
	Node* front;
	Node* rear;
public:
	Queue()
	{
		front = NULL;
		rear = NULL;
	}
	void push(int vertex)
	{
		int data;
		Node* newNode = new Node;
		newNode->num = vertex;
		newNode->next = NULL;
		if (front == NULL)
			front = newNode;
		else
			rear->next = newNode;
		rear = newNode;
	}
	void pop()
	{
		Node* newNode = new Node;
		if (front == NULL)
			return;
		else
		{
			newNode = front;
			front = front->next;
			delete newNode;
		}
	}
	int peek()
	{
		return front->num;
	}
	bool empty()
	{
		if (front == NULL)
			return true;
		else
			return false;
	}
};
struct AdjListNode
{
	int dest;
	float weight;
	struct AdjListNode* next; 
};

struct AdjList
{
	struct AdjListNode *head;
};

class Graph
{
private:
	int V;
	struct AdjList* array;
public:
	Graph(int V)
	{
		this->V = V;
		array = new AdjList[V];
		for (int i = 0; i < V; i++)
		{
			array[i].head = NULL;
		}
	}
	AdjListNode* newAdjListNode(int dest, float weight)
	{
		AdjListNode* newNode = new AdjListNode;
		newNode->dest = dest;
		newNode->weight = weight;
		newNode->next = NULL;
		return newNode;
	}
	void addEdge(int src, int dest, float weight)
	{
		//graphs are undirected so we need 2way pointers
		AdjListNode* newNode = newAdjListNode(dest, weight);
		newNode->next = array[src].head;
		array[src].head = newNode;
		newNode = newAdjListNode(src, weight);
		newNode->next = array[dest].head;
		array[dest].head = newNode;
	}
	void printGraph()
	{
		int v;
		for (v = 0; v < V; v++)
		{
			AdjListNode* iter = array[v].head;
			cout << endl << "Adjacency list of vertex " << v << endl << "Head";
			while (iter)
			{
				cout << "->" << iter->dest;
				iter = iter->next;
			}
			cout << endl;
		}
	}
	void printBFS(int start)
	{
		bool *visited = new bool[V];
		for (int i = 0; i < V; i++)
		{
			visited[i] = false;
		}

		Queue queue;
		queue.push(start);
		cout << endl << "Breadth First Depth Traversal " << endl;
		while (!queue.empty())
		{
			start = queue.peek();
			queue.pop();
			cout << start << "->";
			visited[start] = true;
			AdjListNode* iter = array[start].head;
			while (iter)
			{
				if (!visited[iter->dest])
				{
					visited[iter->dest] = true;
					queue.push(iter->dest);
				}
				iter = iter->next;
			}
		}
	}
	void helperDFS(int start, bool visited[])
	{
		visited[start] = true;
		cout << start << "->";
		AdjListNode* iter = array[start].head;
		while (iter)
		{
			if (!visited[iter->dest])
				helperDFS(iter->dest, visited);
			iter = iter->next;
		}
	}
	void printDFS(int start)
	{
		cout << endl << "Depth First Traversal" << endl;
		bool *visited = new bool[V];
		for (int i = 0; i < V; i++)
			visited[i] = false;
		helperDFS(start, visited);
	}
	struct MinHeapNode
	{
		int  v;
		int dist;
	};
	struct MinHeap
	{
		int size;
		int capacity;
		int *pos;
		struct MinHeapNode **array;
	};
	struct MinHeapNode* newMinHeapNode(int v, int dist)
	{
		struct MinHeapNode* minHeapNode = new MinHeapNode;
		minHeapNode->v = v;
		minHeapNode->dist = dist;
		return minHeapNode;
	};
	struct MinHeap* createMinHeap(int capacity)
	{
		struct MinHeap* minHeap = new MinHeap;
		minHeap->pos = new int;
		minHeap->size = 0;
		minHeap->capacity = capacity;
		minHeap->array = new MinHeapNode*;
		return minHeap;
	}
	void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
	{
		struct MinHeapNode* t = *a;
		*a = *b;
		*b = t;
	}
	void minHeapify(struct MinHeap* minHeap, int idx)
	{
		int smallest, left, right;
		smallest = idx;
		left = 2 * idx + 1;
		right = 2 * idx + 2;

		if (left < minHeap->size &&
			minHeap->array[left]->dist < minHeap->array[smallest]->dist)
			smallest = left;

		if (right < minHeap->size &&
			minHeap->array[right]->dist < minHeap->array[smallest]->dist)
			smallest = right;

		if (smallest != idx)
		{
			MinHeapNode *smallestNode = minHeap->array[smallest];
			MinHeapNode *idxNode = minHeap->array[idx];
			minHeap->pos[smallestNode->v] = idx;
			minHeap->pos[idxNode->v] = smallest;
			swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
			minHeapify(minHeap, smallest);
		}
	}
	int isEmpty(struct MinHeap* minHeap)
	{
		return minHeap->size == 0;
	}
	struct MinHeapNode* extractMin(struct MinHeap* minHeap)
	{
		if (isEmpty(minHeap))
			return NULL;
		struct MinHeapNode* root = minHeap->array[0];
		struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
		minHeap->array[0] = lastNode;
		minHeap->pos[root->v] = minHeap->size - 1;
		minHeap->pos[lastNode->v] = 0;
		--minHeap->size;
		minHeapify(minHeap, 0);
		return root;
	}
	void decreaseKey(struct MinHeap* minHeap, int v, int dist)
	{
		int i = minHeap->pos[v];
		minHeap->array[i]->dist = dist;
		while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
		{
			minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
			minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
			swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
			i = (i - 1) / 2;
		}
	}

	bool isInMinHeap(struct MinHeap *minHeap, int v)
	{
		if (minHeap->pos[v] < minHeap->size)
			return true;
		return false;
	}
	void printArr(int dist[], int n)
	{
		cout << endl << "Vertex   Distance from Source" << endl;;
		for (int i = 0; i < n; ++i)
			cout << i << "           " << dist[i] << endl;
	}
	void dijkstra(int src)
	{
		int *dist = new int[V];
		struct MinHeap* minHeap = createMinHeap(V);
		for (int v = 0; v < V; ++v)
		{
			dist[v] = INT_MAX;
			minHeap->array[v] = newMinHeapNode(v, dist[v]);
			minHeap->pos[v] = v;
		}
		minHeap->array[src] = newMinHeapNode(src, dist[src]);
		minHeap->pos[src] = src;
		dist[src] = 0;
		decreaseKey(minHeap, src, dist[src]);
		minHeap->size = V;
		while (!isEmpty(minHeap))
		{
			struct MinHeapNode* minHeapNode = extractMin(minHeap);
			int u = minHeapNode->v;
			struct AdjListNode* iter = array[u].head;
			while (iter != NULL)
			{
				int v = iter->dest;
				if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
					iter->weight + dist[u] < dist[v])
				{
					dist[v] = dist[u] + iter->weight;
					decreaseKey(minHeap, v, dist[v]);
				}
				iter = iter->next;
			}
		}
		printArr(dist, V);
	}
};
	int main(void)
	{
		int N;
		int src, dest;
		float weight;
		ifstream infile;
		infile.open("input.txt");


		if (!infile)
		{
			cout << "Cannot open input file: inputfile.txt" << endl;
			return 1;
		}

		infile >> N; //number of vertices
		Graph graph(N);
		while (infile >> src >> dest >> weight)
		{
			graph.addEdge(src, dest, weight);
		}
		graph.printGraph();
		graph.printDFS(0);
		cout << endl;
		graph.printBFS(0); //start from vertex 0
		cout << endl;
		graph.dijkstra(0);
		int x;
		cin >> x;
		return 0;
	}