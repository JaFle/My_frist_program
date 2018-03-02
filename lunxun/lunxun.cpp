/*
* 权重轮询调度算法(Weighted Round-Robin Scheduling)
* 在多台机器实现负载均衡的时候,存在调度分配的问题.
* 如果服务器的配置的处理能力都一致的话,平均轮询分配可以直接解决问题,然而有些时候机器的处理能力是不一致的.
* 假如有2台机器 A和B , A的处理能力是B的2倍,则A的权重为2,B的权重为1.权值高的服务器先收到的连接，
* 权值高的服务器比权值低的服务器处理更多的连接，相同权值的服务器处理相同数目的连接数。
* */

#include <stdio.h>  
#include <stdlib.h>  
#include <time.h>  
#include <string.h>  
#include <vector>  
using namespace std;

#define SERVER_COUNT 10  
#define RAND_WEIGHT 10  
#define BUFFER_SIZE 1024  
#pragma warning(disable:4996)






struct srv_info
{
	srv_info()
	{
		ip = new char[BUFFER_SIZE];
		weight = 0;
	}

	char* ip;
	int weight;
};

static vector<srv_info> server;       //服务器信息  
static int i = -1;                  //上一次选择的服务器  
static int cw = 0;                  //当前调度的权值  

int get_gcd();                      //获得所有服务器权值的最大公约数  
int get_max_weight();               //获得所有服务器中的最大权值  
int get_server(srv_info* s);        //轮询调度  

int main(int argc, char **argv)
{
	srand(time(NULL));

	char tmp[BUFFER_SIZE];
	server.clear();
	for (int i = 0; i < SERVER_COUNT; i++)
	{
		const char* s = "192.168.0.10";

		memset(tmp, '\0', BUFFER_SIZE);
		sprintf(tmp, "%s%d", s, i);

		struct srv_info sinfo;
		memcpy(sinfo.ip, tmp, BUFFER_SIZE);
		sinfo.weight = rand() % RAND_WEIGHT;
		server.push_back(sinfo);
	}

	printf("server count: %ld\n", server.size());

	for (size_t i = 0; i < server.size(); i++)
	{
		printf("%s  weight: %d\n", server[i].ip, server[i].weight);
	}

	printf("====================================\n");

	int used_count[SERVER_COUNT] = { 0 };
	srv_info s;
	for (int i = 0; i < 100; i++)
	{
		int ret = get_server(&s);
		if (ret == -1)
			continue;

		printf("%s  weight: %d\n", s.ip, s.weight);

		int count = used_count[ret];
		used_count[ret] = ++count;

	}

	printf("====================================\n");

	for (int i = 0; i < SERVER_COUNT; i++)
	{
		printf("%s  weight:%d   called %d times\n", server[i].ip, server[i].weight, used_count[i]);
	}



	//return 0;
	system("pause");
}

int get_gcd()
{
	return 1;
}

int get_max_weight()
{
	int max = 0;
	for (size_t i = 0; i < server.size(); i++)
	{
		if (server[i].weight > max)
			max = server[i].weight;
	}

	return max;
}

/**
* 算法流程：
* 假设有一组服务器 S = {S0, S1, …, Sn-1} ，有相应的权重，变量i表示上次选择的服务器，
* 权值cw初始化为0，i初始化为-1 ，当第一次的时候取权值取最大的那个服务器，
* 通过权重的不断递减，寻找适合的服务器返回，直到轮询结束，权值返回为0
*/
int get_server(srv_info* s)
{
	static int n = server.size();                       //服务器个数  
	static int gcd = get_gcd();
	static int max = get_max_weight();

	while (true)
	{
		i = (i + 1) % n;
		if (i == 0)
		{
			cw = cw - gcd;
			if (cw <= 0)
			{
				cw = max;
				if (cw == 0)
					return -1;
			}
		}

		if (server[i].weight >= cw)
		{
			s->weight = server[i].weight;
			memcpy(s->ip, server[i].ip, BUFFER_SIZE);
			return i;
		}

	}


	return -1;
	
}