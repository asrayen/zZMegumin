#include "stdio.h"
void test();
int main(int argc, char * argv[]){
	int fd;
	char cat_buf[0x200];
	int i;
	//__asm__("xchg %bx, %bx");
	printf("\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00");
	for(i = 1; i < argc; i++){
		fd = -1;
		fd = open(argv[i], O_RDWR);
		if(fd < 0){
			printf("cat: %s: No such file or directory\n", argv[i]);			
		}
		else{
			read(fd, cat_buf, 0x200);
			write(1, cat_buf, 0x200);
			close(fd);
		}
	}
	//test();
	return 0;	

}
void test(){
	struct dir_entry * pde;
	printf("%d\n", pde);
}
