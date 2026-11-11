# Native ownership audit

## Provenance and complete native inventory

Baseline: `4020ff3efca8222980b366b96ce0eead4a4df599`. Guidance read through the
GitHub contents API: `miyako/skills/4d-skills/skills/4dplugin/SKILL.md` at immutable
`e6b492fcb81e5e1a7f7db723594b420e63bf6eb9`. The SDK headers identify themselves as
`rev : v13`; that is a bundled-file label, not a verified upstream release.
No API implementation, engine/ABI header, project, manifest, binary or existing
release workflow is changed.

All tracked native production implementations are the dispatcher `4DPlugin.cpp`,
`4D Plugin API/4DPluginAPI.c`, and the 16 `Classes/*.cpp` below. Each wrapper has
its corresponding header. Both Xcode's Sources phase and all four MSBuild
Debug/Release x Win32/x64 configurations include all 18 implementation files.
There are no additional uncompiled tracked native implementations. Compiled
membership does not imply public-command reachability.

| Wrapper (in `4D Plugin API/Classes`) | Underlying lifetime / disposition |
|---|---|
| `ARRAY_BOOLEAN` | Owns `CboolArray*`; prohibit implicit shallow copy |
| `ARRAY_DATE` | Owns `CDateArray*`; prohibit implicit shallow copy; contained `C_DATE` values stay copyable |
| `ARRAY_INTEGER` | Owns `CShortArray*`; prohibit implicit shallow copy |
| `ARRAY_LONGINT` | Owns `CIntArray*`; prohibit implicit shallow copy |
| `ARRAY_REAL` | Owns `CDoubleArray*`; prohibit implicit shallow copy |
| `ARRAY_TEXT` | Owns `CUTF16StringArray*`; getters copy borrowed UTF-16 data before mutation; accepted output strings transfer to arrays; prohibit implicit shallow copy |
| `ARRAY_TIME` | Owns `CIntArray*`; prohibit implicit shallow copy |
| `C_BLOB` / internal `CBytes` | Wrapper owns `CBytes*`; prohibit wrapper copy; internal vector is independently copyable |
| `C_DATE` | Scalar date fields, no heap owner; unchanged |
| `C_INTEGER` | Scalar short, no heap owner; unchanged |
| `C_LONGINT` | Scalar int, no heap owner; used by both public commands; unchanged |
| `C_PICTURE` / internal `CPicture` | Wrapper owns `CPicture*`; helper owns picture reference and `CBytes*`; fix command arguments/returns, creation reference and conversion units; prohibit shallow copies of both classes |
| `C_POINTER` | Borrows pointer; use the existing SDK reference constructor, not an owning value getter |
| `C_REAL` | Scalar double, no heap owner; unchanged |
| `C_TEXT` | Owns `CUTF16String*`; copy already deleted in baseline; scalar content setters and independent output storage unchanged |
| `C_TIME` | Scalar seconds, no heap owner; unchanged |

`CUTF8StringReplaceString` is the only free wrapper string helper; it mutates
value-owned strings and is unchanged. `C_TYPES.h` declares STL string/vector
aliases and imports platform declarations. `4DPlugin.h` declares the two command
handlers; `4D Plugin_Prefix.pch` imports Carbon. `4DPluginAPI.def` exports only
`FourDPackex`; that SDK entry installs `gCall4D` and calls `PluginMain`, then the
dispatcher. The harness compiles that complete SDK, not extracted functions.

Both root and bundled resource `manifest.json` declare exactly
`POST VIRTUAL KEY(&L;&L)` and `Test virtual key(&L):L`, both thread-safe.
There are no public text/scalar-string/array/object/collection parameters.
The dispatcher only instantiates `C_LONGINT`. Picture, pointer, blob and array
methods are compiled shared infrastructure but dormant from these commands.
The v13 SDK and wrappers have no object/collection getter surfaces to repair;
the host reports their balances as **n/a**, not zero tested owners.

Exact protected bundled SDK Git blobs (checked before every harness build):

| File in `4D Plugin API` | Git blob |
|---|---|
| `4DPluginAPI.c` | `478081f0238f6bc5393222f574bc2656f010b67b` |
| `4DPluginAPI.h` | `9054b45ffc7ff6f5b8d57529a3788a55d977b314` |
| `EntryPoints.h` | `730f2f77ff2713fb2fe4b7d6562baeedf5f664cb` |
| `Flags.h` | `10a83185333dac06a62dda61b9f9428243d1e2ee` |
| `PrivateTypes.h` | `77053dc0c0a3718d2d3b6f158371a6af78c6ae66` |
| `PublicTypes.h` | `2bb392e73e9c556c274fe0a75b3a6b83e310b77c` |
| `4DPluginAPI.def` | `996ecc4ab11ad165de9192eba2e7478db141c757` |

