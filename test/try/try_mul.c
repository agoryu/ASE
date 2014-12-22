#include "try.h"

static struct ctx_s ctx;

static int mul(int depth) {
	int i;
	switch(scanf("%d", &i)) {
		case EOF:
			return 1;
		case 0:
			return mul(depth+1);
			/*return 1;*/
		case 1:
			if(i) {
				return i * mul(depth+1);
			} else {
				printf("passage dans le throw\n");
				return throw(&ctx, i);
			}
	}
	return -1;
}

int
main(int argc, char** argv)
{
	int product = 0;

	printf("a list of int, please\n");
	product = try(&ctx, *mul, 0);
	printf("product = %d\n", product);
	return EXIT_SUCCESS;
}
