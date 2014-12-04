#ifndef API_WRAPPER_H
#define API_WRAPPER_H

/* drop warning */
#ifdef ReportEvent
#undef ReportEvent
#endif

#import "mscorlib.tlb"
#import "CNCUSBControllerAPI.tlb"

#include <stdint.h>

using namespace CNCUSBControllerAPI;

class CEventHandler : IDispatch
{
public:
	CEventHandler(void);
	~CEventHandler(void);
	uint32_t mRef;
	ICNCPtr mObject;
	DWORD mEventCookie;
	IConnectionPoint *mConPoint;

	int ConnectSink(void);
	int DisconnectSink(void);

	void(*pEvtCallBack)(void);

	STDMETHOD_(ULONG, AddRef)()
	{
		InterlockedIncrement(&mRef);
		return mRef;
	}

	STDMETHOD_(ULONG, Release)()
	{
		InterlockedDecrement(&mRef);
		if (0 == mRef)
		{
			delete this;
			return 0;
		}
		return mRef;
	}
	STDMETHOD(QueryInterface)(REFIID riid, void ** ppvObject)
	{
		if (riid == IID_IUnknown)
		{
			*ppvObject = (IUnknown*)this;
			AddRef();
			return S_OK;
		}

		if ((riid == IID_IDispatch) || (riid == __uuidof(ICNCEvents)))
		{
			*ppvObject = (IDispatch*)this;
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}
	STDMETHOD(GetTypeInfoCount)(UINT* pctinfo)
	{
		if (pctinfo)
		{
			*pctinfo = 0;
		}
		return E_NOTIMPL;
	}

	STDMETHOD(GetTypeInfo)(UINT itinfo, LCID lcid, ITypeInfo** pptinfo)
	{
		if (pptinfo)
		{
			*pptinfo = 0;
		}
		return E_NOTIMPL;
	}

	STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgdispid)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(Invoke)(DISPID dispidMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pdispparams, VARIANT* pvarResult, EXCEPINFO* pexcepinfo, UINT* puArgErr)
	{

		if (1 == dispidMember && pEvtCallBack)
		{
			pEvtCallBack();
		}
		return S_OK;
	}
};

#endif //API_WRAPPER_H