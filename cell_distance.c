#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BUFFER_SIZE 1e5
#define MAX_DIST 34.64

typedef struct{
	short x, y, z;
}Point;

//set string value to one coord for a point
static inline short atop(char *str){
	short d;
	d = 1e4*(str[1] - '0');
	d += 1e3*(str[2] - '0');
	d += 1e2*(str[4] - '0');
	d += 1e1*(str[5] - '0');
	d += str[6] - '0';
	if(str[0]=='+'){
		return d;
	}
	return -d;
}

static inline float to_float(short d){
	return d/1000.0;
}


//input_file: the file to be read from, buffer: point buffer to hold the result
static inline void read_file(FILE *input_file, Point *buffer){
	static char result[9];
	size_t elements_read = 0;
	while(fread(result, 1, 8, input_file) == 8){
		buffer[elements_read].x = atop(result);

		fread(result, 1, 8, input_file);
		buffer[elements_read].y = atop(result);

		fread(result, 1, 8, input_file);
		buffer[elements_read].z = atop(result);
		++elements_read;
	}
}

static inline float point_dist(Point p1, Point p2){
	return sqrt(
			(to_float(p1.x)-to_float(p2.x))*(to_float(p1.x)-to_float(p2.x)) +
			(to_float(p1.y)-to_float(p2.y))*(to_float(p1.y)-to_float(p2.y)) +
			(to_float(p1.z)-to_float(p2.z))*(to_float(p1.z)-to_float(p2.z))
		);
}


int main(){
	FILE *fp;
	fp = fopen("input_files/cell_e1","r");
	Point *buffer = (Point*) malloc(BUFFER_SIZE*sizeof(Point));
	float *output = (float*) malloc(sizeof(float)*1e5*(1e5-1)/2);
	read_file(fp, buffer);
	//this needs to be thread safe
	unsigned int output_occurance[3465];
	for(int i = 0; i < 3465; ++i)
		output_occurance[i] = 0;

	int k = 0;
	for(int i = 0; i < 1e1; ++i){
		for(int j = 1e1-1; j > i; --j){
			output_occurance[(size_t)(point_dist(buffer[i], buffer[j])/0.01 +1)] += 1;
		}
	}
	char out_string[1000000];
	char temp[100];
	for(int i = 0; i < 3465; ++i){
		if(output_occurance[i]){
			sprintf(temp, "%.2f %d\n", MAX_DIST*i/3464.0, output_occurance[i]);
			strcat(out_string, temp);
			memset(temp, 0, 100);
		}
	}

	printf(out_string);
	fclose(fp);
	free(output);
	free(buffer);
	return 0;
}
