//i686-w64-mingw32-gcc -o preinst preinst.c -lsetupapi -static
//code in python pls

#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
// From setupapi.h
#define SPOST_PATH  1

//BOOL WINAPI _stdcall
//   SetupCopyOEMInfA(
//                    IN  PCSTR   SourceInfFileName,
//                    IN  PCSTR   OEMSourceMediaLocation,         
//                    IN  DWORD   OEMSourceMediaType,
//                    IN  DWORD   CopyStyle,
//                    OUT PSTR    DestinationInfFileName,         
//                    IN  DWORD   DestinationInfFileNameSize,
//                    OUT PDWORD  RequiredSize,                   
//                    OUT PSTR   *DestinationInfFileNameComponent 
//                   );
//
//#define SetupCopyOEMInf SetupCopyOEMInfA

int main(int argc, char **argv)
{
    int ret;

    if (argc != 2) {
        printf("\nThis program copies an INF file to the Windows INF directory.\n"
               "Usage: preinstall path-to-inf-file\n");
        return 1;
    }

	  printf("%s %s\n", argv[0], argv[1]);

    ret = SetupCopyOEMInf(argv[1],
                    NULL,
                    SPOST_PATH,
                    0,
                    NULL,
                    0,
                    NULL,
                    NULL);

    if (ret == 0) {
        LPVOID lpMsgBuf;
        int errCode = GetLastError();
        FormatMessage(
                      FORMAT_MESSAGE_ALLOCATE_BUFFER |
                      FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL,
                      errCode,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                      (LPTSTR) &lpMsgBuf,
                      0,
                      NULL
                     );
		    printf("Copying %s failed; error = 0x%08x\n%s\n", argv[1], errCode, lpMsgBuf);
        LocalFree( lpMsgBuf );
        return 1;
    }

    return 0;
}
