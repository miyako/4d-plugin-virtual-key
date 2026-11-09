![version](https://img.shields.io/badge/version-18%2B-EB8E5F)
![platform](https://img.shields.io/static/v1?label=platform&message=mac-intel%20|%20mac-arm%20|%20win-64&color=blue)
[![license](https://img.shields.io/github/license/miyako/4d-plugin-virtual-key)](LICENSE)
![downloads](https://img.shields.io/github/downloads/miyako/4d-plugin-virtual-key/total)

# 4d-plugin-virtual-key

The Virtual Key plugin lets 4D methods simulate native keyboard input and query whether a given key is currently pressed. On macOS it drives Quartz Event Services (`CGEventCreateKeyboardEvent` / `CGEventPost` / `CGEventSourceKeyState`); on Windows it drives the Win32 input API (`SendInput` / `GetKeyState`). It exposes two commands, `POST VIRTUAL KEY` (fire a synthetic key event) and `Test virtual key` (query a key's current state).

> Command names below follow the plugin's standard naming convention (the manifest name matches the exported C function name, with underscores read as spaces). This wasn't verified against a `manifest.json` — if you have one, confirm the exact declared names and syntax tags against it.

| Command | Returns | Purpose |
|---|---|---|
| [POST VIRTUAL KEY](#post-virtual-key) | (none) | Simulate a key press-and-release, optionally with modifier keys held |
| [Test virtual key](#test-virtual-key) | Longint | Query a virtual key's current state |

**Platforms:** macOS, Windows

## Manual release workflows

1. Run **Bump version** on an explicit branch, choosing `patch`, `minor`, or
   `major`. It validates the selected immutable commit and changes source version
   metadata only. It pushes one direct-child commit to that branch, with the
   Copilot co-author trailer. It does not create tags, dispatch Build, or publish.
2. Separately run **Build** on the desired branch. There are **zero custom
   inputs**: the Actions branch selector selects the source, and every checkout
   uses `github.sha`. Source version `X.Y.Z` determines tag/release name `vX.Y.Z`
   and assets `Virtual Key-X.Y.Z.zip` and `Virtual Key-X.Y.Z.dmg`.

The bump requires a clean checkout and a branch still at the dispatch SHA, both
before preparation and immediately before pushing. Its mandatory executable
`.github/scripts/bump-hooks/pre-push` checks the receive-pack advertisement:
exact selected old SHA, exact new local SHA/HEAD, one parent, exact target, and
exactly one branch update. Advances, rewinds, deletion, and an already-pushed
same-result commit fail closed. The non-force push disables follow-tags both in
configuration and on the command line; only the three source metadata paths may
change in the generated commit.

Build retains Windows Release/x64 with the existing `v143`/Windows SDK `10.0`
workflow overrides, then macOS `Default`, `arm64 x86_64`, deployment override
`10.13`, certificate import, credential storage, signing and strict verification,
ZIP/DMG packaging, both notarizations, and DMG stapling. Native project
toolchains/settings, the Deployment-only project signing script, resources,
dependencies, artifact name `windows-binaries`, and the six existing secret
names are unchanged. No native build, signing, notarization, 4D execution, or
Actions dispatch is needed to validate this migration.

### Version authority and migration evidence

At intake `6d8a79b73db21657b831016624ee0fcbb4a19859`, all six
`MARKETING_VERSION` settings and source `Info.plist` agree on `2.4.5`.
Rename-follow history reaches `934bdc4` (`mv`, moving the native sources out of
`Virtual Key/`): that original bump header was marketing-first/current-only
fallback, which alone would **not** authorize changing independent build metadata.
However, `1ab70929242934cbd663d212cd0aa1e4b1ba30dd` explicitly superseded it:
the new header promises the exact same version in
`MARKETING_VERSION`/**`CURRENT_PROJECT_VERSION`** and
`CFBundleShortVersionString`/**`CFBundleVersion`**. `4d0cf89` then corrected
the project path. The body still selected only marketing, leaving all six build
settings at `2.4.2` despite subsequent marketing bumps.

This migration repairs those six proven-drift build literals to the **existing**
`2.4.5`, with **no intake increment**. It also repairs the two stale `2.3`
display versions in `English.lproj/InfoPlist.strings`, which Xcode actually
copies through its variant group and Resources phase. The product name and
`Copyright 2020 miyako.` are retained verbatim. Source `Info.plist` already has
the correct short version and has **no `CFBundleVersion`**; no optional key is
added. No prebuilt bundle, 4D database/catalog, schema/manifest, localization
counter, API/dependency, native implementation, or archive is version authority.

Future bumps validate all six coordinated settings, the source plist and copied
English display strings before any write. Only strict `X.Y.Z` literals and
resolvable `$(MARKETING_VERSION)`/`${MARKETING_VERSION}` and
`$(CURRENT_PROJECT_VERSION)`/`${CURRENT_PROJECT_VERSION}` plist references are
accepted. Build settings may reference marketing, not themselves. Supported
references and absent optional build keys are preserved. Duplicate plist keys
and quoted/unquoted/inline localized duplicates are rejected. Rewrites preserve
BOM, each LF/CRLF/CR ending (including mixed endings), modes, quoting, whitespace,
copyright, and unrelated bytes.

The exact committed prior art was inspected and adapted, not copied as platform
authority: `miyako/4d-plugin-native-encoding-detection` at
`c42ca565857f674bde5ea6ed3435c87375eda4a7` (six coordinated configurations,
strict localized tokenizer, fail-closed Windows cleanup);
`miyako/4d-plugin-file-information` at
`19848973601acfd3a2b5070230fa40cdc7e4edb2` (three coordinated configurations,
UTF-16/binary built strings, explicit no-i386 gate); and the independent-build
contracts in `miyako/4d-plugin-common-string-algorithm` at
`2606a68201866ec9fcbb38b51462d79215f504ef` and corrected
`miyako/4d-plugin-window-button` at
`3f14facb9aaaaff0e22eeb8e07531163680c3a8a`.
Virtual Key's actual later coordinated header controls this repository; the
other repositories' counts, dependencies, platforms, and branch restrictions
are not imported.

### Native freshness and publication safety

The original automatic trigger was only a `v*.*.*` tag push; its manual Build
also required an existing-tag input. Both are replaced by input-free manual
Build; there were no intentional branch restrictions or unrelated workflows
to remove.

The canonical layout remains intentionally `test/Plugins/Virtual Key.bundle`.
The actual Release/x64 `OutDir` in `Virtual Key.vcxproj` is instead
`Virtual Key.4dbase/plugins/Virtual Key.bundle/contents/windows64/Virtual Key.4DX`.
Only that rebuilt Windows binary and existing intermediates are invalidated;
normalization requires that exact nonempty output, not a recursive first match.
The macOS build invalidates only its rebuilt executable, plist, English strings,
signature and intermediates, preserving other resources and the Windows
artifact. It requires both exact architecture slices and verifies the built
plist/localized versions (including Xcode's UTF-16 and compiled plist forms)
against source. Virtual Key's original workflow had no explicit i386 rejection;
no existing gate is removed or replaced by a weaker required-slices check.
Notarytool must report **Accepted** for both submissions before stapling or
publication; a zero exit with `Invalid` is not success.

Publication has one repository-wide, cross-branch concurrency group
`release-publication`, with cancellation disabled. Both before native work and
after all native/sign/notary/staple gates, it checks the exact remote tag and
authenticated, fully paginated release lists, including drafts. Authentication,
network, 404, malformed responses, and errors on any later page stop publication.
Creation is strictly ordered: POST a new ref at the selected SHA, verify that
remote ref, POST a **new public non-prerelease** release with explicit tag,
target SHA, name and original empty body/no generated notes, then POST each
asset to the newly returned release ID. A concurrent same-SHA tag creation is
still a conflict. There is no reuse, edit, deletion, clobber, rollback, or retry.

**Recovery:** on any error, all remote state is retained. Inspect the reported
tag/SHA, release ID and uploaded assets (a failed or malformed response may
already have created state). A release failure can leave just the tag; an upload
failure can leave the public release and earlier assets. Do not retry expecting
repair or automatic cleanup, and do not delete/reuse/clobber that version through
these workflows. Resolve partial publication deliberately outside automation;
use a separately reviewed source bump and fresh version for a new publication.

Offline checks (including real disposable bare-Git races, mocked API payloads
and retention, closed-PATH native stubs, and PowerShell file-only checks):

```sh
actionlint .github/workflows/*.yml
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest discover -s .github/tests -v
```

The same complete suite must pass on live metadata and a disposable post-bump
copy; fixture versions derive from source rather than pinning future releases.
PowerShell (`pwsh`) is required to exercise the Windows shell tests.

---

## Requirements & platform notes

- Both commands take a **native OS virtual-key code**, not a 4D key name — 4D itself does no translation. You need the platform's own key-code constant for the key you want (macOS: `kVK_*` constants from `Carbon/HIToolbox`; Windows: `VK_*` constants from `winuser.h`). The two constant sets use different numeric values for the same physical key, so code that hardcodes a key code is not portable across platforms as-is — branch on platform if you need the same logical key on both.
- **Key-code validation:** `POST VIRTUAL KEY`'s key-code parameter is checked to fit an unsigned 16-bit value (0–65535) before use. A value outside that range makes the command silently do nothing — no 4D error is raised. This validation is a source-level fix; confirm it's present in whatever compiled binary you're actually running before relying on it. `Test virtual key` currently has **no equivalent validation** on its key-code parameter.
- **macOS Accessibility permission:** posting synthetic HID events via `CGEventPost` typically requires the host application (4D) to be granted Accessibility permission (System Settings → Privacy & Security → Accessibility) on modern macOS. This is an OS-level restriction, not something the plugin checks or reports — if `POST VIRTUAL KEY` appears to silently do nothing on a given Mac, check this first. (Exact minimum macOS version for this requirement isn't independently confirmed here — treat this as a strong likelihood, not a verified fact.)
- **Command (⌘) modifier is not honored on Windows.** The modifier bitmask includes a bit for the Command key, and it's respected on macOS — but the Windows code path has no equivalent (no `VK_LWIN`/`VK_RWIN` press/release). Setting that bit has no effect at all on Windows.
- **Function (fn) modifier is macOS-only by design**, not a gap — Windows has no OS-level "fn" modifier to simulate, so there's nothing to wire up there.
- **Caps Lock is a documented no-op on macOS.** `POST VIRTUAL KEY` special-cases the Caps Lock key code and currently does nothing for it on macOS (the source contains only commented-out experimental code, with a comment noting the author wasn't sure how to simulate it). On Windows, Caps Lock is posted like any other key.
- **`Test virtual key` returns platform-divergent semantics** — see the caveat in that command's Description section below. Don't assume parity between the two platforms for this command without checking your target OS.

---

## POST VIRTUAL KEY

### Syntax

```4d
POST VIRTUAL KEY(keyCode ; modifiers)
```

| Parameter | Type | Description |
|---|---|---|
| `keyCode` | Longint | Native OS virtual-key code to simulate (macOS: a `CGKeyCode`/`kVK_*` constant; Windows: a `VK_*` constant). Must fit in an unsigned 16-bit value (0–65535); out-of-range values are silently ignored. |
| `modifiers` | Longint | Bitmask of modifier keys to hold down for the duration of the key event. See the bit table below. |
| Result | — | This command does not return a value. |

**Modifier bit positions** (set bit *n* by adding 2^n to `modifiers`):

| Bit | Modifier | Platform |
|---|---|---|
| 0 | Function (fn) | macOS only — has no effect on Windows |
| 8 | Command (⌘) | macOS only — currently has no effect on Windows |
| 9 | Shift (left) | Both |
| 11 | Option/Alt (left) | Both |
| 12 | Control (left) | Both |
| 13 | Shift (right) | Both |
| 14 | Option/Alt (right) | Both |
| 15 | Control (right) | Both |

### Description

`POST VIRTUAL KEY` posts a key-down event immediately followed by a key-up event for `keyCode`, with whichever modifier bits are set in `modifiers` held down for both.

**On macOS**, this is built from Quartz Event Services: any requested modifiers are combined into a `CGEventFlags` mask and attached to the key-down/key-up pair via `CGEventSetFlags`; if the Function bit is set, a separate fn key-down/key-up pair is posted around the main event. Events are posted to `kCGHIDEventTap`, i.e. at the same level as physical hardware input.

**On Windows**, each requested modifier is sent as its own separate `SendInput` call — first all the modifier key-downs (in Control → Option → Shift order), then the main key down/up pair, then the modifier key-ups (in reverse order). This means the modifiers and the main key are **not** part of a single atomic input batch the way they are on macOS; a very fast‑polling foreground application could theoretically observe the modifiers and main key as more separated events. In practice this is unlikely to matter for typical use, but it's a real behavioral difference worth knowing about if you're driving something latency-sensitive.

There is no way to detect from 4D whether the simulated key event was actually delivered (e.g. blocked by macOS Accessibility permission) — the command always returns silently, whether or not the OS actually acted on it.

### Example

```4d
// Simulate pressing "A" with no modifiers.
// Replace $vk with the actual platform key-code constant for the key you want
// (e.g. macOS kVK_ANSI_A, Windows VK_A) — this plugin does no key-name lookup itself.
C_LONGINT($vk;$mods)
$vk:=0  // fill in the real platform key code
$mods:=0
POST VIRTUAL KEY($vk;$mods)
```

```4d
// Simulate the same key with left Shift + left Control held (bits 9 and 12).
C_LONGINT($vk;$mods)
$vk:=0  // fill in the real platform key code
$mods:=(2^9)+(2^12)
POST VIRTUAL KEY($vk;$mods)
```

```4d
// Loop-posting the same key is fine to call repeatedly — there's no persistent
// OS handle kept open between calls (each invocation opens and releases its own
// event source), so there's no accumulating resource cost from calling this
// command many times in a row.
C_LONGINT($vk;$i)
$vk:=0  // fill in the real platform key code
For ($i;1;5)
	POST VIRTUAL KEY($vk;0)
End for
```

---

## Test virtual key

### Syntax

```4d
$down:=Test virtual key(keyCode)
```

| Parameter | Type | Description |
|---|---|---|
| `keyCode` | Longint | Native OS virtual-key code to query (same constant sets as `POST VIRTUAL KEY`'s `keyCode`). No range validation is applied to this parameter in the current source. |
| Result | Longint | `1` or `0`, with **platform-divergent meaning** — see below. |

### Description

**On macOS**, the result comes from `CGEventSourceKeyState`, which reports whether the key is *currently physically held down* at the moment of the call — `1` if pressed, `0` if not.

**On Windows**, the result comes from `GetKeyState(keyCode) & 1`. This is a caveat worth reading carefully: Win32's `GetKeyState` packs two independent pieces of state into its return value — the **high-order bit** indicates whether the key is currently physically down, and the **low-order bit** indicates whether the key is *toggled on* (a concept that's only meaningful for toggle keys like Caps Lock, Num Lock, and Scroll Lock). Masking with `& 1` reads the **toggle** bit, not the press bit. In practice this means:
- For Caps Lock / Num Lock / Scroll Lock, the Windows result tells you whether the toggle is currently *on* — which is a reasonable, if different, piece of information.
- For an ordinary key (a letter, Shift, Enter, etc.), the toggle bit is not meaningful, and `Test virtual key` will typically report `0` on Windows **regardless of whether the key is actually being held down** — this does not currently match the macOS "is it physically pressed" semantics for non-toggle keys.

If you need consistent "is this key currently pressed" behavior across both platforms, this divergence should be resolved at the source level (reading the high bit on Windows instead) before relying on this command for anything beyond toggle-key state on Windows specifically.

### Example

```4d
// Query Caps Lock's toggle state. This is meaningful on both platforms today,
// since Caps Lock is a toggle key.
C_LONGINT($vk;$down)
$vk:=0  // fill in the real platform key code for Caps Lock
$down:=Test virtual key($vk)
If ($down=1)
	ALERT("Caps Lock is on")
End if
```

```4d
// Caution: for a non-toggle key (e.g. Shift), this currently behaves
// differently per platform (see Description above) — don't assume parity.
C_LONGINT($vk;$down)
$vk:=0  // fill in the real platform key code
$down:=Test virtual key($vk)
Case of
	:($down=1)
		ALERT("Key reports pressed/toggled-on")
	:($down=0)
		ALERT("Key reports not pressed/not toggled")
End case
```

---

## Error handling & troubleshooting

- **`POST VIRTUAL KEY` never raises a 4D error, on any input.** It has no return value and no way to signal failure — an out-of-range key code, a blocked macOS permission, or a successful post all look identical from the calling 4D method's point of view.
- **Out-of-range key codes are silently ignored, not clamped or corrected.** Passing a `keyCode` outside 0–65535 to `POST VIRTUAL KEY` makes the command do nothing at all. (This validation is a source-level fix — confirm your build includes it before relying on the behavior.)
- **The Command (⌘) modifier bit has no effect on Windows.** If a method sets bit 8 expecting a Windows-key-equivalent, nothing will happen there — this is currently a platform gap, not a bug in your calling code.
- **macOS Accessibility permission can silently block all posted events.** If key simulation appears to do nothing on a given Mac, check whether 4D (or the host app it's embedded in) has been granted Accessibility permission before assuming a code-level problem.
- **Caps Lock is a known no-op on macOS** in `POST VIRTUAL KEY` — this is intentional (the underlying OS-level Caps Lock simulation isn't implemented), not a bug to work around in your own code.
- **`Test virtual key`'s result means different things on the two platforms for non-toggle keys.** See the platform-divergence caveat above — treat this command as reliable for toggle-key state (Caps Lock, Num Lock, Scroll Lock) on both platforms, and as macOS-only-reliable for "is this key currently held down" until/unless the Windows branch is changed to read the high-order bit instead of the low-order bit.

---

## Quick reference

```4d
// Post a plain key
C_LONGINT($vk)
$vk:=0
POST VIRTUAL KEY($vk;0)

// Post a key with Shift+Control held (bits 9 and 12)
C_LONGINT($vk;$mods)
$vk:=0
$mods:=(2^9)+(2^12)
POST VIRTUAL KEY($vk;$mods)

// Query a key's state (reliable cross-platform only for toggle keys today)
C_LONGINT($vk;$down)
$vk:=0
$down:=Test virtual key($vk)
```

### Remarks

On Mac, privacy access must be granted.

<img width="573" alt="Screen Shot 2020-02-21 at 7 01 27" src="https://user-images.githubusercontent.com/1725068/74982777-13031800-5478-11ea-9210-019c216ae263.png">