struct udpMessage
{
unsigned char nVersion;
unsigned char nType;
unsigned char nMsgLen;
unsigned long lSeqNum;
char chMsg[1000];
};
