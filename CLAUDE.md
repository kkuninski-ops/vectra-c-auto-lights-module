# CLAUDE.md

Read `HANDOFF-bg.md` first. It holds the owner's requirements, the decisions taken so far and why, the current status and the blocker.

## Project in one paragraph

An inline module for a 2008 Opel Vectra C (D1S xenon, CD30 MP3 + GID, retrofit `AUTO` headlight switch). It sits between the factory light-switch connector and the switch, via an adapter harness, so no factory wiring is cut. It acts only in `AUTO`. When the engine is running, it **emulates the manual "low beam" switch position**, so the factory BCM/UEC/REC drives the xenon, sidelights, dimming and indicators. It is configured over BLE from a phone.

## Hard rules

- Never design anything that directly powers D1S ballasts, sidelights, fogs or rear lamps.
- Never write to CAN to control lights. CAN is read-only, except a possible future GID overlay.
- Fail-safe: a dead or unplugged module must leave the car behaving exactly as factory.
- Manual switch positions always win over the module.
- Xenon must not light on ignition alone. It lights right after engine-running is detected, so the leveling test stays visible.
- Do not finalize the switch output stage or the PCB until the switch connector has been measured (`hardware/switch-measurement-worksheet-bg.md`).
- Treat the original `kkuninski-ops/vectra-c-auto-lights` repo and the Russian ~2007 repair guide as unverified hints.

## Conventions

- Docs are in Bulgarian. Technical terms, identifiers, filenames and commands stay in English.
- The owner is a beginner. Explain steps plainly and in safe order (bench → read-only in the car → outputs).
- Firmware: hardware-independent logic lives in `firmware/src/AutoLightsController.*` and has host tests. Run `cd firmware && make test` after any logic change. ESP32 build: PlatformIO, `firmware/platformio.ini`.
