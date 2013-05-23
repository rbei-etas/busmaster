
class ClientSink : public _IAppEvents
{
private:
DWORD       m_dwRefCount;
public:
ClientSink();
virtual ~ClientSink();

STDMETHODIMP OnMessageReceived(CAN_MSGS RxMsg);

HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **ppvObject);

ULONG STDMETHODCALLTYPE AddRef();

ULONG STDMETHODCALLTYPE Release();
};
