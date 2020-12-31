#include <stdio.h>
#define MAX_VERTEX 10
#define MAX_ELEMENT 100 //heap의 최대크기

int parent[MAX_VERTEX]; //vertex의 부모 노드
int num[MAX_VERTEX]; //집합의 크기


typedef struct {
	int v; //정점 1
	int u; //정점 2
	int key; // 간선의 가중치
}Element;


typedef struct {
	Element heap[MAX_ELEMENT];
	int heap_size;
}HeapType; //heap의 data type


//heap을 초기화하는 함수
void InitHeap(HeapType* h) {
	h->heap_size = 0;
}


//heap에 아까 선언한 Element를 삽입하는 함수
void InsertHeap(HeapType* h, Element item) {
	int i;
	i = ++(h->heap_size);

	//트리를 거슬러 올라가며 부모노드와 비교
	while (i != 1 && item.key < h->heap[i / 2].key) { //i가 root가 아니고, 입력받은 item의 key 값이 i의 부모 key 값보다 작으면 거슬러 올라가야 함
		h->heap[i] = h->heap[i / 2]; //현재노드를 부모노드로 설정
		i /= 2; //index를 부모노드의 index로 설정
	}

	h->heap[i] = item; //더 이상 거슬러 올라갈 곳이 없으므로 현재 index의 위치에 item 삽입
}


//heap에서 삽입한 Element를 삭제하는 함수
Element DeleteHeap(HeapType* h) {
	int parent = 1; //첫 실행의 부모노드를 루트로
	int child = 2; //그 왼쪽 자식을 자식노드로
	Element data, temp;

	data = h->heap[parent]; //반환할 루트노드의 값 data에 저장
	temp = h->heap[(h->heap_size)--]; // 마지막 노드의 값 temp에 저장하고, heap size 1 감소

	while (child <= h->heap_size) { //child가 힙 사이즈보다 작거나 같을 때 비교

		//자식 노드간 작은 수 비교 
		if ((child < h->heap_size) && (h->heap[child].key) > h->heap[child + 1].key)
			child++;

		if (temp.key <= h->heap[child].key) //루트노드인 temp가 더 작다면 위치이동 할 필요 없음
			break; //반복문 빠져나감

		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2; // 다음 자식 노드와 비교
	}

	h->heap[parent] = temp;
	return data;
}


//집합을 초기화하는 함수
void InitUF(void) {
	int i;
	for (i = 0; i < MAX_VERTEX; i++) {
		parent[i] = -1; //부모노드 배열 -1로 초기화
		num[i] = 1; //집합 크기 배열 1로 초기화
	}
}


// vertex가 속하는 집합을 반환하는 함수
int SetFind(int v) {
	int i, p, temp;

	for (i = v; (p = parent[i]) >= 0; i = p); //루트노드까지 반복
	temp = i; // 루트노드의 정보를 temp에 저장

	for (i = v; (p = parent[i]) >= 0; i = p)
		parent[i] = temp; // 집합의 모든 원소들의 부모를 루트노드로 설정

	return temp; //루트 반환
}


//두개의 원소가 속한 집합을 합치는 함수
void SetUnion(int v1, int v2) {

	//자식의 갯수로 비교
	if (num[v1] < num[v2]) { //v2가 더 큰 경우
		parent[v1] = v2; //v1의 부모를 v2로 설정
		num[v1] += num[v2]; //v2의 크기를 v1만큼 더해줌
	}

	else {
		parent[v2] = v1;
		num[v2] += num[v1];
	}
}


//heap에 edge를 삽입하는 함수
void InsertHeapEdge(HeapType* h, int v, int u, int weight) {
	Element node;
	node.v = v;
	node.u = u;
	node.key = weight;

	InsertHeap(h, node);
}


//kruskal algorithm으로 Minimum Spanning Tree 구하는 함수
int kruskal(int n) {
	Element e; //heap의 요소
	HeapType h; //최소 heap
	int uSet, vSet; //vertex u와 vertex v의 집합 번호
	int edgeCount = 0, sum = 0;

	InitHeap(&h); //heap 초기화
	InitUF(); //집합 초기화

	//heap에 모든 edge 삽입
	InsertHeapEdge(&h, 1, 2, 32);
	InsertHeapEdge(&h, 1, 4, 17);
	InsertHeapEdge(&h, 2, 5, 45);
	InsertHeapEdge(&h, 4, 5, 10);
	InsertHeapEdge(&h, 3, 4, 18);
	InsertHeapEdge(&h, 5, 6, 28);
	InsertHeapEdge(&h, 3, 7, 5);
	InsertHeapEdge(&h, 7, 8, 59);
	InsertHeapEdge(&h, 4, 8, 3);
	InsertHeapEdge(&h, 8, 9, 4);
	InsertHeapEdge(&h, 5, 9, 25);
	InsertHeapEdge(&h, 9, 10, 12);
	InsertHeapEdge(&h, 6, 10, 6);

	while (edgeCount < n - 1) { //간선의 갯수가 n-1이 될 때 까지
		e = DeleteHeap(&h); //weight가 가장 작은 간선 획득
		uSet = SetFind(e.u); //vertex u의 집합 번호
		vSet = SetFind(e.v); //vertex v의 집합 번호

		if (uSet != vSet) { //두 집합이 서로 다른 집합에 속하는 경우
			printf("edge = (v%d-v%d), weight = %d\n", e.u, e.v, e.key);
			sum += e.key;
			edgeCount++; //간선을 하나 늘림
			SetUnion(uSet, vSet); //두 집합을 합침
		}
	}

	return 0;
}


int main() {
	printf("Minimum Spanning Tree\n");
	printf("(MST를 만들며 선택되는 edge를 순서대로 표시)\n");
	printf("\n");
	kruskal(13); // 현재 edge의 갯수 13개
}