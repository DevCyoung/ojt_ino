#pragma once
#include <InnoOJTCore.h>
#include <TimeManager.h>
#include <PanelUIManager.h>
#include <LogListUI.h>

#define INNO_CLIENT_FRAME_PER_SECOND 120.f
#define INNO_GRAPH_HISTORY_SECOND 10.f
#define INNO_FRAME_DELTA_TIME (1.f / INNO_CLIENT_FRAME_PER_SECOND)

#define ZS_POS "MS (m)"
#define ZS_SPEED "MS (m/s)"
#define ZS_ACC "MS (m/s^2)"

#define ZU_POS "MU (m)"
#define ZU_SPEED "MU (m/s)"
#define ZU_ACC "MU (m/s^2)"


#define ZR "Road (m)"

#define X_POS "PosX (m)"

#define X_SPEED "SpeedX (m/s)"

#define X_POS_OTHER "Other PosX (m)"

#define INNO_MS "MS"
#define INNO_MU "MU"
#define INNO_KS "KS"
#define INNO_CS "CS"
#define INNO_KT "KT"
#define INNO_SPEED "Speed"
#define INNO_SAMPLINT_TIME "Sampling Time"
#define INNO_BUMP_START "Bump start"
#define INNO_BUMP_END "Bump end"
#define INNO_BUMP_AMP "Bump amp"


#define INNO_MS_TOOLTIP "Sprung Mass, Unit: kg"
#define INNO_MU_TOOLTIP "UnSprung Mass, Unit: kg"
#define INNO_KS_TOOLTIP "Spring rate, Unit: N/m"
#define INNO_CS_TOOLTIP "Damper rate, Unit: N sec/m"
#define INNO_KT_TOOLTIP "Tire Stiffness, Unit: N/m"
#define INNO_SPEED_TOOLTIP "Car Speed, Unit: m/sec"
#define INNO_SAMPLING_TIME_TOOLTOP "Sampling Time, Unit: sec"
#define INNO_BUMP_START_TOOLTIP "Bump Start, Unit: m"
#define INNO_BUMP_END_TOOLTIP "Bump Start, Unit: m"
#define INNO_BUMP_AMP_TOOLTIP "Bump Amp"