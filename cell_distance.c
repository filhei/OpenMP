#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BUFFER_SIZE 1e5

typedef struct{
	char x_d, y_d, z_d;
	short x_f, y_f, z_f;
}Point;

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

//input_file: the file to be read from, buffer: where to place the result
//rows: the number of rows read
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


void print_point(Point point){
	printf("%d.%.3d %d.%.3d %d.%.3d\n", point.x_d, point.x_f, point.y_d, point.y_f, point.z_d, point.z_f);
}

int main(){
	FILE *fp;
	fp = fopen("input_files/cell_e5","r");
	Point *buffer = (Point*) malloc(BUFFER_SIZE*sizeof(Point));

	read_file(fp, buffer);
	for(int i = 0; i < 10; ++i){
		print_point(buffer[i]);
	}	
	size_t i=0;
	for(double f = 0; f <= sqrt(3*4*99.999*99.999); f+=0.01){
		++i;
	}
	printf("%d\n",i+1);
	printf("Bits in memory: %.3f GiBi\n", BUFFER_SIZE*sizeof(Point)*8/1e9);	
	fclose(fp);
	free(buffer);
	return 0;
}
