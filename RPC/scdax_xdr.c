/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "scdax.h"

bool_t
xdr_message (XDR *xdrs, message *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->msg, MSGSIZE))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->msg_size))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->out_msg, MSGSIZE))
		 return FALSE;
	return TRUE;
}
