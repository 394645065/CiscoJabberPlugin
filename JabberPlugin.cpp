// JabberPlugin.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"


/*
// functions declaration
*/


void Log(PCHAR info);

typedef enum
{
	ANSWERCALL_BUTTON=0,
	ENDCALL_BUTTON
}Buttons;



Accessory_CallControlAPI_v1_0_t * gAccessoryProviderAPIs=NULL;
Accessory_CallControlCallback_v1_0_t* gCallbacks=NULL;
Accessory_LoggerAPI_v1_0_t *gAccessoryLoggerAPIs=NULL;
ACCLogger_t *gLogger=NULL;
bool gConnected = false;
bool gIncoming = false;
bool gOutgoing = false;
bool gOnMute   = false;
bool gOnHold = false;
extern HWND hJabberMsgWnd;




static short onAudioMute()
{
	Log("onAudioMute\n");

	short rc = 0;

	printf ("Received event 'MUTE'\n");
	return (rc);
}

static short onAudioUnmute()
{
	Log("onAudioUnMute\n");

	short rc = 0;

	printf ("Received event 'UNMUTE'\n");
	return (rc);
}

static short onIncoming (Accessory_CallInfo_v1_0_t* incomingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo)
{
	Log("onIncoming\n");
	gIncoming=true;

	//set state variable to indicate that a call is incoming


	SendEventToIntegration(Ringer);

	//get the device ID
	//tell the device to display an indication that a call is incoming (e.g. flash an LED)
	//log that a ring was successfully performed

	return 0;
}

static short onCallWaiting (Accessory_CallInfo_v1_0_t* incomingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo)
{
	Log("onCallWaiting\n");
	//get the device ID
	//tell the device to display some information that a second call is waiting to be answered
	//log that we told the device to display such a message


	return 0;
}

static short onPhoneRegistrationStatusChanged(Accessory_PhoneConnectionStatus status)
{
	Log("onPhoneRegistrationStatusChanged\n");
	return 0;
}

static short onRemoteAlerting (Accessory_CallInfo_v1_0_t* outgoingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo)
{
	Log("onRemoteAlerting\n");
	SendEventToIntegration(OffHook);
	return 0;
}

static short onBusy (Accessory_CallInfo_v1_0_t* outgoingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo)
{
	Log("onBusy\n");
	gAccessoryLoggerAPIs -> logMessage(gLogger, AP_LOG_ERROR, "FUNC : onBusy");
	return 0;
}

static short onReorder (Accessory_CallInfo_v1_0_t* outgoingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo)
{
	Log("onReorder\n");
	return 0;
}              

//this will be invoked by the Accessories Manager when ECC has successfully answered the call
static short onConnected (Accessory_CallInfo_v1_0_t* connectedCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo)
{
	Log("onConnected\n");
	gConnected = true;
	gIncoming = false;
	gOutgoing = false;


	//set state variable to indicate that we have a connected call
	SendEventToIntegration(OffHook);

	//get the device ID
	//tell the device to display connection status accordingly (e.g. change color of connection status LED

	return 0;
}

static short onHold (Accessory_CallInfo_v1_0_t* heldCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo)
{
	Log("onHold\n");
	//get the device ID
	//tell the device to display information that a call is on hold. Information about the call can be found in heldCallInfo.
	SendEventToIntegration(Flash);
	return 0;
}

static short onOffHook (Accessory_CallInfo_v1_0_t* outgoingCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo)
{
	Log("onOffHook\n");

	SendEventToIntegration(OffHook);
	return 0;
}

static short onOnHook (Accessory_CallInfo_v1_0_t* endCallInfo, Accessory_CallInfo_v1_0_t* callInfoList[], int numCallInfo)
{
	Log("onOnHook\n");

	SendEventToIntegration(Free);
	return 0;
}


static short onVolumeChanged (Accessory_AudioType audioType, int volumeLevel)
{
	Log("onVolumeChanged\n");
	return 0;
}

static short setDeviceMicrophoneVolume (const char* deviceId, int volumeLevel)
{
	Log("setDeviceMicrophoneVolume\n");
	return 0;
}

