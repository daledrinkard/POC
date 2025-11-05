/*
 * dbg_util.c
 *
 *  Created on: May 16, 2025
 *      Author: daled
 */

#include "dbg_util.h"

fa_t fa;
char print_buffer[1024];
/*
 *   PUBLIC
 */

uint8_t mem_blank(uint8_t* p,uint32_t len)
{
    uint8_t *q = p;
    for(uint32_t i=0;i<len;i++)
    {
        if (*q != 0xff) return 0;
        q++;
    }
    return 1;
}
void dbg_print_grid(uint32_t *p,uint32_t sector_size,uint32_t sectors,uint32_t rows)
{
    uint32_t i,j,k,B1;
    uint32_t *q;
    uint32_t w_size = sector_size/4;
    char c;
    char *s;
    B1 = w_size-1;
    for(k=0;k<rows;k++)
    {
        s = print_buffer;
        q = p+(k*w_size*sectors);
        printf("[0x%08lX] ",(uint32_t) q);
        for(i=0;i<sectors;i++)
        {
            c='.';
            for(j=0;j<w_size;j++)
            {
                if(0xFFFFFFFF != *q)
                {
                    if ('.' == c)
                    {
                        c = (*q == 0x96F3B83D) ? 'M' : '*';
                    }
                    if ((j & B1) == (B1 & 0xFFFFFFC0))  // first word of last 256 bytes in a sector-size segment...
                    {
                        if (*q == 0xFFFFFF01)
                        {
                            c = 'C';
                        }
                    }
                    //q = q + (w_size-j); // look no further
                    //break;
                }
                q++;
            }
            *s = c;
            s++;
        }
        *s = '\n';
        s++;
        *s = 0;

        printf(print_buffer);
    }
}
void dbg_print_data(uint32_t *p,uint32_t len)
{
    uint32_t *q;
    q = p;
    char *s;
    uint32_t j;
    s = &print_buffer[0];
    for(j=0;j<len;j++) // print only the first 16 words
    {
        sprintf(s,"0x%08lX ",*q);
        while(*s) s++;
        q++;
    }
    printf("Data: %s\n",print_buffer);
}
void dbg_print_map(void* f)
{
  fa_t *p_fa;
  p_fa = (fa_t*) f;
  for(int i=0;i<3;i++)
  {
      printf("fm[%d] dev: %d  id: %d off: 0x%08lX  size: 0x%08lX ",
             i,
      p_fa->fa_device_id,
      p_fa->fa_id,
      p_fa->fa_off,
      p_fa->fa_size);
      if (mem_blank((uint8_t*) p_fa->fa_off,p_fa->fa_size))
      {
          printf("  region is blank\n");
      } else
      {
          printf("\n");
          dbg_print_data((uint32_t*) p_fa->fa_off,8);
      }
      p_fa++;
  }
}

void dbg_print_rsp_header(struct boot_rsp *p_rsp)
{

    printf("id: %x  off: 0x%08lX\n",
    p_rsp->br_flash_dev_id,
    p_rsp->br_image_off);
    printf("hdr-> magic: %08lX hdr_size: 0x%X img_size: 0x%lX load_addr: 0x%08lX flags: 0x%08lX\n",
            p_rsp->br_hdr->ih_magic,
            p_rsp->br_hdr->ih_hdr_size,
            p_rsp->br_hdr->ih_img_size,
            p_rsp->br_hdr->ih_load_addr,
            p_rsp->br_hdr->ih_flags);
}

void dbg_print_fmap(fa_t *p_f)
{
    int j;
    fa_t *f = p_f;
    for(j=0;j<3;j++)
    {
        printf("[%d]  (0x%08lX) :",j,f->fa_off);
        dbg_print_data((uint32_t*) f->fa_off,8);
        printf("[%d]  (0x%08lX) :",j,(f->fa_off + f->fa_size - 256));
        dbg_print_data((uint32_t*) (f->fa_off + f->fa_size - 256),8);
        printf("[%d]  (0x%08lX) :",j,(f->fa_off + f->fa_size - 32));
        dbg_print_data((uint32_t*) (f->fa_off + f->fa_size - 32),8);
        f++;
    }
}
const char *D_swap[] = { "ZERO","NONE","TEST","PERM","REVERT","FAIL","PANIC" };
char *dbg_swap_type(int i)
{
    if (i<6)
    {
    char *p = D_swap[i];
    return p;
    }
    else return D_swap[6];
}
