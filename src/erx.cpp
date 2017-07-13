#include "erx.h"
#include <algorithm>
using namespace std;
//Edge Recombination Crossover Function
void erx( vector<int> &a, vector<int> &b)
{
	int l = a.size();
	vector<bool> aflags(l,false),bflags(l,false);
	vector< vector<int> > edgeList(l, vector<int> (4,l));
	//Initiating edge list
	for(int i = 0; i<l ;i++)
	{
		edgeList[a[i]][0] = a[((l+i-1)%l)];
		edgeList[a[i]][1] = a[((i+1)%l)];
		for(int j = 0; j<l ; j++)
		{
			if(b[j]!=a[i])continue;
			int f = 0;
			if((b[((l+j-1)%l)]!=a[((l+i-1)%l)]) &&(b[((l+j-1)%l)]!=a[((i+1)%l)] ))
			{
				edgeList[b[j]][2] =  b[((l+j-1)%l)];
				f=1;
			}
			if((b[((j+1)%l)]!=a[((l+i-1)%l)]) &&(b[((j+1)%l)]!=a[((i+1)%l)] ))
			{
				edgeList[b[j]][2+f] =  b[((j+1)%l)];
			}
			break;
		}
	}
	//Generating first crossover
	int x = b[0],x1 = a[0],i = 0 ,count = l;
	while(i<l)
	{
		a[i] = x;
		aflags[x] = true;
		count--;
		if(i == l-1) break;
		int next = l, max = 4,maxinst = 0;
		for(int j = 0; j<4;j++)
		{
			int tmp = edgeList[x][j],c=0;
			if(tmp>=l)continue;
			for(int k = 0; k<4;k++)
			{
				if((edgeList[tmp][k]<l) && !aflags[edgeList[tmp][k]])c++;
			}
			if(!aflags[tmp] && (c<max))
			{
				next = tmp;
				max = c;
				maxinst=1;
			}
			else if(!aflags[tmp] && (c==max))
			{
				maxinst++;
				if((rand()%maxinst) ==(maxinst-1))
				{
					next = tmp;
				}
			}
		}
		if(next <l)
		{
			x = next; i++;
		}
		else
		{
			next = rand()%count;
			for(int f = 0; f<l ; f++)
			{
				if(!aflags[f])
				{
					if(next>0)next--;
					else
					{
						x = f;
						break;
					}
				}
			}
			i++;
		}
	}
	//Generating second crossover
	x = x1,i = 0 ,count = l;
	while(i<l)
	{
		b[i] = x;
		bflags[x] = true;
		count--;
		if(i == l-1) break;
		int next = l, max = 4,maxinst = 0;
		for(int j = 0; j<4;j++)
		{
			int tmp = edgeList[x][j],c=0;
			if(tmp>=l)continue;
			for(int k = 0; k<4;k++)
			{
				if((edgeList[tmp][k]<l) && !bflags[edgeList[tmp][k]])c++;
			}
			if(!bflags[tmp] && (c<max))
			{
				next = tmp;
				max = c;
				maxinst=1;
			}
			else if(!bflags[tmp] && (c==max))
			{
				maxinst++;
				if((rand()%maxinst) ==(maxinst-1))
				{
					next = tmp;
				}
			}
		}
		if(next <l)
		{
			x = next; i++;
		}
		else //Random selection
		{
			next = rand()%count;
			for(int f = 0; f<l ; f++)
			{
				if(!bflags[f])
				{
					if(next>0)next--;
					else
					{
						x = f;
						break;
					}
				}
			}
			i++;
		}
	}
}