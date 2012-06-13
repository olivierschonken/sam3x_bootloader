
/*---------------------------------------------------------------------------
                                 MODULE INCLUDES
-----------------------------------------------------------------------------*/
#ifndef LINUX_H_
#define LINUX_H_

/*---------------------------------------------------------------------------
                             LOCAL DEFINED CONSTANTS
-----------------------------------------------------------------------------*/
/* Linux boot tags */
#define ATAG_NONE			0x00000000
#define ATAG_CORE			0x54410001
#define ATAG_MEM			0x54410002
#define ATAG_VIDEOTEXT		0x54410003
#define ATAG_RAMDISK		0x54410004
#define ATAG_INITRD2		0x54420005
#define ATAG_SERIAL			0x54410006
#define ATAG_REVISION		0x54410007
#define ATAG_VIDEOLFB		0x54410008
#define ATAG_CMDLINE		0x54410009

#define ATAG_CMD_LINE_LEN	64

#define C1_DC				(1 << 2)		/* dcache off/on */
#define C1_IC				(1 << 12)		/* icache off/on */

/*------------------------------------------------------------------------------
                                LOCAL DATA TYPES
------------------------------------------------------------------------------*/
typedef struct _machine_parms
{
	unsigned int	machine;		/* Linux kernel machine number */
	unsigned int	ram_base;		/* RAM start address */
	unsigned int	ram_size;		/* RAM length in bytes */
	unsigned int	kernel_addr;	/* Address of kernel (should be ) */
	unsigned int	ramdisk_addr;	/* Address of RAM disk */
	unsigned int	kernel_size;	/* Length in bytes of kernel image */
	unsigned int	ramdisk_size;	/* Length in bytes of ramdisk image */
	char	*bootargs;		/* Pointer to Linux commandline parameter string */
}LINUX_MACHINE_PARMS;

typedef struct _uimage_hdr
{
	unsigned int	ih_magic;		/* Image Header Magic Number */
	unsigned int	ih_hcrc;		/* Image Header CRC Checksum */
	unsigned int	ih_time;		/* Image Creation Timestamp  */
	unsigned int	ih_size;		/* Image Data Size           */
	unsigned int	ih_load;		/* Data	 Load  Address       */
	unsigned int	ih_ep;			/* Entry Point Address       */
	unsigned int	ih_dcrc;		/* Image Data CRC Checksum   */
	unsigned char	ih_os;			/* Operating System          */
	unsigned char	ih_arch;		/* CPU architecture          */
	unsigned char	ih_type;		/* Image Type                */
	unsigned char	ih_comp;		/* Compression Type          */
	unsigned char	ih_name[32];	/* Image Name                */
} uimage_hdr;


/* structures for each atag */
struct atag_header
{
	unsigned int size; 			/* length of tag in words including this header */
	unsigned int tag; 				/* tag type */
};

struct atag_core
{
	unsigned int flags;
	unsigned int pagesize;			/* Linux page size (usually 4096 */
	unsigned int rootdev;
};

struct atag_mem
{
	unsigned int	size;
	unsigned int	start;
};

struct atag_videotext
{
	unsigned char	x;
	unsigned char	y;
	unsigned short	video_page;
	unsigned char	video_mode;
	unsigned char	video_cols;
	unsigned short	video_ega_bx;
	unsigned char	video_lines;
	unsigned char	video_isvga;
	unsigned short	video_points;
};

struct atag_ramdisk
{
	unsigned int flags;
	unsigned int size;
	unsigned int start;
};

struct atag_initrd2
{
	unsigned int start;
	unsigned int size;
};

struct atag_serialnr
{
	unsigned int low;
	unsigned int high;
};

struct atag_revision {unsigned int rev;};

struct atag_videolfb
{
	unsigned short	lfb_width;
	unsigned short	lfb_height;
	unsigned short	lfb_depth;
	unsigned short	lfb_linelength;
	unsigned int	lfb_base;
	unsigned int	lfb_size;
	unsigned char	red_size;
	unsigned char	red_pos;
	unsigned char	green_size;
	unsigned char	green_pos;
	unsigned char	blue_size;
	unsigned char	blue_pos;
	unsigned char	rsvd_size;
	unsigned char	rsvd_pos;
};

struct atag_cmdline
{
	char cmdline[1];
};

struct atag
{
	struct atag_header hdr;
	union
	{
		struct atag_core         core;
		struct atag_mem          mem;
		struct atag_videotext    videotext;
		struct atag_ramdisk      ramdisk;
		struct atag_initrd2      initrd2;
		struct atag_serialnr     serialnr;
		struct atag_revision     revision;
		struct atag_videolfb     videolfb;
		struct atag_cmdline      cmdline;
	} u;
};

/*---------------------------------------------------------------------------
                              GLOBAL FUNCTION PROTOTYPES
-----------------------------------------------------------------------------*/
void bootlinux(LINUX_MACHINE_PARMS *lparms);

#endif /* LINUX_H_ */
