#pragma once
/**
 * @file csf_accessory_AccessoryCallControl.h
 *
 * Copyright 2013, Cisco Systems
 */

#ifdef __cplusplus
extern "C"{
#endif

#ifdef WIN32
#include "time.h"
#else
#include "time.h"
#endif

#include "csf_accessory_AccessoryAPIVersion.h"
#include "csf_accessory_AccessoryCommon.h"
#include "csf_accessory_AccessoryLogger.h"

#define MAX_NUMBER_LENGTH  33
#define MAX_NAME_LENGTH  50

#ifndef bool  
#define bool int   
#endif 


#ifndef true 
#define true 1   
#endif 

#ifndef false  
#define false 0  
#endif 




/**
 * Call state
 */
typedef enum {
    CALL_STATE_NONE = 0,
    CALL_STATE_ONHOOK,
    CALL_STATE_OFFHOOK,
    CALL_STATE_DIALING,
    CALL_STATE_REORDER,
    CALL_STATE_BUSY,
    CALL_STATE_REMOTE_ALERTING,
    CALL_STATE_INCOMING,
    CALL_STATE_CALLWAITING,
    CALL_STATE_CONNECTED,
    CALL_STATE_HELD
} Accessory_CallState ;

/**
 * call Direction
 */
typedef enum {
    CALL_DIRECTION_NONE = 0,
    CALL_DIRECTION_OUTGOING,
    CALL_DIRECTION_INCOMING
} Accessory_CallDirection;

/**
 * \brief
 * This structure defines call related information. 
 * 
 * Many of the call control callback functions pass a list of these structures so that the plugin
 * code has a complete record of all current calls and their states.
 */
typedef struct
{
    int                     callId;                        // call Identifier
    Accessory_CallDirection callDirection;
    Accessory_CallState     callState;     
    int                     isConference;                 // 1- indicates call is part of Conference, 0 - otherwise
    char                    remotePartyNumber[MAX_NUMBER_LENGTH]; // Remote party Number
    char                    remotePartyName[MAX_NAME_LENGTH]; // Remote party name
    time_t                  connected_time; //Time of the connected Event, will only be filled once the call is connected.
} Accessory_CallInfo_v1_0_t;

/**
 *\brief
 * A subscriber (aka "Directory Number") is a phone line. 
 *
 * It is represented in code by a structure called Accessory_Subscriber_Info which stores the name and phone number (e.g.: "John Smith", "12345666") 
 * for that phone line. Subscriber information is useful for informational purposes only and is typically consumed by a plugin for display purposes on the device, if the device supports the display of such information. 
 */
typedef struct
{
    char subscriberNumber[MAX_NUMBER_LENGTH]; // user DN
    char subscriberName[MAX_NAME_LENGTH]; // user Name or Label
} Accessory_Subscriber_Info;

/**
 * \brief
 * This struct defines the callback functions implemented by the accessories plugins.
 *
 * Accessories Provider uses these function pointers to notify the plugin about the call control and media events.
 */
typedef struct
{
   /**
    * Notify plugin that the call has been muted. This could occur when the user mute using the application, or has muted with the
    * accessory.
    * @return short  - 0 indicates successful and -1 indicates failure .
    */
    short (*onMicrophoneAudioMute) ();

   /**
    * Notify plugin that the call has been muted. This could occur when the user mute using the application, or has muted with the
    * accessory. This event is also generated at other times, for example when a call is resumed, the application will generate an
    * UNMUTE event.
    * @return short  - 0 indicates successful and -1 indicates failure .
    */
    short (*onMicrophoneAudioUnmute) ();

   /**
    * Notify plugin about video starting to send.
    * This only impacts sending video. The call may or may not be receiving video from the remote party.
    * @return short  - 0 indicates successful and -1 indicates failure .
    */
    short (*onVideoStarted) ();

   /**
    * Notify plugin about video stopping.
    * This only impacts sending video. The call may or may not be receiving video from the remote party.
    * @return short  - 0 indicates successful and -1 indicates failure .
    */
    short (*onVideoStopped) ();

   /**
    * Notify plugin about Phone OFFHOOK. This means that the call is in the offhook state - it may or may not correspond to the pysical state
    * of the accessory. For example the headset may or may not be on its cradle. In most cases the accessory indicators should be based on the 
    * the call state from events such as onConnected, onRemoteAlerting, onBusy, rather than trying to interpret the hook state.
    *
    * Accessories Manager telephone does not support the concept of dial tone, and so on/off hook is not an indication of an audible dialtone.
    * @param outgoingCallInfo - Details of call.
    * @param callInfoList[] - Array of  callinfo structure.  Ordered list of all the available calls including the outgoing call.
    * @param numCallInfo - Number of callInfoList elements in the array.
    * @return short  - 0 indicates successful and -1 indicates failure .
    * Note :- Reference to outgoingCallInfo and callInfoList array is not valid after the method returns
    */
    short (*onOffHook) (Accessory_CallInfo_v1_0_t* outgoingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo);

   /**
    * Notify plugin about REORDER event. Endcall can be invoked while the call in this state. REORDER (also known as Fast Busy) means that the 
    * other phone is unreachable or out-of-service.
    * @param outgoingCallInfo - Details of outgoing call.
    * @param callInfoList[] - Array of  callinfo structure.  Ordered list of all available calls including the outgoing call.
    * @param numCallInfo - Number of callInfo elements in the array.
    * @return short  - 0 indicates successful and -1 indicates failure .
    * Note :- Reference to incomingCallInfo and callInfoList array is not valid after the method returns
    *
    */
    short (*onReorder) (Accessory_CallInfo_v1_0_t* outgoingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo);

   /**
    * Notify plugin about BUSY event. Endcall can be invoked while the call in this state.
    * @param outgoingCallInfo - Details of outgoing call.
    * @param callInfoList[] - Array of  callinfo structure.  Ordered list of all available calls including the outgoing call.
    * @param numCallInfo - Number of callInfo elements in the array.
    * @return short  - 0 indicates successful and -1 indicates failure .
    * Note :- Reference to incomingCallInfo and callInfoList array is not valid after the method returns
    *
    */
    short (*onBusy) (Accessory_CallInfo_v1_0_t* outgoingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo);

   /**
    * Notify plugin about INCOMING call. Plugin can send RING event to device on receipt of this event.
    * @param incomingCallInfo - Details of incoming call.
    * @param callInfoList[] - Array of  callinfo structure.  Ordered list of all available calls including the incoming call.
    * @param numCallInfo - Number of callInfo elements in the array.
    * @return short  - 0 indicates successful and -1 indicates failure .
    * Note :- Reference to incomingCallInfo and callInfoList array is not valid after the method returns
    *
    */
    short (*onIncoming) (Accessory_CallInfo_v1_0_t* incomingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo);

   /**
    * Notify plugin about INCOMING call in call waiting state. Plugin can send RING event to device on receipt of this event.
    * @param incomingCallInfo - Details of incoming call.
    * @param callInfoList[] - Array of  callinfo structure.  Ordered list of all available calls including the incoming call.
    * @param numCallInfo - Number of callInfo elements in the array.
    * @return short  - 0 indicates successful and -1 indicates failure .
    * Note :- Reference to incomingCallInfo and callInfoList array is not valid after the method returns
    *
    */
    short (*onCallWaiting) (Accessory_CallInfo_v1_0_t* incomingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo);

   /**
    * Notify plugin about call CONNECTED state. At this stage call is active and Features such as mute/unmute/endcall
    * are available.
    * Following scenarios trigger this callback.
    * 1) Call State TRANSITION FROM INCOMING/ALERTING To CONNECTED
    * 2) Call State TRANSITION FROM HELD/RESUME to Connected.
    * @param connectedCallInfo - Details of Connected call.
    * @param callInfoList[] - Array of  callinfo structure.  Ordered List of all the available calls including the Connected Call.
    * @param numCallInfo - Number of callInfoList elements in the array.
    * @return short  - 0 indicates successful and -1 indicates failure.
    * Note :- Reference to connectedCallInfo and callInfoList array is not valid after the method returns
    */
    short (*onConnected) (Accessory_CallInfo_v1_0_t* connectedCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo);

   /**
    * Notify plugin about call HOLD. This is only applicable for the connected call.
    * @param heldCallInfo - Details of held call.
    * @param callInfoList[] - Array of  callinfo structure.  Ordered list of all the available calls including the held Call.
    * @param numCallInfo - Number of callInfoList elements in the array.
    * @return short  - 0 indicates successful and -1 indicates failure .
    * Note :- Reference to heldCallInfo and callInfoList array is not valid after the method returns
    */
    short (*onHold) (Accessory_CallInfo_v1_0_t* heldCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo);

   /**
    * Notify plugin about call identified by  termination Phone ONHOOK. This means that the call is in the offhook state - it may or may not correspond to the physical state
    * of the accessory. For example the headset may or may not be on its cradle. In most cases the accessory indicators should be based on the 
    * the call state from events such as onConnected, onRemoteAlerting, onBusy, rather than trying to interpret the hook state.
    *
    * Accessories Manager telephone does not support the concept of dial tone, and so on/off hook is not an indication of an audible dialtone.
    * @param endCallInfo - Details of terminated call.
    * @param callInfoList[] - Array of  callinfo structure.  Ordered list of all the available calls.
    * @param numCallInfo - Number of callInfoList elements in the array.
    * @return short  - 0 indicates successful and -1 indicates failure .
    * Note :- Reference to endCallcallInfo and callInfoList array is not valid after the method returns
    */
    short (*onOnHook) (Accessory_CallInfo_v1_0_t* endCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo);

   /**
    * Notify plugin about remote phone Alerting. Device can end the call while in remote alerting state.
    * @param outgoingCallInfo - Details of outgoing  call.
    * @param callInfoList[] - Array of  callinfo structure.  Ordered list of all the available calls including outgoing call .
    * @param numCallInfo - Number of callInfoList elements in the array.
    * @return short  - 0 indicates successful and -1 indicates failure .
    * Note :- Reference to outgoingCallInfo and callInfoList array is not valid after the method returns
    */
    short (*onRemoteAlerting) (Accessory_CallInfo_v1_0_t* outgoingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo);

   /**
    * Notify plugin(capable of volume level display) about speaker audio level change.
    * @param  audioType - Audio type would be (Ringer Audio | System Audio | Call Audio ) values {"ACCESSORY_RINGER_AUDIO", "ACCESSORY_SYSTEM_AUDIO", "ACCESSORY_CALL_AUDIO"} respectively,
    * @param  volumeLevel - Volume level can range between 0(MIN) to 100(MAX).
    * @return short  - 0 indicates successful and -1 indicates failure .
    */
    short (*onSpeakerVolumeChanged) (Accessory_AudioType audioType, int volumeLevel);

   /**
    * Notify plugin(capable of speaker volume level display) about speaker audio mute. 
    * @param  audioType - Audio type would be (Ringer Audio | System Audio | Call Audio )
    * @return short  - 0 indicates successful and -1 indicates failure .
    */
    short (*onSpeakerAudioMute) (Accessory_AudioType audioType);

   /**
    * Notify device (capable of active device type display) about the change in active device type.
    * Active device is one which is either configured or set to be default based on application policy.
    * @param  deviceType - Valid values are #ACCESSORY_HEADSET, #ACCESSORY_HANDSET, #ACCESSORY_SPEAKER
    * @param  deviceId - char pointer to the device identifier. Identifier identifies device that needs to be made active.
    * @param  deviceName - char pointer to the device Name. Gives the name fo the device for display.
    * @return short  - 0 indicates successful and -1 indicates failure .
    */
    short (*onActiveDeviceChanged) (Accessory_AccessoryType deviceType, const char* deviceId, const char* deviceName);

   /**
    * Notify plugin (capable of displaying voicemail indicators) about the presence of new (unread) voicemails.
    * @param  unreadVoicemailsPresent - is 1 if new (unread) Voicemails available for the user, is 0 if no new voicemails available.
    * @return short - 0 indicates successful and -1 indicates failure.
    */
    short (*onVoicemail) (int unreadVoicemailsPresent);

   /**
    * Set device microphone volume, if the device has a means to display the microphone volume.
    * @param  deviceId - char pointer to the device identifier. Identifier identifies device where the microphone volume needs to be adjusted
    * @param  VolumeLevel - Volume level can range between 0(MIN) to 100(MAX). Plugins can map this range to the range expected by the device.
    * @return short  - 0 indicates successful and -1 indicates failure .
    */
    short (*setDeviceMicrophoneVolume) (const char* deviceId, int volumeLevel);

   /**
    * Set device speaker volume, if the device has a means to display the speaker volume
    * @param  deviceId - char pointer to the device identifier.Identifier identifies device  where the speaker volume needs to be adjusted
    * @param  VolumeLevel - Volume level can range between 0(MIN) to 100(MAX).  Plugins can map this range to the range expected by the device.
    * @return short  - 0 indicates successful and -1 indicates failure
    */
    short (*setDeviceSpeakerVolume) (const char* deviceId, int volumeLevel);

   /**
    * Update the accessory devices with the phone registration status. This allows the accessory to indicate if there is a connection to the CUCM and if calls are possible.
    * @param  status - The new status of the connection between the CUCM and the application.
    * @return short  - 0 indicates successful and -1 indicates failure
    */
    short (*onPhoneRegistrationStatusChanged) (Accessory_PhoneConnectionStatus status);

   /**
    * Update the accessory devices with the phone registration status
    * @param  Accessory_Subscriber_Info - List of lines available for the user with details like Name & Number(DN). The first element is the default Line.
    * @param  numberOfLines - Number of Lines available for the user, 0 - means no line registered.
    * @return short  - 0 indicates successful and -1 indicates failure
    */
    short (*notifySubscriberInfo) (Accessory_Subscriber_Info* subscriberInfoList[], int numberOfLines);

   /**
    * For future use. Will update the accessory device LCD with the dial number as and when entered with digits, and before the call is initiated.
    * @param  Accessory_Subscriber_Info - List of lines available for the user with details like Name & Number(DN). The first element is the default Line.
    * @param  numberOfLines - Number of Lines available for the user, 0 - means no line registered.
    * @return short  - 0 indicates successful and -1 indicates failure
    */
    short (*onDialDigitsEntered) (const char* dialDigits);

   /**
    * Set the locale of the plugin. The locale is represented by a string in either LL or LL_CC form where LL is a two
    * letter code for language and CC is a two letter code for the territory
    * @param locale a posix compliant locale.
    * @return - 0 indicates successful and -1 indicates failure when the specified locale is not available
    */
    short (*setLocale) (const char* locale);

} Accessory_CallControlCallback_v1_0_t;


/**
 * \brief
 * The API for call control and media APIs available to the plugin.
 *
 * Each API is a function pointer which can be invoked on corresponding event from device.
 * Most of the functions require \c deviceId, which is a platform specific identifier.
 */
typedef struct
{
    /**
     * Fetches all the calls.
     * @param callInfoList - pointers to array of preallocated callInfo structures. Caller should ensure allocation of this
     * parameter.
     * @param numCallInfo - Number of elements in callInfoList.
     * @return int - -1 indicates error, 0 indicates no existing calls and any +ve value indicates number of existing calls.
     * If the number of "existing calls" exceed numCallInfo then the return value will still be total count of existing calls and
     * the array of callInfo struct will be filled to it's capacity, i.e. it will have numCallInfo elements.
     * Note :- callInfoList can be safely deallocated after this method returns
     */
    int (*getAllCallInfo) (Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo);

    /**
     * Future functionality. Fetches all voicemails.
     * @param  numberOfNewVoicemails - Number of new Voicemails(un-Read) available for the user, 0 - if no new voicemails available.
     * @return short  - 0 indicates successful and -1 indicates failure.
     */
    short (*getVoicemails) (int * numberOfNewVoicemails);

    /**
     * Fetches the Subscribed user DN's. Currently AM only supports phones with one line, and so only one Accessory_Subscriber_Info
     * will be returned in this list.
     * @param subscriberInfoList - pointers to array of pre allocated Accessory_Subscriber_Info strucutres. Caller should ensure allocation of this
     * parameter.
     * @param  numberOfLines - Max Number of Subscriber Lines that needs to be fetched.
     * @return int - Currently returns 0.
     */
    int (*getSubscriberInfo) (Accessory_Subscriber_Info* subscriberInfoList[], int numberOfLines);

    /**
     * This function
     * will indicate to the application that the users wishes to make a call and the application will present a UI that 
     * allows dialing.
     *
     * Currently the digits parameter is not used. In future it will allow for accessories that have
     * a UI that allows digits to be entered on the accessory.
     * @param  deviceId - identifier for the accessory that went off-hook
     * @param  digits - Array of dialed digits. This should be empty string or NULL in case no digits are dialed
     * @return short  - 0 indicates successful and -1 indicates failure
     * Since the actual operation of this method is asynchoronous in nature, the true success of
     * this method can only be confirmed on receipt of either onRemoteAlerting or onConnected event.
     * So the failure of this method indicates, "failure of preconditions for this operation"
     */
    short (*makeCall) (const char* deviceId, const char* digits);

    /**
     * This function answers the call. The first call in "INCOMING" state, if one exists,  will be answered.
     * @param  deviceId - Identifier for the accessory that answered the call.
     * @return short  - 0 indicates successful and -1 indicates failure.
     * Since the actual operation of this method is asynchoronous in nature, the true success of
     * this method can only be confirmed on receipt of onConnected event.
     * So the failure of this method indicates, "failure of preconditions for this operation"
     */
    short (*answerCall) (const char* deviceId);

    /**
     * Use this function to answer the call in "INCOMING" state using the specified type of device.
     * @param  DeviceType - Valid values are #ACCESSORY_HEADSET, #ACCESSORY_HANDSET, #ACCESSORY_SPEAKER
     * @return short  - 0 indicates successful and -1 indicates failure
     * Since the actual operation of this method is asynchoronous in nature, the true success of
     * this method can only be confirmed on receipt of onConnected event.
     * So the failure of this method indicates, "failure of preconditions for this operation"
     */
    short (*answerCallWithDeviceType) (Accessory_AccessoryType deviceType);

    /**
     * This function will end a call. This function will act on a call in "CONNECTED"/"ALERTING"/"DIALING" state.
     * @param  deviceId - identifier for the accessory that is ending the call.
     * @return short  - 0 indicates successful and -1 indicates failure. This opeartion Failure indicates "precondition failure".
     * Since the actual operation of this method is asynchoronous in nature, the true success of
     * this method can only be confirmed on receipt of onHook event.
     * So the failure of this method indicates, "failure of preconditions for this operation"
     */
    short (*endCall) (const char* deviceId);

    /**
     * This function will pass DTMF digits to the CONNECTED call.
     * @param  deviceId - identifier for the accessory that is sending the digit.
     * @param  digit - DTMF Digit to be sent. valid list of digits are 0-9, #, *, A, B,
     * C, D
     * @return short  - 0 indicates successful and -1 indicates failure
     * Note :- callInfo can be safely deallocated after this method returns
     */
    short (*sendDigit) (const char* deviceId, const char digit);

    /**
     * Use this function to mute the microphone audio of the call. This is performed the application's media
     * stack and so the call will send no audio even if the accessory's own mic has not been disabled. It
     * is only possible on a connected call. It should only be invoked from the active device.
     * @param  deviceId - identifier for the accessory that is sending mute.
     * @return short  - 0 indicates successful and -1 indicates failure
     */
    short (*microphoneAudioMute) (const char* deviceId);

    /**
     * Use this function to unmute the microphone audio of the call. This is performed the application's media
     * stack and so the call will send no audio even if the accessory's own mic has not been disabled. It
     * is only possible on a connected call. It should only be invoked from the active device.
     * @param  deviceId - identifier for the accessory that is sending the unmute event.
     * @return short  - 0 indicates successful and -1 indicates failure
     */
    short (*microphoneAudioUnmute) (const char* deviceId);

    /**
     * This function returns the current muted state of the call.
     * @return int - 1 indicates \b TRUE,  0 indicates \b FALSE
     */
    int (*isMicrophoneMuted)();

    /**
     * Use this function to start sending video on the call. Recommended to be invoked on a connected call.
     * This only impacts sending video. The call may or may not be receiving video from the remote party.
     * @param  deviceId - Identifier for the device that wants to start sending video.
     * @return short  - 0 indicates successful and -1 indicates failure
     */
    short (*startVideo) (const char* deviceId);

    /**
     * Use this function to stop sending video on the call. Recommended to be invoked on a connected video call.
     * This only impacts sending video. The call may or may not be receiving video from the remote party.
     * @param  deviceId - Identifier for the device that wants to stop sending video.
     * @return short  - 0 indicates successful and -1 indicates failure
     */
    short (*stopVideo) (const char* deviceId);

    /**
	 * This function returns the current video state.
     * This only impacts sending video. The call may or may not be receiving video from the remote party.
	 * @return int - 1 indicates \b TRUE, video is sending,  0 indicates \b FALSE, video is not sending
     */
	int (*isVideoSending)();

    /**
     * Use this function to increase speaker phone volume level
     * @param  deviceId - identifier for the accessory that is sending the volumeUp event.
     * @return short  - 0 indicates successful and -1 indicates failure
     */
    short (*speakerVolumeUp) (const char* deviceId);

    /**
     * Use this function to reduce speaker phone volume level
     * @param  deviceId - Identifier of the device that is sending the volumeDown event.
     * @return short  - 0 indicates successful and -1 indicates failure
     */
    short (*speakerVolumeDown) (const char* deviceId);

    /**
     * Use this function to indicate device microphone volume level change
     * @param  deviceId - identifier for the accessory where the microphone volume level is changed
     * @param  volumeLevel - Volume level can range between 0(MIN) to 100(MAX),
     * @return short  - 0 indicates successful and -1 indicates failure
     */
    short (*onDeviceMicrophoneVolumeChanged) (const char* deviceId, int volumeLevel);

    /**
     * Use this function to indicate device speaker volume level change
     * @param  deviceId - identifier for the accessory where the speaker volume level is changed
     * @param  volumeLevel - Volume level can range between 0(MIN) to 100(MAX),
     * @return short  - 0 indicates successful and -1 indicates failure
     */
    short (*onDeviceSpeakerVolumeChanged) (const char* deviceId, int volumeLevel);

    /**
     * This function will to put the call in "CONNECTED" state on hold.
     * @param  deviceId - Identifier of the device that is sending the call hold event.
     * @return short  - 0 indicates successful and -1 indicates failure
     * Since the actual operation of this method is asynchoronous in nature, the true success of
     * this method can only be confirmed on receipt of onHold event.
     * So the failure of this method indicates, "failure of preconditions for this operation"
     * Note :- callInfo can be safely deallocated after this method returns
     */
    short (*hold) (const char* deviceId);

    /**
     * Use this function to resume a call. call in "HELD" state will be resumed.
     * @param  deviceId - Identifier of the device that is sending the call resume event.
     * @return short  - 0 indicates successful and -1 indicates failure
     * Since the actual operation of this method is asynchoronous in nature, the true success of
     * this method can only be confirmed on receipt of onConnected event.
     * So the failure of this method indicates, "failure of preconditions for this operation"
     * Note :- callInfo can be safely deallocated after this method returns
     */
    short (*resume) (const char* deviceId);

   /**
    * Use this function to redial.
    * @param  deviceId - Identifier of the device that is sending the redial event.
    * @return short  - 0 indicates successful and -1 indicates failure
    * Since the actual operation of this method is asynchoronous in nature, the true success of
    * this method can only be confirmed on receipt of onRemoteAlerting or onConnected event.
    * So the failure of this method indicates, "failure of preconditions for this operation"
    */
    short (*redial) (const char* deviceId);

    /**
     * This is used to notify the application about change in active device due to user's action on the accessory.
     * The applicaion will route the call audio path to the device which has been selected as the audio device (This
     * is future functionality on the Windows platform).
     * @param deviceId - Identifier for the active device.
     * @return short - 0 indicates successful and -1 indicates failure.
     */
    short (*activeDeviceChanged) (const char* deviceId);

    /**
     * This is used to notify about disconnection of audio path due to user's action on the accessory.
     * Based on the application device fallback policy next device in the list could be made active.
     * @param deviceId - identifier for the accessory .
     * @return short - 0 indicates successful and -1 indicates failure.
     * Note :- This is specially applicable for bluetooth devices.
     */
    short (*audioDisconnected) (const char* deviceId);

     /**
     * Use this function to register the plugin callback functions. This method is expected to be invoked at the time of library
     * initialization. It is important to ensure that all fields of functionPtrs are initialised. Otherwise there is a danger
     * that an uninitialised pointer-to-function will be invoked when the corresonding call event occurs.
     * @param  functionPtrs - \c #Accessory_CallControlCallback_v1_0_t containing pointers to plug-in callback function
     * implementations.
     * @return short - 0 value indicates success and  -1 indicates failed registration.
     */
    short (*registerCallback) (Accessory_CallControlCallback_v1_0_t* functionPtrs);

    /**
     * Use this function to unregister the plugin callbacks that were registered at the time of library initialization
     * using #registerCallback. This method must be invoked at the time of library uninitialization.
     * @param functionPtrs - pointer to plug-in callback structure \c #Accessory_CallControlCallback_v1_0_t
     * implementations. This must be same pointer used while invoking the \c #registerCallback
     * @return short  - 0 indicates successful and -1 indicates failure
     */
    short (*unregisterCallback) ( Accessory_CallControlCallback_v1_0_t* functionPtrs);

    /**
     * This is used to reject a call (also known as i-divert). The call will be handled according to the CUCM divert policy for this CUCM device,
     * which includes the possibility of voicemail.
     * @param deviceId - identifier for the accessory.
     * @return short - 0 indicates successful and -1 indicates failure or NOT ALLOWED.
     */
    short (*rejectCall) (const char* deviceId);

    /**
     * This is used to dial voicemail, or display Visual Voicemail, depending on a preference set by the application.
     * @param deviceId - Identifier for the accessory .
     * @return short - 0 indicates successful and -1 indicates failure or NOT ALLOWED.(incase of no voicemail configured for the user, it returns -1).
     * Note :- This is specially applicable for devices, which have functionality to reject the incoming call (i-divert).
     */
    short (*dialVoicemail) (const char* deviceId);

    /**
     * This is used to set the default device type for the user. It will over-write the preferences, and make this as the default device type.
     * @param deviceType - identifier for the accessory type. Possible values = {ACCESSORY_NONE | ACCESSORY_HEADSET | ACCESSORY_HANDSET | ACCESSORY_SPEAKER}
     * @return short - 0 indicates successful and -1 indicates failure or NOT ALLOWED.
     * Note :- This is specially applicable for devices, which have functionality to change default deviceType.
     */
    short (*setDefaultDeviceType) (Accessory_AccessoryType deviceType);

}Accessory_CallControlAPI_v1_0_t;

#ifdef __cplusplus
}
#endif
