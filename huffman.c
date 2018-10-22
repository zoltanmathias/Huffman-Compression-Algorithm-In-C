#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
 
typedef struct tree_node{
	int frequency;
	char c;
	struct tree_node *left,*right;
}*node;
 
struct tree_node total[256] = {{0}};
node x[255];
node* q = x - 1;
int num_nodes = 0;
int queue_end = 1;
char *binary[128] = {0};
char buffer[1024];
 
node new(int frequency, char c, node a, node b){
	node n = total + num_nodes++;
	if (frequency) n->c = c, n->frequency = frequency;
	else {
		n->left = a, n->right = b;
		n->frequency = a->frequency + b->frequency;
	}
	return n;
}
 
void queue_insert(node n){
	int j, i = queue_end++;
	while ((j = i / 2)) {
		if (q[j]->frequency <= n->frequency) break;
		q[i] = q[j], i = j;
	}
	q[i] = n;
}
 
node queue_remove(void){
	int i, l;
	node n = q[i = 1];
	if (queue_end < 2) return 0;
	queue_end--;
	while ((l = i * 2) < queue_end) {
		if (l + 1 < queue_end && q[l + 1]->frequency < q[l]->frequency) l++;
		q[i] = q[l], i = l;
	}
	q[i] = q[queue_end];
	return n;
}
 
void construct(node n, char *s, int len){
	static char *out = buffer;
	if (n->c) {
		s[len] = 0;
		strcpy(out, s);
		binary[n->c] = out;
		out += len + 1;
		return;
	}
	s[len] = '0'; 
	construct(n->left,  s, len + 1);
	s[len] = '1'; 
	construct(n->right, s, len + 1);
}
 
void init(const char *s){
	int i, frequency[128] = {0};
	char c[16];
	while (*s) frequency[(int)*s++]++;
	for (i = 0; i < 128; i++)
		if (frequency[i]) queue_insert(new(frequency[i], i, 0, 0));
 
	while (queue_end > 2) 
		queue_insert(new(0, 0, queue_remove(), queue_remove()));
	
	construct(q[1], c, 0);
}
 
int main(void){
	int i;
	const char *input = "zoltan mathias";
    char buffer[1024];
	init(input);
	printf("ASCII message: %s\n", input);
	for (i = 0; i < 128; i++)
		if (binary[i]) printf("Binary representation for '%c': %s\n", i, binary[i]); 
	return 0;
}