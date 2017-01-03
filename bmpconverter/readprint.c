#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define HEIGHT 130
#define WIDTH 256

typedef struct{
  uint8_t blue;
  uint8_t green;
  uint8_t red;
} bmp_palette_t;

bmp_palette_t palette[16];

void writecolor(uint8_t color,bmp_palette_t *palv);

int main(int argc,char **argv){
  FILE *fp;
  uint8_t buffpalette[16*3];
  int i;
  int j;
  
  if(argc==1){
    printf("arg not enough\n");
    return EXIT_FAILURE;
  }
  
  fp = fopen(argv[1],"r");

  while(1){  
    if(fread(buffpalette, 1, 3*16, fp)!=3*16){
      fprintf(stderr,"palet read failed");
      return EXIT_FAILURE;
    }else{
      for(i=0;i<16;i++){
	palette[i].red=buffpalette[i*3+0];
	palette[i].green=buffpalette[i*3+1];
	palette[i].blue=buffpalette[i*3+2];
      }
    }
    
    uint8_t *data;
    unsigned int size;
    size = WIDTH*HEIGHT/2;
    
    data = malloc(size);
    if(data==NULL){
      fprintf(stderr,"malloc faild(%d)",size);
      return EXIT_FAILURE;
    }

    if(fread(data, 1, size, fp)!=size){
      fprintf(stderr,"file size error(%d)\n",size);
      return EXIT_FAILURE;
    }
    for(i=0;i<HEIGHT;i++){
      for(j=0;j<WIDTH/4;j++){
	writecolor(data[i*WIDTH/2+j*2+1]&0xF, palette);
	writecolor(data[i*WIDTH/2+j*2+1]>>4, palette);
	writecolor(data[i*WIDTH/2+j*2+0]&0xF, palette);
	writecolor(data[i*WIDTH/2+j*2+0]>>4, palette);
      }
      //      printf("\n");
      printf("\033[%d;1H",i+1);
    }
    free(data);
  }
}

void setBackColor(int r,int g,int b){
  int n;

  r/=43;
  g/=43;
  b/=43;
  
  n=r;
  n*=6;
  n+=g;
  n*=6;
  n+=b;
  n+=16;
  printf("\033[48;05;%dm",n);
}
void writecolor(uint8_t color,bmp_palette_t *palv){
  setBackColor(palv[color].red,palv[color].green,palv[color].blue);
  printf("  ");
}