
/*---------------------------------------------------------------------------
                                 MODULE INCLUDES
-----------------------------------------------------------------------------*/
//#include "types.h"
#include "linuxboot.h"

/*---------------------------------------------------------------------------
                             LOCAL DEFINED CONSTANTS
-----------------------------------------------------------------------------*/
#define tag_next(t)     ((struct atag *)((unsigned int *)(t) + (t)->hdr.size))
#define tag_size(type)  ((sizeof(struct atag_header) + sizeof(struct type)) >> 2)

/*---------------------------------------------------------------------------
                                  LOCAL VARIABLES
-----------------------------------------------------------------------------*/
static struct atag *params; /* used to point at the current tag */

/*---------------------------------------------------------------------------
                              LOCAL FUNCTION PROTOTYPES
-----------------------------------------------------------------------------*/
static void setup_core_tag(void * address, long pagesize);
static void setup_ramdisk_tag(unsigned int size);
static void setup_initrd2_tag(unsigned int start, unsigned int size);
static void setup_mem_tag(unsigned int start, unsigned int len);
static void setup_cmdline_tag(const char *line);
static void setup_end_tag(void);

/*---------------------------------------------------------------------------
  Function   : bootlinux
  Purpose    : Boots Linux kernel using pre-stored images in RAM
  Parameters : lparms  - Pointer to machine-dependent parameters
  Returns    : None
  Notes      : Locates tags at offset 0x100 in RAM (standard practice)
-----------------------------------------------------------------------------*/
void bootlinux(LINUX_MACHINE_PARMS *lparms)
{
	void (*theKernel)(int zero, int arch, unsigned int params);
	unsigned int exec_at = (unsigned int)-1;
	unsigned int parm_at = (unsigned int)-1;
	unsigned long r;

	exec_at = lparms->kernel_addr;
	parm_at = lparms->ram_base + 0x100;

	/* Set up the Linux boot tags */
    setup_core_tag((unsigned int *)parm_at, 4096L);/* Standard core tag 4k page size */
    setup_mem_tag(lparms->ram_base, lparms->ram_size);
    setup_ramdisk_tag(4096);				/* Create 4Mb ramdisk */
    setup_initrd2_tag(lparms->ramdisk_addr, lparms->ramdisk_size);
    setup_cmdline_tag(lparms->bootargs);	/* Command line setting root device */
    setup_end_tag();						/* End of tags */

    /* Set Supervisor mode and disable interrupts */
	/* Note: lint -e19 Turns OFF lint error on useless definition */
#if 0
    asm(".equ PSR_MODE_SVC, 0x13      ");
	asm(".equ PSR_F_BIT,    0x40      ");
	asm(".equ PSR_I_BIT,    0x80      ");
    asm("msr cpsr_c, #(PSR_MODE_SVC | PSR_I_BIT | PSR_F_BIT)");


	/* Turn off and flush I/D-cache */
	asm ("mrc p15, 0, %0, c1, c0, 0":"=r" (r));
	r &= ~(C1_DC | C1_IC);
	asm ("mcr p15, 0, %0, c1, c0, 0": :"r" (r));
	r = 0;
	asm ("mcr p15, 0, %0, c7, c7, 0": :"r" (r));
#endif
	/* Set the kernel address and call it with register set */
	theKernel = (void (*)(int, int, unsigned int))(exec_at | 0x01);
	theKernel(0, lparms->machine, parm_at);
}

/*---------------------------------------------------------------------------
  Function   : setup_core_tag
  Purpose    : Initialises the Linux core tag
  Parameters : address  - The tags start address
               pagesize - The Linux system page size (usually 4KB)
  Returns    : None
  Notes      : None
-----------------------------------------------------------------------------*/
static void setup_core_tag(void * address, long pagesize)
{
    params = (struct atag *)address;         /* Initialise parameters to start at given address */

    params->hdr.tag = ATAG_CORE;            /* start with the core tag */
    params->hdr.size = tag_size(atag_core); /* size the tag */

    params->u.core.flags = 0;               /* ensure read/write file system */
    params->u.core.pagesize = pagesize;     /* systems pagesize (4k) */
    params->u.core.rootdev = 0;             /* zero root device (typicaly overidden from commandline )*/

    params = tag_next(params);              /* move pointer to next tag */
}

