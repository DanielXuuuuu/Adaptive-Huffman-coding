//encode using Adaptive Huffman Coding
#include "Adpt_Huffman_Tree.h"

void menu() {
	cout << "--------------------Adaptive Huffman Coding-------------------\n" << endl;
	cout << "                       1¡¢Compress" << endl;
	cout << "                       2¡¢Extract" << endl;
	cout << "                       3¡¢Quit\n"<< endl;
	cout << "--------------------------------------------------------------\n" << endl;
	cout << "Enter your choice: ";
}


int main() {
	Adpt_Huffman_Tree huffman;

	menu();
	int choice;
	string filePath_in, filePath_out;
	cin >> choice;
	while (1) {
		switch (choice) {
		case 1:
			cout << "Please enter the filename that you want to compress: ";
			cin >> filePath_in;
			cout << "Please enter the filename after compress: ";
			cin >> filePath_out;
			huffman.compress(filePath_in, filePath_out);
			huffman.reset();
			cout << "\nEnter your choice: ";
			break;
		case 2:
			cout << "Please enter the filename that you want to extract: ";
			cin >> filePath_in;
			cout << "Please enter the filename after extract: ";
			cin >> filePath_out;
			huffman.extract(filePath_in, filePath_out);
			huffman.reset();
			cout << "\nEnter your choice: ";
			break;
		case 3:
			cout << "Bye!" << endl;
			break;
		default:
			cout << "wrong choice! Try again£º";
		}
		if (choice == 3)
			break;
		cin >> choice;
	}
	return 0;
}