Relevant baseline wrapper implementation blobs: `C_PICTURE.cpp`
`73da2f1a502863d0c06638092f4d6f455e21021d`; `C_POINTER.cpp`
`b2bcbc66a4481a4b9b1d08e09dcbe277eb621b10`; `ARRAY_TEXT.cpp`
`534855e0959020ce9a66cef08f8434799c174495`; `C_TEXT.cpp`
`45ea6432dff52b681a5c6a7cea58ad5fc15df66b`; `C_BLOB.cpp`
`a5dea70d2bb536d76af6617be70d639bd816bfec`.
All remaining original files are recoverable by the baseline SHA, and the same
final harness obtains its baseline sources with a path-limited `git archive`.
It never extracts the tracked 4D database, user preferences or packaged binaries.

## Exact source contracts and changes

Line references here are to the immutable baseline `4DPluginAPI.c`.

| SDK function | Contract / consequence |
|---|---|
| `PA_CreateUnistring`, 244; `PA_DisposeUnistring`, 252 | Creation forwards UTF-16 content to the engine; disposal forwards the owned descriptor and zeros it |
| `PA_SetUnistring`, 263; `PA_ReturnString`, 2262 | Engine-backed content replacement, not a struct-transfer array setter; wrapper scalar output remains independent |
| `PA_CreatePicture`, 380; `PA_DuplicatePicture`, 447 | Creation owns a reference; retainOnly=1 forwards retain (engine parameter zero), as the SDK comment states |
| `PA_GetPictureVariable`, 3730 | Explicitly borrowed from the variable; retain before clearing its owning argument |
| `PA_CreateVariable`, 3523 | Allocates initial empty picture/string/array element owners; shallow setters do not release these |
| `PA_SetStringVariable`, 3826; `PA_SetPictureVariable`, 3852 | Shallow assignment; use Undefined initialization before adopting a distinct owned value; clear exactly once or explicitly detach a borrowed/handoff value |
| `PA_ClearVariable`, 3947 | Engine-backed variable cleanup, also resets the SDK error |
| `PA_GetStringInArray`, 4266 | Borrowed element alias; `ARRAY_TEXT` independently copies content before any array replacement |
| `PA_SetStringInArray`, 4444 | Checks Unicode-array kind, non-null handle, index 0 through count inclusive and non-null descriptor; disposes old element then shallow-transfers new descriptor; rejection retains caller ownership |
| `PA_ConvertCharsetToCharset`, 4776 | Comment explicitly says returned size is **bytes**, and engine forwarding uses byte capacity; divide by `sizeof(PA_Unichar)` for `CUTF16String` units |
| `PA_ExecuteCommandByID`, 5257 | Success returns an owning variable; allocation failure can leave raw fType=0; failed engine return may be unspecified; capture error before kind checks or cleanup |
| `PA_GetVariableKind`, 3681 | Normalizes raw kind 0 to string; it cannot establish that an unspecified failed return can be cleared |
| `PA_SetVariableOrFieldReference`, 3923 | Exact existing scope-to-reference kinds `{26,9,35,13}` and pointer metadata; no fetching/ownership of a pointed-to value required |

`fromFileAtPath` previously assigned its owned path over an allocated empty
string, explicitly disposed the transferred path, then cleared the same path
again. Its output picture argument was never cleared. Now the path argument
starts Undefined, owns the path until one clear, and the output picture is
retained by `setPicture` before clearing its argument. `setBytes` releases the
creation reference after `setPicture` retains it.

`createCopy` now converts the byte result to UTF-16 units. Copy/gray/size/thumbnail
input arguments that are immediately shallow-assigned start Undefined instead
of abandoning newly allocated empty pictures. Existing independently retained
copy/gray handoffs and detached borrowed size/thumbnail inputs are preserved.

All five command helpers use one local helper that captures `PA_GetLastError`
immediately after the exact SDK call, clears only a successful return, and
never examines or clears a failed unspecified descriptor. There is no new
exception, worker throw, error channel or success-shaped fallback. Legacy
behavior on command failure remains: path can adopt the initial empty output,
copy/gray can hand back their retained input, size reads its initialized zero
arguments, and thumbnail can hand back its initial empty picture. These are
existing failure-reporting gaps, not promises of successful conversion.
Argument cleanup follows each argument's known ownership independently of
whether the synchronous command succeeded.

`C_POINTER::getVariable` previously fetched an owning value, copied its payload
union as reference metadata, and leaked the value. The variable-reference case
now delegates to the bundled SDK helper; the field branch stays unchanged.

Ten heap-owning types had implicit copy construction/assignment. Named baseline
sanitizer probes demonstrate use-after-free, rather than inferring a problem
from pointers alone. The fixed types delete copy/assignment, matching baseline
`C_TEXT` prior art. No production copy call was found; these are latent hazards,
not demonstrated command crashes. Full actual-consumer compilation includes
`ARRAY_DATE`'s required copies of value-owned `C_DATE`, which stay supported.
For the `CPicture` copy probe alone an extra picture reference isolates its
raw `CBytes` double-delete from the earlier duplicate picture-release error.

## Reproducible synthetic evidence

