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
	cout << endl << "词法分析:" << endl;
	exchang(temp, p);
	cout << endl << "语法分析:" << endl;
	read_the_text3();
	//get_first();//first集
	printclosure();//closure
	analysis();//
	apart();
	cout << endl;

	system("pause");

	return 0;
}
