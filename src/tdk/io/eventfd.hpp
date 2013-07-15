#include <sys/eventfd.h>

namespace tdk {
namespace io {

class eventfd {
public:
    eventfd( void );
    ~eventfd( void );

    bool set( void );
    void reset( void );

    int handle( void );
private:
    int _event_fd;
};


}}
