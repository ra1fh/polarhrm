#ifndef _OVERVIEW_H
#define _OVERVIEW_H


class Overview {

	
  private:

  protected:
		


  public:

	int number_of_sessions; // the values should be private but for code compatillity they are now set to public
    int used_bytes;


	Overview(){};
	Overview(int, int);
		
	int getSessionNumber(void);
	void setSessionNumber(int);	
		
    int getUsedBytes(void);
	void setUsedBytes(int);
		
};
#endif
