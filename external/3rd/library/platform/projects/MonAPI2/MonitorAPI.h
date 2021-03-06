#ifndef __MONITOR_API__
#define __MONITOR_API__

/**********************************************************************************************
		Please contact dtaylor@soe.sony.com for any comments or request for chagnes.
**********************************************************************************************


Required libs

  UdpLibrary
  zlib



**********************************  DO NOT EDIT THIS FILE  ***********************************
**********************************************************************************************
**********************************************************************************************/
#include "MonitorData.h"

class CMonitorAPI {

private:

	CMonitorData		*mMonitorData;
	UdpManager	*mManager;
	MonitorManager		*mObjectManager;
	char				*mAddress;
	unsigned short		mPort;
	bool				mbprint;

public:

	/*******************  CMonitorAPI   *************************************
	*
	*	configFile = Included in the zip file, is a file "mon.cfg". In this file is a password and
	*	             ip address's that are allowed to connect to your server. You can rename the 
	*	             file or place it in another directory. Use this param to access this file.
	*
	*	
	*	port =  use port 2200 as a default. When using more than one server on a machine, use another port, range 2200-2220
	*
	*	debug = turn on/off debugging.
	*
	*	address = (NOT USED) Leave as nullptr
	*
	*	UdpManager * mang = (NOT USED) Leave as nullptr
	*	
	*	// GenericNotifier *notifier = (NOT USED) Leave as nullptr
	*
	*/
	CMonitorAPI( const char *configFile, unsigned short port, bool debug = false , char * address = nullptr, UdpManager * mang = nullptr );
	
	~CMonitorAPI();


	/**************** Process communication from collectors ******************
	*
	*	This needs to be called in the main loop every processing cycle.
	*	Update will read the socket for request and send updates to the 
	*	collector. 
	*
	*	Note: Update is non-blocking
	*/
	void Update();


	/****************  Add element to list  **********************************
	*	
	*	label =	name of the element.
	*		Elements labels must be unique. "." is use to group labels 
	*		Example:
	*				Server.Zone 1.process
	*				Server.Zone 1.CPU
	*				Server.Zone 2.process
	*				Server.Zone 2.CPU
	*				
	*		Will show as
	*               Server -
	*                       |
	*                       -  Zone 1 -
	*                                 - process
	*                                 - CPU
	*                       |- Zone 2 -
	*                                 - process
	*                                 - CPU
	*
	*		Note: do not use "/" char.
	*
	*	Key = This id must be unigue and key use to identify the element when setting it.
	*		 Any positvie intenger can be use.
	*
	*	mode =  
	*		#define MON_HISTORY		will store values (avg 5min), you will be able to graph.
	*		#define MON_ONLY_SHOW	will not store vlaue, only for showing.
	*
	*	
	*	
	*	description = descritpion of the element. Tell us what the element means.  Hostname, BPS, Kbytes, Seconds,.....
	*
	*	Note: All values are sent to the collector every two minutes (forced). In between when values are
	*	      forced, only values that are changed will be sent to the collector.
	*
	*  ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
	*
	*	Required:
	*		One element is requried. Name the elment "Population" and in the descritpion please add the follwing,
	*       <Current version of your server> <Hostname and the port >
	*
	*	! ! ! ! ! ! ! ! ! ! ! ! ! !  ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! ! !
	*	
	*	Max Limited
	*	#define ELEMENT_MAX_START 2000
	*/
	int add( const char *label, int Key, int mode = MON_HISTORY, const char *Description = nullptr );


	/****************  Set the description of an element  **
	*
	*	Sets the description to an element. 
	*
	*	Key = element key.
	*
	*	Description = description of element.  
	*
	*	Note: Descriptions take longer to update, do not use description to 
	*		  display vital informaiton.
	*
	*/
	void setDescription( int Key, const char *Description ) ;
	

	/****************  Set the value of an element *********
	*
	*	Sets the element to the current value.
	*
	*	Key = element key.
	*
	*	value = current value to set.
	*
	*	Note: When a element is assoicated with a Server.
	*		  Use the following values to signify the status.
	*		  If you would like to add another status mode,
	*		  please contact dtaylor@soe.sony.com
	*
	*	#define STATUS_LOCKED		-1		// Server is locking out players, mataince mode
	*	#define STATUS_DOWN			-2		// Server is down.
	*	#define STATUS_NOCONNECT	-3		// Server is not connected
	*	#define STATUS_LOADING		-4		// Server is in a loading state
	*
	*/
	void set(int Key, int value);
	

	/****************  Remove element from list  ***********
	*
	*	Removes the element.
	*
	*	Key = element key
	*/
	void remove(int Key);
	

	/*****************  Debug  dump   **********************
	*
	*	Will printf a list of all the elelments.
	*	Format:
	*
	*		Label, Key, Value, Mode
	*/
	void dump();


    // **************  Do not use  *************************
    void setMax(int max){ mMonitorData->setMax(max);}

};


inline void CMonitorAPI::set(int Id, int value){ mMonitorData->set(Id,value); }

#endif


