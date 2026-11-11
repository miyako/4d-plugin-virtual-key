#import <Carbon/Carbon.h>
#import <Cocoa/Cocoa.h>
#include <cstring>

// Force-included for every actual source, including the complete bundled SDK.
@interface OwnershipURL : NSObject
- (id)initWithString:(NSString *)value;
- (NSString *)absoluteString;
@end
@interface OwnershipImage : NSObject
- (id)initWithCGImage:(CGImageRef)image size:(NSSize)size;
- (NSData *)TIFFRepresentation;
@end
CFURLRef OwnershipCreateURL(CFAllocatorRef, CFStringRef, CFURLPathStyle, Boolean);
CFStringRef OwnershipCopyPath(CFURLRef, CFURLPathStyle);
CGEventSourceRef OwnershipEventSource(CGEventSourceStateID);
CGEventRef OwnershipKeyboardEvent(CGEventSourceRef, CGKeyCode, bool);
void OwnershipEventFlags(CGEventRef, CGEventFlags);
void OwnershipEventPost(CGEventTapLocation, CGEventRef);
bool OwnershipKeyState(CGEventSourceStateID, CGKeyCode);
void OwnershipLog(NSString*, ...);
void OwnershipRelease(CFTypeRef);
#define NSURL OwnershipURL
#define NSImage OwnershipImage
#define CFURLCreateWithFileSystemPath OwnershipCreateURL
#define CFURLCopyFileSystemPath OwnershipCopyPath
#define CGEventSourceCreate OwnershipEventSource
#define CGEventCreateKeyboardEvent OwnershipKeyboardEvent
#define CGEventSetFlags OwnershipEventFlags
#define CGEventPost OwnershipEventPost
#define CGEventSourceKeyState OwnershipKeyState
#define NSLog OwnershipLog
#define CFRelease(value) OwnershipRelease(value)

// Only the Windows dispatcher is compiled against these synthetic input types.
typedef unsigned short WORD;
struct INPUT {
    unsigned type;
    struct { WORD wVk; unsigned dwFlags; } ki;
};
enum {
    INPUT_KEYBOARD = 1, KEYEVENTF_KEYUP = 2,
    VK_LCONTROL = 0xa2, VK_RCONTROL = 0xa3, VK_LMENU = 0xa4, VK_RMENU = 0xa5,
    VK_LSHIFT = 0xa0, VK_RSHIFT = 0xa1
};
unsigned OwnershipSendInput(unsigned, INPUT*, int);
short OwnershipGetKeyState(int);
#define SendInput OwnershipSendInput
#define GetKeyState OwnershipGetKeyState
#define ZeroMemory(p, n) std::memset(p, 0, n)
