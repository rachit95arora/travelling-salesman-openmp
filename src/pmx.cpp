#include "pmx.h"
using namespace std;
//Partially Mapped Crossover
void pmx( vector<int> &a, vector<int> &b)
{
	int l = a.size();
	int left = rand()%l, right = rand()%l;
	if(left > right){
		int tmp = left; left = right; right = tmp;
	}
	bool done0,done1;
	vector<int> c = a, d = b;
	int tmp;
	for(int i = left;i<=right;i++)
	{
		done0 = done1 = false;
		for(int j = 0; j<l; j++)
		{
			if((done0 = (c[j] == b[i]))) c[j] = c[i]; //Mapping selected region
			if((done1 = (d[j] == a[i]))) d[j] = d[i];
			if(done0 && done1) break;
		}
	}
	for(int i = 0 ; i<l ; i++)
	{
		if((i>=left) && (i<=right)) {
			tmp = a[i];
			a[i] = b[i];
			b[i] = tmp;
		}else{
			a[i] = c[i];
			b[i] = d[i];
		}
	}

}