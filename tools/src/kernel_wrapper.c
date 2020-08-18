extern void kernel_entry(void* pParam);

void kernel_wrapper()
{
    kernel_entry(0);
}