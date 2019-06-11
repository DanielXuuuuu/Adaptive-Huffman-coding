#include "Adpt_Huffman_Tree.h"

Adpt_Huffman_Tree::Adpt_Huffman_Tree() {
	root = new Node(0);//初始结点，就是一个NEW,其权重为0
	newSignal = root;
}
Adpt_Huffman_Tree::~Adpt_Huffman_Tree() {
	destory(root);
}
bool Adpt_Huffman_Tree::compress(string filePath_in, string filePath_out) {
	ifstream fin(filePath_in, ios::in);
	ofstream fout(filePath_out, ios::out | ios::binary);
	if (!fin.is_open() || !fout.is_open()) {
		cout << "error: file is not exist!" << endl;
		return false;
	}

	char c;
	Node* node;
	string huffmanCode;
	int byte;
	Bits2Byte bits;

	long long originBit = 0;
	long long compressBit = 0;
	cout << "Compressing..." << endl;

	while (fin.peek() != EOF) {
		c = fin.get();//读一个字符
		originBit += 8;
		if ((huffmanCode = getCode(c, node)) != "") {
			//cout << c << "已存在，当前编码为: " << huffmanCode << endl;
			bits.addBits(huffmanCode);
			update(c, node); //更新树 
		}
		else {
			//cout << c << "首次出现，设定其编码为原ascii码, 并在之前先编码NEW" << endl;
			bits.addBits(getCode(newSignal));
			bits.addBits(c);
			update(c, NULL);
		}
		if (bits.size() >= 8) {
			byte = bits.generateByte();
			const char* code = new char(byte);
			fout.write(code, sizeof(char));
			compressBit += 8;
			delete code;
		}
	}

	bits.addBits(getCode(newSignal));
	bits.addBits(NEW);

	bits.addZero();
	while (bits.size() != 0) {
		byte = bits.generateByte();
		const char* code = new char(byte);
		fout.write(code, sizeof(char));
		compressBit += 8;
		delete code;
	}
	
	fin.close();
	fout.close();

	cout << "Compress completed! Compression rate: " << (double)originBit / compressBit << endl;
	return true;
}

bool Adpt_Huffman_Tree::extract(string filePath_in, string filePath_out) {
	ifstream fin(filePath_in, ios::in | ios::binary);
	ofstream fout(filePath_out, ios::out);
	if (!fin.is_open() || !fout.is_open()) {
		cout << "error: file is not exist!" << endl;
		return false;
	}

	int bit = 0;
	char c;
	bool firstBit = true, end = false;
	Node* node = root;
	Bits2Byte bits;
	char buffer[1];

	cout << "Extract..." << endl;

	while (fin.peek() != EOF && end != true) {
		fin.read(buffer, sizeof(buffer));
		bits.addBits(buffer[0]);

		//对于第一个0特殊处理
		if (firstBit) {
			bits.readBit();
			firstBit = false;
		}

		while (node->left != NULL && node->left != NULL && end != true) {
			bit = bits.readBit();
			if (bit == -1)
				break;
			if (bit == 0)
				node = node->left;
			else
				node = node->right;
		}
		if (bit == -1)
			continue;

		if (node == newSignal) {
			if (bits.size() < 8) {
				fin.read(buffer, sizeof(buffer));
				bits.addBits(buffer[0]);
			}
			c = (char)bits.generateByte();
			if (c == NEW) {
				end = true;
				continue;
			}
			update(c, NULL);
			fout << c;
		}
		else {
			update(nodeSet[node], node);
			fout << nodeSet[node];
		}
		node = root;
	}

	while (bits.size() != 0 && end != true) {
		while (node->left != NULL && node->left != NULL && end != true) {
			bit = bits.readBit();
			if (bit == -1)
				break;
			if (bit == 0)
				node = node->left;
			else
				node = node->right;
		}

		if (node == newSignal) {
			if (bits.size() < 8) {
				fin.read(buffer, sizeof(buffer));
				bits.addBits(buffer[0]);
			}
			c = (char)bits.generateByte();
			if (c == NEW) {
				end = true;
				continue;
			}
			update(c, NULL);
			fout << c;
		}
		else {
			update(nodeSet[node], node);
			fout << nodeSet[node];
		}
		node = root;
	}

	cout << "Extract completed!" << endl;

	fin.close();
	fout.close();
	return true;
}

