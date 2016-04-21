#ifndef Globals_h__
#define Globals_h__
#define STREAM_VERSION QDataStream::Qt_5_5
namespace MtgGlobals {
    enum BbComCodes : qint32{
        ReqID = 87
        , RequestCompleted
        , SendRequest
        , GetResult
    };
    enum {
        BbComPort = 1222
    };
    typedef quint32 BlockSizeType;
}
#endif // Globals_h__
