Managign Memory 

Today´s applications in our computer requires sometimes more memory resource than we can imagine. No matter how much memory is available.
The more that we advance in our applications the more the space is require, for example, It doesn’t seem so long ago that 256 MB of RAM 
was considered sufficient, but now 2GB of RAM is commonplace as a sensible minimum requirement even for desktop system, with servers 
usefully having significantly more. So We can offer a solution through linux to managing memory. Linux applications, Linux applications,
except for a few specialized embedded applications, are never permitted to access physical memory directly. Linux provides applications 
with a clean view of a huge directly addressable memory space. Also provides protection so that different applications are protected from
each other. 
  
    Simple Memory Allocation
    You allocate memory using the malloc call in the standard C library:
            #include <stdlib.h>
            void *malloc(size_t size);
            
Freeing Memory

  Up to now, we’ve been simply allocating memory and then hoping that when the program ends, the
  memory we’ve used hasn’t been lost. Fortunately, the Linux memory management system is quite capable
  of reliably ensuring that memory is returned to the system when a program ends. However, most pro-
  grams don’t simply want to allocate some memory, use it for a short period, and then exit. A much more
  common use is dynamically using memory as required.
  
  Programs that use memory on a dynamic basis should always release unused memory back to the
malloc memory manager using the free call. This enables separate blocks to be remerged and enables
the malloc library to look after memory, rather than have the application manage it. 
  
  
Other Memory Allocation Functions
Two other memory allocation functions are not used as often as malloc and free: calloc and realloc.The prototypes are
    #include <stdlib.h>
    void *calloc(size_t number_of_elements, size_t element_size);
    void *realloc(void *existing_memory, size_t new_size);
  
  
  
  //************************************COMPILING PROGRAM*******************************

To use this code donwload lab5.1 file and execute the comand "make", this will create into the same "lab5.1" the .o files for every 
memory#.c . If you want to create one by one just tape "make memory#.c" and you will see an .o file created on the "lab5.1" file, but in this 
time just for the specified memory#.c 

have fun
