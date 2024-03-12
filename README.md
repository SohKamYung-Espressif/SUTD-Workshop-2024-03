# ESP-NOW Initiator and Responder Example for SUTD Workshop, March 2024

This is an example of how to use ESP-NOW to set up system with multiple initiators and a single responder.

The responder controls a 7/8 segment LED. Each initiator, once it has found the responder, turns on or off a LED segment on the responder.

## Feedback and Questions

If you have feedback or questions on this example, you can submit it as an **Issue** in this repository.

If you have questions or wish to know more about ESP-NOW, you can check and submit issues on the [Espressif ESP-NOW](https://github.com/espressif/esp-now) repository.

## Responder and Initiator in Operation

The following diagram shows what the Responder and an Initiator would do in the workshop.

![Responder and an Initiator in action](/doc/sutd_workshop_espnow.svg "Responder and an Initiator in action")
