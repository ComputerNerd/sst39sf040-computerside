//To compile do: gcc dump.c rs232.c -Wall -O2 -o dump
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "rs232.h"
void waitRDY(void)
{
	static const char Sig[]="RDY";
	uint8_t tempC,x;
	uint32_t junkC=0;
	puts("Waiting for RDY command");
	for (x=0;x<3;x++)
	{
		do {
			RS232_PollComport(24,&tempC,1);
			if (tempC != Sig[x])
			{
				junkC++;
				printf("Junk Char %d or %c while waiting for %c so far skiped %d\n",tempC,tempC,Sig[x],junkC);
			}
			putchar(tempC);
		} while (tempC != Sig[x]);
	}
	if (junkC != 0)
		printf("%d junk bytes skipped\n",junkC);
	puts("Ready recived");
}
int main(int argc,char ** argv)
{
	if (argc!=2)
	{
		puts("Usage: dump filename");
	}
	FILE * infile=fopen(argv[1],"wb");
	if(RS232_OpenComport(24,1000000))
	{
		puts("Com-port 24 could not be opened");
		return 1;
	}
	waitRDY();
	RS232_SendByte(24,'R');
	RS232_SendByte(24,'D');
	waitRDY();
	uint8_t id;
	RS232_PollComport(24,&id,1);
	printf("Manufacture ID: 0x%X\n",id);
	RS232_PollComport(24,&id,1);
	printf("Device ID: 0x%X\n\n",id);
	uint32_t x;
	for (x=0;x<524288;x++)
	{
		uint8_t data;
		RS232_PollComport(24,&data,1);
		fputc(data,infile);
		printf("Progress : %% %f\r",(float)x/524288.0*100.0);
	}
	fclose(infile);
	return 0;
}
