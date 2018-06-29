#pragma once
/**
 * @file csf_accessory_AccessoryCommon.h
 *
 * Copyright 2013, Cisco Systems
 */

#ifdef __cplusplus
extern "C"{
#endif

/**
 * AccesssoryType
 */
typedef enum {
    ACCESSORY_NONE = 0,
    ACCESSORY_HEADSET,
    ACCESSORY_HANDSET,
    ACCESSORY_SPEAKER
} Accessory_AccessoryType;

/**
 * AudioType
 */
typedef enum {
    ACCESSORY_CALL_AUDIO = 0,
    ACCESSORY_RINGER_AUDIO,
    ACCESSORY_SYSTEM_AUDIO
} Accessory_AudioType;

/**
 * EventType
 */
typedef enum {
    ACC_NONE,
    ACC_MAKE_CALL,
    ACC_ANSWER_CALL,
    ACC_END_CALL,
    ACC_AUDIO_MUTE,
    ACC_AUDIO_UNMUTE,
    ACC_START_VIDEO,
    ACC_STOP_VIDEO,
    ACC_VOLUME_UP,
    ACC_VOLUME_DOWN,
    ACC_MICROPHONE_VOLUME_CHANGED,
    ACC_SPEAKER_VOLUME_CHANGED,
    ACC_REDIAL,
    ACC_HOLD,
    ACC_RESUME,
    ACC_SEND_DIGIT,
    ACC_ACTIVE_DEVICE_CHANGED,
    ACC_VOICEMAIL,
    ACC_AUDIO_DISCONNECTED,
    ACC_SET_DEFAULT_AUDIO_PATH,
    ACC_REJECT_CALL
} Accessory_EventType;

/**
 * Phone Connection Status
 */
typedef enum {
    ACCESSORY_PHONE_NOT_REGISTERED = 0,
    ACCESSORY_PHONE_REGISTERING,
    ACCESSORY_PHONE_REGISTERED,
    ACCESSORY_PHONE_RECOVERY_PENDING,
    ACCESSORY_PHONE_NO_NETWORK
} Accessory_PhoneConnectionStatus;

/**
 * VideoPreference
 */
typedef enum {
    ACCESSORY_VIDEO_DIRECTION_INACTIVE,
    ACCESSORY_VIDEO_DIRECTION_SENDONLY,
    ACCESSORY_VIDEO_DIRECTION_RECVONLY,
    ACCESSORY_VIDEO_DIRECTION_SENDRECV
} Accessory_VideoPreference;

/**
 * Voicemail Preference
 */
typedef enum {
    ACCESSORY_NO_VOICEMAIL = 0,
    ACCESSORY_VOICEMAIL,
    ACCESSORY_VISUAL_VOICEMAIL
} Accessory_VoicemailPreference;

/**
 * Plugin Loader Result
 */
typedef enum {
	ACCESSORY_PLUGINS_SUCCESS = 0,
	ACCESSORY_PLUGINS_ERROR_NO_PLUGINS_AVAILABLE,
	ACCESSORY_PLUGINS_ERROR_INVALID_HANDLE,
	ACCESSORY_PLUGINS_ERROR_UNINITIALIZED,
	ACCESSORY_PLUGINS_ERROR_UNABLE_TO_LOAD_ALL_PLUGINS,
	ACCESSORY_PLUGINS_ERROR_INVALID_AM_PLUGIN
} Accessory_PluginLoaderResult;

#ifdef __cplusplus
}
#endif
