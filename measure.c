#include <stdio.h>
#include <stdlib.h>
int get_cpu_usage(void) {
	FILE *pp;
	pp = popen("top -b -n 1 | tail -n +8 | awk '{ sum += $9 } END {print sum }'", "r");
	if (pp != NULL) {
		char *line;
		char buf[1000];
		line = fgets(buf,sizeof buf, pp);
		if(line == NULL){
			return -1;
		}
		int cpu_usage = atoi(line);
		return cpu_usage;
		
	}	
	return -1;
}

