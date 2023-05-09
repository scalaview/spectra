#ifndef EXT2_H
#define EXT2_H
#include <stdint.h>

#include "file.h"
#include "stream.h"
#include "config.h"

#define BLOCK_OFFSET(block, block_size) ((MBR_END_POSITION + block_size) + (block-1)*block_size)

#define EXT2_SUPERBLOCK_OFFSET  1024
#define EXT2_MIN_SUPERBLOCK_SIZE    1024
#define EXT2_ROOT_INODE_NO      2
#define EXT2_SUPER_MAGIC	0xEF53

#define EXT2_S_IFSOCK   0xC000	//socket
#define EXT2_S_IFLNK	0xA000	//symbolic link
#define EXT2_S_IFREG	0x8000	//regular file
#define EXT2_S_IFBLK	0x6000	//block device
#define EXT2_S_IFDIR	0x4000	//directory
#define EXT2_S_IFCHR	0x2000	//character device
#define EXT2_S_IFIFO	0x1000	//fifo
/*-- process execution user/group override --*/
#define EXT2_S_ISUID	0x0800	//Set process User ID
#define EXT2_S_ISGID	0x0400	//Set process Group ID
#define EXT2_S_ISVTX	0x0200	//sticky bit
/* -- access rights -- */
#define EXT2_S_IRUSR	0x0100	//user read
#define EXT2_S_IWUSR	0x0080	//user write
#define EXT2_S_IXUSR	0x0040	//user execute
#define EXT2_S_IRGRP	0x0020	//group read
#define EXT2_S_IWGRP	0x0010	//group write
#define EXT2_S_IXGRP	0x0008	//group execute
#define EXT2_S_IROTH	0x0004	//others read
#define EXT2_S_IWOTH	0x0002	//others write
#define EXT2_S_IXOTH	0x0001	//others execute

#define	EXT2_NDIR_BLOCKS		12
#define	EXT2_IND_BLOCK			EXT2_NDIR_BLOCKS
#define	EXT2_DIND_BLOCK			(EXT2_IND_BLOCK + 1)
#define	EXT2_TIND_BLOCK			(EXT2_DIND_BLOCK + 1)
#define	EXT2_N_BLOCKS			(EXT2_TIND_BLOCK + 1)

#define S_ISDIR(mode)       (mode & 0x4000)
#define S_ISREG(mode)       (mode & 0x8000)

struct ext2_superblock {
    uint32_t inodes_count;			// Total # of inodes
    uint32_t blocks_count;			// Total # of blocks
    uint32_t r_blocks_count;		// # of reserved blocks for superuser
    uint32_t free_blocks_count;
    uint32_t free_inodes_count;
    uint32_t first_data_block;
    uint32_t log_block_size;		// 1024 << Log2 block size  = block size
    uint32_t log_frag_size;
    uint32_t blocks_per_group;
    uint32_t frags_per_group;
    uint32_t inodes_per_group;
    uint32_t mtime;					// Last mount time, in POSIX time
    uint32_t wtime;					// Last write time, in POSIX time
    uint16_t mnt_count;				// # of mounts since last check
    uint16_t max_mnt_count;			// # of mounts before fsck must be done
    uint16_t magic;					// 0xEF53
    uint16_t state;
    uint16_t errors;
    uint16_t minor_rev_level;
    uint32_t lastcheck;
    uint32_t checkinterval;
    uint32_t creator_os;
    uint32_t rev_level;
    uint16_t def_resuid;
    uint16_t def_resgid;
} __attribute__((packed));

struct ext2_block_group_descriptor {
    uint32_t block_bitmap;
    uint32_t inode_bitmap;
    uint32_t inode_table;
    uint16_t free_blocks_count;
    uint16_t free_inodes_count;
    uint16_t used_dirs_count;
    uint16_t bg_pad;
    uint32_t bg_reserved[3];
} __attribute__((packed));

struct ext2_inode {
    uint16_t mode;			// Format of the file, and access rights
    uint16_t uid;			// User id associated with file
    uint32_t size;			// Size of file in bytes
    uint32_t atime;			// Last access time, POSIX
    uint32_t ctime;			// Creation time
    uint32_t mtime;			// Last modified time
    uint32_t dtime;			// Deletion time
    uint16_t gid;			// POSIX group access
    uint16_t links_count;	// How many links
    uint32_t blocks;		// # of 512-bytes blocks reserved to contain the data
    uint32_t flags;			// EXT2 behavior
    union {
        struct {
            uint32_t  l_i_reserved1;
        } linux1;
        struct {
            uint32_t  h_i_translator;
        } hurd1;
        struct {
            uint32_t  m_i_reserved1;
        } masix1;
    } osd1; /* OS dependent 1 */
    uint32_t block[15];		// Block pointers. Last 3 are indirect
    uint32_t generation;	// File version
    uint32_t file_acl;		// Block # containing extended attributes
    uint32_t dir_acl;
    uint32_t faddr;			// Location of file fragment
    union {
        struct {
            uint8_t	l_i_frag;	/* Fragment number */
            uint8_t	l_i_fsize;	/* Fragment size */
            uint16_t	i_pad1;
            uint16_t	l_i_uid_high;	/* these 2 fields    */
            uint16_t	l_i_gid_high;	/* were reserved2[0] */
            uint32_t	l_i_reserved2;
        } linux2;
        struct {
            uint8_t	h_i_frag;	/* Fragment number */
            uint8_t	h_i_fsize;	/* Fragment size */
            uint16_t	h_i_mode_high;
            uint16_t	h_i_uid_high;
            uint16_t	h_i_gid_high;
            uint32_t	h_i_author;
        } hurd2;
        struct {
            uint8_t	m_i_frag;	/* Fragment number */
            uint8_t	m_i_fsize;	/* Fragment size */
            uint16_t	m_pad1;
            uint32_t	m_i_reserved2[2];
        } masix2;
    } osd2;				/* OS dependent 2 */
} __attribute__((packed));

struct ext2_dir_entry_2 {
    uint32_t inode;			// Inode
    uint16_t rec_len;		// Total size of entry, including all fields
    uint8_t name_len;		// Name length, least significant 8 bits
    uint8_t file_type;		// Type indicator
    uint8_t name[];
} __attribute__((packed));

struct ext2_fs_descriptor
{
    uint32_t block_size;
    struct ext2_superblock header;
    struct ext2_inode root_directory;
    struct ext2_block_group_descriptor group_descriptor;
    struct disk_stream* cluster_read_stream;
    struct disk_stream* directory_stream;
};

struct ext2_file_descriptor
{
    struct ext2_inode* inode;
    uint32_t position;
};

struct filesystem* ext2_initialize();

#endif