#include <dirent.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define VPD_TREE "/proc/device-tree/vpd"
#define NEST_DIR "/proc/device-tree/nest-counters/"
#define DIMM "dimm"
#define CHIP "chip@"
#define MAX_DIMM_RATE "max-dimm-rate"
#define PPC64LE "ppc64le"

int find_total_dimms(void)
{
    int nr_dimms = 0;
    DIR *d;
    struct dirent *dir;

    d = opendir(VPD_TREE);
    if (d) {
	while ((dir = readdir(d)) != NULL)
	    if (!strncmp(DIMM, dir->d_name, strlen(DIMM)))
		nr_dimms++;

	closedir(d);
    }

    return nr_dimms;
}

long int find_max_bw_per_dimm(void)
{
    char file[PATH_MAX];
    DIR *d;
    struct dirent *dir;
    int ret;
    struct utsname name;
    FILE *fp;
    unsigned swapped, value;
    struct stat st;

    memset(file, '\0', PATH_MAX);
    d = opendir(NEST_DIR);
    if (d) {
	while ((dir = readdir(d)) != NULL) {
	    if (!strncmp(dir->d_name, CHIP, strlen(CHIP))) {
		    sprintf(file, "%s%s%c%s", NEST_DIR, dir->d_name, '/',
			    MAX_DIMM_RATE);
		}
	}
    }

    closedir(d);

    if ('\0' == file[0]) {
	return -1;
    }

    ret = stat(file, &st);
    if (ret < 0) {
	fprintf(stderr, "Could not stat : %s", file);
	return -1;
    }

    fp = fopen(file, "rb");
    if (fp == NULL) {
	fprintf(stderr, "Could not open : %s\n", file);
	return -1;
    }

    /* Read 4 bytes */
    ret = fread(&value, 4, 1, fp);
    if (ret == 0) {
	fprintf(stderr, "Error in reading %s\n", file);
	return -1;
    }

    ret = uname(&name);
    if (ret < 0) {
	fprintf(stderr, "uname failed, ret : %d\n", ret);
	return -1;
    }
    if (strncmp(name.machine, PPC64LE, strlen(PPC64LE)))
	return (long)value;

    /* Swapping only for ppc64 LE machines */
    value >>= 16;

    swapped = ((value>>24)&0xff) | ((value<<8)&0xff0000) | ((value>>8)&0xff00) |
	((value<<24)&0xff000000);
    swapped >>= 16;

    fclose(fp);
    return (long)swapped;
}

int max_bandwidth_per_node(int nodes, void *bw)
{
    int nr_dimms;
    long int dimm_max_bw;
    double *value = bw;

    nr_dimms = find_total_dimms();
    if (nr_dimms < 0)
	return -1;

    dimm_max_bw = find_max_bw_per_dimm();
    if (dimm_max_bw < 0)
	return -1;

    *value = (double)((double)(nr_dimms * dimm_max_bw) / (double)nodes);
    return 0;
}
