#pragma once
/**
 * @file csf_accessory_AccessoryAPIVersion.h
 *
 * Copyright 2013, Cisco Systems
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Various API types
*/
#define ACCESSORY_CALL_CONTROL 1
#define ACCESSORY_LOGGER       2


/**
 * \brief
 * This structure defines the elements of Accessory API version, allowing a plugin to request an API of a specific version, 
 * which allows newer versions of AM to be compatible with plugins built for earlier versions. It is used as a parameter when requesting an API from AM.
 */
typedef struct {
	/**
	 * Valid values are #ACCESSORY_CALL_CONTROL, #ACCESSORY_LOGGER
	 */
	short type;
	/**
	 * API major version
	 */
	short major;
	/**
	 * API Minor version
	 */
	short minor;  //
} Accessory_APIVersion_t;



/**
 * Function to get the CSF API corresponding to the version requested
 * @param apiVersion - this struct should contain the API type, major and minor version.
 * @param apiFunctionPointers - pointer to struct returning the APIs
 * @return short  - 0 indicates successful and -1 indicates failure
 */
short getAccessoryAPI(Accessory_APIVersion_t apiVersion, void *apiFunctionPointers);

/**
 * This is a function pointer that is passed to plugin as a patameter to the initialize function. It is the
 * starting point for accessing any of the APIs available from Accessory Manager. The plugin can use this
 * function to request a specific API (for example ACCESSORY_LOGGER) and a specific version ( to allow 
 * backward compatibility.
 */
typedef short (*accessoryAPIEntryPoint_t) (Accessory_APIVersion_t, void*);

/*
 * \note
 * \b PLUGIN LIBRARY initialize and uninitialize functions \b.
 * \par
 * Following are the methods plugin library must implement to initialize and uniinitialize the library.The initialize method should be used
 * for library specific initialization. This method takes a function pointer to getCsfAPI as an argument. This function pointer can be used
 * to get the desired Accessories Provider API implementation. The function takes 2 parameters, a struct csfApiVersion instance followed by
 * a pointer to duly allocated Accessory_CallControlAPI_v1_0_t strucutre.
 * \par
 * As shown above the "struct Accessory_CallControlAPI_v1_0_t" defines the call control, media APIs and the method availble for setting the
 * Plugin callback(struct Accessory_CallControlCallback_v1_0_t ) exposed by the Accessories Provider. The "initialize" function should be
 * implemented in  a non-blocking manner. On successful initialization  implementer should return short value with 0 indicating  successful
 * or  -1 indicating  failure
 * \par
 *  The "unitialize" function provides an option for plugin library to perform any cleanup activity before the library is unloaded. This
 *  function is invoked prior to unloading the plugin library, On successful initialization  implementer should return short value with 0
 *  indicating  successful or -1 indicating  failure
 * 
 *  Here below is the sample code which shows how the required Accessory API can fetched.
 *
 *  \code
 *  Accessory_CallControlAPI_v1_0_t * accessoryProviderAPIs;
 *  int initialize (accessoryAPIEntryPoint_t accessoryApi_fptr)
 *  {
 *      Accessory_APIVersion_t apiVersion;
 *      apiVersion.type =  ACCESSORY_CALL_CONTROL;
 *      apiVersion.major = 1;
 *      apiVersion.minor = 0;
 *      accessoryProviderAPIs = (Accessory_CallControlAPI_v1_0_t*) malloc (sizeof (Accessory_CallControlAPI_v1_0_t));
 *      if (accessoryProviderAPIs == NULL)
 *      {
 *          printf ("Accessories Provider API is null");
 *          return (-1);
 *      }
 *      if (accessoryApi_fptr(apiVersion, (void*)accessoryProviderAPIs) == -1 )
 *      {
 *          return (-1);
 *      }
 *   }
 *
 *   short uninitialize()
 *   {
 *      accessoryProviderAPIs->unregisterCallback();
 *      free(accessoryProviderAPIs);
 *
 *   }
 *  \endcode
 */
#ifdef _WIN32
extern __declspec(dllexport) int initialize (accessoryAPIEntryPoint_t accessoryApi_fptr);
extern __declspec(dllexport) short uninitialize();  
#else
/**
 * This function must be implemented by the plugin. Its role is to ask AM for the required APIs
 * includeing ACCESSORY_LOGGER and ACCESSORY_CALL_CONTROL. It also registers callbacks so that the
 * plugin will get notified of events.
 * @param accessoryApi_fptr - This paramater is passed from AM to the plugin, and can then be called by
 * the plugin to access the API's it requires. Normally the accessoryApi_fptr funciton pointer would only be called from
 * inside the initialize function.
 */
extern  int initialize (accessoryAPIEntryPoint_t accessoryApi_fptr);
/**
 * This function must be implemented by the plugin. Its role is tidy up any resources allocated in 
 * \c initialize, and to unregister the callbacks.
 */
extern  short uninitialize();
#endif

#ifdef __cplusplus
}
#endif
