#include <iostream>
#include "VMP-Hwid-Decoder.hpp"

int main()
{
    const char* pHwid = "FNxgtsXADfyDZ9xSikSrsNJPqKyujoTMdgLvwd769V3uozna";

    std::list < C_Component > cComponents {};
    VMP::DecodeHwid( pHwid, cComponents, DF_ALL );

    printf( "%s: \n", pHwid );

    for ( auto& cComp : cComponents )
        printf( "   %s -> %p \n", cComp.GetName( ), cComp.GetSerialHash( ) );
}
