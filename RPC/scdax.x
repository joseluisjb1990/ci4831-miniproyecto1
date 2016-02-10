/*		scdax.x			*/

const RCVBUFSIZE = 32;   /* Size of receive buffer */
const OUTBUFSIZE = 160;
const LETTERSIZE = 26;
const BACONSIZE  = 6;
const MSGSIZE = 2048;

struct message
{
	string msg<MSGSIZE>;
	int msg_size;
	string out_msg<MSGSIZE>;
};

program SCDAX_PROG {
	version SCDAX_VERS {
		int ENCRYPT_MSG(message)=1;
		int DECRYPT_MSG(message)=2;
	}=1;
}= 0x31111112;
