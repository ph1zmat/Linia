#include <ff.h>



FRESULT f_mount(FATFS* /*fs*/, const TCHAR * /*path*/, unsigned char /*opt*/)
{
    return FR_OK;
}


FRESULT f_opendir(DIR* /*dp*/, const void* /*path*/)
{
    return FR_OK;
}


FRESULT f_readdir(DIR* /*dp*/, FILINFO* /*fno*/)
{
    return FR_OK;
}


FRESULT f_closedir(DIR* /*dp*/)
{
    return FR_OK;
}


FRESULT f_open(FIL* /*fp*/, const void* /*path*/, unsigned char /*mode*/)
{
    return FR_OK;
}


FRESULT f_write(FIL* /*fp*/, const void* /*buff*/, UINT /*btw*/, UINT* /*bw*/)
{
    return FR_OK;
}


FRESULT f_close(FIL* /*fp*/)
{
    return FR_OK;
}


FRESULT f_utime(const void* /*path*/, const FILINFO* /*fno*/)
{
    return FR_OK;
}
