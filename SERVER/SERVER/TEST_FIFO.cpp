#include "stdafx.h"
//������� queue֧�� empty() size() front() back() push() pop()  
//By MoreWindows(http://blog.csdn.net/MoreWindows)  
#include <queue>  
#include <vector>  
#include <list>  
#include <cstdio>  
using namespace std;  

int main()  
{  
	//����ʹ��list��Ϊ������е�������Ĭ����ʹ��deque�ġ�  
	queue<int, list<int>> a;  
	queue<unsigned int>        b;  

	queue<char*> pcFIFO;

	pcFIFO.push("A");
	pcFIFO.push("B");
	pcFIFO.push("C");
	pcFIFO.push("D");
	pcFIFO.push("E");
	pcFIFO.push("F");

	pcFIFO.push("F");

	printf("%x %x\n", &pcFIFO.front(), &pcFIFO.back());  
	printf("%s %s\n\n", pcFIFO.front(), pcFIFO.back()); 

	pcFIFO.pop();
	printf("Poped\n"); 


	printf("%x %x\n", &pcFIFO.front(), &pcFIFO.back());  
	printf("%s %s\n", pcFIFO.front(), pcFIFO.back()); 
	
	pcFIFO.pop();
	printf("Poped\n"); 

	printf("%x %x\n", &pcFIFO.front(), &pcFIFO.back());  
	printf("%s %s\n", pcFIFO.front(), pcFIFO.back()); 

	pcFIFO.pop();
	printf("Poped\n"); 



#if 0

	int i;  



	//ѹ������  
	for (i = 0; i < 10; i++)  
	{  
		a.push(i);  
		b.push(i);  
	}  

	//������еĴ�С  
	printf("%d %d\n", a.size(), b.size());  

	//����ͷ�Ͷ���β  
	printf("%d %d\n", a.front(), a.back());  
	printf("%d %d\n", b.front(), b.back());  

	//ȡ������������ݲ��������Ƴ��������  
	while (!a.empty())  
	{  
		printf("%d ", a.front());  
		a.pop();
	}  
	putchar('\n');  

	while (!b.empty())  
	{  
		printf("%d ", b.front());  
		b.pop();  
	}  
	putchar('\n'); 


#endif

	return 0;  
}  