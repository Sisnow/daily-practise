#include <iostream>
#include <cstdio>
using namespace std;

int main() {
	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		string s1, s2, s;
		cin >> s1 >> s2 >> s;
		int res = 0;
		if (s.find(s1) != -1 && s.find(s2) != -1)
		{
			int res1 = (s.rfind(s2) - 1) - (s.find(s1) + s1.length()) + 1;
			int res2 = (s.rfind(s1) - 1) - (s.find(s2) + s2.length()) + 1;
			res = res1 > res2 ? res1 : res2;
			if (res < 0) res = 0;
		}
		printf("case #%d:\n", i);
		printf("%d\n", res);
	}
	return 0;
}