static short setDeviceSpeakerVolume (const char* deviceId, int volumeLevel)
{
	Log("setDeviceSpeakerVolume\n");
	return 0;
}

static short onActiveDeviceChanged(Accessory_AccessoryType deviceType, const char* deviceId, const char* deviceName)
{
	Log("onActiveDeviceChanged\n");
	return 0;
}

int getLastError ()
{
	return 0;
}

const char* getErrorDescription(int errorCode)
{
	return "nothing";
}

static short notifySubscriberNumbers (Accessory_Subscriber_Info *subscriberInfoList[], int numberOfLines)
{
	Log("notifySubscriberNumbers\n");
	return 0;
}

//Local plugin function to detect button presses on the device. This function would be
//called by the plugin regularly (i.e. in a thread) to poll for button presses on the device
static void handleBtnsStateChange()
{
	//get this button state information from low level device code
	Buttons button; //=GetDeviceButtonStateInfo()

	//query the two buttons on the USB device to determine if either have been pressed
	//here "button" is variable representing the button which was pressed on the device
	switch(button)
	{
		//in this example, the ANSWERCALL_BUTTON can answer an incoming call, put a connected call on hold, or resume a call which is on hold
	case ANSWERCALL_BUTTON:                     
		//if there is an incoming call (gIncoming == true)
		gIncoming=false;
		//invoke the call control API answerCall() to answer the call 
		//Eventually we will receive an onConnected() callback as a result:
		gAccessoryProviderAPIs->answerCall("device ID here");
		//log that a request was made to answer the call
		//else if there is a call currently connected (gConnected==true)                            
		//invoke the call control API holdCall() to put the call on hold
		//Eventually we will receive an onHold() callback as a result:
		gAccessoryProviderAPIs->hold("device ID here");
		gOnHold = true;
		//else if there is a call on hold (gOnHold == true)
		//invoke the call control API resumeCall() to set the call on hold back to the active call
		gAccessoryProviderAPIs->resume("device ID here");                     
		break;

	case ENDCALL_BUTTON:
		//ensure there is a connected call
		if(gConnected)
		{
			//invoke the call control API to end the call
			//eventually we will receive an onOnHook() callback as a result
			gAccessoryProviderAPIs->endCall("device ID here");
		}
		break;
	}
}

