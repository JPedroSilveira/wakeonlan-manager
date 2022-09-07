#ifndef ELECTOR_HEADER
#define ELECTOR_HEADER

/**************************************************
 * Broadcast election to the network
 * - Send requests to let other machines know that
 * the election is happening
 * Answers (Alive) to the election message.
 * Winner sends (Victory) message to others.
 * Winner is decided by PID (The highest becomes the
 * new Coordinator))
 **************************************************/
#include <stdio.h>
#include <sys/socket.h> 


void AnnounceElection ();

void ElectionResponse ();

void AnnounceWinner ();

#endif