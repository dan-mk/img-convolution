#include <cstdio>

int main(){
	int s;
	float m;
	int sum = 0;
	int a;
	scanf("%d %f", &s, &m); s *= s;
	while(s--){
		scanf("%d", &a);
		sum += a;
	}
	printf("%f\n", 1.0 / sum);
	return 0;
}
