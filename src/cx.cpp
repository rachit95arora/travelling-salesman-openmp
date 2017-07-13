#include "cx.h"
using namespace std;
//Cyclic Crossover : Too bad
void cx( vector<int> &a, vector<int> &b)
{
	int l = a.size();
	int cycle = true;
	vector<int> c = a, d = b;
	vector<bool> cflags(l, false), dflags(l,false);
	while(true)
	{
		// cout<<"Looping\n";
		int start = l;
		for(int i = 0; i<l; i++)
		{
			if(!cflags[i]) {start = i; break;}
		}
		// cout<<(int)start<<endl;
		if(start == l)break;
		int x = start;
		do
		{
			if(cycle)
			{
				a[x] = c[x];
				b[x] = d[x];
			}
			else
			{
				a[x] = d[x];
				b[x] = c[x];
			}
			cflags[x] = dflags[x] = true;
			for(int i = 0; i<l ; i++)
			{
				if(c[i] == d[x])
				{
					x = i;
					break;
				}
			}
		}
		while(x!=start);
		cycle = !cycle;
	}
}