#ifndef ADPT_HUFFMAN_TREE
#define ADPT_HUFFMAN_TREE

#include <fstream>
#include <iostream>
#include <queue>
#include <map>
#include <stack>
#include <string>
#include "Bits2Byte.h"
using namespace std;

#define NEW 0x00
enum nodeSide { Root, LeftChild, RightChild };

struct Node
{
	int weight;
	Node* parent, *left, *right;

	Node(int c, Node* p = NULL, Node* l = NULL, Node* r = NULL)
		:weight(c), parent(p), left(l), right(r)
	{}
};


class Adpt_Huffman_Tree {
public:
	Adpt_Huffman_Tree();
	~Adpt_Huffman_Tree();
	bool compress(string filePath_in, string filePath_out);
	bool extract(string filePath, string filePath_out);
	void destory();
	void reset();
private:
	Node* root;
	Node* newSignal;

	map<char, Node*> charSet; //��¼ÿ���ַ���Ӧ�Ľ��
	map<Node*, char> nodeSet; //��¼�ڵ��Ӧ���ַ�
	void update(char c, Node* node);
	void addNodeWeight(Node* node);//�ݹ����ӣ�update����ͨ�������������ʵ��
	Node* findNode(int weight); //Ѱ��ͬweight��index����
	nodeSide getNodeSide(Node* node);
	void swapNode(Node* a, Node* b);
	string getCode(char c, Node*& node);
	string getCode(Node* node);

	void destory(Node* root);
};

#endif