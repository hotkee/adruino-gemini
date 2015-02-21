/* lmss : Bitmap */

#ifndef _BITMAP_H_
#define _BITMAP_H_


/* Bitmap */
template <int n>
class Bitmap
{
    public:
        u8 body[n];
        u8 size;

        Bitmap(void)
        {
            this->size = n;
            for (u8 i = 0; i != n; i++)
            { body[i] = 0; }
        }
        /* Find spare */
        s8 spare(void)
        {
            for (u8 i = 0; i != n; i++){
                u8 c = body[i];
                if(c==0xff) {continue;}
                // case 0: 11111111
                // case 1: 00011011
                c = c ^ (c+1);
                // case 0: 11111111 ^ 00000000  =>  11111111
                // case 1: 00011011 ^ 00011100  =>  00000111
                c = count4bit(c); 
                return (i << 3) + c-1;
            }
                
            return -1;
        }
        /* Find busy */
        s8 busy(void)
        {
            for (u8 i = 0; i != n; i++){
                u8 c = body[i];
                if(c==0) {continue;}
                // case 0: 00000000
                // case 1: 00011000
                c = c ^ (c-1);
                // case 0: 00000000 ^ 11111111  =>  11111111
                // case 1: 00011000 ^ 00010111  =>  00001111
                c = count4bit(c); 
                return (i << 3) + c-1;
            }
                
            return -1;
        }
        /* Set */
        void set(u8 index)
        { body[index >> 3] |= 1 << (index & 0x7); }
        /* Clear */
        void clr(u8 index)
        { body[index >> 3] &= ~(1 << (index & 0x7)); }
        void clr_all(void)
        {
            for (u8 i = 0; i != n; i++)
            { body[i] = 0; }
        }
        /* Value */
        u8 value(u8 index)
        { return (body[index >> 3] >> (index & 0x7)) & 0x1; }

#if 0
// genrate static table
        u8 num_used(void)
        {
            u8 count = 0;

            for (s8 i = 0; i != n; i++)
                if (body[i] != 0xFF)
                    for (s8 j = 0; j != 8; j++)
                        if ((body[i] & (1<<j)) != 0)
                            count++;
            return count;
        }
#include <iostream>
        void count4bit_genrate(){
            clr_all();
            std::cout<<"static u8 count4bit_tab[] = {";
            for(u8 i=0; i<1<<4; i++){
                if(i%4==0) {std::cout<<std::endl<<"    ";}
                body[0] = i;
                std::cout<<(unsigned int)num_used()<<", ";
            }
            std::cout<<std::endl<<"};"<<std::endl<<std::endl;
        }
#else
        /* used bit */
        u8 num_used(void)
        {
            u8 count = 0;
            
            for(u8 i=0; i<n; i++){
                count += count4bit(body[i]);
            }
            return count;
        }
#endif
        u8 num_total(void)
        {
            return n << 3;
        }
    private:
        inline static u8 count4bit(u8 v){
            static u8 count4bit_tab[] = {
                0, 1, 1, 2, 
                1, 2, 2, 3, 
                1, 2, 2, 3, 
                2, 3, 3, 4, 
            };
            return count4bit_tab[v&0xf] + count4bit_tab[(v>>4)&0xf];
        }
};


#endif

