#ifndef DATANODE_H
#define DATANODE_H


class Datanode {


  private:

	unsigned char *buf; // length set by constructor [RS800CX_MAX_PACKET_SIZE]
	int len;


  public:

	Datanode(unsigned char*, int);

	void setBuffer(unsigned char*);
	unsigned char* getBuffer(void);

	void setBuffer(unsigned char* buf, int len);

	void setBufLen(int);
	int getBufLen(void);

};
#endif /* DATANODE_H */