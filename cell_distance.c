#include <omp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BUFFER_SIZE 1e5
#define MAX_DIST 34.64

//global variables
size_t file_position = 0;

typedef struct{
	short x, y, z;
}Point;

//converts a row from the input file to a Point
static inline void str2point(char *str, Point *p){
	short d1,d2,d3;
	d1 = 1e4*(str[1] - '0');
	d1 += 1e3*(str[2] - '0');
	d1 += 1e2*(str[4] - '0');
	d1 += 1e1*(str[5] - '0');
	d1 += str[6] - '0';
	
	d2 = 1e4*(str[9] - '0');
	d2 += 1e3*(str[10] - '0');
	d2 += 1e2*(str[12] - '0');
	d2 += 1e1*(str[13] - '0');
	d2 += str[14] - '0';
	
	d3 = 1e4*(str[17] - '0');
	d3 += 1e3*(str[18] - '0');
	d3 += 1e2*(str[20] - '0');
	d3 += 1e1*(str[21] - '0');
	d3 += str[22] - '0';
	
	p->x = (str[0] == '+' ? d1 : -d1);
	p->y = (str[8] == '+' ? d2 : -d2);
	p->z = (str[16] == '+' ? d3 : -d3);
}

//converts float to string for output
static inline void f2str(char *str, float f){
	int temp = f*100;
	str[0] = temp/1000 + '0';
	temp%=1000;
	str[1] = temp/100 + '0';
	temp%=100;
	str[2] = '.';
	str[3] = temp/10 + '0';
	str[4] = temp%10 + '0';
}

static inline size_t i2str(char *str, int i){
	if(i < 10){
		str[0] = i + '0';
		str[1] = '\n';
		return 2;
	}else if(i < 100){
		str[0] = i/10 + '0';
		str[1] = i%10 + '0';
		str[2] = '\n';
		return 3;
	}else{
		size_t length = 0;
		for(int j = 1; j < 1e10 && i/j != 0; j*=10){
			++length;
		}
		for(int j = 0; j < length; ++j){
			str[length-j-1] = i%10 + '0';
			i/=10;
		}
		str[length] = '\n';
		return length+1;
	}
	return 0;
}

/* WITH CURRENT IMPLEMENTATION DO NOT MULTITHREAD
 * fread is performed by single thread, parsing multi*/	
//input_file: the file to be read from, buffer: point buffer to hold the result
static inline void read_file(FILE *input_file, Point *buffer){
	char *file_content = (char*) malloc(24*BUFFER_SIZE);
	size_t bytes_read = fread(file_content,1,24*BUFFER_SIZE, input_file), current_byte = 0;

	#pragma omp parallel for shared(buffer, file_content)
	for(size_t current_byte = 0; current_byte < bytes_read; current_byte+=24){
		str2point(&file_content[current_byte], &buffer[current_byte/24]);
	}
	free(file_content);
	if(bytes_read != 24*BUFFER_SIZE){
		//set file position to BOF
		fseek(input_file,0,SEEK_SET);
	}
}

static inline short point_index(Point p1, Point p2){
	return (short)sqrt((float)
			(p1.x-p2.x)*(p1.x-p2.x) +
			(p1.y-p2.y)*(p1.y-p2.y) +
			(p1.z-p2.z)*(p1.z-p2.z)
		)/10.0 + 0.5;
}

int main(){
	omp_set_num_threads(20);
	FILE *fp;
	fp = fopen("input_files/cell_e5","r");
	Point *buffer = (Point*) malloc(BUFFER_SIZE*sizeof(Point));
	read_file(fp, buffer);
	//this needs to be thread safe
	unsigned int output_occurance[3465];
	memset(output_occurance,0,sizeof(int)*3465);

	//+0.5 seems to fix the rounding errors
	/*
 	* This part is where 99% of the time is spent.
 	* Making two copies and calculate both in the same for loop
 	* seemed to slightly increase performance, might be worth it
 	*/
	#pragma omp parallel for
	for(int i = 0; i < 100000; ++i){
		int output_occurance_local[3465];
		memset(output_occurance_local,0,sizeof(int)*3465);
		for(int j = 100000-1; j > i; --j){
			//awful locality, the index is random
			output_occurance_local[
				point_index(buffer[i], buffer[j])
			] += 1;
		}
		#pragma omp critical
		for(int j = 0; j < 3465; ++j){
			output_occurance[j] += output_occurance_local[j];
		}
	}
	char out_string[20*3465];
	char temp[20];
	int size = 0, i2str_count = 0;
	//might be a good idea to combine i2str and f2str for performance
	for(int i = 0; i < 3465; ++i){
		if(output_occurance[i]){
			f2str(temp, MAX_DIST*i/3464.0);
			temp[5] = ' ';
			i2str_count = i2str(&temp[6], output_occurance[i]);
			memcpy(&out_string[size], temp, 6+i2str_count);
			size += 6 + i2str_count;
		}
	}
	printf(out_string);
	
	fclose(fp);
	free(buffer);
	return 0;
}
