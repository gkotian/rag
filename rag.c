/*
Copyright (c) 2012 Gautam Kotian

This file is part of "Random Address Generator".

"Random Address Generator" is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

"Random Address Generator" is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with "Random Address
Generator". If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef __cplusplus
extern "C" {
#endif

/*=================================================================================================
         HEADER FILES
=================================================================================================*/
#include "rag.h"
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

/*=================================================================================================
         LOCAL DATA STRUCTURES
=================================================================================================*/
typedef enum
{
    ADDR_TYPE_MAC,
    ADDR_TYPE_IPV4,
    ADDR_TYPE_IPV6

} ADDR_TYPE_T;

/*=================================================================================================
         LOCAL FUNCTION PROTOTYPES
=================================================================================================*/
static void getRandomAddress(ADDR_TYPE_T, U8 *, U8);

/*=================================================================================================
         LOCAL FUNCTION DEFINITIONS
=================================================================================================*/
static void getRandomAddress(ADDR_TYPE_T addrType, U8 *pAddrBuf, U8 bufLen)
{
    S32 fp;
    U8  i = 0;

    do
    {
        if ((pAddrBuf == NULL) || (bufLen <= 0))
        {
            break;
        }

        fp = open("/dev/urandom", O_RDONLY);

        while (i < bufLen)
        {
            read(fp, &pAddrBuf[i], sizeof(pAddrBuf[i]));
            if (pAddrBuf[i] <= 0xF)
            {
                /* because I couldn't be bothered to figure out how to convert a
                 * '0' to '00'
                 * '1' to '01'
                 *  ...
                 * and so on. */
                continue;
            }

            i++;
        }

        close(fp);
    }
    while(0);
}

/*=================================================================================================
         GLOBAL FUNCTION DEFINITIONS
=================================================================================================*/
void getRandomMacAddress(U8 *pMacAddrBuf, U8 bufLen)
{
    do
    {
        getRandomAddress(ADDR_TYPE_MAC, pMacAddrBuf, bufLen);

        /* Discard multicast MAC addresses (where LSB of the first byte is 1). */
        if (pMacAddrBuf[0] & 0x1)
        {
            continue;
        }

        /* The probability of getting an all ones (broadcast) or all zeroes MAC address is so low
         * that I won't even bother with these checks. */

        break;
    }
    while(1);
}

void getRandomIpv4Address(U8 *pIpv4AddrBuf, U8 bufLen)
{
    U32 ipv4AddrVal;

    do
    {
        getRandomAddress(ADDR_TYPE_IPV4, pIpv4AddrBuf, bufLen);
        ipv4AddrVal = pIpv4AddrBuf[3];
        ipv4AddrVal = (ipv4AddrVal << 8) | pIpv4AddrBuf[2];
        ipv4AddrVal = (ipv4AddrVal << 8) | pIpv4AddrBuf[1];
        ipv4AddrVal = (ipv4AddrVal << 8) | pIpv4AddrBuf[0];

#if 0 /* Check taken from 'IPAddressCheckFunction' */
        if ((IN_CLASSA(ipv4AddrVal) &&
                (ipv4AddrVal & 0x7f000000) &&
                ((ipv4AddrVal & 0x7f000000) != 0x7f000000) &&
                (ipv4AddrVal & IN_CLASSA_HOST) &&
                ((ipv4AddrVal & IN_CLASSA_HOST) != 0x00ffffff)) ||
            (IN_CLASSB(ipv4AddrVal) &&
                (ipv4AddrVal & IN_CLASSB_HOST) &&
                ((ipv4AddrVal & IN_CLASSB_HOST) != 0x0000ffff)) ||
            (IN_CLASSC(ipv4AddrVal) &&
                (ipv4AddrVal & IN_CLASSC_HOST) &&
                ((ipv4AddrVal & IN_CLASSC_HOST) != 0x000000ff)))
#else /* Check taken from 'IPPrefixCheckFunction' */
         if ((IN_CLASSA(ipv4AddrVal) &&
                (ipv4AddrVal & 0x7f000000) &&
                ((ipv4AddrVal & 0x7f000000) != 0x7f000000)) ||
             (IN_CLASSB(ipv4AddrVal)) ||
             (IN_CLASSC(ipv4AddrVal)))
#endif
         {
             /* Whew!! Its a valid random IPv4 address. */
             break;
         }
    }
    while(1);
}

void getRandomIpv6Address(U8 *pIpv6AddrBuf, U8 bufLen)
{
    do
    {
        getRandomAddress(ADDR_TYPE_IPV6, pIpv6AddrBuf, bufLen);

        /* TODO: validate the IPv6 address. */

        break;
    }
    while(1);
}

const U8 * macAddrToStr(const U8 *pMacAddrBuf, U8 *pMacAddrStr)
{
    do
    {
        if ((pMacAddrBuf == NULL) || (pMacAddrStr == NULL))
        {
            pMacAddrStr = NULL;
            break;
        }

        sprintf((char *)pMacAddrStr,
                "%02X:%02X:%02X:%02X:%02X:%02X",
                pMacAddrBuf[0], pMacAddrBuf[1], pMacAddrBuf[2],
                pMacAddrBuf[3], pMacAddrBuf[4], pMacAddrBuf[5]);

        pMacAddrStr[MAC_ADDRSTRLEN - 1] = '\0';
    }
    while(0);

    return pMacAddrStr;
}

const U8 * ipv4AddrToStr(const U8 *pIpv4AddrBuf, U8 *pIpv4AddrStr)
{
    struct sockaddr_in ipv4AddrStruct;
    U32                ipv4AddrVal;

    do
    {
        if ((pIpv4AddrBuf == NULL) || (pIpv4AddrStr == NULL))
        {
            pIpv4AddrStr = NULL;
            break;
        }

        ipv4AddrVal = pIpv4AddrBuf[3];
        ipv4AddrVal = (ipv4AddrVal << 8) | pIpv4AddrBuf[2];
        ipv4AddrVal = (ipv4AddrVal << 8) | pIpv4AddrBuf[1];
        ipv4AddrVal = (ipv4AddrVal << 8) | pIpv4AddrBuf[0];

        ipv4AddrStruct.sin_addr.s_addr = ipv4AddrVal;

        inet_ntop(AF_INET, &(ipv4AddrStruct.sin_addr), pIpv4AddrStr, INET_ADDRSTRLEN);
    }
    while(0);

    return pIpv4AddrStr;
}

const U8 * ipv6AddrToStr(const U8 *pIpv6AddrBuf, U8 *pIpv6AddrStr)
{
    struct sockaddr_in6 ipv6AddrStruct;

    do
    {
        if ((pIpv6AddrBuf == NULL) || (pIpv6AddrStr == NULL))
        {
            pIpv6AddrStr = NULL;
            break;
        }

        memcpy(ipv6AddrStruct.sin6_addr.s6_addr, pIpv6AddrBuf, sizeof(ipv6AddrStruct.sin6_addr.s6_addr));

        inet_ntop(AF_INET6, &(ipv6AddrStruct.sin6_addr), pIpv6AddrStr, INET6_ADDRSTRLEN);
    }
    while(0);

    return pIpv6AddrStr;
}

/*=================================================================================================
         MAIN FUNCTION
=================================================================================================*/
int main(int argc, char **pArgv)
{
    do
    {
        if (ragLaunchGui(&argc, pArgv) != OK)
        {
            printf("RAG: failed to launch GUI\n");
            break;
        }
    }
    while(0);

    return 0;
}

#ifdef __cplusplus
}
#endif

