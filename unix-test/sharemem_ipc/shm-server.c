/* shm_server.c : the time server using shared memory, a bizarre application  */
#include	<stdio.h>
#include	<sys/shm.h>
#include	<time.h>
#include        <stdlib.h>
#include        <unistd.h>
#include        <string.h>

#define	TIME_MEM_KEY	99	/* like a filename      */
#define	SEG_SIZE	((size_t)100)	/* size of segment      */

main()
{
  long now;
  int n;
  /* create a shared memory segment */
  int seg_id = shmget(TIME_MEM_KEY, SEG_SIZE, IPC_CREAT | 0777);

  /* attach to it and get a pointer to where it attaches */
  char *mem_ptr = shmat(seg_id, NULL, 0);

  /* run for a minute */
  for (n = 0; n < 60; n++) {
    time(&now);			/* get the time */
    strcpy(mem_ptr, ctime(&now));	/* write to mem */
    sleep(1);			/* wait a sec   */
  }
  shmctl(seg_id, IPC_RMID, NULL);
}
