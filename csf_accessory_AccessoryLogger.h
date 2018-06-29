#pragma once
/**
 * @file csf_accessory_AccessoryLogger.h
 *
 * Copyright 2013, Cisco Systems
 */

/**
 * Accessory_LogMsg_Severity
 * Enum defining various severity levels.
 */
typedef enum {
	AP_LOG_FATAL =1,
	AP_LOG_ERROR,
	AP_LOG_WARNING,
	AP_LOG_INFO,
	AP_LOG_DEBUG,
	AP_LOG_TRACE
} Accessory_LogMsg_Severity;

/**
 * \brief
 * ACCLogger
 * This is the logger that can be passed to each message being logged. 
 *
 * Usually only one instance is required, but creating seperate
 * loggers allows you to control the string printed with each log message allowing different catagories to be seperated.
 */
typedef struct ACCLogger
{
	/**
	 *Pointer to Accessory provider internal logger representation
	 **/
	void *pLogger;
} ACCLogger_t;

/**
 * \brief
 * An API to allow the plugin to pass debug and informational log messages to the application so the 
 * application can store them with its own log file.
 *
 * The plugin can request the logger API using the
 * getAccessoryAPI(Accessory_APIVersion_t apiVersion, void *apiFunctionPointers) function.
 * The below code snippet shows how to get Accessory Logger APIs
 * \code
 *  Accessory_LoggerAPI_v1_0_t * accessoryLoggerAPIs;
 *  int initialize (accessoryAPIEntryPoint_t accessoryApi_fptr)
 *  {
 *      Accessory_APIVersion_t apiVersion;
 *      apiVersion.type =  ACCESSORY_LOGGER;
 *      apiVersion.major = 1;
 *      apiVersion.minor = 0;
 *      accessoryLoggerAPIs = (Accessory_LoggerAPI_v1_0_t*) malloc (sizeof (Accessory_LoggerAPI_v1_0_t));
 *      if (accessoryLoggerAPIs == NULL){
 *      	printf ("Accessories Logger API is null");
 *      	return (-1);
 *      }
 *      if (accessoryApi_fptr(apiVersion, (void*)accessoryLoggerAPIs) == -1 ){
 *      	return (-1);
 *      }
 *      logger = (ACCLogger_t *) malloc(sizeof(ACCLogger_t));
 *      if (logger == NULL)
 *      {
 *            printf (" ACCLogger_t, Memory allocation Error \n");
 *            return (-1);
 *      }
 *      
 *      if (-1 == accessoryLoggerAPIs->getLogger("cisco.print_only_plugin", logger))
 *      {
 *          printf ("Error Failed to fetch logger instance\n");
 *          return (-1);
 *      }

 *      if (logger == NULL)
 *      {
 *          printf ("Error couldn't fetch logger from AM\n");
 *          return (-1);
 *      }

 *		accessoryLoggerAPIs->logMessage(logger, AP_LOG_INFO, "Logging has been initialized.");
 *   }
 *
 *   short uninitialize()
 *   {
 *   	free(accessoryLoggerAPIs);
 *   }
 * \endcode
 */
typedef struct
{
    /**
     *
     * @param pluginName - A vendor defined unique name following the below mentioned naming convention,
     * \b VendorName.PluginName.
     * Ex, for a vendor XYZ, providing USB plug-in the name would be "XYZ.USBHeadsetPlugin". Though there is
     * no restriction on the size of pluginName it is advisible to limit it to 20 - 25 charcters. To ensure
     * uniqueness of the \p pluginName  the logger will prepend "csf.accessory." to the \p pluginName supplied.
     * So in the above given example the name would look like "csf.accessory.XYZ.USBHeadsetPlugin"
     * @param logger - \c pointer to an allocated but uninitialized \c #ACCLogger_t struct.
     *  @return short - 0 indicates successful creation of logger instance  & -1 indicates error while creating logger instance
     * \attention Please ensure \c #ACCLogger_t instance is deallocated during library \c #uninitialize
     */
     short (*getLogger) (char* pluginName, ACCLogger_t* logger);

    /**
     * This function is meant for plug-in to write any debug/info/error messages to
     * accessory provider log.
     * @param logger - Pointer to the specific logger.
     * @param severity - one of the catagories of log message
     * @param format - format string similar to printf followed by variable number of arguments.
     */
    void (*logMessage) (ACCLogger_t *logger,  Accessory_LogMsg_Severity severity, const char* format, ...);

} Accessory_LoggerAPI_v1_0_t;
