#include "file_man.h"
#include "string.h"
#include <unistd.h>

int run_file_manager(int argc, char const *argv[]) {
  char line[128], cmd[16], pathname[64];
  char *rootdev;
  if (argc > 1)
    rootdev = (char *)argv[1];
  fs_init();
  mount_root(rootdev);
  while (1) {
    printf("P%d running: ", running->pid);
    printf("input command : ");
    fgets(line, 128, stdin);
    line[strlen(line) - 1] = 0;
    if (!line[0])
      continue;
    sscanf(line, "%s %s", cmd, pathname);
    printf("ECHO: cmd=%s pathnam=%s\n", cmd, pathname);
  }
}

int fs_init() {
  int i, j;
  // initialize all minodes as PROC_FREE
  for (i = 0; i < NUM_MINODES; i++)
    minode_arr[i].refCount = 0;
  // initialize mtable entries as PROC_FREE
  for (i = 0; i < NUM_MENTRIES; i++)
    mentry_arr[i].dev = 0;
  // initialize PROCs
  for (i = 0; i < NUM_PROCS; i++) {
    proc_arr[i].status = PROC_FREE;
    proc_arr[i].pid = i;
    // P0 is a superuser process
    proc_arr[i].uid = i;
    // initialize PROC file descriptors to NULL
    for (j = 0; j < NUM_FD; j++)
      proc_arr[i].fd[j] = 0;
    proc_arr[i].next = &proc_arr[i + 1];
  }
  // circular list
  proc_arr[NUM_PROCS - 1].next = &proc_arr[0];
  // P0 runs first
  running = &proc_arr[0];
  return 0;
}

minode *mialloc()
// allocate a PROC_FREE minode for use
{
  int i;
  for (i = 0; i < NUM_MINODES; i++) {
    minode *mp = &minode_arr[i];
    if (mp->refCount == 0) {
      mp->refCount = 1;
      return mp;
    }
  }
  printf("FS panic : out of minodes\n");
  return 0;
}
int midalloc(minode *mip) // release a used minode
{
  mip->refCount = 0;
  return 0;
}

// get_block/put_block functions:
// We assume that a block device, e.g. a real or virtual disk, can
// only be read or written in unit of block size. For real disks, this is due to
// hardware constraints. For virtual disks, we assume that read/write is
// als0 by
// block size, so that the code can be ported to real disks if desired. For
// a virtual disk, we first open it for R|W mode and use the file descriptor
// as the device number. The following functions read/write a virtual disk
// block into/from a buffer area in memory.

int get_block(int dev, int blk, char *buf) {
  lseek(dev, blk * BLKSIZE, SEEK_SET);
  int n = read(dev, buf, BLKSIZE);
  if (n < 0)
    printf("get_block[% d % d] error \n", dev, blk);
  return 0;
}

int put_block(int dev, int blk, char *buf) {
  lseek(dev, blk * BLKSIZE, SEEK_SET);
  int n = write(dev, buf, BLKSIZE);
  if (n != BLKSIZE)
    printf("put_block [%d %d] error\n", dev, blk);
  return 0;
}

// iget(dev, ino) function:
// This function returns a pointer to the in-memory minode containing the
// INODE of (dev, ino). The returned minode is unique, i.e. only one copy of the
// INODE exists in memory. In a real file system, the returned minode is locked
// for exclusive use until it is either released or unlocked. For
// simplicity,we
// shall assume that minode locking is unnecessary, which will
// be explained later.

minode *iget(int dev, int ino) {
  minode *mip;
  mount_entry *mp;
  inode *ip;
  int i, block, offset;
  char buf[BLKSIZE];
  // serach in-memory minodes first
  for (i = 0; i < NUM_MINODES; i++) {
    minode *mip = &minode_arr[i];
    if (mip->refCount && (mip->dev == dev) && (mip->ino == ino)) {
      mip->refCount++;
      return mip;
    }
  }
  mip = mialloc();
  mip->dev = dev;
  mip->ino = ino;
  block = (ino - 1) / 8 + iblock;
  offset = (ino - 1) % 8;
  get_block(dev, block, buf);
  ip = (inode *)buf + offset;
  mip->inode = *ip;
  // initialize minode
  mip->refCount = 1;
  mip->mounted = 0;
  mip->dirty = 0;
  mip->mntPtr = 0;
  return mip;
}