Run on macOS with installed Apple clang:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 .github/tests/test_ownership.py
```

`OWNERSHIP_LOG=/absolute/path/results.json` optionally retains every leaf command's
arguments, exit status, stdout and sanitizer diagnostic. The same final test
files and assertions compile both live actual sources and the immutable
baseline source archive. No SDK replacement or source rewriting is used.
Local compiler: Apple clang 21.0.0, arm64-apple-darwin25.6.0.
ASan and UBSan are enabled with `halt_on_error=1`; alignment checking alone is
disabled for the SDK's intentionally packed ABI structs. LeakSanitizer is
disabled on macOS; explicit allocation and picture-reference maps instead
require final string/picture/handle balances of zero for every passing leaf.
Synthetic event/source CF-memory references also balance to zero.

| Named leaf group | Cases per source | Fixed | Baseline |
|---|---:|---|---|
| Picture path/copy text | 16 | Pass | Fail: path double cleanup/extra reference, ASCII copy heap-buffer-overflow, other copies unbalanced owners |
| Picture bytes/gray/size/thumbnail | 4 | Pass | Fail: creation reference or abandoned/returned owners |
| Array transfer, borrowed copy, churn, replace, shrink, clear, Undefined destination and rejection | 2 | Pass | Pass controls |
| Scalar content setter and post-wrapper lifetime | 8 | Pass | Pass controls |
| Five picture helpers x handle/command failure x raw-zero/poisoned return kind | 20 | Pass | Fail: double cleanup or abandoned initial owners |
| SDK failure predicates | 1 | Pass | Pass control; demonstrates resize inconsistency, not OOM safety |
| Five helpers x successful Undefined/string/picture return | 15 | Pass | Fail: leaked or doubly cleared owners |
| Pointer variable reference, all four scopes | 1 | Pass | Fail: fetched owning value |
| Pointer field metadata | 1 | Pass | Pass control |
| Ten heap-owner copy probes | 10 | Noncopy traits pass | Ten named ASan heap-use-after-free failures |
| Existing `C_TEXT` noncopy / value-and-borrowed copy traits | 2 | Pass | Pass controls |
| Actual macOS and synthetic Windows-branch dispatchers | 2 | Pass | Pass controls |
| **Total** | **82** | **82 pass** | **66 expected failures, 16 passing controls** |

Text fixtures are ASCII, BMP U+65E5, supplementary U+1F600 (two UTF-16 units),
and empty text, each with allocated and null engine empty-string representations.
The converter checks byte capacities and terminator boundaries and returns
bytes, never convenient unit counts. Scalar outputs survive wrapper destruction.
Array tests include accepted element zero and rejected negative/oversized indices,
wrong kind, null data handle and null descriptor, without freeing transferred
strings or losing rejected owned input.

Before **any** native compilation, the force-included platform header maps every
actual input operation (`CGEventSourceCreate`, keyboard event creation, flags,
posting, key-state lookup, `SendInput`, `GetKeyState`) to in-memory stubs.
Path/URL and native image boundaries are also replaced. Only Foundation/CF
memory/string operations are real; no input is captured or posted. Unexpected
engine selectors abort. An import gate rejects real input/event/TIS/keymap,
device, file/path, process and UI boundaries before launching either executable.
Synthetic dispatch assertions cover key state return, invalid-key guards,
maximum key, modifier order/flags, function-key wrapping, macOS Caps Lock no-op,
unknown selector no-op, and event/source cleanup.

The second dispatcher compiles the **actual Windows conditional branch** against
minimal synthetic INPUT/Win32 declarations while linking the unchanged macOS SDK
and all actual macOS wrapper consumers. This is stronger than treating a no-op
Mac branch as Windows evidence, but is **not** a Windows compiler/SDK/4D ABI test.
No real 4D host, native input, packaged plugin or Windows crash was exercised.

## Deferred contracts and limits

`PA_GetPictureData`'s successful metadata-string ownership is not stated by this
bundled SDK. `_GetCount` and `loadImageOfType` are inventoried, but adding disposal
would assume ownership. They remain unchanged pending an authoritative v13
engine contract. The harness returns an enumeration error and poison descriptor
there; it proves failed metadata is not touched, not successful metadata cleanup.

The exact SDK updates an array's logical count even when handle resize fails.
The predicate test verifies unchanged physical size and changed logical count,
then repairs **only its synthetic descriptor** for cleanup. It does not certify
array wrappers safe under OOM. Handle allocation/resize failure preservation,
including `CBytes::toParamAtIndex` disposing an old handle before new allocation,
and converter/creation allocation failures need separately specified engine
contracts and error behavior. No protected SDK rewrite or speculative rollback
is included. Successful fixture cases do not establish OOM safety.

## Build and automation boundaries

Inspected all existing workflows, release/source-version scripts, bump hook,
Xcode phases/settings and MSBuild configurations before validation. Xcode's
Deployment phase signs, packages and notarizes; project outputs also point into
tracked bundle trees. None was invoked. Existing Build and Bump workflows are
manual-only and unchanged. No release, tag, signing, notarization, packaging,
publication, manual dispatch, rerun or dependency installation was performed.

The new read-only `Native ownership` workflow runs only on relevant path pushes
to `master` and exact audit branch `miyako-virtual-key-ownership`, using
`macos-latest`, a full-history checkout for the immutable baseline and the
unsigned synthetic test command. It uploads nothing and never invokes the
project build/release scripts.
