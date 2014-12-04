#include "stdafx.h"
#include "api_wrapper.h"
/* drop warning */
#ifdef ReportEvent
#undef ReportEvent
#endif

#import "mscorlib.tlb"
#import "CNCUSBControllerAPI.tlb"

#include <stdint.h>

using namespace CNCUSBControllerAPI;

CEventHandler::CEventHandler( void ) { mRef = 0; mObject = 0; mEventCookie = 0; pEvtCallBack = 0; }
CEventHandler::~CEventHandler( void ) { }

int CEventHandler::ConnectSink( void )
{
    IConnectionPointContainer*	pIConnectionPointContainer = NULL;
    IUnknown*					pIUnknown = NULL;

	if( !mObject )
		return S_FALSE;

    this->QueryInterface( IID_IUnknown, (void**)&pIUnknown );

    if (pIUnknown)
    {
      mObject->QueryInterface( IID_IConnectionPointContainer, (void**)&pIConnectionPointContainer );
	
      if ( pIConnectionPointContainer )
      {
	    pIConnectionPointContainer->FindConnectionPoint(__uuidof(ICNCEvents), &mConPoint);
	    pIConnectionPointContainer->Release();
	    pIConnectionPointContainer = NULL;
      }
	
      if ( mConPoint )
      {
	    mConPoint->Advise( pIUnknown, &mEventCookie );
      }
	
      pIUnknown->Release();
	  pIUnknown=NULL;
    }
	return S_OK;
}

int CEventHandler::DisconnectSink( void )
{
	if ( mConPoint )
    {
	  mConPoint->Unadvise( mEventCookie );
	  mConPoint->Release();
	  mConPoint = NULL;
    }
	return S_OK;
}
