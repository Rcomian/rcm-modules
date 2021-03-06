
# RCM plugins

![pack](/res/screenshot.png?raw=true "pack")

## Piano Roll

A monophonic quantized sequencer. With `1v/oct`, `gate`, `retrigger` & `velocity` outputs. 64 patterns with up to 16 measures per pattern, up to 16 beats per measure and up to 16 divisions per beat. `EOP` will trigger when the last note in the pattern has been triggered. Patterns always loop (currently).

#### Controls

* Left click a cell to activate that note.
* Right click an active note to cause it to retrigger.
* Hold down shift then drag a note up & down to alter the velocity, when dragging, press ctrl for more fine-grained control.
* Use the panel on the right to alter the current pattern and its make-up.
* Send a pulse to clock-in to advance each step (no internal clock).
* Send a pulse to reset to prepare to play the current pattern from the start (plays 1st note on receiving first clock).
* Click the area above the notes to set the play position. Triggers the notes at that position. You can drag across.
* CV input for pattern selection is based on 1V/oct pitch information. Send C4 for first pattern, C#4 for second, D4 for third, etc.
* Click the area below the notes to switch the current measure.
* Hold down a measure for 1 second to lock that measure. Playing will now loop around that measure only.
* Hold down in the measure area for 1 second to unlock measures again.

#### Clock sync - "Run" input

The following inputs are generally for clock synchronisation: `clk`, `reset`, `run`.

Note that the `run` input will toggle the run state whenever it receives a trigger - it doesn't actually know if the clock is running or not.

The module defaults to the "Running" state. If your clock is paused when you hook it up, you will need to manually click the "Running" indicator to switch it into "Paused" mode so that it matches the clock.

When the module is "Paused", `clk` inputs are ignored.

#### Right click menu

* Patterns and measures can be copied and pasted within the same module (not between modules).
* All notes in the current pattern can be deleted, letting you start fresh.
* You choose how many notes to show, from 1 to 5 octaves. (actually shows 1 extra note so you can more easily see which octaves are involved).
* The clock input can be delayed by a few samples if you have timing issues with switching patterns.

#### Clock Delay

If you use one piano roll to control the pattern selection for another piano roll, you may run in to timing issues. If both modules are driven from the same clock source, your main piano roll may receive a clock signal before receiving the changed pattern signal from the other module. In this situation, the first note of the original pattern will play again and then the first note of the new pattern will play on the next clock tick. To avoid this, you can artificially delay the clock processing by a few samples to ensure that the clock and the new pattern signal arrive together or the clock arrives after the pattern change signal.

In the menu you can delay clock processing by up to 10 samples. By manually editing the presets, you can delay the clock processing by up to 15 samples.

#### Recording

You can record pitch information from a source (eg, midi input) by connecting the `1v/oct`, `gate`, `rtrg` and `vel` sources to the inputs on the right hand side (`rtrg` and `vel` inputs are optional for this).

If you send a trigger to the `rec` input, the module will go into "Prerecord" mode. Once the play position wraps around to the first position in the pattern (or measure, if you've locked the current measure), then recording will start.

Send a second trigger to the `rec` input to cancel recording or the prerecord mode.

Once the end of the pattern has been reached, the module will automatically drop out of recording mode and start playing what has been recorded.

Remember your clock input can be any source, and each received trigger moves forward one division, use a manual trigger source for more fine gained control over what you're recording.

When your inputs are set up, their values are mirrored to the outputs so that you can hear what you're playing. The mirrored outputs are *not* quantised - they are just pass throughs.

The recording inputs can have other uses. For example, you could have a blank pattern in your song and use the inputs to improvise that section of the song using the same instrument that was being sequenced by the other patterns.


#### Chaining

Piano Rolls can be chained together for chords or other effects. The inputs on the left (`clk`, `reset`, `ptrn`, `run`, `rec`) are mirrored to the outputs below.

## Sync

Sync is a simplified and shrunk version of SEQ Adapter (see below).
Any incoming "RESET" triggers are held until the next "CLOCK" input unless the current CLOCK input is high, in which case the RESET is played immediately.

This should do a better job of keeping sequencers in sync with resets even while running.


## SEQ Adapter

This module fixes the behaviour of some sequencers that skip or fail to sound the first step of their sequence when their clock is started after a reset.

Simple sequencers (like SEQ-3) normally skip the first step in their sequence when you stop the clock, reset, then start the clock again. This can fail to sound the first step and possibly put the sequencer out of sync with other modules in the patch.

The `SEQ Adapter` takes the `run`, `reset` and `clock` outputs from the clock module. It then creates its own `reset` and `clock` outputs which are used by the simple sequencers. These are timed in such a way that the first steps are sounded by the sequencers and they are kept in sync with the rest of the patch.

The `SEQ Adapter` works by detecting when a reset has been triggered when the clock isn't running. When that happens, instead of forwarding the reset signal, it arms the module. When armed, the next `clock` signal received gets duplicated to the `reset` output. This has the effect that when you start the clock, the module is reset to the start of the sequence with the clock active. For most modules, this means that we start playing on the first step. SEQ-3 in this case will output the first gate. Hora drum sequencer will play the first drum step, etc.

This does rely on the sequencers handling simultaneous `reset` and `clock` inputs correctly, so your mileage may still vary.

Resetting while the patch is running should work exactly as it does now - the `reset` and `clock` inputs are simply duplicated to the outputs.

With Impromptu's Clocked module, it actually works slighly better if the `Outputs reset high when not running` option is turned off. This will ensure that SEQ-3 will sound its gate, whereas it might sometimes miss it if this option is turned on (it's on by default).


## Reverb

Based on the GVerb GPL'd reverb code.

This is a dirty, messy and unstable reverb. Makes for some interesting, if not repeatable effects.
The reset button is there because it often gets itself into a state that just needs you to stop everything and start again.
Reset only resets the engine, it doesn't reset any of the knob values.

GVerb is a mono in, stereo out reverb. I'm emulating stereo in by running 2 engines and mixing the output.
The Spread feature cannot be modified without recreating the engine, so it's fixed to 90 degrees and uses mixing to simulate a spread effect.

The output is normalised using an internal envelope following, because the output gets crazy high voltages otherwise. This follower can generate its own effects at times when things are out of hand.

## Interface 16

A reskinned version of the Core Audio 16 interface. Included solely for backward compatibility with pre v1.0 patches.

## Duck

A basic audio ducking module. Works in either stereo or mono mode.
Audio in the "over" channels are prioritised above the "under" channel.
The amount of priority and the recovery rate once the "over" channel goes silent are configurable.
