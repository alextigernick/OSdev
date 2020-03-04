

struct fmem {
	char flags;			//open,flash,000000
	unsigned short *contents;	//dynamically intialized to file size plus free space in sector
	unsigned short cur;		//current file location
	unsigned short size;		//file size
	unsigned short bufSize;		//free space size
}

struct flock {
	fmem files[NUM_SECTORS*NUM_FILES];
}

struct fdisk {
	char name[8];			//file name, first 7 for name, last 1 for extension. Uses 4 words via byte packing.
	unsigned short size;		//file size
}

struct fat {
	fdisk fs[NUM_FILES];		//table on disk containing two file entries
}