// iput(INODE *mip) function:
// This function releases a used minode pointed by mip. Each
// minode has a refCount, which represents the number of users that are using
// the minode. iput() decrements the refCount by 1. If the refCount is non -
// zero, meaning that the minode still has other users, the caller simply
// returns. If the caller is the last user of the minode (refCount 1⁄4 0), the
// INODE is written back to disk if it is modified (dirty).
int iput(minode *mip) {
  inode *ip;
  int i, block, offset;
  char buf[BLKSIZE];
  if (mip == 0)
    return 0;
  mip->refCount--;
  if (mip->refCount > 0)
    return 0;
  if (mip->dirty == 0)
    return 0;
  block = (mip->ino - 1) / 8 + iblock;
  offset = (mip->ino - 1) % 8;
  // get block containing this inode
  get_block(mip->dev, block, buf);
  ip = (inode *)buf + offset;
  *ip = mip->inode;
  put_block(mip->dev, block, buf);
  midalloc(mip);
  return 0;
}

// getino() function:
// The getino() function implements the file system tree traversal algorithm.
// It returns the INODE number (ino) of a specified pathname. To begin with, we
// assume that in the level-1 file system implementation, the file system
// resides on a single root device, so that there are no mounted devices and
// mounting point crossings. Mounted file systems and mounting point
// crossing will be considered later in level-3 of the file system
// implementation Thus,
// the getino() function essentially returns the (dev, ino) of a pathname. The
// function first uses the tokenize() function to break up pathname into
// component strings. We assume that the tokenized strings are in a
// global data
// area, each pointed by a name[i] pointer and the number of
// token strings is nname. Then it calls the search() function
// to search for the token strings in
// successive directories. The following shows the tokenize() and search()
// functions.

int tokenize(char *pathname) {
  char *s;
  strcpy(gline, pathname);
  nname = 0;
  s = strtok(gline, "/");
  while (s) {
    name[nname++] = s;
    s = strtok(0, "/");
  }
  return 0;
}

int search(minode *mip, char *name) {
  int i;
  char *cp, temp[256], sbuf[BLKSIZE];
  dir_entry *dp;
  for (i = 0; i < 12; i++) { // search dir_entry direct blocks only
    if (mip->inode.i_block[i] == 0)
      return 0;
    get_block(mip->dev, mip->inode.i_block[i], sbuf);
    dp = (dir_entry *)sbuf;
    cp = sbuf;
    while (cp < sbuf + BLKSIZE) {
      strncpy(temp, dp->name, dp->name_len);
      temp[dp->name_len] = 0;
      printf("%8d%8d%8u %s\n", dp->inode, dp->rec_len, dp->name_len, temp);
      if (strcmp(name, temp) == 0) {
        printf("found %s : inumber = %d\n", name, dp->inode);
        return dp->inode;
      }
      cp += dp->rec_len;
      dp = (dir_entry *)cp;
    }
  }
  return 0;
}

int getino(char *pathname) {
  minode *mip;
  int i, ino;
  if (strcmp(pathname, "/") == 0) {
    return 2; // return root ino = 2
  }
  if (pathname[0] == '/')
    mip = root; // if absolute
  else
    mip = running->cwd; // if relative
  mip->refCount++;

  tokenize(pathname);

  for (i = 0; i < nname; i++) {
    // search for each component string
    if (!S_ISDIR(mip->inode.i_mode)) { // check DIR type
      printf("%s is not a directory\n", name[i]);
      iput(mip);
      return 0;
    }
    ino = search(mip, name[i]);
    if (!ino) {
      printf("no such component name %s\n", name[i]);
      iput(mip);
      return 0;
    }
    iput(mip);
    // release current minode
    mip = iget(dev, ino);
    // switch to new minode
  }
  iput(mip);
  return ino;
}

// Use of getino()/iget()/iput():
//  In a file system, almost every operation begins with a pathname,
// e.g. mkdir pathname, cat pathname, etc. Whenever a pathname is specified,
// its inode must be loaded into memory for reference. The pattern of using
// an inode
//. ino = getino(pathname);
//. mip = iget(dev, ino);
// . use mip->inode, which may modify the inode;
// . iput(mip);