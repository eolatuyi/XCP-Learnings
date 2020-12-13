/*
 * xcp_config_parameters.h
 *
 *  Created on     : Jul 25, 2020
 *      Author     : Ebenezer O
 *      Description: Declaration and definitions of precompile interface for configuration
 *                   of XCP Basic constant parameters
 */

#ifndef CONFIGS_INCLUDES_XCP_CONFIG_PARAMETERS_H_
#define CONFIGS_INCLUDES_XCP_CONFIG_PARAMETERS_H_

/* XCP slave device identification (optional) */
#define kXcpStationIdLength 6    /* Slave device identification length uint8 */

extern uint8_t const kXcpStationId[kXcpStationIdLength];

#define DEFINE_XCP_CONFIG_PARAMETERS(map_file_name) \
        uint8_t const kXcpStationId[kXcpStationIdLength] = map_file_name;

#endif /* CONFIGS_INCLUDES_XCP_CONFIG_PARAMETERS_H_ */
