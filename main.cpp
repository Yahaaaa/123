#include "main.h"
#include <iostream>
#include <stdio.h>
using namespace std;

int main() {
	line_e temp;
	temp.len = 0;
	FILE *fp;
	fp = fopen("process.txt", "w");
	fclose(fp);
	int p;
	read_the_text(temp);
	temp.len = 0;
	read_the_text2(temp);
	cout << endl << "�ʷ�����:" << endl;
	exchang(temp, p);
	cout << endl << "�﷨����:" << endl;
	read_the_text3();
	//get_first();//first��
	printclosure();//closure
	analysis();//
	apart();
	cout << endl;

	system("pause");

	return 0;
}
