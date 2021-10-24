/*
  ==============================================================================

    Defines.h
    Created: 14 Oct 2021 9:21:36am
    Author:  Coleman Jenkins

  ==============================================================================
*/

#pragma once

#define DELAY_LENGTH_MS_MIN     50 // ms
#define DELAY_LENGTH_MS_DEFAULT 150
#define DELAY_LENGTH_MS_MAX     2000
#define DELAY_LENGTH_MS_INTERVAL 1
#define DELAY_LENGTH_MS_SKEW    0.5

#define FEEDBACK_MIN            0 // Percent
#define FEEDBACK_DEFAULT        50
#define FEEDBACK_MAX            100
#define FEEDBACK_INTERVAL       1

#define FILTER_FC_MIN           20 // Hz
#define FILTER_FC_MAX           20000
#define FILTER_FC_SKEW          0.25
#define FILTER_FC_INTERVAL      0.1

#define LOW_CUT_DEFAULT_FC      200
#define LOW_CUT_Q               0.5
#define HIGH_CUT_DEFAULT_FC     5000
#define HIGH_CUT_Q              0.5

#define DRY_WET_MIN             0 // Percent
#define DRY_WET_MAX             100
#define DRY_WET_DEFAULT         50
#define DRY_WET_INTERVAL        1

#define MATCH_LR_DEFAULT        false

#define FILTER_SUFFIX_HZ        " Hz"
#define FILTER_SUFFIX_KHZ       " kHz"
#define DELAY_SUFFIX            " ms"
#define DRY_WET_SUFFIX          " %"
#define FEEDBACK_SUFFIX         " %"

// GUI
#define UNIT_LENGTH_X           24
#define UNIT_LENGTH_Y           26

#define CONTAINER_WIDTH         24*UNIT_LENGTH_X
#define CONTAINER_HEIGHT        23*UNIT_LENGTH_Y