int initialize (accessoryAPIEntryPoint_t accessoryApi_fptr)
{
	printf ("My plugin initialized successfully\n");

	//specify the API version to use
	Accessory_APIVersion_t ccapiVer;
	ccapiVer.type =  ACCESSORY_CALL_CONTROL;
	ccapiVer.major = 1;
	ccapiVer.minor = 0;

	//specify the logger API version information
	Accessory_APIVersion_t loggerapiVer;
	loggerapiVer.major = 1;
	loggerapiVer.minor = 0;
	loggerapiVer.type =  ACCESSORY_LOGGER;

	//allocate space for function pointers
	gAccessoryProviderAPIs = (Accessory_CallControlAPI_v1_0_t*) malloc (sizeof (Accessory_CallControlAPI_v1_0_t));
	if(gAccessoryProviderAPIs == NULL)
	{
		printf ("Accessories Provider API is null");
		return (-1);
	}

	//pass our function pointers to the host app along with the API version we're using
	if (accessoryApi_fptr(ccapiVer, (void*)gAccessoryProviderAPIs) == -1)
	{
		return (-1);
	}

	//allocate the logger API
	gAccessoryLoggerAPIs = (Accessory_LoggerAPI_v1_0_t*) malloc (sizeof (Accessory_LoggerAPI_v1_0_t));
	if (gAccessoryLoggerAPIs == NULL)
	{
		printf ("Accessory_LoggerAPI_v1_0_t, Memory allocation Error \n");
		return (-1);
	}

	//fetch the logger API
	if (accessoryApi_fptr(loggerapiVer, (void*)gAccessoryLoggerAPIs) == -1 )
	{
		printf ("Error couldn't fetch logger API\n");
		return (-1);
	}

	//allocate the logger - this will be used in conjunction with gAccessoryLoggerAPIs
	//to log messages throughout the life of the plugin
	gLogger = (ACCLogger_t *) malloc(sizeof(ACCLogger_t));
	if (gLogger == NULL)
	{
		printf ("ACCLogger_t, Memory allocation Error \n");
		return (-1);
	}

	//get the logger from the API
	if (gAccessoryLoggerAPIs->getLogger("Howard.USBHeadsetPlugin", gLogger) == -1)
	{
		return (-1);
	}


	if (gLogger == NULL)
	{
		printf ("Error couldn't fetch logger from AM\n");
		return (-1);
	}

	gAccessoryLoggerAPIs -> logMessage(gLogger, AP_LOG_ERROR, "Logging has been initialized.");

	//allocate and set our callbacks
	gCallbacks = (Accessory_CallControlCallback_v1_0_t*) malloc (sizeof(Accessory_CallControlCallback_v1_0_t));       
	if (gCallbacks != NULL)
	{

		// Ensure that any callbacks that are explicitly set in the list below are set to NULL pointers.
		// Otherwise a random pointer may be used when the event occurs
		memset (gCallbacks, 0, sizeof(Accessory_CallControlCallback_v1_0_t));

		gCallbacks->onMicrophoneAudioMute = &onAudioMute;
		gCallbacks->onMicrophoneAudioUnmute = &onAudioUnmute;
		gCallbacks->onIncoming = &onIncoming;
		gCallbacks->onCallWaiting = &onCallWaiting;
		gCallbacks->onRemoteAlerting  = &onRemoteAlerting;
		gCallbacks->onBusy = &onBusy;
		gCallbacks->onReorder = &onReorder;
		gCallbacks->onConnected = &onConnected;
		gCallbacks->onHold = &onHold;
		gCallbacks->onOffHook = &onOffHook;
		gCallbacks->onOnHook = &onOnHook;
		gCallbacks->onSpeakerVolumeChanged = &onVolumeChanged;
		gCallbacks->setDeviceMicrophoneVolume = &setDeviceMicrophoneVolume;
		gCallbacks->setDeviceSpeakerVolume = &setDeviceSpeakerVolume;
		gCallbacks->onActiveDeviceChanged =&onActiveDeviceChanged;
		//gCallbacks->getLastError = &getLastError;
		//gCallbacks->getErrorDescription = &getErrorDescription;
		gCallbacks->notifySubscriberInfo = &notifySubscriberNumbers;
		gCallbacks->onPhoneRegistrationStatusChanged = &onPhoneRegistrationStatusChanged;

		//now register the callbacks
		gAccessoryProviderAPIs->registerCallback(gCallbacks);
	}
	else
	{
		return -1;
	}



	DWORD threadID;
	HANDLE hThread = CreateThread(NULL, 0, CommucationThreadProc, NULL, 0, &threadID); // Create new thread

	return 0;
}

short uninitialize()
{
	if(gAccessoryLoggerAPIs != NULL)
	{
		free(gAccessoryLoggerAPIs);
		gAccessoryLoggerAPIs = NULL;
	}

	if (gLogger != NULL)
	{
		free(gLogger);
		gLogger = NULL;
	}

	if (gAccessoryProviderAPIs!=NULL)
	{
		gAccessoryProviderAPIs->unregisterCallback(gCallbacks);
		free(gAccessoryProviderAPIs);
		gAccessoryProviderAPIs = NULL;
	}

	if(gCallbacks!=NULL)
	{
		free(gCallbacks);
		gCallbacks = NULL;
	}


	SendMessage(hJabberMsgWnd, WM_CLOSE,0,0);

	printf ("My plugin uninitialized successfully\n");

	return 0;
}


void Log(PCHAR info)
{
	FILE * pFile;
	fopen_s (&pFile,"D:\\myfile.txt","a+");
	if (pFile!=NULL)
	{
		fputs (info,pFile);
		fclose (pFile);
	}
}
