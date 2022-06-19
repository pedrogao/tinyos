#include "os.h"
#include "syscall.h"

int sys_gethid(unsigned int *ptr_hid)
{
    printf("--> sys_gethid, arg0 = 0x%x\n", ptr_hid);
    if (ptr_hid == NULL)
    {
        return -1;
    }
    else
    {
        *ptr_hid = r_mhartid();
        return 0;
    }
}

void do_syscall(struct context *ctx)
{
    uint32_t syscall_num = ctx->a7;

    switch (syscall_num)
    {
    case SYS_gethid:
        ctx->a0 = sys_gethid((unsigned int *)(ctx->a0));
        break;

    default:
        printf("Unknown syscall no: %d\n", syscall_num);
        ctx->a0 = -1;
    }
}