/*---------------------------------------------------------------------------
  Function   : setup_ramdisk_tag
  Purpose    : Initialises the Linux RAM disk tag
  Parameters : size  - The RAM size in bytes
  Returns    : None
  Notes      : None
-----------------------------------------------------------------------------*/
static void setup_ramdisk_tag(unsigned int size)
{
    params->hdr.tag = ATAG_RAMDISK;         /* Ramdisk tag */
    params->hdr.size = tag_size(atag_ramdisk);  /* size tag */

    params->u.ramdisk.flags = 0;            /* Load the ramdisk */
    params->u.ramdisk.size = size;          /* Decompressed ramdisk size */
    params->u.ramdisk.start = 0;            /* Unused */

    params = tag_next(params);              /* move pointer to next tag */
}

/*---------------------------------------------------------------------------
  Function   : setup_initrd2_tag
  Purpose    : Initialises the Linux initrd2 tag
  Parameters : start  - The inird start address
               size   - The size of the initrd in bytes
  Returns    : None
  Notes      : None
-----------------------------------------------------------------------------*/
static void setup_initrd2_tag(unsigned int start, unsigned int size)
{
    params->hdr.tag = ATAG_INITRD2;         /* Initrd2 tag */
    params->hdr.size = tag_size(atag_initrd2);  /* size tag */

    params->u.initrd2.start = start;        /* physical start */
    params->u.initrd2.size = size;          /* compressed ramdisk size */

    params = tag_next(params);              /* move pointer to next tag */
}

/*---------------------------------------------------------------------------
  Function   : setup_mem_tag
  Purpose    : Initialises the Linux memory tag
  Parameters : start  - The start address of the system RAM
               len    - The size of the system RAM in bytes
  Returns    : None
  Notes      : None
-----------------------------------------------------------------------------*/
static void setup_mem_tag(unsigned int start, unsigned int len)
{
    params->hdr.tag = ATAG_MEM;             /* Memory tag */
    params->hdr.size = tag_size(atag_mem);  /* size tag */

    params->u.mem.start = start;            /* Start of memory area (physical address) */
    params->u.mem.size = len;               /* Length of area */

    params = tag_next(params);              /* move pointer to next tag */
}

/*---------------------------------------------------------------------------
  Function   : setup_cmdline_tag
  Purpose    : Initialises the Linux commandline tag
  Parameters : line  - Pointer to the commandline string
  Returns    : None
  Notes      : None
-----------------------------------------------------------------------------*/
static void setup_cmdline_tag(const char *line)
{
	int i;

	i = ATAG_CMD_LINE_LEN-2;
	params->hdr.tag = ATAG_CMDLINE;			/* Commandline tag */
	params->hdr.size = (sizeof(struct atag_header) + i + 1 + 4) >> 2;

	/* Place commandline into tag */
	for (i = 0; i < ATAG_CMD_LINE_LEN; i++)
	{
		params->u.cmdline.cmdline[i] = line[i];
		if (line[i] == '\0')
			break;
	}
	/* Truncate command line if necessary */
	if (i == ATAG_CMD_LINE_LEN)
		params->u.cmdline.cmdline[ATAG_CMD_LINE_LEN-1] = '\0';

	params = tag_next(params);	/* move pointer to next tag */
}

/*---------------------------------------------------------------------------
  Function   : setup_end_tag
  Purpose    : Initialises the Linux end tag
  Parameters : None
  Returns    : None
  Notes      : None
-----------------------------------------------------------------------------*/
static void setup_end_tag(void)
{
    params->hdr.tag = ATAG_NONE;            /* Empty tag ends list */
    params->hdr.size = 0;                   /* zero length */
}



