#pragma once

#define FALSE                    0
#define TRUE                     1
#define NULL           ((void *)0)

/* INET_ADDRSTRLEN & INET6_ADDRSTRLEN are already defined in 'netinet/in.h' */
#define MAC_ADDRSTRLEN          18 /* To accommodate all bytes in "XX:XX:XX:XX:XX:XX" format */

/* The entire value represented by a particular alphabet is stored in one byte. So the last
 * alphabet corresponds to the number of bytes needed. */
#define MAC_ADDRBUFLEN           6 /* AA:BB:CC:DD:EE:FF ---> F is last, so 6. */
#define INET_ADDRBUFLEN          4 /* AAA.BBB.CCC.DDD ---> D is last, so 4. */
#define INET6_ADDRBUFLEN        16 /* AABB:CCDD:EEFF:GGHH:IIJJ:KKLL:MMNN:OOPP ---> P is last, so 16. */

typedef enum
{
    OK,
    ERROR
} STATUS;

typedef int             S32;
typedef short           S16;
typedef char             S8;
typedef unsigned int    U32;
typedef unsigned short  U16;
typedef unsigned char    U8;
typedef unsigned char  BOOL;

STATUS ragLaunchGui(void);
void getRandomMacAddress(U8 *, U8);
void getRandomIpv4Address(U8 *, U8);
void getRandomIpv6Address(U8 *, U8);
const U8 * macAddrToStr(const U8 *, U8 *);

