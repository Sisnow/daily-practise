#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

const int SIZE = 100;
string arr = "ABCDEGH";
string F[SIZE][2];
int n;

//�����Լ��ıհ�
string compute_closure(string src, int n) {
	string tmp = src;
	string res = tmp;
	while (1)
	{
		for (int i = 0; i < n; i++) {
			string a = F[i][0];
			string b = F[i][1];
			int if_belong = 1;
			for (int j = 0; j < a.size(); j++) {
				if (tmp.find(a[j]) == -1) {
					if_belong = 0;
					break;
				}
			}
			if (if_belong) {
				for (int j = 0; j < b.size(); j++) {
					if (tmp.find(b[j]) == -1)
						tmp += b[j];
				}
			}
		}
		if (res == tmp)
			break;
		else
			res = tmp;
	}
	sort(res.begin(), res.end());
	return res;
}

//�����г���
void print_superkey(string s, string prefix = "") {

	if (s.empty()) {
		if(compute_closure(prefix, n) == arr)
			cout << prefix << endl;
		return;
	}

	for (int i = 0; i < s.size(); i++) {
		prefix += s[i];
		print_superkey(s.substr(i + 1), prefix);
		prefix.pop_back();
	}
}

int main() {
	n = 0;
	cout << "�������ϵ��,0 0����" << endl;
	while (1) {
		string a, b;
		cin >> a >> b;
		if (a == "0" && b == "0")
			break;
		F[n][0] = a;
		F[n][1] = b;
		n++;
	}
	print_superkey(arr);
	return 0;
}