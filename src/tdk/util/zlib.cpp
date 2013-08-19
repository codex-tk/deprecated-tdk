#include <tdk/util/zlib.hpp>
#include <zlib.h>

namespace tdk {
namespace util {    

static const int ZLIB_CHUNK_SIZE = 16384;

bool zlib::compress( const tdk::buffer::memory_block& in , tdk::buffer::memory_block& out ) {
    z_stream strm;
    uint8_t out_chunk[ZLIB_CHUNK_SIZE];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    int ret = deflateInit(&strm , 1);
    if (ret != Z_OK)
        return false;
    
    strm.avail_in = in.length();
    strm.next_in = in.rd_ptr();
    do {
        strm.avail_out = ZLIB_CHUNK_SIZE;
        strm.next_out = out_chunk;
        ret = deflate( &strm , Z_FINISH );
        if ( ret == Z_STREAM_ERROR || ret == Z_BUF_ERROR ) {
            deflateEnd(&strm);
            return false;
        }
        out.write( out_chunk , ZLIB_CHUNK_SIZE - strm.avail_out );
    } while ( strm.avail_out == 0 );
    deflateEnd( &strm );
    return true;
}

bool zlib::uncompress( const tdk::buffer::memory_block& in , tdk::buffer::memory_block& out ) {
    z_stream strm;
    uint8_t out_chunk[ZLIB_CHUNK_SIZE];

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = 0;

    int ret = inflateInit(&strm);
    if (ret != Z_OK)
        return false;
    
    strm.avail_in = in.length();
    strm.next_in = in.rd_ptr();
    do {
        strm.avail_out = ZLIB_CHUNK_SIZE;
        strm.next_out = out_chunk;
        ret = inflate( &strm , Z_NO_FLUSH );
        switch( ret ) {
            case Z_NEED_DICT:
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                inflateEnd( &strm );
                return false;
        }
        out.write( out_chunk , ZLIB_CHUNK_SIZE - strm.avail_out );
    } while ( strm.avail_out == 0 );
    inflateEnd( &strm );
    return true;
}

}}

