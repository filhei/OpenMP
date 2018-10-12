#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BUFFER_SIZE 1e5
#define MAX_DIST 34.6410161514

typedef struct{
	char x_d, y_d, z_d;
	short x_f, y_f, z_f;
}Point;

//set string value to one coord for a point
static inline void atop(char *str, char *d, short *f){
	int sign;
	char d_temp = 0;
	short f_temp = 0;
	if(str[0]=='+'){
		sign = 1;
	}else{
		sign = -1;
	}
	d_temp += 10*(str[1] - '0');
	d_temp += str[2] - '0';
	f_temp += 100*(str[4] - '0');
	f_temp += 10*(str[5] - '0');
	f_temp += str[6] - '0';
	*d = d_temp;
	*f = sign*f_temp;
}

static inline float to_float(char d, short f){
	if(f > 0){
		return (float)d + f/1000.0;
	}
	return (float)-d + f/1000.0;
}

int compare_float(float *f1, float *f2){
	return (int) ((*f1 - *f2)*1000);
}

//input_file: the file to be read from, buffer: point buffer to hold the result
static inline void read_file(FILE *input_file, Point *buffer){
	static char result[9];
	size_t elements_read = 0;
	while(fread(result, 1, 8, input_file) == 8){
		atop(result, &buffer[elements_read].x_d, &buffer[elements_read].x_f);

		fread(result, 1, 8, input_file);
		atop(result, &buffer[elements_read].y_d, &buffer[elements_read].y_f);

		fread(result, 1, 8, input_file);
		atop(result, &buffer[elements_read].z_d, &buffer[elements_read].z_f);
		++elements_read;
	}
}

static inline float point_dist(Point p1, Point p2){
	return sqrt(
			(to_float(p1.x_d, p1.x_f)-to_float(p2.x_d,p2.x_f))*(to_float(p1.x_d, p1.x_f)-to_float(p2.x_d,p2.x_f)) +
			(to_float(p1.y_d, p1.y_f)-to_float(p2.y_d,p2.y_f))*(to_float(p1.y_d, p1.y_f)-to_float(p2.y_d,p2.y_f)) +
			(to_float(p1.z_d, p1.z_f)-to_float(p2.z_d,p2.z_f))*(to_float(p1.z_d, p1.z_f)-to_float(p2.z_d,p2.z_f))
		);
}


void print_point(Point point){
	printf("%d.%.3d %d.%.3d %d.%.3d\n", point.x_d, point.x_f, point.y_d, point.y_f, point.z_d, point.z_f);
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
	char out_string[100000];
	char temp[100];
	for(int i = 0; i < 3465; ++i){
		if(output_occurance[i]){
			sprintf(temp, "%.2f %d\n", MAX_DIST*i/3465.0, output_occurance[i]);
			strcat(out_string, temp);
			memset(temp, 0, 100);
		}
	}
	printf(out_string);
	//qsort(output, 1e4*(1e4-1)/2, sizeof(float), compare_float);

	//for(int i = 0; i < 1e5*(1e5-1)/2; ++i)
		//printf("%.2f\n", output[i]);

	fclose(fp);
	free(output);
	free(buffer);
	return 0;
}
