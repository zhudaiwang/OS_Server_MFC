#include "stdafx.h"
//单向队列 queue支持 empty() size() front() back() push() pop()  
//By MoreWindows(http://blog.csdn.net/MoreWindows)  
#include <queue>  
#include <vector>  
#include <list>  
#include <cstdio>  
using namespace std;  

int main()  
{  
	//可以使用list作为单向队列的容器，默认是使用deque的。  
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



	//压入数据  
	for (i = 0; i < 10; i++)  
	{  
		a.push(i);  
		b.push(i);  
	}  

	//单向队列的大小  
	printf("%d %d\n", a.size(), b.size());  

	//队列头和队列尾  
	printf("%d %d\n", a.front(), a.back());  
	printf("%d %d\n", b.front(), b.back());  

	//取单向队列项数据并将数据移出单向队列  
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