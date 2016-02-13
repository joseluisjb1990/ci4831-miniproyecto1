/*		scdax.x			*/

const RCVBUFSIZE = 2450;
const OUTBUFSIZE = 2431;
const LETTERSIZE = 26;
const BACONSIZE  = 6;
const MES_SIZE  = 2450;
const FILE_SIZE = 2401;

struct message
{
	string msg<MES_SIZE>;
	string ip_source<MES_SIZE>;
	string out_msg<MES_SIZE>;
};

program SCDAX_PROG {
	version SCDAX_VERS {
		string ENCRYPT_MSG(message)=1;
	}=1;
}= 0x31111112;
