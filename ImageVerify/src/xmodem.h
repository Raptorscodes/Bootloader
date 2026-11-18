#ifndef XMODEM_H
#define XMODEM_H

#include <stdint.h>

/* Receive a file into flash starting at destination.
 * Returns  0  on success (EOT received, image written)
 *         -1  if nobody answered 'C' (timeout)
 *         -2  on a bad/unexpected header mid-transfer
 */
int xmodem_recieve(uint32_t destination);

#endif