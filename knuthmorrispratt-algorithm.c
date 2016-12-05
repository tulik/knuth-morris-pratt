#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int kmp(char *target, int tsize, const char *pattern, int psize);
int *compute_prefix_function(const char *pattern, int psize);

int main(int argc, const char *argv[])
{
	struct timespec tsp;
	int timer = (int) tsp.tv_sec;
	int i;
	char *buffer = 0;
	long length;
	FILE *file = fopen (argv[1], "rb");
  fseek (file, 0, SEEK_END);
  length = ftell (file);
  fseek (file, 0, SEEK_SET);
  buffer = malloc (length);
  fread (buffer, 1, length, file);
  fclose (file);

	char *target = buffer;
	char *ch = target;
	const char *pattern = argv[2];
	i = kmp(target, strlen(target), pattern, strlen(pattern));
	if (i >= 0)
		printf("matched @: %.200s\n", ch + i);

	clock_gettime(CLOCK_REALTIME, &tsp);
	fprintf(stdout, "\nTotal execution time: %d nsec\n", (int) tsp.tv_nsec - timer);
	return 0;
}

int *compute_prefix_function(const char *pattern, int psize)
{
	int k = -1;
	int i = 1;
	int *pi = malloc(sizeof(int)*psize);
	if (!pi)
		return NULL;

	pi[0] = k;
	for (i = 1; i < psize; i++) {
		while (k > -1 && pattern[k+1] != pattern[i])
			k = pi[k];
		if (pattern[i] == pattern[k+1])
			k++;
		pi[i] = k;
	}
	return pi;
}

int kmp(char *target, int tsize, const char *pattern, int psize)
{
	int i;
	int *pi = compute_prefix_function(pattern, psize);
	int k = -1;
	if (!pi)
		return -1;
	for (i = 0; i < tsize; i++) {
		while (k > -1 && pattern[k+1] != target[i])
			k = pi[k];
		if (target[i] == pattern[k+1])
			k++;
		if (k == psize - 1) {
			free(pi);
			return i-k;
		}
	}
	free(pi);
	return -1;
}
