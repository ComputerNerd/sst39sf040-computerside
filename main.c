//SST39SF040 FLASHER

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "rs232.h"

uint COM_PORT = -1;

//Wait for "ready" status from the Arduino
void waitRDY(void)
{
	static const char Sig[]="RDY";
	uint8_t tempC,x;
	uint32_t junkC=0;
	printf("Waiting for RDY command\n");
	for (x=0;x<3;++x){
		do {
			RS232_PollComport(COM_PORT,&tempC,1);
			if (tempC != Sig[x]){
				junkC++;
				printf("Junk Char %d or %c while waiting for %c so far skiped %d\n",tempC,tempC,Sig[x],junkC);
			}
			putchar(tempC);
		} while (tempC != Sig[x]);
	}
	if (junkC != 0)
		printf("%d junk bytes skipped\n",junkC);
	printf("Ready recived\n");
}

//Send a byte of data to the flash chip
void programByte(uint8_t dat)
{
	uint8_t datr;
//again:
	RS232_SendByte(COM_PORT,dat);
	RS232_PollComport(COM_PORT,&datr,1);
	if (datr!='N')
		printf("ERROR: Programming byte letter code '%c' failed\n",datr);
}

//Show help info
void help(void){
	printf("Usage: flash filename [-d]\n");
	printf("-d is optional\n");
	printf("If you specify -d that means to dump flash chip instead of write and file will be for output instead of input\n");
}

//Main
int main(int argc,char ** argv)
{
	//Input COM port
	printf("Input COM port number: ");
	scanf("%d", &COM_PORT);
	uint8_t dump=0;
	if (argc!=2&&argc!=3){
		help();
		return 1;
	}
	else if(argc==3){
		if(strcmp("-d",argv[2])==0)
			dump=1;
		else{
			printf("To specify dumping you need to use -d but you did %s instead\nThis program will show help and exit\n",argv[2]);
			help();
			return 1;
		}
	}
	if(RS232_OpenComport(COM_PORT,500000)){
		printf("Com-port %i could not be opened\n", COM_PORT);
		return 1;
	}
	waitRDY();
	RS232_SendByte(COM_PORT,'R');
	if(dump)
		RS232_SendByte(COM_PORT,'R');
	else
		RS232_SendByte(COM_PORT,'W');
	waitRDY();
	uint8_t id,manid;
	RS232_PollComport(COM_PORT,&manid,1);
	printf("Manufacture ID: 0x%X\nDetcted as: ",manid);
	if(manid==0xBF)
			printf("SST/Microchip");
	else
		printf("Unknown manufacturer\n");
	RS232_PollComport(COM_PORT,&id,1);
	printf("Device ID: 0x%X\n",id);
	uint32_t capcity=524288;
	switch(id){
		case 0xB5:
			printf("SST39SF010A\n");
			capcity=131072;
		case 0xB6:
			printf("SST39SF020A\n");
			capcity=262144;
		break;
		case 0xB7:
			printf("SST39SF040\n");
			capcity=524288;
		break;
		default:
			printf("ERROR: Cannot determine chip capacity, defaulting to 524288\n");
		break;
	}
	FILE* fp;
	uint8_t* dat;
	if(!dump){
		fp=fopen(argv[1],"rb");
		fseek(fp, 0L, SEEK_END);
		size_t size = ftell(fp);
		if (size > capcity){
			printf("ERROR: File too large\n");
			fclose(fp);
			return 1;
		}
		rewind(fp);
		dat=(uint8_t*)calloc(1,capcity);
		if (dat==0){
			printf("ERROR: Cannot allocate memory\n");
			fclose(fp);
			return 1;
		}
		fread(dat,1,size,fp);
		fclose(fp);
		RS232_PollComport(COM_PORT,&id,1);
		//putchar(id);//should be upercase 'D'
		if(id!='D'){
			printf("An error has occured, exiting...\n");
			free(dat);
			return 1;
		}
		putchar('\n');
		RS232_PollComport(COM_PORT,&id,1);
		if (id == 'S')
			printf("Chip is erased\n");
		else{
			printf("ERROR: Erasing chip code %c failed\n",id);
			free(dat);
			return 1;
		}
	}else
		fp=fopen(argv[1],"wb");	
	//now program the chip
	putchar('\n');
	uint32_t x;
	for (x=0;x<capcity;++x){
		uint8_t data;
		if(dump){
			RS232_PollComport(COM_PORT,&data,1);
			fputc(data,fp);
		}else{
			programByte(dat[x]);
			RS232_PollComport(COM_PORT,&data,1);
			if (data!=dat[x])
				printf("Byte %d at address %d should be %d\n\n",data,x,dat[x]);
		}
		printf("Progress : %% %f\r",(float)x/(float)capcity*100.0);
	}
	if(dump)
		fclose(fp);
	free(dat);
	return 0;
}
