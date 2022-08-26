#pragma once
#include "Core/Misc/Delegate.h"
#include "InputType.h"


DECLARE_MULTICAST_DELEGATE_MOREPARAM(FCaptureOnMousesDelegate, void, EMouseState, int, int);
DECLARE_MULTICAST_DELEGATE_MOREPARAM(FCaptureOnMouseMoveDelegate, void, int, int);
DECLARE_MULTICAST_DELEGATE_MOREPARAM(FCaptureOnMousesWheelsDelegate, void, int, int, float);

DECLARE_MULTICAST_DELEGATE_MOREPARAM(FCaptureOnKeyDelegate, void, FInputKey);

extern FCaptureOnMousesWheelsDelegate MousesWheelsDelegate; //wheel

extern FCaptureOnMousesDelegate MouseDownDelegate; //mouse down
extern FCaptureOnMousesDelegate MouseUpDelegate;// mouse up

extern FCaptureOnMouseMoveDelegate MouseMoveDelegate;// mouse move

extern FCaptureOnKeyDelegate	KeyDownDelegate;//key down
extern FCaptureOnKeyDelegate	KeyUpDelegate;//key down