void Adpt_Huffman_Tree::destory()
{
	destory(root);
}

void Adpt_Huffman_Tree::reset()
{
	destory(root);
	root = new Node(0);
	newSignal = root;
}

void Adpt_Huffman_Tree::destory(Node* root) {
	if (root != NULL)
		return;

	destory(root->left);
	destory(root->right);
	delete root;
	root = NULL;
}

void Adpt_Huffman_Tree::update(char c, Node* node) {
	if (node == NULL) {
		//新增字母
		Node* cc = new Node(1, newSignal);
		Node* nn = new Node(0, newSignal);
		newSignal->left = nn;
		newSignal->right = cc;
		newSignal = newSignal->left;

		//放入map
		charSet[c] = cc;
		nodeSet[cc] = c;

		addNodeWeight(cc->parent);
	}
	else {
		addNodeWeight(node);
	}
}

void Adpt_Huffman_Tree::addNodeWeight(Node* node) {
	while (node != nullptr) {
		Node* toSwap = findNode(node->weight);
		if (node != toSwap && node->parent != toSwap) {
			swapNode(node, toSwap);
		}
		node->weight++;
		node = node->parent;
	}
}

Node* Adpt_Huffman_Tree::findNode(int weight) {
	Node* temp = NULL;
	queue<Node*> q;
	q.push(root);
	while (!q.empty()) {
		temp = q.front();
		if (temp->weight == weight) {
			return temp;
		}
		q.pop();
		if (temp->right) q.push(temp->right);
		if (temp->left) q.push(temp->left);
	}
	cout << endl;
	return temp;
}

nodeSide Adpt_Huffman_Tree::getNodeSide(Node* node) {
	if (node->parent == NULL)
		return Root;
	else if (node->parent->left == node)
		return LeftChild;
	else
		return RightChild;

}

void Adpt_Huffman_Tree::swapNode(Node* a, Node* b) {
	nodeSide a_side = getNodeSide(a);
	nodeSide b_side = getNodeSide(b);
	if (a_side == LeftChild) {
		if (b_side == LeftChild) {
			a->parent->left = b;
			b->parent->left = a;
		}
		else {
			a->parent->left = b;
			b->parent->right = a;
		}
	}
	else {
		if (b_side == LeftChild) {
			a->parent->right = b;
			b->parent->left = a;
		}
		else {
			a->parent->right = b;
			b->parent->right = a;
		}
	}
	Node* temp = a->parent;
	a->parent = b->parent;
	b->parent = temp;
}
string Adpt_Huffman_Tree::getCode(char c, Node*& node) {
	string huffmanCode = "";
	map<char, Node*>::iterator it;

	if ((it = charSet.find(c)) != charSet.end()) {
		node = it->second;
		Node* temp = it->second;
		stack<char> s;
		nodeSide side = getNodeSide(temp);
		while (side != Root) {
			if (side == LeftChild)
				s.push('0');
			else
				s.push('1');
			temp = temp->parent;
			side = getNodeSide(temp);
		}

		while (!s.empty()) {
			huffmanCode += s.top();
			s.pop();
		}
	}
	return huffmanCode;
}

string Adpt_Huffman_Tree::getCode(Node* node)
{
	//获取NEW的编码
	string huffmanCode = "";
	Node* temp = node;
	stack<char> s;
	nodeSide side = getNodeSide(temp);

	// 第一次编码NEW时，位于根节点，所以要另外考虑
	if (side == Root) {
		return "0";
	}

	while (side != Root) {
		if (side == LeftChild)
			s.push('0');
		else
			s.push('1');
		temp = temp->parent;
		side = getNodeSide(temp);
	}

	while (!s.empty()) {
		huffmanCode += s.top();
		s.pop();
	}
	return huffmanCode;
}
