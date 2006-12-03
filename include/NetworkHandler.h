/*
 * ============================================================================
 *  Nazwa    : SiecHandler z SiecHandler.h
 *  Projekt  : BeGadu
 *  Authorzy : 
 *		Artur Wyszynski <artur.wyszynski@bellstream.pl>
 *  Opis:
 *		Handler sieci
 *  Version  : 1.2
 * ============================================================================
 */

#ifndef __BEGADU_NETWORKHANDLER_H__
#define __BEGADU_NETWORKHANDLER_H__

/* zewnetrzne klasy, includowane w zrodle */
class Network;
class MainWindow;

class NetworkHandler
	{
	public:
		NetworkHandler( Network* aNetwork, int id, int fd, int cond, void *data );
		void Run();
		void Stop();
		
		void HandleEvent( struct gg_event *event );
		void HandleEventConnected( struct gg_event *event );
		void HandleEventConnFailed( struct gg_event *event );
		void HandleEventMsg( struct gg_event *event );
		void HandleEventUserlist( struct gg_event *event );
		void HandleEventNotify( struct gg_event *event );
		void HandleEventNotify60( struct gg_event *event );
		void HandleEventStatus( struct gg_event *event );
		void HandleEventStatus60( struct gg_event *event );
		void HandlePingTimeoutCallback(  time_t &pingTimer );
		Network* GetNetwork() const;
		
		int				iId;
		int				iFd;
		int				iCond;
		void 		*	iData;
		volatile bool 	iDie;
		Network		*	iNetwork;
	
	private:
		friend	int32	HandlerThread( void* );
		volatile int	iThreadID;
	};

static int Expired( time_t timer );
static void Rearm( int seconds );

#endif /* __BEGADU_NETWORKHANDLER_H__ */
