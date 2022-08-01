# VMP-Hwid-Decoder
 Decoder for VMProtect hwids

## Example

```cpp
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
```

## Console output
```
FNxgtsXADfyDZ9xSikSrsNJPqKyujoTMdgLvwd769V3uozna:
   Processor -> B660DC14
   Host -> FC0DC0C4
   Disk -> 52DC6780
   Network -> B0AB4488
   Network -> ACA84FD0
   Network -> CC848EAC
   Network -> C1EF0274
   Network -> 5DF5FADC
   Network -> DA39A3EC
```

## VMProtect output
![image](https://user-images.githubusercontent.com/57635764/182196767-d294a5bb-dd57-42e6-9e62-7134d8a436b2.png)
