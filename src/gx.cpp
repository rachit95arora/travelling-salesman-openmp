#include "gx.h"
using namespace std;
//Greedy Crossover
void gx (vector<int> &a, vector<int> &b, vector< vector<float> >&dist)
{
	int l = a.size(),count = l;
	vector<int> c = a, d = b;
	vector<bool> aflags(l,false);
	a[0] = (rand()%2)?c[0]:d[0];
	int s = a[0], i = 1;
	aflags[s] = true;
	count--;
	while(i<l)
	{
		int cs=-1,ds=-1;
		for(int j = 0; j<l ; j++)
		{
			cs = (c[j]==s)?j:cs;  //Locating current chromosome element in c and d
			ds = (d[j]==s)?j:ds;
			if((cs>=0) &&(ds>=0))break;
		}
		if(aflags[c[(cs+1)%l]])
		{
			if(aflags[d[(ds+1)%l]])
			{
				//Selecting a random neighbour
				int sel = rand()%count;
				for(int ik = 0 ; ik<l ; ik++)
				{
					if(aflags[ik])continue;
					if(sel==0)
					{
						a[i] = s = ik;
						aflags[ik] = true;
						count--;
						i++;
						break;
					}
					sel--;
				}
			}
			else
			{
				a[i] = s = d[(ds+1)%l];
				i++;
				aflags[s] = true;
				count--;
			}
		}
		else
		{
			if(aflags[d[(ds+1)%l]])
			{
				a[i] = s = c[(cs+1)%l];
				i++;
				aflags[s] = true;
				count--;
			}
			else // Greedy select one of the neighbours along c,d
			{
				a[i] = ((dist[a[i-1]][c[(cs+1)%l]]) <(dist[a[i-1]][d[(ds+1)%l]]))? c[(cs+1)%l] : d[(ds+1)%l];
				s = a[i];
				i++;
				aflags[s] = true;count--;
			}
		}

	}
	//Evaluate a similar crossover with reversed original chromosomes
	count = l;
	reverse(c.begin(), c.end());
	reverse(d.begin(), d.end());
	vector<bool> bflags(l,false);
	b[0] = (rand()%2)?c[0]:d[0];
	s = b[0], i = 1;
	bflags[s] = true;
	count--;
	while(i<l)
	{
		int cs=-1,ds=-1;
		for(int j = 0; j<l ; j++)
		{
			cs = (c[j]==s)?j:cs;
			ds = (d[j]==s)?j:ds;
			if((cs>=0) &&(ds>=0))break;
		}
		if(bflags[c[(cs+1)%l]])
		{
			if(bflags[d[(ds+1)%l]])
			{
				int sel = rand()%count;
				for(int ik = 0 ; ik<l ; ik++)
				{
					if(bflags[ik])continue;
					if(sel==0)
					{
						b[i] = s = ik;
						i++;
						bflags[ik] = true;
						count--;
						break;
					}
					sel--;
				}
			}
			else
			{
				b[i] = s = d[(ds+1)%l];
				i++;
				bflags[s] = true;count--;
			}
		}
		else
		{
			if(bflags[d[(ds+1)%l]])
			{
				b[i] = s = c[(cs+1)%l];
				i++;
				bflags[s] = true;count--;
			}
			else
			{
				b[i] = ((dist[b[i-1]][c[(cs+1)%l]]) <(dist[b[i-1]][d[(ds+1)%l]]))?c[(cs+1)%l]:d[(ds+1)%l];
				s = b[i];
				i++;
				bflags[s] = true;count--;
			}
		}
	}
	
}