/*
* Ȩ����ѯ�����㷨(Weighted Round-Robin Scheduling)
* �ڶ�̨����ʵ�ָ��ؾ����ʱ��,���ڵ��ȷ��������.
* ��������������õĴ���������һ�µĻ�,ƽ����ѯ�������ֱ�ӽ������,Ȼ����Щʱ������Ĵ��������ǲ�һ�µ�.
* ������2̨���� A��B , A�Ĵ���������B��2��,��A��Ȩ��Ϊ2,B��Ȩ��Ϊ1.Ȩֵ�ߵķ��������յ������ӣ�
* Ȩֵ�ߵķ�������Ȩֵ�͵ķ����������������ӣ���ͬȨֵ�ķ�����������ͬ��Ŀ����������
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

static vector<srv_info> server;       //��������Ϣ  
static int i = -1;                  //��һ��ѡ��ķ�����  
static int cw = 0;                  //��ǰ���ȵ�Ȩֵ  

int get_gcd();                      //������з�����Ȩֵ�����Լ��  
int get_max_weight();               //������з������е����Ȩֵ  
int get_server(srv_info* s);        //��ѯ����  

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
* �㷨���̣�
* ������һ������� S = {S0, S1, ��, Sn-1} ������Ӧ��Ȩ�أ�����i��ʾ�ϴ�ѡ��ķ�������
* Ȩֵcw��ʼ��Ϊ0��i��ʼ��Ϊ-1 ������һ�ε�ʱ��ȡȨֵȡ�����Ǹ���������
* ͨ��Ȩ�صĲ��ϵݼ���Ѱ���ʺϵķ��������أ�ֱ����ѯ������Ȩֵ����Ϊ0
*/
int get_server(srv_info* s)
{
	static int n = server.size();                       //����������  
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