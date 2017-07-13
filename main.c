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
	printf("Waiting for RDY\n");
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
	printf(" recived\n");
	if (junkC != 0)
		printf("/n%d junk bytes skipped\n",junkC);
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
int main(int argc, char* argv[])
{	
	//Input COM port
	printf("Input COM port number: ");
	scanf("%d", &COM_PORT);

	//Determine flashing/dumping mode	
	uint8_t dump=0;
	if (argc!=2&&argc!=3){
		help();
		return 1;
	}
	else if(argc==3){
		if(strcmp("-d",argv[2])==0){
			printf("Dumping mode\n");
			dump=1;
		} else {
			printf("To specify dumping you need to use -d but you did %s instead\nThis program will show help and exit\n",argv[2]);
			help();
			return 1;
		}
	}

	//Open COM port
	if(RS232_OpenComport(COM_PORT,500000)){
		printf("Com-port %i could not be opened\n", COM_PORT);
		return 1;
	}

	//Wait for RDY from the Arduino
	waitRDY();

	//Get chip signature
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
			printf("SST/Microchip\n");
	else
		printf("Unknown manufacturer\n");
	RS232_PollComport(COM_PORT,&id,1);
	printf("Device ID: 0x%X (",id);
	
	//Determine flash size
	uint32_t capcity=524288;
	switch(id){
		case 0xB5:
			printf("SST39SF010A)\n");
			capcity=131072;
		case 0xB6:
			printf("SST39SF020A)\n");
			capcity=262144;
		break;
		case 0xB7:
			printf("SST39SF040)\n");
			capcity=524288;
		break;
		default:
			printf("ERROR: Cannot determine chip capacity, defaulting to 524288\n");
		break;
	}
	
	//File to flash from/dump to
	FILE* fp;
	//Binary data array
	uint8_t* dat;
	
	//Flashing mode
	if(!dump){
		//Open file for reading
		fp=fopen(argv[1],"rb");

		//Get file size
		fseek(fp, 0L, SEEK_END);
		size_t size = ftell(fp);
		
		//Check for size mismatch
		if (size > capcity){
			printf("ERROR: File too large\n");
			fclose(fp);
			return 1;
		}

		//Return to beginning of file
		rewind(fp);

		//Allocate memory for binary data
		dat=(uint8_t*)calloc(1,capcity);

		//Check for allocation errors
		if(dat==0){
			printf("ERROR: Cannot allocate memory\n");
			fclose(fp);
			return 1;
		}
		
		//Read file data into array
		fread(dat,1,size,fp);
		
		//Close file
		fclose(fp);

		//Flash erasing procedure
		printf("Erasing chip...\n");
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
			printf("Erasing complete\n");
		else{
			printf("ERROR: Erasing chip code %c failed\n",id);
			free(dat);
			return 1;
		}
		printf("Begin flashing %s\n", argv[1]);

	//Dumping mode
	} else {
		//Open file for writing
		fp=fopen(argv[1],"wb");	
		printf("Begin dumping to %s\n", argv[1]);
	}

	//Flashing procedure
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
	
	printf("Operation completed!\n");

	//Close file 
	if(dump) fclose(fp);
	else free(dat);

	//Successful exit
	return 0;
}
