
#ifndef __BEGADU_NETWORKHANDLER_H__
#define __BEGADU_NETWORKHANDLER_H__

class Network;
class MainWindow;

class NetworkHandler
	{
	public:
		NetworkHandler( Network* aNetwork, int fd);
		void Run();
		void Stop();

		inline Network* GetNetwork() const { return iNetwork; }
		inline int Fd() const { return iFd; }

	private:
		void HandleEvent( struct gg_event *event );
		void HandleEventConnected( struct gg_event *event );
		void HandleEventConnFailed( struct gg_event *event );
		void HandleEventDisconnected( struct gg_event *event );
		void HandleEventMsg( struct gg_event *event );
		void HandleEventUserlist( struct gg_event *event );
		void HandleEventNotify( struct gg_event *event );
		void HandleEventNotify60( struct gg_event *event );
		void HandleEventStatus( struct gg_event *event );
		void HandleEventStatus60( struct gg_event *event );
		void HandlePingTimeoutCallback(  time_t &pingTimer );

		friend	int32	HandlerThread( void* );
		volatile int	iThreadID;

		volatile bool 	iDie;

		int				iFd;
		Network		*	iNetwork;

	};

#endif /* __BEGADU_NETWORKHANDLER_H__ */
