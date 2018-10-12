#include <stdio.h>
#include <stdlib.h>

typedef struct {
	float x,y,z;
} Point;







int main() {
	
	//int r, c, k;

	int a[12] = {12345,23456,34567,45678,56789,67890,78901,89012,90123,012345,11111,22222};
	char * s = malloc(27);
	int d;
	/*
	for (int i = 0; i<12; ++i) {
		d = 216 - 18*i - 18;
		s[0] = s[0]|((a[i] & 0x80000000) << d);
		s[0] = s[0]|((a[i] & 0x0001FFFF) << d);		
	}
	printf("Storage: %u \n", s[0]);

	int * b = malloc(sizeof(int) * 12);
	
	for (int i = 0; i<12; ++i) {
		d = 216 - 18*i -18;
		b[i] = b[i]|(s[0] & (0x80000000 << d));
		b[i] = b[i]|(s[0] & (0x0001FFFF << d));
	}

	for (int i = 0; i<12; ++i) {
		printf("Element %d: %d\n", i, b[i]);
	}
	*/
	/*
	s[0] = 0;
	int t = 91234;
	printf("Hex = %d\n", 0x0001FFFF);
	printf("s[0]: %d\n", s[0]);
	s[0] = s[0]|((t>>31)<<7);
	printf("s[0] only sign: %d\n", s[0]);
	s[0] = s[0]|((t>>10)&0x7F);
	printf("t & hex: %d\n", t&0x0001FFFF);
	printf("s[0]: %d\n", s[0]);
	*/
	


	/*
	int a = 7;
	int b = 16;
	int c = 65537;
	int d = 131071;

	printf("%u\n", a<<4);
	*/

	
	int bit_idx = 0;
	int s_idx = 0;
	int t_idx = 0;
	int offset = 0;
	int mask = 0;
	int shift = 0;

	int t[3] = {91234, 70000, 123};
	for (int i = 0; i < 3; ++i) {
		printf("===========================\n========== i = %d ==========\n===========================\n", i);
		s_idx = bit_idx/8;
		printf("s_idx = %d\n", s_idx);

		offset = bit_idx - s_idx*8;
		printf("offset = %d\n", offset);

		s[s_idx] |=(t[i]>>31)<<(8-offset);
		printf("t[%d]>>31)<<(8-offset) = %d\n", i, (t[i]>>31)<<(8-offset));
		
		bit_idx++;
		printf("bit_idx = %d\n", bit_idx);

		t_idx = 0;

		printf("Entering while loop.\n");
		while(t_idx < 17) {
			printf("t_idx = %d\n", t_idx);
			s_idx = bit_idx/8;
			offset = bit_idx - s_idx*8;
			mask = 0;
			printf("s_idx = %d, offset = %d\n", s_idx, offset);
			for (int k = 0; k<8-offset; ++k) {
				mask = mask<<1 | 1;
			}
			printf("mask = %d\n", mask);

			d = 17-t_idx-8+offset;
			printf("d = %d\n", d);
			if (d >= 0) {
				s[s_idx] |= ((t[i]>>d)&mask);	
				printf("(t[%d]>>d)&mask = %d\n", i, ((t[i]>>d)&(mask)));
			} else { 
				//printf("d megative => t[i]<<(-d) = %d, (t[i]<<(-d))&mask = %d\n", t[i]<<(-d), (t[i]<<(-d))&mask);
				s[s_idx] |= ((t[i]<<(-d))&mask);	
				printf("(t[%d]<<(-d)&mask) = %d\n", i, (t[i]<<(-d))&mask);
			
			}
			bit_idx += (17-t_idx) < (8-offset) ? 17-t_idx : 8-offset;
			t_idx += 8-offset;
		}
	}
	
	printf("First byte of s: %d\n", s[0]);
	
	printf("\nStorage complete! \n\n");
	
	int * values = malloc(sizeof(int)*3);
	s_idx = 0;
	bit_idx = 0;
	offset = 0;
	int temp = 0;
	for (int i = 0; i<3; ++i) {
		printf("\n======= Value %d =======\n", i);
		offset = bit_idx - s_idx*8;
		values[i] = s[s_idx] >> (7-offset);  
		printf("Sign bit assigned to values[%d] = %d\n", i, values[i]);
		values[i] <<= 31;
		printf("Bit shifted valued[%d] = %d\n", i, values[i]);
		bit_idx += 1;
		t_idx = 0;
		while (t_idx < 17) {
			s_idx = bit_idx/8;
			offset = bit_idx - s_idx*8;
			d = 17-t_idx-8+offset;
			printf("d = %d, offset = %d\n", d, offset);
			temp = 0;
			temp |= (s[s_idx]<<offset)>>offset;
			printf("s[%d] = %d, temp = %d\n", s_idx, s[s_idx], temp);
			if (d >= 0) {
				temp<<=d;
			} else {
				temp >>=(-d);
			}
			printf("After bitshift temp = %d\n", temp);
			values[i]|=temp;
			printf("values[%d] = %d\n", i, values[i]);
			bit_idx += (17-t_idx) < (8-offset) ? 17-t_idx : 8-offset;
			t_idx += 8-offset;
		}
	}
	for (int i = 0; i<3; ++i) {
		printf("Integer nr %d = %d\n", i, values[i]);	
	}


}
